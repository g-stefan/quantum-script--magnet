# Getting started

## 1. Build and install

The extension is built with [fabricare](https://github.com/g-stefan/fabricare),
the build tool used by all XYO C++ projects. `quantum-script` (and everything
below it: `xyo-system`, `xyo-encoding`, ...) and **every bundled extension**
(the 31 `quantum-script--*` projects listed in `fabricare.json`, with their
own dependencies such as `xyo-cryptography`, `xyo-networking`,
`xyo-pixel32`, `file-xml`, OpenSSL) must be installed to the SDK first. From
the repository root:

```bash
fabricare make       # build into output/
fabricare test       # build and run test/test.01 (run make first)
fabricare install    # copy output/{bin,include,lib} to ~/.fabricare/<platform>
fabricare clean      # remove output/ and temp/
```

`fabricare.json` declares two projects:

| Project | Kind | Purpose |
|---------|------|---------|
| `quantum-script--magnet` | `dll-or-lib`: shared library in a dynamic build, static library in a static build | the extension |
| `test.01` | executable, category `test` | runs `test/test.01.js` in a host that registers only `Magnet` |

After `fabricare install`, `quantum-script--magnet.dll` (Windows) /
`quantum-script--magnet.so` (Linux) sits in the SDK `bin` folder next to
`quantum-script.exe` and the other extension libraries it links to, which is
where `Script.requireExtension("Magnet")` finds it.

When a bundled extension changes its public C++ API (or its version major),
rebuild and reinstall Magnet after it, then the `magnet` application.

## 2. The `magnet` application

The `magnet` repository builds the `magnet` executable: the Quantum Script
interpreter with this extension preloaded. Its start-up code is:

```cpp
Extension::Magnet::registerInternalExtension(executive);
executive->compileString("Script.requireExtension=Script.requireInternalExtension;");
executive->compileString("Script.requireInternalExtension(\"Magnet\");");
```

So in `magnet` scripts:

- `Magnet` is already loaded, every bundled extension can be required
  directly;
- `Script.requireExtension` is `Script.requireInternalExtension`: it never
  loads a DLL, only bundled extensions can be loaded (an extension outside
  the bundle, for example `SCard`, fails with `Unable to open "SCard"`);
- threads get the same set-up (the init code runs again for every thread).

```javascript
// hello-magnet.js
Script.requireExtension("Console");
Script.requireExtension("Shell");
Script.requireExtension("SHA256");

var files = Shell.getFileList("*.js");
for (var i = 0; i < files.length; ++i) {
	Console.writeLn(SHA256.fileHash(files[i]) + "  " + files[i]);
};
```

```bash
magnet hello-magnet.js
magnet --run "Script.requireExtension(\"Console\"); Console.writeLn(\"ok\");"
magnet --execution-time hello-magnet.js
```

Options: `script.js`, `--run "code"` (the code is the next argument),
`--cmd script` (skip the first two lines, for `.cmd` launchers),
`--execution-time`, `--execution-time-cmd`, `--license`. Without arguments it
prints its version and usage.

A Windows `.cmd` launcher written in Quantum Script:

```bat
@magnet --cmd "%~f0" %*
@exit /b %ERRORLEVEL%
Script.requireExtension("Console");
Script.requireExtension("Application");
Console.writeLn("arguments: " + Application.getCmdN());
```

An uncaught exception prints `Error: ...` and the stack trace on stdout and
makes `magnet` exit with code 1. Note: `magnet` 5.9.0 returns 0 after a
script that ends normally, even when it called `Script.exit(n)` /
`Script.setExitCode(n)` (the `quantum-script` interpreter returns `n`);
throw an exception when a caller must see a failure.

## 3. Load Magnet from `quantum-script`

The dynamic `quantum-script` interpreter loads Magnet like any other
extension DLL:

```javascript
Script.requireExtension("Magnet");
Script.requireExtension("Console");
Script.requireExtension("Base16");
Console.writeLn(Base16.encode("ab"));      // 6162
```

`Script.requireExtension(name)` looks for an external
`quantum-script--<name>` library first (the file as named, then every include
path folder: next to the interpreter, next to the script), then for an
internal extension. In the SDK `bin` folder the separate extension DLLs are
present too, so `Script.requireExtension("Base16")` may load
`quantum-script--base16.dll` instead of the bundled copy: the code is the
same (Magnet links to those DLLs), only `getExtensionList()[...].fileName`
differs. Use `Script.requireInternalExtension("Base16")` to always get the
bundled one.

The **static** `quantum-script` (`win64-msvc-2026.static`) cannot load
extension DLLs: `Script.requireExtension("Magnet")` fails with
`Unable to open "Magnet"`. Use `magnet`, or a static host that links Magnet
(section 6).

## 4. Register it in a C++ host

A host that embeds Quantum Script gets the whole standard set with one
registration in its init callback (this is what `test/test.01.cpp` does):

```cpp
#include <XYO/QuantumScript.hpp>
#include <XYO/QuantumScript.Extension/Magnet.hpp>

using namespace XYO::QuantumScript;

void initExecutive(Executive *executive) {
	Extension::Magnet::registerInternalExtension(executive);
};

int main(int cmdN, char *cmdS[]) {
	if (ExecutiveX::initExecutive(cmdN, cmdS, initExecutive)) {
		if (!ExecutiveX::executeString(
		        "Script.requireExtension(\"Magnet\");"
		        "Script.requireExtension(\"Console\");"
		        "Script.requireExtension(\"JSON\");"
		        "Console.writeLn(JSON.encode({a: [1, 2]}));")) {
			printf("%s\n", (ExecutiveX::getError()).value());
			printf("%s", (ExecutiveX::getStackTrace()).value());
		};
		ExecutiveX::endProcessing();
	};
	return 0;
};
```

Registering only makes `Magnet` *available*. The bundled extensions are
registered when a script loads `Magnet`: before that,
`Script.requireInternalExtension("JSON")` fails. To have them ready without
a `requireExtension("Magnet")` line in every script, load it from the init
callback, as `magnet` does:

```cpp
void initExecutive(Executive *executive) {
	Extension::Magnet::registerInternalExtension(executive);
	executive->compileString("Script.requireInternalExtension(\"Magnet\");");
};
```

In the host's `fabricare.json`:

```json
{
	"name": "my-host",
	"make": "exe",
	"sourcePath": "XYO/MyHost",
	"dependency": [
		"quantum-script--magnet"
	]
}
```

fabricare resolves the bundled extensions and their libraries transitively
from `quantum-script--magnet.json` in the SDK.

## 5. Threads

Each thread that runs scripts (`Thread`, `Job`, `Make`, ...) has its own
engine, initialised with the host's init callback. In a host that only
registers Magnet, a thread function must load it again:

```javascript
Script.requireExtension("Magnet");
Script.requireExtension("Thread");

var thread = Thread.newThread(function(text) {
	Script.requireExtension("Magnet");      // this thread's engine
	Script.requireExtension("Base16");
	return Base16.encode(text);
}, null, ["ab"]);
thread.join();
thread.getReturnedValue();                  // "6162"
```

Without the `Magnet` line the thread fails (`getReturnedValue()` is
`undefined`). In `magnet`, and in hosts whose init callback loads Magnet,
the line is not needed (it does nothing when Magnet is already loaded).

## 6. Static builds

The project is `dll-or-lib`: on a static platform (for example
`win64-msvc-2026.static`, which sets `XYO_PLATFORM_COMPILE_STATIC`) the same
project builds a static library and links the static variants of the
bundled extensions. The export macros become empty and the
`quantumScriptExtension` DLL entry point is left out (it is compiled only
with `XYO_PLATFORM_COMPILE_DYNAMIC_LIBRARY`).

A static host must register Magnet with `registerInternalExtension`
(section 4): external DLLs cannot be loaded into a host that does not use
the engine DLL. There is no `quantum-script--magnet.static` project for
statically linked tools on a dynamic platform; to compile the sources into
another library define `XYO_QUANTUMSCRIPT_EXTENSION_MAGNET_LIBRARY` (see
[C++ API](cpp-api.md#build-defines)).

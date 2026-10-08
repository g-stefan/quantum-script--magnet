---
name: quantum-script--magnet
description: >-
  How to use the Quantum Script Magnet extension (quantum-script--magnet),
  the all-in-one extension: Script.requireExtension("Magnet") registers the
  31 standard extensions (Application, ApplicationVersion, Base16, Base32,
  Base64, Buffer, Console, Crypt, CSV, DateTime, File, HTTP, Job, JSON,
  Make, Math, MD5, OpenSSL, Pixel32, ProcessInteractive, Random, SHA256,
  SHA512, Shell, ShellFind, Socket, SSHRemote, Task, Thread, URL, XML) as
  internal extensions, after which scripts still require each one by name;
  Magnet defines no objects itself; no "Version" extension (it is
  ApplicationVersion); requireExtension prefers an external
  quantum-script--<name>.dll, requireInternalExtension forces the bundled
  one; the magnet application (Magnet preloaded, requireExtension replaced
  by requireInternalExtension, --run / --cmd / --execution-time, exit code
  0 even after Script.exit(n)); loading Magnet from the quantum-script
  interpreter (not the static one); threads need
  Script.requireExtension("Magnet") again unless the host's init callback
  loads it; the C++ side (Extension::Magnet::registerInternalExtension,
  initExecutive, quantumScriptExtension entry point, dll-or-lib,
  XYO_QUANTUMSCRIPT_EXTENSION_MAGNET_LIBRARY, Version / Copyright / License),
  registration order and overriding a bundled extension, adding an
  extension to the bundle. Use when writing or reviewing magnet scripts,
  C++ hosts that include <XYO/QuantumScript.Extension/Magnet.hpp> or depend
  on "quantum-script--magnet" in fabricare.json, or when working inside the
  quantum-script--magnet repository.
---

# quantum-script--magnet

All-in-one extension of Quantum Script (see the `quantum-script` skill for
the language and its differences from JavaScript; its rules apply — no
hoisting, `typeof(x)` needs parentheses, `&&` / `||` return booleans,
statements and blocks end with `;`). Purpose: **make every standard
Quantum Script extension available with one registration / one
`requireExtension`**, without searching for 31 separate libraries. It is
the engine of the `magnet` application.

Full documentation: `docs/` in the quantum-script--magnet repository
(`X:\Storage\XYO\Gitea\CPP\quantum-script--magnet\docs` on this machine):
README (purpose), getting-started (build, magnet app, quantum-script, C++
host, threads, static), **extensions** (catalog: objects each extension
defines, what it loads, where its docs are), cpp-api (registration
mechanics, adding an extension), reference (errors, tables). The API of a
bundled extension is in its own repository
(`quantum-script--<lowercase name>`, `docs/` and its skill); `Console` is in
`quantum-script/docs/standard-library.md`. When in doubt read
`source/XYO/QuantumScript.Extension/Magnet/Library.cpp` (the whole logic)
or run it: `magnet --run "..."`.

## Script usage

```javascript
Script.requireExtension("Magnet");      // registers the bundle (already done in magnet)

Script.requireExtension("Console");     // then load what you use, as usual
Script.requireExtension("Shell");
Script.requireExtension("JSON");

var info = JSON.decode(Shell.fileGetContents("version.json"));
Console.writeLn(JSON.encodeWithIndentation(info));
```

## Hard rules

1. **Magnet registers, it does not load.** After
   `requireExtension("Magnet")` every extension used still needs its own
   `Script.requireExtension(name)`. Magnet defines no global object.
2. **Names**: `Application`, `ApplicationVersion`, `Base16`, `Base32`,
   `Base64`, `Buffer`, `Console`, `Crypt`, `CSV`, `DateTime`, `File`,
   `HTTP`, `Job`, `JSON`, `Make`, `Math`, `MD5`, `OpenSSL`, `Pixel32`,
   `ProcessInteractive`, `Random`, `SHA256`, `SHA512`, `Shell`,
   `ShellFind`, `Socket`, `SSHRemote`, `Task`, `Thread`, `URL`, `XML`.
   Not case sensitive. There is **no `Version`** extension
   (`Unable to open "Version"`): use `ApplicationVersion`. `SCard` and
   `Example` are not bundled.
3. **Objects that are not named like the extension**: `ApplicationVersion`
   also defines `VersionCompare`; `OpenSSL` defines `OpenSSL` **and
   `HTTPS`** (and adds `Crypt.publicEncrypt`, ...); `Make` → `Make`,
   `MakeError`; `Task` → `Task`, `TaskQueue`; `Thread` → `Thread`,
   `CurrentThread`, `Atomic`, `Processor`; `XML` → `XML`, `XMLDocument`,
   `XMLNode`, `XMLAttribute(s)`, `XMLNodeType`, `XMLParserMode`. Some load
   others (`HTTP` → `File`, `Socket`, `URL`, `JSON`; `Job` → `Thread`,
   `Shell`, `Math`; `Pixel32` → `Random`; most binary ones → `Buffer`).
4. **DLL first.** `Script.requireExtension(name)` loads an external
   `quantum-script--<name>.dll` / `.so` (as named, then each include path
   folder) before the internal one; in the SDK `bin` folder both exist (same
   code). `Script.requireInternalExtension(name)` uses only the bundle.
   `getExtensionList()[i].fileName` is `""` for internal.
5. **Before Magnet is loaded nothing bundled is registered**: in a host
   that registers only Magnet, `requireInternalExtension("JSON")` before
   `requireExtension("Magnet")` throws `Unable to open "JSON"`.
6. **Threads have their own engine** (Thread, Job, Make): the host's init
   callback runs again, nothing else carries over. In a host that only
   registers Magnet, the thread function must
   `Script.requireExtension("Magnet")` first, otherwise it fails
   (`getReturnedValue()` → `undefined`). In `magnet` it is preloaded.
7. **The `magnet` application**: Magnet preloaded in every thread;
   `Script.requireExtension` **is** `Script.requireInternalExtension` (no DLL
   is ever loaded, non-bundled extensions fail). Options: `script.js`,
   `--run "code"` (code is the next argument; `quantum-script` uses
   `--run=code`), `--cmd file` (skips 2 lines, for `.cmd` launchers:
   `@magnet --cmd "%~f0" %*` / `@exit /b %ERRORLEVEL%`),
   `--execution-time`, `--execution-time-cmd`, `--license`. Uncaught error:
   message + stack trace on stdout, exit code 1. **`Script.exit(n)` /
   `setExitCode(n)` still exit 0 in magnet 5.9.0**: throw to signal failure.
8. **quantum-script interpreter**: the dynamic one loads
   `quantum-script--magnet.dll` (its 31 dependency DLLs must be next to it);
   the static one (`win64-msvc-2026.static`) cannot:
   `Unable to open "Magnet"`.
9. Iterate `Script.getExtensionList()` with a `Script.isNil(list[i])` check
   (indexed with holes). Magnet's entry: `name` `"Magnet"`, `version`
   `"5.9.0.8"`, `info` `"Magnet\r\n"` + copyright + short MIT notice.

## C++

```cpp
#include <XYO/QuantumScript.hpp>
#include <XYO/QuantumScript.Extension/Magnet.hpp>
using namespace XYO::QuantumScript;

void initExecutive(Executive *executive) {                   // host init callback
	Extension::Magnet::registerInternalExtension(executive);
	// optional: have the bundle ready before any script (runs after this callback)
	executive->compileString("Script.requireInternalExtension(\"Magnet\");");
};
```

- `registerInternalExtension` registers only `"Magnet"`;
  `Magnet::initExecutive` (run when a script loads Magnet) calls
  `Extension::<Name>::registerInternalExtension` for each bundled
  extension. Do not call `initExecutive` yourself.
- `Executive::registerInternalExtension` with an existing name (no case)
  **replaces** it, so loading Magnet overrides a host's own `"Console"` /
  `"JSON"` registration unless that extension was **loaded** before Magnet
  (`compileString("Script.requireInternalExtension(\"Console\");"
  "Script.requireInternalExtension(\"Magnet\");")`). Code from
  `compileString` in the init callback runs after the callback returns.
- fabricare.json: `"dependency": ["quantum-script--magnet"]` pulls in
  `quantum-script`, Console and all bundled extensions transitively. The
  project is `dll-or-lib` (DLL on dynamic platforms, static lib on
  `*.static`); there is no `quantum-script--magnet.static` project. Define
  `XYO_QUANTUMSCRIPT_EXTENSION_MAGNET_LIBRARY` to compile the sources into
  another library (empty export, no DLL entry point). The DLL entry point
  `extern "C" quantumScriptExtension(Executive *, void *)` exists only with
  `XYO_PLATFORM_COMPILE_DYNAMIC_LIBRARY`.
- Metadata: `Magnet::Version::version()` / `build()` /
  `versionWithBuild()` / `datetime()`, `Magnet::Copyright::*`,
  `Magnet::License::license()` / `shortLicense()`.

## Working in this repository

- All logic is `Magnet/Library.cpp`: one `#include` and one
  `Extension::<Name>::registerInternalExtension(executive);` line per
  bundled extension (alphabetical). Adding one: dependency in
  `fabricare.json`, include + registration in `Library.cpp`, a row in the
  `extensions` table of `test/test.01.js`, then `README.md`,
  `docs/extensions.md`, `docs/reference.md`, this skill, and the `magnet`
  application's README.
- Build: install `quantum-script` and every bundled extension first, then
  `fabricare make`, `fabricare test` (`test/test.01.cpp` registers **only**
  Magnet; `test/test.01.js` uses `check(name, value, expected)` with `!==`
  and throws on mismatch: nothing internal before Magnet, every extension
  loadable with `requireInternalExtension` and defining its objects,
  `Version` not an extension, the `getExtensionList` entry, smoke calls,
  a thread loading Magnet again), `fabricare install`, `fabricare clean`
  (see the `fabricare` skill; clear `NoDefaultCurrentDirectoryInExePath`
  on Windows).
- Code style: tabs (width 8), `.clang-format`, CRLF, statements and blocks
  end with `};`. SPDX: MIT for `source/` and `docs/`, Unlicense for
  `test/`, `.claude/`, `fabricare.json`, `.vscode/`, `.github/` (see
  `.reuse/dep5`; check with `python -m reuse lint`).

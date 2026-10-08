# C++ API

For hosts that embed Quantum Script and for maintainers of the extension.
Read the `quantum-script` repository's `docs/embedding.md` and
`docs/writing-extensions.md` first: the init callback, internal and external
extensions work the same way here.

## Headers and namespace

```cpp
#include <XYO/QuantumScript.Extension/Magnet.hpp>   // Library.hpp

using namespace XYO::QuantumScript;
```

Namespace: `XYO::QuantumScript::Extension::Magnet`. The header does not
include the headers of the bundled extensions; include
`<XYO/QuantumScript.Extension/JSON.hpp>`, ... yourself if the host calls
their C++ functions.

## Build defines

Export macro: `XYO_QUANTUMSCRIPT_EXTENSION_MAGNET_EXPORT`.

| Define | Effect |
|--------|--------|
| `XYO_QUANTUMSCRIPT_EXTENSION_MAGNET_INTERNAL` (or `QUANTUM_SCRIPT__MAGNET_INTERNAL`, set by fabricare while building the DLL) | export macro = `XYO_PLATFORM_LIBRARY_EXPORT` |
| none | export macro = `XYO_PLATFORM_LIBRARY_IMPORT` (consumers of the DLL) |
| `XYO_QUANTUMSCRIPT_EXTENSION_MAGNET_LIBRARY` | export macro empty, no `quantumScriptExtension` entry point (sources compiled into another library) |
| `XYO_PLATFORM_COMPILE_STATIC` (static platforms) | `XYO_PLATFORM_LIBRARY_EXPORT` / `IMPORT` are empty |

The `quantumScriptExtension` entry point is compiled only when
`XYO_PLATFORM_COMPILE_DYNAMIC_LIBRARY` is defined and
`XYO_QUANTUMSCRIPT_EXTENSION_MAGNET_LIBRARY` is not.

## Functions

```cpp
void Extension::Magnet::registerInternalExtension(Executive *executive);
void Extension::Magnet::initExecutive(Executive *executive, void *extensionId);

const char *Extension::Magnet::Version::version();            // "5.9.0"
const char *Extension::Magnet::Version::build();              // "8"
const char *Extension::Magnet::Version::versionWithBuild();   // "5.9.0.8"
const char *Extension::Magnet::Version::datetime();           // "2026-09-16 23:03:10"
const char *Extension::Magnet::Copyright::copyright();
const char *Extension::Magnet::Copyright::publisher();
const char *Extension::Magnet::Copyright::company();
const char *Extension::Magnet::Copyright::contact();
std::string Extension::Magnet::License::license();            // full MIT text
std::string Extension::Magnet::License::shortLicense();       // copyright + short MIT notice
```

- `registerInternalExtension` registers `"Magnet"` as an internal
  extension; call it from the host's init callback (see
  [Getting started](getting-started.md#4-register-it-in-a-c-host)). It does
  **not** register the bundled extensions.
- `initExecutive` is the extension's init function, run by the engine when a
  script first requires `Magnet` in a thread. It sets the extension name
  (`"Magnet"`), info (`"Magnet\r\n"` plus the short license text), version
  (`versionWithBuild()`) and public flag, then calls
  `Extension::<Name>::registerInternalExtension(executive)` for each of the
  31 bundled extensions. Do not call it directly.
- The DLL build also exports
  `extern "C" void quantumScriptExtension(Executive *, void *)`, which
  forwards to `initExecutive`; it is what `Script.requireExtension("Magnet")`
  looks up in `quantum-script--magnet.dll`.

## How registration works

The engine keeps two lists per thread:

- the **internal extension list**: name → init function, filled by
  `Executive::registerInternalExtension(name, proc)`; a second registration
  of the same name (compared without case) **replaces** the init function;
- the **loaded extension list**: what `Script.getExtensionList()` shows.

`Script.requireExtension(name)`:

1. returns at once if an extension with that name is loaded;
2. looks for `quantum-script--<lowercase name>.dll` / `.so` as given, then in
   each include path folder, and loads it if found;
3. otherwise runs the init function from the internal list;
4. otherwise throws `Unable to open "<name>"`.

`Script.requireInternalExtension(name)` does 1, 3 and 4 only.

Loading `Magnet` runs step 3 (or step 2 for the DLL) with
`Magnet::initExecutive`, which fills the internal list with the bundled
extensions. Consequences:

- Before `Magnet` is loaded, `Script.requireInternalExtension("JSON")`
  fails in a host that registered only Magnet.
- A host that registers its own extension under a bundled name (for example
  a custom `"Console"`) loses that registration when Magnet is loaded:
  Magnet's registration replaces it. An extension that is already
  *loaded* stays loaded (registration never unloads or reloads anything, and
  `requireExtension` returns at once for a loaded name), so load the host's
  version first:

  ```cpp
  void initExecutive(Executive *executive) {
  	executive->registerInternalExtension("Console", myConsoleInitExecutive);
  	Extension::Magnet::registerInternalExtension(executive);
  	executive->compileString("Script.requireInternalExtension(\"Console\");"
  	                         "Script.requireInternalExtension(\"Magnet\");");
  };
  ```

  Code passed to `compileString` in the init callback is compiled there and
  runs after the callback returns, in order, before any script.

Threads (`Thread`, `Job`, `Make`, `Task` with threads) create a new engine
and run the host's init callback again; the internal list starts with what
that callback registers.

## Adding an extension to the bundle

1. Add the dependency to `fabricare.json` (`"quantum-script--<name>"`).
2. In `Magnet/Library.cpp`, add
   `#include <XYO/QuantumScript.Extension/<Name>.hpp>` and
   `Extension::<Name>::registerInternalExtension(executive);` in
   `initExecutive` (alphabetical order, as the existing lines).
3. Add the extension to `test/test.01.js` (the `extensions` table: name and
   the objects it defines) and, if it is cheap, a smoke check.
4. Update `README.md`, `docs/extensions.md`, `docs/reference.md` and the skill
   in `.claude/skills/quantum-script--magnet/`.
5. `fabricare make`, `fabricare test`, `fabricare install`, then rebuild the
   `magnet` application (its README lists the extensions too).

Removing an extension is the reverse; scripts that require it then fail
with `Unable to open "<Name>"` in `magnet`.

## Notes for maintainers

- The extension has no script code and no state: everything is in
  `Library.cpp`. `Magnet.Amalgam.cpp` includes all `.cpp` files for single
  translation unit builds.
- `test/test.01.cpp` registers only `Magnet` (not even `Console`), so the
  test proves that every extension comes from Magnet. `test/test.01.js`
  checks that nothing is internal before Magnet is loaded, that every
  bundled extension can be loaded with `Script.requireInternalExtension` and
  defines its objects, that names like `Version` are not extensions, the
  `Magnet` entry of `Script.getExtensionList()`, a few smoke calls
  (Base16 / 32 / 64, JSON, MD5, SHA256, SHA512, Math, URL,
  ApplicationVersion, DateTime) and that a thread can load Magnet again. A
  failed check throws, so `fabricare test` fails.
- Version, copyright and license follow the XYO template
  (`Version.rh` is generated from `Version.Template.rh` by
  `fabricare version`).

# API reference

## Script

| Call | Result |
|------|--------|
| `Script.requireExtension("Magnet")` | loads Magnet (DLL first, then internal); registers the bundled extensions as internal; nothing when already loaded |
| `Script.requireInternalExtension("Magnet")` | the same, never loads a DLL |
| `Script.requireExtension(name)` after Magnet | loads a bundled extension; an external `quantum-script--<name>` library on the include path is used first |
| `Script.requireInternalExtension(name)` after Magnet | loads the bundled extension, never a DLL |
| `Script.getExtensionList()` | array of `{name, version, info, fileName}` for loaded extensions; may contain holes (test with `Script.isNil`) |

`Magnet` defines no script objects or functions.

### Bundled extension names

`Application`, `ApplicationVersion`, `Base16`, `Base32`, `Base64`, `Buffer`,
`Console`, `Crypt`, `CSV`, `DateTime`, `File`, `HTTP`, `Job`, `JSON`,
`Make`, `Math`, `MD5`, `OpenSSL`, `Pixel32`, `ProcessInteractive`,
`Random`, `SHA256`, `SHA512`, `Shell`, `ShellFind`, `Socket`, `SSHRemote`,
`Task`, `Thread`, `URL`, `XML`.

Objects each one defines and the extensions each one loads:
[Extensions](extensions.md#catalog).

### `getExtensionList()` entry for Magnet

| Property | Value |
|----------|-------|
| `name` | `"Magnet"` (the name used in `requireExtension`) |
| `version` | `"5.9.0.8"` (`version.build`) |
| `info` | `"Magnet\r\n"` + copyright line + `"\r\n"` + short MIT notice |
| `fileName` | `""` when internal, else the path of `quantum-script--magnet.dll` / `.so` |

## Errors

| Situation | What happens |
|-----------|--------------|
| `requireExtension("Magnet")` in a host without Magnet and without the DLL on the include path | throws `Unable to open "Magnet"` |
| `requireExtension("Magnet")` in the static `quantum-script` | throws `Unable to open "Magnet"` (no DLL loading) |
| `requireInternalExtension("JSON")` before Magnet is loaded (host registered only Magnet) | throws `Unable to open "JSON"` |
| `requireExtension("Version")` | throws `Unable to open "Version"`: the name is `ApplicationVersion` |
| `requireExtension("SCard")` / `("Example")` in `magnet` | throws: not bundled, and `magnet` never loads DLLs |
| A thread function uses a bundled extension without loading Magnet in that thread (host registers only Magnet) | throws in the thread; `getReturnedValue()` is `undefined` |
| `quantum-script--magnet.dll` loaded but a bundled extension DLL missing next to it | the Magnet DLL fails to load: `Unable to open "Magnet"` |

## The `magnet` application

| Behaviour | Value |
|-----------|-------|
| Preloaded | `Magnet` (in every thread) |
| `Script.requireExtension` | replaced by `Script.requireInternalExtension`: bundled extensions only |
| Options | `script.js`, `--run "code"`, `--cmd script` (skip 2 lines), `--execution-time`, `--execution-time-cmd`, `--license` |
| Uncaught exception | `Error: <message>` and stack trace on stdout, exit code 1 |
| `Script.exit(n)` / `setExitCode(n)` | exit code is 0 in `magnet` 5.9.0 (not `n`) |

## C++

```cpp
#include <XYO/QuantumScript.Extension/Magnet.hpp>
```

| Symbol | Purpose |
|--------|---------|
| `Extension::Magnet::registerInternalExtension(Executive *)` | register `"Magnet"` as internal extension (host init callback) |
| `Extension::Magnet::initExecutive(Executive *, void *extensionId)` | extension init: name, info, version, public, registers the bundled extensions |
| `extern "C" quantumScriptExtension(Executive *, void *)` | DLL entry point (dynamic builds without `XYO_QUANTUMSCRIPT_EXTENSION_MAGNET_LIBRARY`) |
| `Extension::Magnet::Version::version()` / `build()` / `versionWithBuild()` / `datetime()` | `"5.9.0"`, `"8"`, `"5.9.0.8"`, `"2026-09-16 23:03:10"` |
| `Extension::Magnet::Copyright::copyright()` / `publisher()` / `company()` / `contact()` | metadata strings |
| `Extension::Magnet::License::license()` / `shortLicense()` | MIT license text, short notice |
| `XYO_QUANTUMSCRIPT_EXTENSION_MAGNET_EXPORT` | export macro |
| `XYO_QUANTUMSCRIPT_EXTENSION_MAGNET_INTERNAL` / `QUANTUM_SCRIPT__MAGNET_INTERNAL` | building the DLL: export |
| `XYO_QUANTUMSCRIPT_EXTENSION_MAGNET_LIBRARY` | compiled into another library: no export, no DLL entry point |

## fabricare

| Item | Value |
|------|-------|
| Project | `quantum-script--magnet`, `dll-or-lib`, MIT |
| Dependency name | `"quantum-script--magnet"` |
| Pulls in | `quantum-script`, `quantum-script--console` and the 30 other bundled extensions with their libraries |
| Library file | `quantum-script--magnet.dll` (Windows), `quantum-script--magnet.so` (Linux), `quantum-script--magnet.lib` (static) |
| Test | `test.01` (`fabricare test`, after `fabricare make`) |

Version numbers in this page are those of release 5.9.0 build 8; the
library's `Version::` functions and `getExtensionList()` give the current
ones.

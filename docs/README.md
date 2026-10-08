# Quantum Script Extension Magnet — Documentation

`quantum-script--magnet` is the **all-in-one extension of Quantum Script**.
It has no script functions of its own: loaded with
`Script.requireExtension("Magnet")`, it registers the standard Quantum Script
extensions as *internal* extensions of the running engine, so a script can
then load any of them by name:

```javascript
Script.requireExtension("Magnet");      // registers Application, ..., XML

Script.requireExtension("Shell");       // now available without quantum-script--shell.dll
Script.requireExtension("JSON");
var info = JSON.decode(Shell.fileGetContents("version.json"));
```

Quantum Script keeps its core small: files, processes, JSON, math, threads
and so on live in separate extensions (`quantum-script--shell`,
`quantum-script--json`, ...), each one a library of its own. Magnet links all
of them into one library and makes them available with one call.

- **One dependency instead of thirty.** A C++ host depends on
  `quantum-script--magnet` and registers one extension; a script loads
  `Magnet` once.
- **No search for libraries.** Bundled extensions are internal: they do not
  depend on `quantum-script--<name>.dll` being on the include path.
- **The `magnet` application.** `magnet` is Quantum Script with this
  extension preloaded: every bundled extension is available to scripts
  through the usual `Script.requireExtension`.
- **Same scripts everywhere.** Scripts written for `magnet` also run in the
  `quantum-script` interpreter after `Script.requireExtension("Magnet")`, and
  in any host that registers Magnet.

```
scripts: magnet, quantum-script + Magnet, your C++ host, ...
quantum-script--magnet        <-- this extension: registers the others as internal
quantum-script--application, --applicationversion, --base16, --base32, --base64,
--buffer, --console, --crypt, --csv, --datetime, --file, --http, --job, --json,
--make, --math, --md5, --openssl, --pixel32, --processinteractive, --random,
--sha256, --sha512, --shell, --shellfind, --socket, --sshremote, --task,
--thread, --url, --xml        (the bundled extensions)
quantum-script                (Executive, internal extension list, requireExtension)
xyo-system, xyo-cryptography, xyo-networking, xyo-pixel32, file-xml, ...
```

## Why it exists

| Need | What `Magnet` gives |
|------|---------------------|
| A Quantum Script interpreter with batteries included | the `magnet` application is built on it |
| Ship one tool without thirty extension DLLs next to it | one library (or a static link) holds them all |
| Give an embedded engine the full standard set | `Extension::Magnet::registerInternalExtension(executive)` |
| Scripts that must not pick up other extension DLLs | bundled extensions are internal; `Script.requireInternalExtension` uses only them |
| Know which set of extensions a script runs against | `Script.getExtensionList()` reports `Magnet` with its version |

## Concepts at a glance

| Need | Use | Notes |
|------|-----|-------|
| Load the bundle | `Script.requireExtension("Magnet");` | already done by the `magnet` application |
| Load one bundled extension | `Script.requireExtension("JSON");` | after `Magnet`; with the DLL engine an external `quantum-script--json.dll` on the include path is used first |
| Force the bundled one | `Script.requireInternalExtension("JSON");` | never loads a DLL |
| List what is loaded | `Script.getExtensionList()` | `name`, `version`, `info`, `fileName` (`""` for internal) |
| Use it from C++ | `Extension::Magnet::registerInternalExtension(executive)` in the init callback | scripts still require `Magnet` |
| Threads | `Script.requireExtension("Magnet")` again in the thread | each thread has its own engine |

Magnet registers extensions, it does not load them: scripts still call
`Script.requireExtension` for every extension they use, exactly as with the
separate libraries. The API of each extension is documented in its own
repository; see [Extensions](extensions.md).

## Contents

| Document | What it covers |
|----------|----------------|
| [Getting started](getting-started.md) | Build and install, the `magnet` application, loading Magnet from `quantum-script`, registering it in a C++ host, threads, static builds |
| [Extensions](extensions.md) | Every bundled extension: name, objects it defines, what it is for, where its documentation is |
| [C++ API](cpp-api.md) | `registerInternalExtension`, `initExecutive`, the DLL entry point, build defines, how registration works, adding an extension to the bundle |
| [API reference](reference.md) | Every script and C++ symbol and the edge cases on one page |

Quantum Script itself (the language, `Script.requireExtension`, embedding,
writing extensions) is documented in the `quantum-script` repository,
`docs/`.

## Source map

```
source/XYO/QuantumScript.Extension/Magnet.hpp            umbrella header, include this from C++
source/XYO/QuantumScript.Extension/Magnet.Amalgam.cpp    the whole extension in one translation unit
source/XYO/QuantumScript.Extension/Magnet/
    Dependency.hpp                                       <XYO/QuantumScript.hpp>, export macro
    Library[.hpp/.cpp]                                   initExecutive (registers the bundled extensions),
                                                         registerInternalExtension, DLL entry point
    Copyright / License / Version                        library metadata
    Library.rc, *.rh                                     Windows version resource
fabricare.json                                           the library (dll-or-lib) and its 32 dependencies, test.01
test/test.01.cpp                                         C++ host registering only Magnet
test/test.01.js                                          registration, objects, extension list, smoke tests, threads
```

## AI assistant skill

A Claude Code skill describing how to use this extension lives in
[`.claude/skills/quantum-script--magnet/`](../.claude/skills/quantum-script--magnet/SKILL.md).
It is picked up automatically inside this repository; copy the folder to
`~/.claude/skills/` to have it available in the projects that use Magnet
(`magnet` scripts, C++ hosts that embed Quantum Script).

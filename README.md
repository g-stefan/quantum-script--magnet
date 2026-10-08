# Quantum Script Extension Magnet

Quantum Script extension
- One extension that brings all the others: `Script.requireExtension("Magnet")`
registers the 31 standard Quantum Script extensions (files, shell, processes,
JSON, XML, HTTP / HTTPS, threads, hashes, images, ...) as internal extensions
of the running engine.
- After that every `Script.requireExtension("JSON")`, `("Shell")`, ... works
without any other library on the include path.
- The engine of the `magnet` application (Quantum Script with everything
included); also usable from the `quantum-script` interpreter and from any
C++ host with a single `registerInternalExtension` call.

```javascript
Script.requireExtension("Magnet");

Script.requireExtension("Console");
Script.requireExtension("JSON");
Console.writeLn(JSON.encode({extension: "Magnet"}));
```

## Quantum Script Extensions internal registered:

```javascript
Magnet
Application
ApplicationVersion
Base16
Base32
Base64
Buffer
Console
Crypt
CSV
DateTime
File
HTTP
Job
JSON
Make
Math
MD5
OpenSSL
Pixel32
ProcessInteractive
Random
SHA256
SHA512
Shell
ShellFind
Socket
SSHRemote
Task
Thread
URL
XML
```

## Documentation

- [Overview](docs/README.md) - purpose and design
- [Getting started](docs/getting-started.md) - build, use from magnet, quantum-script and a C++ host, threads
- [Extensions](docs/extensions.md) - the bundled extensions, the objects each one defines, where their documentation is
- [C++ API](docs/cpp-api.md) - registration, DLL entry point, build defines, adding an extension to the bundle
- [API reference](docs/reference.md)

A Claude Code skill for this extension is in
[.claude/skills/quantum-script--magnet](.claude/skills/quantum-script--magnet/SKILL.md).

## License

Copyright (c) 2020-2026 Grigore Stefan
Licensed under the [MIT](LICENSE) license.

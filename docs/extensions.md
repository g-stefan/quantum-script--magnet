# Extensions

`Script.requireExtension("Magnet")` registers the following 31 extensions as
internal extensions of the engine. Each one is a separate repository
(`quantum-script--<lowercase name>`) with its own `README.md` and `docs/`;
their API is not repeated here.

A script still loads every extension it uses:

```javascript
Script.requireExtension("Magnet");
Script.requireExtension("Shell");
Script.requireExtension("XML");
```

Extension names are not case sensitive (`"json"` is `"JSON"`). Loading an
extension that is already loaded does nothing.

## Catalog

| Extension | Objects it defines | What it is for | Also loads |
|-----------|--------------------|----------------|------------|
| `Application` | `Application` | command line of the running program: `getCmdN`, `getCmdS`, `hasFlag`, `getFlagValue`, `getArgument`, path of the executable | |
| `ApplicationVersion` | `ApplicationVersion`, `VersionCompare` | compare version strings `major.minor.patch.build` part by part | |
| `Base16` | `Base16` | hexadecimal encode / decode of strings and buffers | `Buffer` |
| `Base32` | `Base32` | Base32 (RFC 4648) encode / decode | `Buffer` |
| `Base64` | `Base64` | Base64 (RFC 4648) encode / decode | `Buffer` |
| `Buffer` | `Buffer` | binary data: a block of bytes, byte access, hex and string conversion | |
| `Console` | `Console` | `write`, `writeLn`, `readLn` on the console | |
| `Crypt` | `Crypt` | symmetric, signed encryption of strings, buffers and files | `Buffer` |
| `CSV` | `CSV` | decode / encode one CSV line | |
| `DateTime` | `DateTime` | current local date and time, Unix time, millisecond timestamps | |
| `File` | `File` | streaming files and `stdin` / `stdout` / `stderr`, text and binary, 64-bit seek | `Buffer` |
| `HTTP` | `HTTP` | plain `http://` client: JSON requests, posts, downloads | `File`, `Socket`, `URL`, `JSON` (`Shell` when downloading) |
| `Job` | `Job` | run programs and script functions in parallel, a few at a time | `Thread`, `Shell`, `Math` |
| `JSON` | `JSON` | `JSON.decode`, `JSON.encode`, `JSON.encodeWithIndentation` | |
| `Make` | `Make`, `MakeError` | make-style incremental builds with parallel recipes | `Thread`, `Shell` |
| `Math` | `Math` | the JavaScript `Math` object | |
| `MD5` | `MD5` | MD5 digests (checksums, not security) | `Buffer` |
| `OpenSSL` | `OpenSSL`, `HTTPS` | `https://` client, TLS connections, RSA encryption (adds `Crypt.publicEncrypt`, `Crypt.privateDecrypt`, ...) | `Socket`, `File`, `URL`, `JSON`, `Buffer`, `Crypt` (`Shell` when downloading) |
| `Pixel32` | `Pixel32` | RGBA images: PNG load / save, resize, crop, blend, filters, noise | `Random` |
| `ProcessInteractive` | `ProcessInteractive` | run a program with pipes to its input and output | `Buffer` |
| `Random` | `Random` | seedable Mersenne Twister generator (not for security) | |
| `SHA256` | `SHA256` | SHA-256 of strings, buffers and files | `Buffer` |
| `SHA512` | `SHA512` | SHA-512 of strings, buffers and files | `Buffer` |
| `Shell` | `Shell` | files, directories, paths, environment, processes | `Buffer`, `ShellFind` |
| `ShellFind` | `ShellFind` | directory iterator with wildcards | |
| `Socket` | `Socket` | TCP clients and servers, IPv4 and IPv6 | `Buffer` |
| `SSHRemote` | `SSHRemote` | run commands and copy files over SSH with PuTTY `plink` / `pscp` | `SHA512`, `Shell`, `URL`, `Random`, `DateTime` |
| `Task` | `Task`, `TaskQueue` | timers (`setTimeout`, `setInterval`) and polled tasks with an event loop | `Thread`, `DateTime` |
| `Thread` | `Thread`, `CurrentThread`, `Atomic`, `Processor` | run script functions on other threads | |
| `URL` | `URL` | percent-encoding, split a URL into its parts | |
| `XML` | `XML`, `XMLDocument`, `XMLNode`, `XMLAttribute`, `XMLAttributes`, `XMLNodeType`, `XMLParserMode` | load, query, change and write XML documents | |

"Also loads" lists the extensions an extension requires itself when it is
loaded; they become loaded (and their objects defined) too. All of them are
part of the bundle, so this works in any host that loaded Magnet.

`Magnet` itself defines no object. `Example` (the template extension) and
`SCard` (smart cards, Windows) are Quantum Script extensions that are **not**
in the bundle; there is no extension named `Version` (the version compare
extension is `ApplicationVersion`).

## Where the bundled code comes from

Magnet does not copy the extensions' code: it calls each extension's
`registerInternalExtension` and links to its library.

- **Dynamic platforms** (`win64-msvc-2026`, `ubuntu-*`): the code stays in
  `quantum-script--json.dll`, `quantum-script--shell.dll`, ...;
  `quantum-script--magnet.dll` imports them, so they must be next to it (the
  SDK `bin` folder, or the folder of the application that ships Magnet).
  "Internal" here means *found without searching the include path*.
- **Static platforms** (`win64-msvc-2026.static`): the static libraries of
  all extensions are linked into the executable; nothing is loaded at run
  time.

The version of each loaded extension is the version of that extension's
library, reported by `Script.getExtensionList()`:

```javascript
Script.requireExtension("Magnet");
Script.requireExtension("Console");
Script.requireExtension("JSON");

var list = Script.getExtensionList();
for (var i = 0; i < list.length; ++i) {
	if (Script.isNil(list[i])) {           // the list can have holes
		continue;
	};
	Console.writeLn(list[i].name + " " + list[i].version + " [" + list[i].fileName + "]");
};
// JSON 6.0.0.7 []
// Console 7.0.0.19 []
// Magnet 5.9.0.8 []
```

`fileName` is empty for internal extensions and the library path for one
loaded from a DLL. Iterate with a `Script.isNil` check: the list is built
by index and can contain holes.

## Extension documentation

| Extension | Repository |
|-----------|------------|
| `Console` | `quantum-script` (`docs/standard-library.md`) |
| every other extension | `quantum-script--<lowercase name>`: `README.md`, `docs/` and the skill in `.claude/skills/quantum-script--<lowercase name>/` |

On this machine the repositories are next to this one
(`X:\Storage\XYO\Gitea\CPP\quantum-script--json`, ...); on GitHub they are
`https://github.com/g-stefan/quantum-script--<lowercase name>`.

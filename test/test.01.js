// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2020-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

// The host (test.01.cpp) registers only "Magnet" as internal extension.

function check(name, value, expected) {
	if (value !== expected) {
		throw "test " + name + " failed: [" + value + "], expected [" + expected + "]";
	};
};

function hasInternal(name) {
	try {
		Script.requireInternalExtension(name);
		return true;
	} catch (e) {
	};
	return false;
};

// --- Before Magnet is loaded nothing else is registered

check("01", hasInternal("JSON"), false);
check("02", hasInternal("Console"), false);

// --- Load Magnet

Script.requireExtension("Magnet");
Script.requireExtension("Magnet"); // loading twice does nothing
Script.requireExtension("Console");

// --- Every bundled extension is registered as internal and defines its objects

var extensions = [
	["Application", ["Application"]],
	["ApplicationVersion", ["ApplicationVersion", "VersionCompare"]],
	["Base16", ["Base16"]],
	["Base32", ["Base32"]],
	["Base64", ["Base64"]],
	["Buffer", ["Buffer"]],
	["Console", ["Console"]],
	["Crypt", ["Crypt"]],
	["CSV", ["CSV"]],
	["DateTime", ["DateTime"]],
	["File", ["File"]],
	["HTTP", ["HTTP"]],
	["Job", ["Job"]],
	["JSON", ["JSON"]],
	["Make", ["Make"]],
	["Math", ["Math"]],
	["MD5", ["MD5"]],
	["OpenSSL", ["OpenSSL", "HTTPS"]],
	["Pixel32", ["Pixel32"]],
	["ProcessInteractive", ["ProcessInteractive"]],
	["Random", ["Random"]],
	["SHA256", ["SHA256"]],
	["SHA512", ["SHA512"]],
	["Shell", ["Shell"]],
	["ShellFind", ["ShellFind"]],
	["Socket", ["Socket"]],
	["SSHRemote", ["SSHRemote"]],
	["Task", ["Task", "TaskQueue"]],
	["Thread", ["Thread", "CurrentThread"]],
	["URL", ["URL"]],
	["XML", ["XML", "XMLDocument"]]
];

var i, j;
for (i = 0; i < extensions.length; ++i) {
	check("03 " + extensions[i][0], hasInternal(extensions[i][0]), true);
	for (j = 0; j < extensions[i][1].length; ++j) {
		check("04 " + extensions[i][1][j], Script.isNil(global[extensions[i][1][j]]), false);
	};
};

// Names that are not extensions
check("05", hasInternal("Version"), false);
check("06", hasInternal("Example"), false);

// Extension names are not case sensitive
check("07", hasInternal("json"), true);

// --- Extension list

var list = Script.getExtensionList();
var magnet;
for (i = 0; i < list.length; ++i) {
	if (Script.isNil(list[i])) {
		continue;
	};
	if (list[i].name == "Magnet") {
		magnet = list[i];
	};
};
check("08", Script.isNil(magnet), false);
check("09", magnet.version.length > 0, true);
check("10", magnet.info.indexOf("Magnet") === 0, true);
check("11", magnet.info.indexOf("MIT") > 0, true);

// --- The extensions work

check("12", Base16.encode("ab"), "6162");
check("13", Base32.decode(Base32.encode("magnet")), "magnet");
check("14", Base64.encode("magnet"), "bWFnbmV0");
check("15", JSON.encode(JSON.decode("{\"a\":[1,true,null]}")), "{\"a\":[1,true,null]}");
check("16", MD5.hash(""), "d41d8cd98f00b204e9800998ecf8427e");
check("17", SHA256.hash("abc"), "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");
check("18", SHA512.hash("").length, 128);
check("19", Math.sqrt(16), 4);
check("20", URL.decodeComponent(URL.encodeComponent("a b/c")), "a b/c");
check("21", ApplicationVersion.compare("1.2.0", "1.10.0") < 0, true);
check("22", DateTime.timestampInMilliseconds() > 0, true);

// --- A thread has its own engine: the host registers Magnet again, the script loads it again

var thread = Thread.newThread(function(text) {
	Script.requireExtension("Magnet");
	Script.requireExtension("Base16");
	return Base16.encode(text);
}, null, ["ab"]);
thread.join();
check("23", thread.getReturnedValue(), "6162");

Console.writeLn("-> test 01 ok");

// Quantum Script Extension Magnet
// Copyright (c) 2020-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2020-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/QuantumScript.Extension/Magnet/Library.hpp>
#include <XYO/QuantumScript.Extension/Magnet/Copyright.hpp>
#include <XYO/QuantumScript.Extension/Magnet/License.hpp>
#include <XYO/QuantumScript.Extension/Magnet/Version.hpp>

#include <XYO/QuantumScript.Extension/Application.hpp>
#include <XYO/QuantumScript.Extension/Buffer.hpp>
#include <XYO/QuantumScript.Extension/Base16.hpp>
#include <XYO/QuantumScript.Extension/Base32.hpp>
#include <XYO/QuantumScript.Extension/Base64.hpp>
#include <XYO/QuantumScript.Extension/Console.hpp>
#include <XYO/QuantumScript.Extension/Crypt.hpp>
#include <XYO/QuantumScript.Extension/CSV.hpp>
#include <XYO/QuantumScript.Extension/DateTime.hpp>
#include <XYO/QuantumScript.Extension/File.hpp>
#include <XYO/QuantumScript.Extension/HTTP.hpp>
#include <XYO/QuantumScript.Extension/Job.hpp>
#include <XYO/QuantumScript.Extension/JSON.hpp>
#include <XYO/QuantumScript.Extension/Make.hpp>
#include <XYO/QuantumScript.Extension/Math.hpp>
#include <XYO/QuantumScript.Extension/MD5.hpp>
#include <XYO/QuantumScript.Extension/ProcessInteractive.hpp>
#include <XYO/QuantumScript.Extension/Random.hpp>
#include <XYO/QuantumScript.Extension/SHA256.hpp>
#include <XYO/QuantumScript.Extension/SHA512.hpp>
#include <XYO/QuantumScript.Extension/Shell.hpp>
#include <XYO/QuantumScript.Extension/ShellFind.hpp>
#include <XYO/QuantumScript.Extension/Socket.hpp>
#include <XYO/QuantumScript.Extension/Task.hpp>
#include <XYO/QuantumScript.Extension/Thread.hpp>
#include <XYO/QuantumScript.Extension/URL.hpp>
#include <XYO/QuantumScript.Extension/ApplicationVersion.hpp>
#include <XYO/QuantumScript.Extension/XML.hpp>
#include <XYO/QuantumScript.Extension/OpenSSL.hpp>
#include <XYO/QuantumScript.Extension/Pixel32.hpp>
#include <XYO/QuantumScript.Extension/SSHRemote.hpp>

namespace XYO::QuantumScript::Extension::Magnet {

	void registerInternalExtension(Executive *executive) {
		executive->registerInternalExtension("Magnet", initExecutive);
	};

	void initExecutive(Executive *executive, void *extensionId) {
		String info = "Magnet\r\n";
		info << License::shortLicense().c_str();

		executive->setExtensionName(extensionId, "Magnet");
		executive->setExtensionInfo(extensionId, info);
		executive->setExtensionVersion(extensionId, Extension::Magnet::Version::versionWithBuild());
		executive->setExtensionPublic(extensionId, true);

		Extension::Application::registerInternalExtension(executive);
		Extension::Base16::registerInternalExtension(executive);
		Extension::Base32::registerInternalExtension(executive);
		Extension::Base64::registerInternalExtension(executive);
		Extension::Buffer::registerInternalExtension(executive);
		Extension::Console::registerInternalExtension(executive);
		Extension::Crypt::registerInternalExtension(executive);
		Extension::CSV::registerInternalExtension(executive);
		Extension::DateTime::registerInternalExtension(executive);
		Extension::File::registerInternalExtension(executive);
		Extension::HTTP::registerInternalExtension(executive);
		Extension::Job::registerInternalExtension(executive);
		Extension::JSON::registerInternalExtension(executive);
		Extension::Make::registerInternalExtension(executive);
		Extension::Math::registerInternalExtension(executive);
		Extension::MD5::registerInternalExtension(executive);
		Extension::OpenSSL::registerInternalExtension(executive);
		Extension::Pixel32::registerInternalExtension(executive);
		Extension::ProcessInteractive::registerInternalExtension(executive);
		Extension::Random::registerInternalExtension(executive);
		Extension::SHA256::registerInternalExtension(executive);
		Extension::SHA512::registerInternalExtension(executive);
		Extension::Shell::registerInternalExtension(executive);
		Extension::ShellFind::registerInternalExtension(executive);
		Extension::Socket::registerInternalExtension(executive);
		Extension::SSHRemote::registerInternalExtension(executive);
		Extension::Task::registerInternalExtension(executive);
		Extension::Thread::registerInternalExtension(executive);
		Extension::URL::registerInternalExtension(executive);
		Extension::ApplicationVersion::registerInternalExtension(executive);
		Extension::XML::registerInternalExtension(executive);
	};

};

#ifdef XYO_COMPILE_DYNAMIC_LIBRARY
extern "C" XYO_QUANTUMSCRIPT_EXTENSION_MAGNET_EXPORT void quantumScriptExtension(XYO::QuantumScript::Executive *executive, void *extensionId) {
	XYO::QuantumScript::Extension::Magnet::initExecutive(executive, extensionId);
};
#endif

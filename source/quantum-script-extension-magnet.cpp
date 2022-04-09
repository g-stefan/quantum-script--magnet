//
// Quantum Script Extension Magnet
//
// Copyright (c) 2020-2022 Grigore Stefan <g_stefan@yahoo.com>
// Created by Grigore Stefan <g_stefan@yahoo.com>
//
// MIT License (MIT) <http://opensource.org/licenses/MIT>
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "quantum-script-extension-magnet-license.hpp"
#include "quantum-script-extension-magnet.hpp"
#ifndef QUANTUM_SCRIPT_EXTENSION_MAGNET_NO_VERSION
#	include "quantum-script-extension-magnet-version.hpp"
#endif

#include "quantum-script-variablenumber.hpp"
#include "quantum-script-variablestring.hpp"

#include "quantum-script-extension-application.hpp"
#include "quantum-script-extension-buffer.hpp"
#include "quantum-script-extension-base16.hpp"
#include "quantum-script-extension-base32.hpp"
#include "quantum-script-extension-base64.hpp"
#include "quantum-script-extension-console.hpp"
#include "quantum-script-extension-crypt.hpp"
#include "quantum-script-extension-csv.hpp"
#include "quantum-script-extension-datetime.hpp"
#include "quantum-script-extension-file.hpp"
#include "quantum-script-extension-http.hpp"
#include "quantum-script-extension-job.hpp"
#include "quantum-script-extension-json.hpp"
#include "quantum-script-extension-make.hpp"
#include "quantum-script-extension-math.hpp"
#include "quantum-script-extension-md5.hpp"
#include "quantum-script-extension-processinteractive.hpp"
#include "quantum-script-extension-random.hpp"
#include "quantum-script-extension-sha256.hpp"
#include "quantum-script-extension-sha512.hpp"
#include "quantum-script-extension-shell.hpp"
#include "quantum-script-extension-shellfind.hpp"
#include "quantum-script-extension-socket.hpp"
#include "quantum-script-extension-task.hpp"
#include "quantum-script-extension-thread.hpp"
#include "quantum-script-extension-url.hpp"
#include "quantum-script-extension-version.hpp"
#include "quantum-script-extension-xml.hpp"
#include "quantum-script-extension-openssl.hpp"
#include "quantum-script-extension-pixel32.hpp"
#include "quantum-script-extension-sshremote.hpp"

//#define QUANTUM_SCRIPT_VM_DEBUG_RUNTIME

namespace Quantum {
	namespace Script {
		namespace Extension {
			namespace Magnet {

				using namespace XYO;
				using namespace Quantum::Script;

				void registerInternalExtension(Executive *executive) {
					executive->registerInternalExtension("Magnet", initExecutive);
				};

				void initExecutive(Executive *executive, void *extensionId) {

					String info = "Magnet\r\n";
					info << License::shortContent();

					executive->setExtensionName(extensionId, "Magnet");
					executive->setExtensionInfo(extensionId, info);
#ifndef QUANTUM_SCRIPT_EXTENSION_MAGNET_NO_VERSION
					executive->setExtensionVersion(extensionId, Extension::Magnet::Version::versionWithBuild());
#endif
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
					Extension::Version::registerInternalExtension(executive);
					Extension::XML::registerInternalExtension(executive);

					//
					executive->compileString("Script.requireExtension=Script.requireInternalExtension;");
					//
				};

			};
		};
	};
};

#ifdef XYO_COMPILE_DYNAMIC_LIBRARY
extern "C" QUANTUM_SCRIPT_EXTENSION_MAGNET_EXPORT void quantumScriptExtension(Quantum::Script::Executive *executive, void *extensionId) {
	Quantum::Script::Extension::Magnet::initExecutive(executive, extensionId);
};
#endif

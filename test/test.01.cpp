// Created by Grigore Stefan <g_stefan@yahoo.com>
// Public domain (Unlicense) <http://unlicense.org>
// SPDX-FileCopyrightText: 2020-2026 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: Unlicense

#include <XYO/QuantumScript.hpp>
#include <XYO/QuantumScript.Extension/Magnet.hpp>

using namespace XYO::QuantumScript;

// Only Magnet is registered by the host, every other extension must come from it
void initExecutive(Executive *executive) {
	Extension::Magnet::registerInternalExtension(executive);
};

void test(int cmdN, char *cmdS[]) {

	const char *codeFile = "../../test/test.01.js";

	if (ExecutiveX::initExecutive(cmdN, cmdS, initExecutive)) {
		ExecutiveX::includePath(Shell::getFilePath(codeFile));
		if (ExecutiveX::executeFile(codeFile)) {
			int exitCode = ExecutiveX::getExitCode();
			ExecutiveX::endProcessing();
			if (exitCode != 0) {
				throw std::runtime_error("Exit code");
			};
			return;
		};
		printf("%s\n", (ExecutiveX::getError()).value());
		printf("%s", (ExecutiveX::getStackTrace()).value());
		ExecutiveX::endProcessing();

		throw std::runtime_error("Code");
	};

	throw std::runtime_error("Init");
};

int main(int cmdN, char *cmdS[]) {
	try {

		test(cmdN, cmdS);

		return 0;

	} catch (const std::exception &e) {
		printf("* Error: %s\n", e.what());
	} catch (...) {
		printf("* Error: Unknown\n");
	};

	return 1;
};

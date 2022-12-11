// Quantum Script Extension Magnet
// Copyright (c) 2022 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2022 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#include <XYO/QuantumScript.Extension/Magnet/Copyright.hpp>
#include <XYO/QuantumScript.Extension/Magnet/Copyright.rh>

namespace XYO::QuantumScript::Extension::Magnet::Copyright {

	static const char *copyright_ = XYO_QUANTUMSCRIPT_EXTENSION_MAGNET_COPYRIGHT;
	static const char *publisher_ = XYO_QUANTUMSCRIPT_EXTENSION_MAGNET_PUBLISHER;
	static const char *company_ = XYO_QUANTUMSCRIPT_EXTENSION_MAGNET_COMPANY;
	static const char *contact_ = XYO_QUANTUMSCRIPT_EXTENSION_MAGNET_CONTACT;

	const char *copyright() {
		return copyright_;
	};

	const char *publisher() {
		return publisher_;
	};

	const char *company() {
		return company_;
	};

	const char *contact() {
		return contact_;
	};

};

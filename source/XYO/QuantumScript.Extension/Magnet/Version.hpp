// Quantum Script Extension Magnet
// Copyright (c) 2020-2024 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2020-2024 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_MAGNET_VERSION_HPP
#define XYO_QUANTUMSCRIPT_EXTENSION_MAGNET_VERSION_HPP

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_MAGNET_DEPENDENCY_HPP
#	include <XYO/QuantumScript.Extension/Magnet/Dependency.hpp>
#endif

namespace XYO::QuantumScript::Extension::Magnet::Version {

	XYO_QUANTUMSCRIPT_EXTENSION_MAGNET_EXPORT const char *version();
	XYO_QUANTUMSCRIPT_EXTENSION_MAGNET_EXPORT const char *build();
	XYO_QUANTUMSCRIPT_EXTENSION_MAGNET_EXPORT const char *versionWithBuild();
	XYO_QUANTUMSCRIPT_EXTENSION_MAGNET_EXPORT const char *datetime();

};

#endif

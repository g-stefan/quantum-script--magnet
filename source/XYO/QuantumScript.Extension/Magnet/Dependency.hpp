// Quantum Script
// Copyright (c) 2020-2023 Grigore Stefan <g_stefan@yahoo.com>
// MIT License (MIT) <http://opensource.org/licenses/MIT>
// SPDX-FileCopyrightText: 2020-2023 Grigore Stefan <g_stefan@yahoo.com>
// SPDX-License-Identifier: MIT

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_MAGNET_DEPENDENCY_HPP
#define XYO_QUANTUMSCRIPT_EXTENSION_MAGNET_DEPENDENCY_HPP

#ifndef XYO_QUANTUMSCRIPT_HPP
#	include <XYO/QuantumScript.hpp>
#endif

// -- Export

#ifndef XYO_QUANTUMSCRIPT_EXTENSION_MAGNET_INTERNAL
#	ifdef QUANTUM_SCRIPT__MAGNET_INTERNAL
#		define XYO_QUANTUMSCRIPT_EXTENSION_MAGNET_INTERNAL
#	endif
#endif

#ifdef XYO_QUANTUMSCRIPT_EXTENSION_MAGNET_INTERNAL
#	define XYO_QUANTUMSCRIPT_EXTENSION_MAGNET_EXPORT XYO_LIBRARY_EXPORT
#else
#	define XYO_QUANTUMSCRIPT_EXTENSION_MAGNET_EXPORT XYO_LIBRARY_IMPORT
#endif

#endif

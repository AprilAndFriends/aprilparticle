/// @file
/// @version 2.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines macros for DLL exports/imports.

#ifndef APRILUIPARTICLE_EXPORT_H
#define APRILUIPARTICLE_EXPORT_H

	/// @def apriluiparticleExport
	/// @brief Macro for DLL exports/imports.
	/// @def apriluiparticleFnExport
	/// @brief Macro for function DLL exports/imports.
	#ifdef _LIB
		#define apriluiparticleExport
		#define apriluiparticleFnExport
	#else
		#ifdef _WIN32
			#ifdef APRILUIPARTICLE_EXPORTS
				#define apriluiparticleExport __declspec(dllexport)
				#define apriluiparticleFnExport __declspec(dllexport)
			#else
				#define apriluiparticleExport __declspec(dllimport)
				#define apriluiparticleFnExport __declspec(dllimport)
			#endif
		#else
			#define apriluiparticleExport __attribute__ ((visibility("default")))
			#define apriluiparticleFnExport __attribute__ ((visibility("default")))
		#endif
	#endif
	#ifndef DEPRECATED_ATTRIBUTE
		#ifdef _MSC_VER
			#define DEPRECATED_ATTRIBUTE(message) __declspec(deprecated(message))
		#elif !defined(__APPLE__) || __has_extension(attribute_deprecated_with_message) || (defined(__GNUC) && ((GNUC >= 5) || ((GNUC == 4) && (GNUC_MINOR__ >= 5))))
			#define DEPRECATED_ATTRIBUTE(message) __attribute__((deprecated(message)))
		#else
			#define DEPRECATED_ATTRIBUTE(message) __attribute__((deprecated))
		#endif
	#endif

#endif


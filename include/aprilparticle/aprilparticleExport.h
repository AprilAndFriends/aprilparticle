/// @file
/// @author  Domagoj Cerjan
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Defines macros for DLL exports/imports.

#ifndef APRILPARTICLE_EXPORT_H
#define APRILPARTICLE_EXPORT_H

	#ifdef _LIB
		#define aprilparticleExport
		#define aprilparticleFnExport
	#else
		#ifdef _WIN32
			#ifdef APRILPARTICLE_EXPORTS
				#define aprilparticleExport __declspec(dllexport)
				#define aprilparticleFnExport __declspec(dllexport)
			#else
				#define aprilparticleExport __declspec(dllimport)
				#define aprilparticleFnExport __declspec(dllimport)
			#endif
		#else
			#define aprilparticleExport __attribute__ ((visibility("default")))
			#define aprilparticleFnExport __attribute__ ((visibility("default")))
		#endif
	#endif
	#ifndef DEPRECATED_ATTRIBUTE
		#ifdef _MSC_VER
			#define DEPRECATED_ATTRIBUTE __declspec(deprecated("function is deprecated"))
		#else
			#define DEPRECATED_ATTRIBUTE __attribute__((deprecated))
		#endif
	#endif

#endif


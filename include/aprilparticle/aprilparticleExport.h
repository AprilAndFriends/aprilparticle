/************************************************************************************\
* This source file is part of the C++ Geometry Types Library (libgtypes)             *
* For latest info, see http://libgtypes.sourceforge.net/                             *
**************************************************************************************
* Copyright (c) 2010 Domagoj Cerjan, Kresimir Spes, Boris Mikic                      *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILPARTICLE_EXPORT_H
#define APRILPARTICLE_EXPORT_H

	#ifdef _STATICLIB
		#define aprilparticleExport
	#else
		#ifdef _WIN32
			#ifdef APRILPARTICLE_EXPORTS
				#define aprilparticleExport __declspec(dllexport)
			#else
				#define aprilparticleExport __declspec(dllimport)
			#endif
		#else
			#define aprilparticleExport __attribute__ ((visibility("default")))
		#endif
	#endif

#endif


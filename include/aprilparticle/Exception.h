/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef APRILUI_EXCEPTION_H
#define APRILUI_EXCEPTION_H

#include <hltypes/hstring.h>
#include <hltypes/exception.h>
#include "aprilparticleExport.h"

struct xml_node;

namespace April
{
	class Dataset;
	
	class aprilparticleExport _GenericException : public hltypes::exception
	{
	public:
		hstr mType;
		
		_GenericException(chstr errorText, chstr type = "", const char* file = "", int line = 0);
		hstr getType(){ return mType; }
		hstr getErrorText() { return msg; }
	};

	#define GenericException(msg) _GenericException(msg, "GenericException", __FILE__, __LINE__)

	class aprilparticleExport _XMLException : public _GenericException
	{
	public:
		_XMLException(chstr err_msg, xml_node* node, chstr type, const char* file, int line);
	};

	#define XMLException(msg, node) _XMLException(msg, node, type, __FILE__, __LINE__)
	#define XMLPropertyNotExistsException(msg, node) _XMLException(hstr("XML property doesn't exist: ") + msg, node, "XMLException", __FILE__, __LINE__)
	#define XMLUnknownClassException(msg, node) _XMLException(hstr("Unknown class detected in XML file: ") + msg, node, "XMLUnknownClassException", __FILE__, __LINE__)

}

#endif

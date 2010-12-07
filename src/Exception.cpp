/************************************************************************************\
This source file is part of the APRIL User Interface Library                         *
For latest info, see http://libaprilui.sourceforge.net/                              *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes, Boris Mikic                                        *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <xmlHelper.h>
#include "Exception.h"

namespace April
{
	_GenericException::_GenericException(chstr errorText, chstr type, const char* file, int line) : 
					   hltypes::exception("", file, line)
	{
		msg += errorText;
	}

	_XMLException::_XMLException(chstr err_msg, xml_node* node, chstr type, const char* file, int line):
				   _GenericException("", type, file, line)
	{
		msg += err_msg + ", in file " + hstr((char*)node->doc->URL) + ", line " + hstr(node->line);
	}
}

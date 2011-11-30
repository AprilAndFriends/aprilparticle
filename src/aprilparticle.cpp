/// @file
/// @author  Boris Mikic
/// @version 1.4
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <hltypes/exception.h>
#include <hltypes/hstring.h>

#include "aprilparticle.h"

namespace aprilparticle
{
	void aprilparticle_writelog(chstr message)
	{
		printf("%s\n", message.c_str());
	}
	void (*g_logFunction)(chstr) = aprilparticle_writelog;
	
	extern void initForceField();
    void init()
    {
		log("initializing AprilParticle");
		initForceField(); // so we can shoot the bad guys
    }
    
    void destroy()
    {
		log("destroying AprilParticle");
    }

	void setLogFunction(void (*fnptr)(chstr))
	{
		g_logFunction = fnptr;
	}
	
	void log(chstr message, chstr prefix)
	{
		g_logFunction(prefix + message);
	}
	
}

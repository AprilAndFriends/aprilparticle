/// @file
/// @author  Boris Mikic
/// @version 1.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <gtypes/Vector2.h>
#include <gtypes/Vector3.h>
#include <hltypes/exception.h>
#include <hltypes/harray.h>
#include <hltypes/hstring.h>

#include "aprilparticle.h"

namespace aprilparticle
{
	void aprilparticle_writelog(chstr message)
	{
		printf("%s\n", message.c_str());
	}
	void (*g_logFunction)(chstr) = aprilparticle_writelog;
	
	extern void initDirectionalForceField();
    void init()
    {
		log("initializing AprilParticle");
		initDirectionalForceField(); // so we can shoot the bad guys
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
	
	hstr gvec2_to_str(gvec2 vector)
	{
		return hsprintf("%f" APRILPARTICLE_VALUE_SEPARATOR "%f", vector.x, vector.y);
	}

	hstr gvec3_to_str(gvec3 vector)
	{
		return hsprintf("%f" APRILPARTICLE_VALUE_SEPARATOR "%f" APRILPARTICLE_VALUE_SEPARATOR "%f", vector.x, vector.y, vector.z);
	}

	gvec2 str_to_gvec2(chstr string)
	{
		harray<hstr> data = string.split(APRILPARTICLE_VALUE_SEPARATOR);
		if (data.size() != 2)
		{
			throw hl_exception("Error! Cannot convert string '" + string + "' to gltypes::Vector2.");
		}
		return gvec2(data[0], data[1]);
	}

	gvec3 str_to_gvec3(chstr string)
	{
		harray<hstr> data = string.split(APRILPARTICLE_VALUE_SEPARATOR);
		if (data.size() != 3)
		{
			throw hl_exception("Error! Cannot convert string '" + string + "' to gltypes::Vector3.");
		}
		return gvec3(data[0], data[1], data[2]);
	}

}

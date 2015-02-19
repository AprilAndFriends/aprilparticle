/// @file
/// @version 2.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/RenderSystem.h>
#include <hltypes/hexception.h>
#include <hltypes/hlog.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "Affectors.h"
#include "aprilparticle.h"
#include "System.h"
#include "Texture.h"

#define REGISTER_AFFECTOR_TYPE(name) aprilparticle::registerAffectorFactory(#name, &Affectors::name::createInstance)

namespace aprilparticle
{
	hstr logTag = "aprilparticle";

	static bool useCache = true;
	static hmap<hstr, Affector* (*)(chstr)> gAffectorFactories;
	static hmap<hstr, aprilparticle::Texture*> gTextureCache;
	static hmap<hstr, System*> gSystemCache;

	extern void initForceField();
	void init()
	{
		hlog::write(aprilparticle::logTag, "Initializing AprilParticle.");
		initForceField(); // so we can shoot the bad guys
		useCache = true;
		REGISTER_AFFECTOR_TYPE(Attractor);
		REGISTER_AFFECTOR_TYPE(CallbackAffector);
		REGISTER_AFFECTOR_TYPE(ColorChanger);
		REGISTER_AFFECTOR_TYPE(ColorChangerTimed);
		REGISTER_AFFECTOR_TYPE(ForceField);
		REGISTER_AFFECTOR_TYPE(LinearForce);
		REGISTER_AFFECTOR_TYPE(Resizer);
		REGISTER_AFFECTOR_TYPE(ResizerTimed);
		REGISTER_AFFECTOR_TYPE(Revolutor);
		REGISTER_AFFECTOR_TYPE(Rotator);
		REGISTER_AFFECTOR_TYPE(Scaler);
		REGISTER_AFFECTOR_TYPE(ScalerTimed);
	}
	
	void destroy()
	{
		hlog::write(aprilparticle::logTag, "Destroying AprilParticle.");
		gAffectorFactories.clear();
		foreach_m (aprilparticle::System*, it, gSystemCache)
		{
			delete it->second;
		}
		gSystemCache.clear();
		foreach_m (aprilparticle::Texture*, it, gTextureCache)
		{
			delete it->second;
		}
		gTextureCache.clear();
	}

	bool isUseCache()
	{
		return useCache;
	}

	void setUseCache(bool value)
	{
		useCache = value;
	}

	void registerAffectorFactory(chstr typeName, Affector* (*factory)(chstr))
	{
		if (gAffectorFactories.hasKey(typeName))
		{
			throw Exception("Affector Factory '" + typeName + "' already exists!");
		}
		gAffectorFactories[typeName] = factory;
	}

	Affector* createAffector(chstr typeName, chstr name)
	{
		if (gAffectorFactories.hasKey(typeName))
		{
			return (*gAffectorFactories[typeName])(name);
		}
		return NULL;
	}

	aprilparticle::Texture* loadTexture(chstr filename, bool cached)
	{
		aprilparticle::Texture* texture = NULL;
		if (gTextureCache.hasKey(filename))
		{
			texture = gTextureCache[filename];
		}
		else
		{
			cached |= useCache;
			april::Texture* aprilTexture = april::rendersys->createTextureFromResource(filename, april::Texture::TYPE_IMMUTABLE, cached ? april::Texture::LOAD_ASYNC : april::Texture::LOAD_ON_DEMAND);
			if (aprilTexture != NULL)
			{
				texture = new aprilparticle::Texture(aprilTexture, filename, cached);
				if (cached)
				{
					gTextureCache[filename] = texture;
				}
			}
		}
		return texture;
	}

	void clearCache()
	{
		hlog::write(logTag, "clearing cache");
		foreach_m (aprilparticle::Texture*, it, gTextureCache)
		{
			it->second->getTexture()->unload();
		}
		foreach_m (aprilparticle::System*, it, gSystemCache)
		{
			delete it->second;
		}
		gSystemCache.clear();
	}

	System* loadSystem(chstr filename, chstr name)
	{
		hstr key = name;
		if (key == "")
		{
			key = filename;
		}
		aprilparticle::System* system = gSystemCache.tryGet(key, NULL);
		if (system == NULL)
		{
			system = new aprilparticle::System(filename, name);
			if (useCache)
			{
				gSystemCache[key] = system;
			}
		}
		system->load();
		return new aprilparticle::System(*system);
	}

}

/// @file
/// @author  Domagoj Cerjan
/// @author  Boris Mikic
/// @version 1.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <april/Texture.h>
#include <gtypes/Vector3.h>
#include <hltypes/hlist.h>
#include <hltypes/hmap.h>
#include <hlxml/Document.h>
#include <hlxml/Node.h>
#include <hlxml/Property.h>

#include "Affectors.h"
#include "aprilparticle.h"
#include "Emitter.h"
#include "System.h"
#include "Util.h"

#define TRY_LOAD_AFFECTOR(affector, type, name) \
	if (type == #name) \
	{ \
		affector = new Affectors::name(); \
	}

namespace aprilparticle
{
	System::System(chstr filename, chstr name) : ActiveObject(name == "" ? generateName("System") : name)
	{
		this->filename = filename;
		this->loaded = false;
	}
	
	System::~System()
	{
		foreach (Emitter*, it, this->emitters)
		{
			delete (*it);
		}
		foreach (Affector*, it, this->affectors)
		{
			delete (*it);
		}
		foreach_m (april::Texture*, it, this->textures)
		{
			delete it->second;
		}
	}
	
	bool System::registerEmitter(Emitter* emitter)
	{
		if (this->emitters.contains(emitter))
		{
			return false;
		}
		this->emitters += emitter;
		emitter->_setSystem(this);
		return true;
	}

	bool System::unregisterEmitter(Emitter* emitter)
	{
		if (!this->emitters.contains(emitter))
		{
			return false;
		}
		this->emitters -= emitter;
		emitter->_setSystem(NULL);
		return true;
	}

	bool System::registerAffector(Affector* affector)
	{
		if (this->affectors.contains(affector))
		{
			return false;
		}
		this->affectors += affector;
		affector->_setSystem(this);
		return true;
	}

	bool System::unregisterAffector(Affector* affector)
	{
		if (!this->affectors.contains(affector))
		{
			return false;
		}
		this->affectors -= affector;
		affector->_setSystem(NULL);
		return true;
	}

	bool System::registerTexture(april::Texture* texture, chstr name)
	{
		hstr newName = (name == "" ? generateName(this->name + "_Texture") : name);
		if (this->textures.has_key(newName) || this->textures.has_value(texture))
		{
			return false;
		}
		this->textures[newName] = texture;
		return true;
	}

	bool System::unregisterTexture(april::Texture* texture)
	{
		if (!this->textures.has_value(texture))
		{
			return false;
		}
		this->textures.remove_value(texture);
		return true;
	}

	bool System::unregisterTexture(chstr name)
	{
		if (!this->textures.has_key(name))
		{
			return false;
		}
		this->textures.remove_key(name);
		return true;
	}

	Emitter* System::getEmitter(chstr name)
	{
		foreach (Emitter*, it, this->emitters)
		{
			if ((*it)->getName() == name)
			{
				return (*it);
			}
		}
		return NULL;
	}

	april::Texture* System::getTexture(chstr name)
	{
		return this->textures.try_get_by_key(name, NULL);
	}

	void System::load()
	{
		if (this->filename == "" || this->loaded)
		{
			return;
		}
		aprilparticle::log("loading " + this->filename);
		this->loaded = true;
		hlxml::Document newDoc(filename);
		hlxml::Node* root = newDoc.root();
		foreach_xmlnode (node, root)
		{
			if (*node == "Properties")
			{
				foreach_xmlproperty (prop, node)
				{
					this->setProperty(prop->name(), prop->value());
				}
			}
			else if (*node == "Emitter")
			{
				this->_loadEmitter(node);
			}
			else if (*node == "Affector")
			{
				this->_loadAffector(node);
			}
			else if (*node == "Texture")
			{
				this->_loadTexture(node);
			}
		}
		foreach_map (Emitter*, harray<hstr>, it, this->_mappedAffectors)
		{
			foreach (hstr, it2, it->second)
			{
				it->first->addAffector(this->getAffector(*it2));
			}
		}
		foreach_map (Emitter*, hstr, it, this->_mappedTextures)
		{
			it->first->setTexture(this->getTexture(it->second));
		}
		this->_mappedAffectors.clear();
		this->_mappedTextures.clear();
	}

	void System::_loadEmitter(hlxml::Node* root)
	{
		Emitter* emitter = new Emitter();
		foreach_xmlproperty (prop, root)
		{
			emitter->setProperty(prop->name(), prop->value());
		}
		foreach_xmlnode (node, root)
		{
			if (*node == "Affector")
			{
				this->_loadAffector(node, emitter);
			}
			else if (*node == "Texture")
			{
				this->_loadTexture(node, emitter);
			}
		}
		this->registerEmitter(emitter);
	}

	void System::_loadAffector(hlxml::Node* root, Emitter* emitter)
	{
		Affector* affector = NULL;
		if (root->pexists("type"))
		{
			hstr type = root->pstr("type");
			TRY_LOAD_AFFECTOR(affector, type, Attractor);
			TRY_LOAD_AFFECTOR(affector, type, CallbackAffector);
			TRY_LOAD_AFFECTOR(affector, type, ColorChanger);
			TRY_LOAD_AFFECTOR(affector, type, ColorMultiChanger);
			TRY_LOAD_AFFECTOR(affector, type, ForceField);
			TRY_LOAD_AFFECTOR(affector, type, LinearForce);
			TRY_LOAD_AFFECTOR(affector, type, Resizer);
			TRY_LOAD_AFFECTOR(affector, type, Revolutor);
			TRY_LOAD_AFFECTOR(affector, type, Rotator);
			TRY_LOAD_AFFECTOR(affector, type, Scaler);
			if (affector != NULL)
			{
				foreach_xmlproperty (prop, root)
				{
					affector->setProperty(prop->name(), prop->value());
				}
				this->registerAffector(affector);
				if (emitter != NULL)
				{
					emitter->addAffector(affector);
				}
			}
		}
		else if (emitter != NULL && root->pexists("reference"))
		{
			if (!this->_mappedAffectors.has_key(emitter))
			{
				this->_mappedAffectors[emitter] = harray<hstr>();
			}
			this->_mappedAffectors[emitter] += root->pstr("reference");
		}
	}

	void System::_loadTexture(hlxml::Node* root, Emitter* emitter)
	{
		april::Texture* texture = NULL;
		if (root->pexists("filename"))
		{
			texture = april::rendersys->loadTexture(get_basedir(this->filename) + "/" + root->pstr("filename"));
			this->registerTexture(texture, root->pstr("name"));
			if (emitter != NULL)
			{
				emitter->setTexture(texture);
			}
		}
		else if (emitter != NULL && root->pexists("reference"))
		{
			this->_mappedTextures[emitter] = root->pstr("reference");
		}
	}

	void System::update(float k)
	{
		foreach (Emitter*, it, this->emitters)
		{
			(*it)->update(k);
		}
	}

	void System::draw(gvec3 point)
	{
		foreach (Emitter*, it, this->emitters)
		{
			(*it)->draw(point, this->direction);
		}
	}

	void System::draw2D()
	{
		foreach (Emitter*, it, this->emitters)
		{
			(*it)->draw2D();
		}
	}

}

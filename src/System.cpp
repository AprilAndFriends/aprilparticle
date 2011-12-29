/// @file
/// @author  Domagoj Cerjan
/// @author  Boris Mikic
/// @version 1.4
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <april/Color.h>
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

namespace aprilparticle
{
	System::System(chstr filename, chstr name) : ActiveObject(name == "" ? generateName("System") : name)
	{
		this->filename = filename;
		this->up.set(0.0f, 1.0f, 0.0f);
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
	
	bool System::isRunning()
	{
		foreach (Emitter*, it, this->emitters)
		{
			if ((*it)->isRunning())
			{
				return true;
			}
		}
		return false;
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

	int System::getParticleCount()
	{
		int count = 0;
		foreach (Emitter*, it, this->emitters)
		{
			count += (*it)->getParticleCount();
		}
		return count;
	}

	bool System::isExpired()
	{
		foreach (Emitter*, it, this->emitters)
		{
			if (!(*it)->isExpired())
			{
				return false;
			}
		}
		return true;
	}

	hstr System::getProperty(chstr name, bool* property_exists)
	{
		if (property_exists != NULL)
		{
			*property_exists = true;
		}
		if (name == "up")	return gvec3_to_hstr(this->getUp());
		return ActiveObject::getProperty(name, property_exists);
	}

	bool System::setProperty(chstr name, chstr value)
	{
		if		(name == "up")	this->setUp(hstr_to_gvec3(value));
		else return ActiveObject::setProperty(name, value);
		return true;
	}

	void System::reset()
	{
		foreach (Emitter*, it, this->emitters)
		{
			(*it)->reset();
		}
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
		this->name = "";
		foreach_xmlproperty (prop, root)
		{
			this->setProperty(prop->name(), prop->value());
		}
		if (this->name == "")
		{
			this->name = this->filename.rsplit('.', -1, true).first();
		}
		foreach_xmlnode (node, root)
		{
			if (*node == "Emitter")
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
		Affector* affector;
		foreach_map (Emitter*, harray<hstr>, it, this->_mappedAffectors)
		{
			foreach (hstr, it2, it->second)
			{
				affector = this->getAffector(*it2);
				if (affector == NULL)
				{
					throw hl_exception("Error! Affector reference '" + (*it2) + "' does not exist!");
				}
				it->first->addAffector(affector);
			}
		}
		april::Texture* texture;
		foreach_map (Emitter*, hstr, it, this->_mappedTextures)
		{
			texture = this->getTexture(it->second);
			if (texture == NULL)
			{
				throw hl_exception("Error! Texture reference '" + it->second + "' does not exist!");
			}
			it->first->setTexture(texture);
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
			if (root->pexists("timings"))
			{
				type += "Timed";
			}
			affector = createAffector(type, root->pstr("name", ""));
			if (affector == NULL)
			{
				throw hl_exception("Error! Affector type '" + type + " does not exist!");
			}
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
			hstr filename = root->pstr("filename");
			texture = april::rendersys->loadTexture(get_basedir(this->filename) + "/" + filename);
			if (texture == NULL)
			{
				throw hl_exception("Error! Texture file '" + filename + "' does not exist!");
			}
			this->registerTexture(texture, (root->pexists("name") ? root->pstr("name") : filename));
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
		if (this->enabled)
		{
			foreach (Emitter*, it, this->emitters)
			{
				(*it)->update(k);
			}
		}
	}

	void System::finish()
	{
		foreach (Emitter*, it, this->emitters)
		{
			(*it)->setRunning(false);
		}
	}

	void System::draw(gvec3 point)
	{
		if (this->visible)
		{
			foreach (Emitter*, it, this->emitters)
			{
				(*it)->draw(point, this->up);
			}
		}
	}

	void System::draw(gvec2 offset, april::Color color)
	{
		if (this->visible)
		{
			if (color == APRIL_COLOR_WHITE)
			{
				foreach (Emitter*, it, this->emitters)
				{
					(*it)->draw(offset);
				}
			}
			else
			{
				foreach (Emitter*, it, this->emitters)
				{
					(*it)->draw(offset, color);
				}
			}
		}
	}

}

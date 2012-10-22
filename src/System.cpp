/// @file
/// @author  Domagoj Cerjan
/// @author  Boris Mikic
/// @version 1.62
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <april/Color.h>
#include <april/Texture.h>
#include <gtypes/Vector3.h>
#include <hltypes/hlist.h>
#include <hltypes/hlog.h>
#include <hltypes/hmap.h>
#include <hlxml/Document.h>
#include <hlxml/Node.h>
#include <hlxml/Property.h>

#include "Affectors.h"
#include "aprilparticle.h"
#include "aprilparticleUtil.h"
#include "Emitter.h"
#include "System.h"
#include "Texture.h"

namespace aprilparticle
{
	System::System(chstr filename, chstr name) : ActiveObject(name == "" ? generateName("System") : name)
	{
		this->filename = filename;
		this->up.set(0.0f, 1.0f, 0.0f);
		this->loaded = false;
	}
	
	System::System(const System& other) : ActiveObject(other)
	{
		this->filename = other.filename;
		this->up = other.up;
		this->loaded = other.loaded;
		// copy textures
		hmap<hstr, aprilparticle::Texture*> textures = other.textures;
		foreach_m (aprilparticle::Texture*, it, textures)
		{
			this->registerTexture(aprilparticle::loadTexture(it->second->getFilename(), it->second->isCached()), it->first);
		}
		// copy affectors
		hmap<Affector*, hmap<hstr, hstr> > _affectorProperties = other._affectorProperties;
		hmap<hstr, hstr> properties;
		harray<Affector*> affectors = other.affectors;
		Affector* affector;
		foreach (Affector*, it, affectors)
		{
			properties = _affectorProperties[*it];
			affector = createAffector(properties["type"], properties["name"]);
			foreach_m (hstr, it, properties)
			{
				if (it->first != "type")
				{
					affector->setProperty(it->first, it->second);
				}
			}
			this->registerAffector(affector);
			this->_affectorProperties[affector] = properties;
		}
		// copy emitters
		harray<Emitter*> emitters = other.emitters;
		hmap<Emitter*, hstr> _mappedTextures = other._mappedTextures;
		hmap<Emitter*, harray<hstr> > _mappedAffectors = other._mappedAffectors;
		Emitter* emitter;
		foreach (Emitter*, it, emitters)
		{
			emitter = new Emitter(*(*it));
			this->registerEmitter(emitter);
			this->_mappedTextures[emitter] = _mappedTextures[*it];
			this->_mappedAffectors[emitter] = _mappedAffectors[*it];
		}
		this->_assignEmitterData();
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
		foreach_m (aprilparticle::Texture*, it, this->textures)
		{
			if (!it->second->isCached())
			{
				delete it->second;
			}
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
		this->addAffector(affector);
		return true;
	}

	bool System::unregisterAffector(Affector* affector)
	{
		if (!this->affectors.contains(affector))
		{
			return false;
		}
		this->removeAffector(affector);
		return true;
	}

	void System::addAffector(Affector* affector)
	{
		ActiveObject::addAffector(affector);
		affector->_setSystem(this);
	}

	void System::removeAffector(Affector* affector)
	{
		ActiveObject::removeAffector(affector);
		affector->_setSystem(NULL);
	}

	bool System::registerTexture(aprilparticle::Texture* texture, chstr name)
	{
		hstr key = (name == "" ? generateName(this->name + "_Texture") : name);
		if (this->textures.has_key(key) || this->textures.has_value(texture))
		{
			return false;
		}
		this->textures[key] = texture;
		return true;
	}

	bool System::unregisterTexture(aprilparticle::Texture* texture)
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

	aprilparticle::Texture* System::getTexture(chstr name)
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
		if (name == "up")	this->setUp(hstr_to_gvec3(value));
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
			hlog::write(aprilparticle::logTag, "Already loaded: " + this->filename);
			return;
		}
		hlog::write(aprilparticle::logTag, "Loading system: " + this->filename);
		this->loaded = true;
		hlxml::Document* doc = hlxml::open(filename);
		hlxml::Node* root = doc->root();
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
		hlxml::close(doc);
		this->_assignEmitterData();
	}

	void System::_assignEmitterData()
	{
		// assigning emitter data
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
		aprilparticle::Texture* texture;
		foreach_map (Emitter*, hstr, it, this->_mappedTextures)
		{
			texture = this->getTexture(it->second);
			if (texture == NULL)
			{
				throw hl_exception("Error! Texture reference '" + it->second + "' does not exist!");
			}
			it->first->setTexture(texture->getTexture());
		}
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
		bool map = false;
		hstr name = "";
		if (root->pexists("type"))
		{
			hmap<hstr, hstr> properties;
			foreach_xmlproperty (prop, root)
			{
				properties[prop->name()] = prop->value();
			}
			hstr type = properties["type"];
			if (properties.has_key("timings"))
			{
				type += "Timed";
			}
			properties["type"] = type;
			affector = createAffector(type, root->pstr("name", ""));
			if (affector == NULL)
			{
				throw hl_exception("Error! Affector type '" + type + " does not exist!");
			}
			name = affector->getName();
			properties["name"] = name;
			this->_affectorProperties[affector] = properties;
			foreach_m (hstr, it, properties)
			{
				if (it->first != "type")
				{
					affector->setProperty(it->first, it->second);
				}
			}
			this->registerAffector(affector);
			map = true;
		}
		else if (root->pexists("reference"))
		{
			name = root->pstr("reference");
			map = true;
		}
		if (emitter != NULL && map)
		{
			if (!this->_mappedAffectors.has_key(emitter))
			{
				this->_mappedAffectors[emitter] = harray<hstr>();
			}
			this->_mappedAffectors[emitter] += name;
		}
	}

	void System::_loadTexture(hlxml::Node* root, Emitter* emitter)
	{
		bool map = false;
		hstr name = "";
		if (root->pexists("filename"))
		{
			hstr filename = root->pstr("filename");
			name = root->pstr("name", filename);
			aprilparticle::Texture* texture = aprilparticle::loadTexture(get_basedir(this->filename) + "/" + filename, root->pbool("cached", false));
			this->registerTexture(texture, name);
			map = true;
		}
		else if (root->pexists("reference"))
		{
			name = root->pstr("reference");
			map = true;
		}
		if (emitter != NULL && map)
		{
			this->_mappedTextures[emitter] = name;
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

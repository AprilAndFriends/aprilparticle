/// @file
/// @author  Domagoj Cerjan
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <april/Color.h>
#include <april/Texture.h>
#include <gtypes/Vector3.h>
#include <hltypes/hdir.h>
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
#include "Space.h"
#include "System.h"
#include "Texture.h"

namespace aprilparticle
{
	System::System(chstr filename, chstr name) : ActiveObject(name == "" ? generateName("System") : name), AffectorContainer()
	{
		this->filename = filename;
		this->loaded = false;
	}
	
	System::System(const System& other) : ActiveObject(other), AffectorContainer(other)
	{
		this->filename = other.filename;
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
		// copy spaces
		harray<Space*> spaces = other.spaces;
		Space* space;
		hmap<Space*, harray<hstr> > _mappedAffectors = other._mappedAffectors;
		hmap<Emitter*, hstr> _mappedTextures = other._mappedTextures;
		harray<Emitter*> emitters;
		Emitter* emitter;
		foreach (Space*, it, spaces)
		{
			space = new Space(*(*it));
			this->registerSpace(space);
			this->_mappedAffectors[space] = _mappedAffectors[*it];
			// copy emitters in space
			emitters = (*it)->getEmitters();
			foreach (Emitter*, it2, emitters)
			{
				emitter = new Emitter(*(*it2));
				space->registerEmitter(emitter);
				this->_mappedTextures[emitter] = _mappedTextures[*it2];
			}
		}
		this->_assignObjectData();
	}
	
	System::~System()
	{
		foreach (Space*, it, this->spaces)
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

	harray<Emitter*> System::getEmitters()
	{
		harray<Emitter*> emitters;
		foreach (Space*, it, this->spaces)
		{
			emitters += (*it)->getEmitters();
		}
		return emitters;
	}
	
	bool System::isRunning()
	{
		foreach (Space*, it, this->spaces)
		{
			if ((*it)->isRunning())
			{
				return true;
			}
		}
		return false;
	}

	bool System::registerSpace(Space* space)
	{
		if (this->spaces.contains(space))
		{
			return false;
		}
		this->spaces += space;
		space->_setSystem(this);
		return true;
	}

	bool System::unregisterSpace(Space* space)
	{
		if (!this->spaces.contains(space))
		{
			return false;
		}
		this->spaces -= space;
		space->_setSystem(NULL);
		return true;
	}

	bool System::registerAffector(Affector* affector)
	{
		return this->_addAffector(affector);
	}

	bool System::unregisterAffector(Affector* affector)
	{
		return this->_removeAffector(affector);
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

	aprilparticle::Space* System::getSpace(chstr name)
	{
		foreach (Space*, it, this->spaces)
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
	
	aprilparticle::Emitter* System::getEmitter(chstr name)
	{
		harray<Emitter*> emitters = this->getEmitters();
		foreach (Emitter*, it, emitters)
		{
			if ((*it)->getName() == name)
			{
				return (*it);
			}
		}
		return NULL;
	}
	
	int System::getParticleCount()
	{
		int count = 0;
		foreach (Space*, it, this->spaces)
		{
			count += (*it)->getParticleCount();
		}
		return count;
	}

	bool System::isExpired()
	{
		foreach (Space*, it, this->spaces)
		{
			if (!(*it)->isExpired())
			{
				return false;
			}
		}
		return true;
	}

	void System::reset()
	{
		foreach (Space*, it, this->spaces)
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
			if (*node == "Space")
			{
				this->_loadSpace(node);
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
		this->_assignObjectData();
	}

	void System::_assignObjectData()
	{
		// assigning affectors to spaces
		Affector* affector = NULL;
		foreach_map (Space*, harray<hstr>, it, this->_mappedAffectors)
		{
			foreach (hstr, it2, it->second)
			{
				affector = this->getAffector(*it2);
				if (affector == NULL)
				{
					throw hl_exception("Affector reference '" + (*it2) + "' does not exist!");
				}
				it->first->addAffector(affector);
			}
		}
		// assigning textures to emitters
		aprilparticle::Texture* texture = NULL;
		foreach_map (Emitter*, hstr, it, this->_mappedTextures)
		{
			texture = this->getTexture(it->second);
			if (texture == NULL)
			{
				throw hl_exception("Texture reference '" + it->second + "' does not exist!");
			}
			it->first->setTexture(texture->getTexture());
		}
	}

	void System::_loadSpace(hlxml::Node* root)
	{
		Space* space = new Space();
		this->registerSpace(space);
		foreach_xmlproperty (prop, root)
		{
			space->setProperty(prop->name(), prop->value());
		}
		foreach_xmlnode (node, root)
		{
			if (*node == "Emitter")
			{
				this->_loadEmitter(node, space);
			}
			else if (*node == "Affector")
			{
				this->_loadAffector(node, space);
			}
		}
	}

	void System::_loadEmitter(hlxml::Node* root, Space* space)
	{
		Emitter* emitter = new Emitter();
		space->registerEmitter(emitter);
		foreach_xmlproperty (prop, root)
		{
			emitter->setProperty(prop->name(), prop->value());
		}
		foreach_xmlnode (node, root)
		{
			if (*node == "Texture")
			{
				this->_loadTexture(node, emitter);
			}
		}
	}

	void System::_loadAffector(hlxml::Node* root, Space* space)
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
				throw hl_exception("Affector type '" + type + " does not exist!");
			}
			this->registerAffector(affector);
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
			map = true;
		}
		else if (root->pexists("reference"))
		{
			name = root->pstr("reference");
			map = true;
		}
		if (space != NULL && map)
		{
			if (!this->_mappedAffectors.has_key(space))
			{
				this->_mappedAffectors[space] = harray<hstr>();
			}
			this->_mappedAffectors[space] += name;
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
			aprilparticle::Texture* texture = aprilparticle::loadTexture(hdir::join_path(hdir::basedir(this->filename), filename, false), root->pbool("cached", false));
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
			foreach (Space*, it, this->spaces)
			{
				(*it)->update(k);
			}
		}
	}

	void System::finish()
	{
		foreach (Space*, it, this->spaces)
		{
			(*it)->finish();
		}
	}

	void System::draw(gvec3 point)
	{
		if (this->visible)
		{
			foreach (Space*, it, this->spaces)
			{
				(*it)->draw(point);
			}
		}
	}

	void System::draw(gvec2 offset, april::Color color)
	{
		if (this->visible)
		{
			foreach (Space*, it, this->spaces)
			{
				(*it)->draw(offset, color);
			}
		}
	}

}

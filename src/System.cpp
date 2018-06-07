/// @file
/// @version 2.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/Color.h>
#include <april/Texture.h>
#include <gtypes/Vector3.h>
#include <hltypes/hdir.h>
#include <hltypes/hfile.h>
#include <hltypes/hlist.h>
#include <hltypes/hlog.h>
#include <hltypes/hmap.h>
#include <hlxml/Document.h>
#include <hlxml/Node.h>

#include "Affectors.h"
#include "aprilparticle.h"
#include "aprilparticleUtil.h"
#include "Emitter.h"
#include "Space.h"
#include "System.h"
#include "Texture.h"

#define XML_HEADER "<?xml version=\"1.0\" encoding=\"utf-8\"?>"

#define MAKE_OPEN_NODE(name, values) hstr(values.size() > 0 ? "<" name " " + values.joined(" ") + ">" : "<" name ">")
#define MAKE_CLOSED_NODE(name, values) hstr(values.size() > 0 ? "<" name " " + values.joined(" ") + "/>" : "<" name "/>")

namespace aprilparticle
{
	hmap<hstr, PropertyDescription> System::_propertyDescriptions;

	System::System(chstr filename, chstr name) : ActiveObject(name == "" ? april::generateName("System") : name), AffectorContainer()
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
		Affector* affector = NULL;
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
		Space* space = NULL;
		hmap<Space*, harray<hstr> > _mappedAffectors = other._mappedAffectors;
		hmap<Emitter*, hstr> _mappedTextures = other._mappedTextures;
		harray<Emitter*> emitters;
		Emitter* emitter = NULL;
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
			if (it->second != NULL && !it->second->isCached())
			{
				delete it->second;
			}
		}
	}

	hmap<hstr, PropertyDescription>& System::getPropertyDescriptions() const
	{
		if (System::_propertyDescriptions.size() == 0)
		{
			System::_propertyDescriptions = ActiveObject::getPropertyDescriptions();
		}
		return System::_propertyDescriptions;
	}

	harray<Emitter*> System::getEmitters() const
	{
		harray<Emitter*> emitters;
		foreachc (Space*, it, this->spaces)
		{
			emitters += (*it)->getEmitters();
		}
		return emitters;
	}
	
	bool System::isRunning() const
	{
		HL_LAMBDA_CLASS(_isRunning, bool, ((Space* const& space) { return space->isRunning(); }));
		return this->spaces.matchesAny(&_isRunning::lambda);
	}

	bool System::registerSpace(Space* space)
	{
		if (this->spaces.has(space))
		{
			return false;
		}
		this->spaces += space;
		space->_setSystem(this);
		return true;
	}

	bool System::unregisterSpace(Space* space)
	{
		if (!this->spaces.has(space))
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
		hstr key = (name == "" ? april::generateName(this->name + "_Texture") : name);
		if (this->textures.hasKey(key) || this->textures.hasValue(texture))
		{
			return false;
		}
		this->textures[key] = texture;
		return true;
	}

	bool System::unregisterTexture(aprilparticle::Texture* texture)
	{
		return this->textures.removeValue(texture);
	}

	bool System::unregisterTexture(chstr name)
	{
		return this->textures.removeKey(name);
	}

	aprilparticle::Space* System::getSpace(chstr name) const
	{
		foreachc (Space*, it, this->spaces)
		{
			if ((*it)->getName() == name)
			{
				return (*it);
			}
		}
		return NULL;
	}
	
	aprilparticle::Texture* System::getTexture(chstr name) const
	{
		return this->textures.tryGet(name, NULL);
	}
	
	aprilparticle::Emitter* System::getEmitter(chstr name) const
	{
		harray<Emitter*> emitters = this->getEmitters();
		foreachc (Emitter*, it, emitters)
		{
			if ((*it)->getName() == name)
			{
				return (*it);
			}
		}
		return NULL;
	}
	
	int System::getParticleCount() const
	{
		int count = 0;
		foreachc (Space*, it, this->spaces)
		{
			count += (*it)->getParticleCount();
		}
		return count;
	}

	bool System::isExpired() const
	{
		HL_LAMBDA_CLASS(_isExpired, bool, ((Space* const& space) { return space->isExpired(); }));
		return this->spaces.matchesAny(&_isExpired::lambda);
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
			hlog::write(logTag, "Already loaded: " + this->filename);
			return;
		}
		hlog::write(logTag, "Loading system: " + this->filename);
		this->loaded = true;
		hlxml::Document doc(filename);
		hlxml::Node* root = doc.root();
		this->name = "";
		foreach_m (hstr, it, root->properties)
		{
			this->setProperty(it->first, it->second);
		}
		if (this->name == "")
		{
			this->name = this->filename.rsplit('.', -1, true).first();
		}
		foreach_xmlnode (node, root)
		{
			if ((*node)->name == "Space")
			{
				this->_loadSpace(*node);
			}
			else if ((*node)->name == "Affector")
			{
				this->_loadAffector(*node);
			}
			else if ((*node)->name == "Texture")
			{
				this->_loadTexture(*node);
			}
		}
		this->_assignObjectData();
	}

	void System::save(chstr filename)
	{
		harray<hstr> lines;
		harray<hstr> values;
		harray<Emitter*> emitters;
		harray<Affector*> affectors;
		april::Texture* emitterTexture = NULL;
		hstr name;
		hstr value;
		hmap<hstr, PropertyDescription> descriptions = this->getPropertyDescriptions(); // TODO - not using hmap&, because there seems to be an issue with the assignment operator
		foreach_m (PropertyDescription, it, descriptions)
		{
			value = this->getProperty(it->first);
			if (it->second.getType() == PropertyDescription::Type::Bool)
			{
				value = ((bool)value ? "true" : "false"); // compatibility hack
			}
			if (value != it->second.getDefaultValue())
			{
				values += it->first + "=\"" + value + "\"";
			}
		}
		lines += MAKE_OPEN_NODE("ParticleSystem", values);
		if (this->spaces.size() > 0)
		{
			lines += "";
		}
		foreach (Space*, it, this->spaces)
		{
			values.clear();
			descriptions = (*it)->getPropertyDescriptions();
			foreach_m (PropertyDescription, it2, descriptions)
			{
				value = (*it)->getProperty(it2->first);
				if (it2->second.getType() == PropertyDescription::Type::Bool)
				{
					value = ((bool)value ? "true" : "false"); // compatibility hack
				}
				if (value != it2->second.getDefaultValue())
				{
					values += it2->first + "=\"" + value + "\"";
				}
			}
			emitters = (*it)->getEmitters();
			affectors = (*it)->getAffectors();
			if (emitters.size() > 0 || affectors.size() > 0)
			{
				lines += "\t" + MAKE_OPEN_NODE("Space", values);
				foreach (Emitter*, it2, emitters)
				{
					values.clear();
					descriptions = (*it2)->getPropertyDescriptions();
					foreach_m (PropertyDescription, it3, descriptions)
					{
						value = (*it2)->getProperty(it3->first);
						if (it3->second.getType() == PropertyDescription::Type::Bool)
						{
							value = ((bool)value ? "true" : "false"); // compatibility hack
						}
						if (value != it3->second.getDefaultValue())
						{
							values += it3->first + "=\"" + value + "\"";
						}
					}
					emitterTexture = (*it2)->getTexture();
					if (emitterTexture != NULL)
					{
						lines += "\t\t" + MAKE_OPEN_NODE("Emitter", values);
						foreach_m (Texture*, it4, this->textures)
						{
							if (it4->second->getTexture() == emitterTexture)
							{
								lines += "\t\t\t<Texture reference=\"" + it4->first + "\"/>";
								break;
							}
						}
						lines += "\t\t</Emitter>";
					}
					else
					{
						lines += "\t\t" + MAKE_CLOSED_NODE("Emitter", values);
					}
				}
				foreach (Affector*, it2, affectors)
				{
					if (this->_mappedAffectors[*it].has((*it2)->getName()))
					{
						values.clear();
						descriptions = (*it2)->getPropertyDescriptions();
						foreach_m (PropertyDescription, it3, descriptions)
						{
							value = (*it2)->getProperty(it3->first);
							if (it3->second.getType() == PropertyDescription::Type::Bool)
							{
								value = ((bool)value ? "true" : "false"); // compatibility hack
							}
							if (value != it3->second.getDefaultValue())
							{
								values += it3->first + "=\"" + value + "\"";
							}
						}
						lines += "\t\t" + MAKE_CLOSED_NODE("Affector", values);
					}
					else
					{
						lines += "\t\t<Affector reference=\"" + (*it2)->getName() + "\"/>";
					}
				}
				lines += "\t</Space>";
			}
			else
			{
				lines += "\t" + MAKE_CLOSED_NODE("Space", values);
			}
		}
		harray<hstr> allNames;
		foreach_map (Space*, harray<hstr>, it, this->_mappedAffectors)
		{
			allNames += it->second;
		}
		allNames.removeDuplicates();
		harray<Affector*> externalAffectors;
		foreach (Affector*, it, this->affectors)
		{
			if (!allNames.has((*it)->getName()))
			{
				externalAffectors += (*it);
			}
		}
		if (externalAffectors.size() > 0)
		{
			lines += "";
		}
		foreach (Affector*, it, externalAffectors)
		{
			values.clear();
			descriptions = (*it)->getPropertyDescriptions();
			foreach_m (PropertyDescription, it2, descriptions)
			{
				value = (*it)->getProperty(it2->first);
				if (it2->second.getType() == PropertyDescription::Type::Bool)
				{
					value = ((bool)value ? "true" : "false"); // compatibility hack
				}
				if (value != it2->second.getDefaultValue())
				{
					values += it2->first + "=\"" + value + "\"";
				}
			}
			lines += "\t" + MAKE_CLOSED_NODE("Affector", values);
		}
		if (this->textures.size() > 0)
		{
			lines += "";
		}
		foreach_m (Texture*, it, this->textures)
		{
			values.clear();
			values += "filename=\"" + it->first + "\"";
			if (it->second->isCached())
			{
				values += "cached=\"true\"";
			}
			lines += "\t" + MAKE_CLOSED_NODE("Texture", values);
		}
		lines += "";
		lines += "</ParticleSystem>";
		hfile file;
		file.open(filename, hfile::AccessMode::Write);
		file.writeLine(XML_HEADER);
		file.writeLine(lines.joined("\n"));
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
					throw Exception("Affector reference '" + (*it2) + "' does not exist!");
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
				throw Exception("Texture reference '" + it->second + "' does not exist!");
			}
			it->first->setTexture(texture->getTexture());
		}
	}

	void System::_loadSpace(hlxml::Node* root)
	{
		Space* space = new Space();
		this->registerSpace(space);
		foreach_m (hstr, it, root->properties)
		{
			space->setProperty(it->first, it->second);
		}
		foreach_xmlnode (node, root)
		{
			if ((*node)->name == "Emitter")
			{
				this->_loadEmitter((*node), space);
			}
			else if ((*node)->name == "Affector")
			{
				this->_loadAffector((*node), space);
			}
		}
	}

	void System::_loadEmitter(hlxml::Node* root, Space* space)
	{
		Emitter* emitter = new Emitter();
		space->registerEmitter(emitter);
		foreach_m (hstr, it, root->properties)
		{
			emitter->setProperty(it->first, it->second);
		}
		foreach_xmlnode (node, root)
		{
			if ((*node)->name == "Texture")
			{
				this->_loadTexture((*node), emitter);
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
			hmap<hstr, hstr> properties = root->properties;
			hstr type = properties["type"];
			if (properties.hasKey("timings"))
			{
				type += "Timed";
				properties["type"] = type;
			}
			affector = createAffector(type, root->pstr("name", ""));
			if (affector == NULL)
			{
				throw Exception("Affector type '" + type + " does not exist!");
			}
			this->registerAffector(affector);
			name = affector->getName();
			properties["name"] = name;
			this->_affectorProperties[affector] = properties;
			properties.removeKey("type");
			foreach_m (hstr, it, properties)
			{
				affector->setProperty(it->first, it->second);
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
			if (!this->_mappedAffectors.hasKey(space))
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
			aprilparticle::Texture* texture = aprilparticle::loadTexture(hdir::joinPath(hdir::baseDir(this->filename), filename, false), root->pbool("cached", false));
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

	void System::update(float timeDelta)
	{
		if (this->enabled)
		{
			foreach (Space*, it, this->spaces)
			{
				(*it)->update(timeDelta);
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

	void System::draw(cgvec3f point)
	{
		if (this->visible)
		{
			foreach (Space*, it, this->spaces)
			{
				(*it)->draw(point);
			}
		}
	}

	void System::draw(cgvec2f offset, const april::Color& color)
	{
		if (this->visible && color.a > 0)
		{
			foreach (Space*, it, this->spaces)
			{
				(*it)->draw(offset, color);
			}
		}
	}

}

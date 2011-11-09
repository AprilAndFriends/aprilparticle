/// @file
/// @author  Domagoj Cerjan
/// @author  Boris Mikic
/// @version 1.2
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
#include "Emitter.h"
#include "System.h"
#include "Util.h"

namespace aprilparticle
{
	System::System(chstr filename, chstr name)
	{
		this->filename = filename;
		this->name = (name == "" ? aprilparticle::generateName("System") : name);
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
		foreach (april::Texture*, it, this->textures)
		{
			delete (*it);
		}
	}
	
	void System::registerEmitter(Emitter* emitter)
	{
		this->emitters += emitter;
	}

	void System::unregisterEmitter(Emitter* emitter)
	{
		this->emitters -= emitter;
	}

	void System::registerAffector(Affector* affector)
	{
		this->affectors += affector;
	}

	void System::unregisterAffector(Affector* affector)
	{
		this->affectors -= affector;
	}

	void System::registerTexture(april::Texture* texture)
	{
		this->textures += texture;
	}

	void System::unregisterTexture(april::Texture* texture)
	{
		this->textures -= texture;
	}

	void System::load()
	{
		if (this->filename == "")
		{
			return;
		}
		hlist<hstr> affectorTable;
		hlist<hstr> textureTable;
		
		hlxml::Document newDoc(filename);
		hlxml::Node* root = newDoc.root();
		foreach_xmlnode (node, root)
		{
			if (*node == "Properties")
			{
				printf("Properties\n");
				foreach_xmlproperty (prop, node)
				{
					// TODO
					printf("\t%s : %s\n", prop->name().c_str(), prop->value().c_str());
				}
			}
			else if (*node == "Emitter")
			{
				printf("Emitter\n");
				foreach_xmlproperty (prop, node)
				{
					// TODO
					printf("\t%s : %s\n", prop->name().c_str(), prop->value().c_str());
				}
				foreach_xmlnode (child, node)
				{
					if (*node == "Affector")
					{
						printf("Affector\n");
						foreach_xmlproperty (prop, child)
						{
							// TODO
							printf("\t%s : %s\n", prop->name().c_str(), prop->value().c_str());
						}
					}
				}
			}
			else if (*node == "Affector")
			{
				printf("Affector\n");
				foreach_xmlproperty (prop, node)
				{
					// TODO
					printf("\t%s : %s\n", prop->name().c_str(), prop->value().c_str());
				}
			}

			/*
			if (*node != "text")
			{
				printf("\n%s\n", (const char*)node->name);
			}
				foreach_xmlproperty (prop, node)
				{
					printf("\t%s : %s\n", prop->name().c_str(), prop->value().c_str());
				}
			foreach_xmlnode (child, node)
			{
				if (*child != "text")
				{
					printf("\n%s\n", (const char*)child->name);
				}
				foreach_xmlnode (cchild, child)
				{
					if (*cchild != "text")
					{
						printf("\t\t%s - ", (const char*)cchild->name);
						foreach_xmlproperty (pprop, cchild)
						{
							printf("%s : %s ", pprop->name().c_str(), pprop->value().c_str());
							if (*cchild == "Affector")
							{
								bool found = false;
								foreach(hstr, it, affectorTable)
								{
									if ((*it) == pprop->value())
									{
										found = true;
									}
								}
								if (!found)
								{
									affectorTable += pprop->value();
								}
							}
							else if (*cchild == "Property" && pprop->name() == "name" && pprop->value() == "Texture")
							{
								bool found = false;
								foreach(hstr, it, textureTable)
								{
									if ((*it) == pprop->next()->value())
									{
										found = true;
										break;
									}
								}
								if (!found)
								{
									affectorTable += pprop->next()->value();
								}
							}
						}
						printf("\n");
					}
				}
			}
			*/
		}
		/*
		foreach (hstr, it, affectorTable)
		{
			printf("%s\n", (*it).c_str());
		}
		foreach (hstr, it, textureTable)
		{
			printf("%s\n", (*it).c_str());
		}
		*/
	}

	void System::update(float k)
	{
		foreach (Emitter*, it, this->emitters)
		{
			(*it)->update(k);
		}
	}

	void System::draw(gvec3 point, gvec3 up)
	{
		foreach (Emitter*, it, this->emitters)
		{
			(*it)->draw(point, up, this->position);
		}
	}

}

/// @file
/// @author  Domagoj Cerjan
/// @author  Boris Mikic
/// @version 1.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <hltypes/hlist.h>
#include <hltypes/hmap.h>
#include <hlxml/Document.h>
#include <hlxml/Node.h>
#include <hlxml/Property.h>

#include "ParticleSystem.h"
#include "Util.h"

namespace aprilparticle
{
	ParticleSystem::ParticleSystem(chstr filename, chstr name) 
	{
		this->filename = filename;
		this->name = (name == "" ? aprilparticle::generateName("ParticleSystem") : name);
	}
	
	ParticleSystem::~ParticleSystem()
	{
	}
	
	void ParticleSystem::load()
	{
		hlist<hstr> textureTable;
		hlist<hstr> affectorTable;
		
		hlxml::Document newDoc(filename);
		hlxml::Node* root = newDoc.root();
		foreach_xmlnode (node, root)
		{
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
								foreach_l(hstr, it, affectorTable)
								{
									if ((*it) == pprop->value())
									{
										found = true;
									}
								}
								if (!found)
								{
									affectorTable.push_back(pprop->value());
								}
							}
							else if (*cchild == "Property" && pprop->name() == "name" && pprop->value() == "Texture")
							{
								bool found = false;
								foreach_l(hstr, it, textureTable)
								{
									if ((*it) == pprop->next()->value())
									{
										found = true;
										break;
									}
								}
								if (!found)
								{
									affectorTable.push_back(pprop->next()->value());
								}
							}
						}
						printf("\n");
					}
				}
			}
		}
		foreach_l (hstr, it, affectorTable)
		{
			printf("%s\n", (*it).c_str());
		}
		foreach_l (hstr, it, textureTable)
		{
			printf("%s\n", (*it).c_str());
		}
	}

}

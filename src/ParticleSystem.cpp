#include "ParticleSystem.h"

namespace April
{
	ParticleSystem::ParticleSystem() 
	{
		
	}
	
	ParticleSystem::~ParticleSystem()
	{
		
	}
	
	CompositeParticleObject* ParticleSystem::loadParticleObject(hstr fileName, hstr name)
	{
		hstr newName;
		
		std::list<hstr> textureTable;
		std::list<hstr> affectorTable;
		
		if(name == "")
		{
			newName = hstr("untitled_") + hstr(_mNumerator);
			_mNumerator++;
		}
		
		xml_doc newDoc(fileName);
		for( xml_node *iter = newDoc.root()->iter_children(); iter != NULL; iter = iter->next())
		{
			if(hstr((const char*) iter->name) != "text")
				std::cout << "\n" << iter->name << std::endl;
			for( xml_prop *prop = iter->iter_properties(); prop != NULL; prop = prop->next())
			{
				std::cout << "\t" << prop->name() << ":" << prop->value() << std::endl;
			}
			for( xml_node *child = iter->iter_children(); child != NULL; child = child->next())
			{
				if(hstr((const char*) child->name) != "text")
					std::cout << "\t" << child->name << std::endl;
				for( xml_node *cchild = child->iter_children(); cchild != NULL; cchild = cchild->next())
				{
					if(hstr((const char*) cchild->name) != "text")
					{
						std::cout << "\t\t" << cchild->name << " - ";
						for( xml_prop *pprop = cchild->iter_properties(); pprop != NULL; pprop = pprop->next())
						{
							std::cout << pprop->name() << " : " << pprop->value() << " ";
							if(hstr((const char*)cchild->name) == "Affector")
							{
								bool v = false;
								for(std::list<hstr>::iterator it = affectorTable.begin(); it != affectorTable.end(); it++)
									if(*it == pprop->value())
										v = true;
								if(!v)
									affectorTable.push_back(pprop->value());
							}
							else if(hstr((const char*)cchild->name) == "Property"
									&& pprop->name() == "name"
									&& pprop->value() == "Texture")
							{
								bool v = false;
								for(std::list<hstr>::iterator it = textureTable.begin(); it != textureTable.end(); it++)
									if(*it == pprop->next()->value())
										v = true;
								if(!v)
									affectorTable.push_back(pprop->next()->value());
							}
						}
						std::cout << std::endl;
					}
						
				}
			}
		}
		
		for(std::list<hstr>::iterator it = affectorTable.begin(); it != affectorTable.end(); it++)
		{
			std::cout << *it << std::endl;
		}
		
		for(std::list<hstr>::iterator it = textureTable.begin(); it != textureTable.end(); it++)
		{
			std::cout << *it << std::endl;
		}
	}
}
/*
 	Copyright 2015 Francisco Javier Martinez Garcia
	Copyright 2015 Alvaro Perez Corral
	Copyright 2015 Luis Valero Martin
	Copyright 2015 Adrian Vizcaino Gonzalez
	This file is part of Through the galaxy.
	Through the galaxy is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	any later version.
	Through the galaxy is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	You should have received a copy of the GNU General Public License
	along with Through the galaxy.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "XmlParser.h"

#include <log.h>

#include <fstream>

#include "RapidXml/rapidxml.hpp"

namespace Common
{
	namespace Util
	{
		namespace XMLParser
		{
			const char* const LOG_CXMLPARSER = "Common::Util::XMLParser::CXMLParser";

			Common::Data::Blueprints::TBluePrintsMap CXMLParser::loadBluePrints
																(const std::string &filename)
			{
				log_trace(LOG_CXMLPARSER,"Parsing xml file %s...\n",filename.c_str());

				using Common::Data::Blueprints::TBluePrintsMap;
				using Common::Data::Blueprints::TComponentsVector;
				using Common::Data::Blueprints::TXMLComponent;

				TBluePrintsMap blueprints;

				std::ifstream in(filename.c_str());
				std::string xml = "";
				while(in){
					std::string line;
					std::getline(in,line);
					xml += line;
				}

				using namespace rapidxml;
				xml_document<> doc;
				std::vector<char> text(xml.begin(),xml.end());
				text.push_back('\0');
				try {
					doc.parse<0>(&text[0]);
				} catch (rapidxml::parse_error e) {
					log_error(LOG_CXMLPARSER,"Error while parsing...\n");
					log_error(LOG_CXMLPARSER,"Exception %s\n",e.what());
					return blueprints;
				}

				xml_node<>* root = doc.first_node("entities");
				xml_node<>* currEntityNode = root->first_node("entity");
				while(currEntityNode){
					std::string type = currEntityNode->first_attribute("type")->value();
					log_trace(LOG_CXMLPARSER,"EntityNode found with type value %s\n",type.c_str());
					//Parse components
					TComponentsVector components;
					xml_node<>* currComponentNode = currEntityNode->first_node("component");
					while(currComponentNode){
						std::string name = currComponentNode->first_attribute("name")->value();
						int num = std::stoi(currComponentNode->first_attribute("number")->value());
						int pri = 
							std::stoi(currComponentNode->first_attribute("priority")->value());
						log_trace(LOG_CXMLPARSER,
									"ComponentNode found with name %s, number %d, priority %d\n",
									name.c_str(),num,pri);
						TXMLComponent component;
						component.name = name;
						component.number = num;
						component.priority = pri;

						components.push_back(component);

						currComponentNode = currComponentNode->next_sibling();
					}
					blueprints.insert(std::pair<std::string,TComponentsVector>(type,components));
					currEntityNode = currEntityNode->next_sibling();
				}

				return blueprints;
			}
		}
	}
}
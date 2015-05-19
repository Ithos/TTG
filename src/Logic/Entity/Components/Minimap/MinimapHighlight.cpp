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

#include "MinimapHighlight.h"

#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>

#include <Common/Map/MapEntity.h>
#include <Common/Data/Spawn_Constants.h>

#include "../../../Scene/Scene.h"
#include "../../Entity.h"

#include "../Graphic/Graphics.h"

namespace Logic
{
	using namespace Component;
	IMP_FACTORY(CMinimapHighlight);

	CMinimapHighlight::~CMinimapHighlight()
	{
		if (m_alphaNode) {
			m_alphaNode->detachAllObjects();
			m_ogreScene->destroySceneNode(m_alphaNode);
			m_alphaNode = nullptr;
		}

		if (m_alphaNode) {
			m_alphaNode->detachAllObjects();
			m_ogreScene->destroySceneNode(m_alphaNode);
			m_alphaNode = nullptr;
		}

		if (m_alphaEntity) {	
			m_ogreScene->destroyEntity(m_alphaEntity);
			m_alphaEntity = nullptr;
		}

		if (m_glowEntity) {	
			m_ogreScene->destroyEntity(m_glowEntity);
			m_glowEntity = nullptr;
		}

	}

	bool CMinimapHighlight::spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo)
	{
		if(!IComponent::spawn(entity,scene,entityInfo))
			return false;

		if(entityInfo->hasAttribute(Common::Data::Spawn::MINIMAP_ENTITY_SCALE))
			m_scale = entityInfo->getFloatAttribute(Common::Data::Spawn::MINIMAP_ENTITY_SCALE);

		if(entityInfo->hasAttribute(Common::Data::Spawn::MINIMAP_ENTITY_ENEMY))
			m_enemy = entityInfo->getBoolAttribute(Common::Data::Spawn::MINIMAP_ENTITY_ENEMY);

		m_ogreScene = m_entity->getScene()->getSceneManager();

		createGraphicEntity(entityInfo, entity);
		
		
		if(!m_alphaEntity || !m_glowEntity)
			return false;
		
		return true;
	}

	void CMinimapHighlight::createGraphicEntity(const Map::CMapEntity* entityInfo, CEntity* entity)
	{
		if(!m_enemy)
		{
			m_alphaEntity = static_cast<CGraphics*>(entity->getComponentByName("CGraphics"))->getOgreEntity()->clone(
				static_cast<CGraphics*>(entity->getComponentByName("CGraphics"))->getOgreEntity()->getName() + "_minimap_alpha");

			m_alphaEntity->setRenderQueueGroup(RENDER_QUEUE_FULL_GLOW_ALPHA_GLOW); 
			m_alphaEntity->setMaterialName("cg/alpha_glow");
			m_alphaEntity->setVisibilityFlags(Common::Data::ONLY_TEXTURE_MASK);
			m_alphaNode = static_cast<CGraphics*>(entity->getComponentByName("CGraphics"))
				->getOgreNode()->createChildSceneNode(static_cast<CGraphics*>(entity->getComponentByName("CGraphics"))->getOgreNode()->getName() + "_minimap_alpha_node"); 
			m_alphaNode->setScale(m_scale,m_scale,m_scale);
			m_alphaNode->attachObject(m_alphaEntity); 

			// full glow alpha glow
			m_glowEntity = static_cast<CGraphics*>(entity->getComponentByName("CGraphics"))->getOgreEntity()->clone(
				static_cast<CGraphics*>(entity->getComponentByName("CGraphics"))->getOgreEntity()->getName() + "_minimap_glow");
			m_glowEntity->setRenderQueueGroup(RENDER_QUEUE_FULL_GLOW_GLOW); 
			m_glowEntity->setMaterialName("cg/no_depth_check_glow"); 
			m_glowEntity->setVisibilityFlags(Common::Data::ONLY_TEXTURE_MASK);
			m_glowNode = static_cast<CGraphics*>(entity->getComponentByName("CGraphics"))
				->getOgreNode()->createChildSceneNode(static_cast<CGraphics*>(entity->getComponentByName("CGraphics"))->getOgreNode()->getName() + "_minimap_glow_node");
			m_glowNode->setScale(m_scale,m_scale,m_scale);

			m_glowNode->attachObject(m_glowEntity);

			m_alphaEntity->setQueryFlags(Common::Data::NULL_MASK);
			m_glowEntity->setQueryFlags(Common::Data::NULL_MASK);
		}else{

			m_alphaEntity = static_cast<CGraphics*>(entity->getComponentByName("CGraphics"))->getOgreEntity()->clone(
				static_cast<CGraphics*>(entity->getComponentByName("CGraphics"))->getOgreEntity()->getName() + "_minimap_alpha");

			m_alphaEntity->setRenderQueueGroup(RENDER_QUEUE_FULL_GLOW_ALPHA_GLOW); 
			m_alphaEntity->setMaterialName("cg/alpha_redglow");
			m_alphaEntity->setVisibilityFlags(Common::Data::ONLY_TEXTURE_MASK);
			m_alphaNode = static_cast<CGraphics*>(entity->getComponentByName("CGraphics"))
				->getOgreNode()->createChildSceneNode(static_cast<CGraphics*>(entity->getComponentByName("CGraphics"))->getOgreNode()->getName() + "_minimap_alpha_node"); 
			m_alphaNode->setScale(m_scale,m_scale,m_scale);
			m_alphaNode->attachObject(m_alphaEntity); 

			// full glow alpha glow
			m_glowEntity = static_cast<CGraphics*>(entity->getComponentByName("CGraphics"))->getOgreEntity()->clone(
				static_cast<CGraphics*>(entity->getComponentByName("CGraphics"))->getOgreEntity()->getName() + "_minimap_glow");
			m_glowEntity->setRenderQueueGroup(RENDER_QUEUE_FULL_GLOW_GLOW); 
			m_glowEntity->setMaterialName("cg/no_depth_check_redglow"); 
			m_glowEntity->setVisibilityFlags(Common::Data::ONLY_TEXTURE_MASK);
			m_glowNode = static_cast<CGraphics*>(entity->getComponentByName("CGraphics"))
				->getOgreNode()->createChildSceneNode(static_cast<CGraphics*>(entity->getComponentByName("CGraphics"))->getOgreNode()->getName() + "_minimap_glow_node");
			m_glowNode->setScale(m_scale,m_scale,m_scale);

			m_glowNode->attachObject(m_glowEntity);

			m_alphaEntity->setQueryFlags(Common::Data::NULL_MASK);
			m_glowEntity->setQueryFlags(Common::Data::NULL_MASK);

		}

	}

	bool CMinimapHighlight::activate()
	{
		if(!IComponent::activate()) return false;
		if(m_alphaEntity)m_alphaEntity->setVisible(true);
		if(m_glowEntity)m_glowEntity->setVisible(true);

		return true;
	}

	void CMinimapHighlight::deactivate()
	{
		IComponent::deactivate();
		if(m_alphaEntity)m_alphaEntity->setVisible(false);
		if(m_glowEntity)m_glowEntity->setVisible(false);
	}

}
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

#include "Graphics.h"

#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreAny.h>

#include <Common/Map/MapEntity.h>

#include "../../../Scene/Scene.h"
#include "../../Entity.h"

#include "../Movement/Transform.h"
#include "../Movement/Interpolation.h"
#include <common/Particles/ParticleManager.h>

using namespace Common::Data;

namespace Logic
{
	using namespace Component;
	IMP_FACTORY(CGraphics);

	CGraphics::~CGraphics()
	{
		if (m_ogreNode) {
			m_ogreNode->detachAllObjects();
			m_ogreScene->destroySceneNode(m_ogreNode);
			m_ogreNode = nullptr;
		}
		if (m_ogreEntity) {	
			m_ogreScene->destroyEntity(m_ogreEntity);
			m_ogreEntity = nullptr;
		}

        if (m_entity->getName() == "Star")
            Common::Particles::CParticleManager::getInstance()->releaseStars();
	}


	bool CGraphics::spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo)
	{
		if(!IComponent::spawn(entity,scene,entityInfo))
			return false;

		m_ogreScene = m_entity->getScene()->getSceneManager();

		if(entityInfo->hasAttribute("model"))
			m_model = entityInfo->getStringAttribute("model");

		m_interpolation = static_cast<CInterpolation*>(m_entity->getComponentByName(INTERPOLATION_COMP));
		m_ogreEntity    = createGraphicEntity(entityInfo);
		
        if (entity->getName() == "Star") {
            Common::Particles::CParticleManager::getInstance()->addStar(star_t::SYSTEM_STAR);
            Common::Particles::CParticleManager::getInstance()->startStarEffect();
        }
		
		if (!m_ogreEntity)
			return false;
		
		return true;
	}

	void CGraphics::tick(unsigned int msecs)
	{
		if(!m_activate) return;
		IComponent::tick(msecs);
       
        if (m_interpolation) { 
            m_ogreNode->setPosition(m_interpolation->getRenderTransform().getTrans());
            m_ogreNode->setOrientation(m_interpolation->getRenderTransform().extractQuaternion());
        }
        else {
            CTransform* t_target = static_cast<CTransform*>(m_entity->getComponentByName(TRANSFORM_COMP));
		    m_ogreNode->setPosition(t_target->getPosition());
		    m_ogreNode->setOrientation(t_target->getOrientation());
        }
	}


	Ogre::Entity* CGraphics::createGraphicEntity(const Map::CMapEntity* entityInfo)
	{
		try {
			m_ogreEntity = m_ogreScene->createEntity(m_entity->getName(), m_model);
			//m_ogrePrimitive = m_ogreScene->createEntity(m_entity->getName() + "_primitive","RedSphere.mesh");
		} catch (std::exception e) {
			return nullptr;
		}

		if(entityInfo->hasAttribute("VisibilityMask")){
			std::string mask = entityInfo->getStringAttribute("VisibilityMask");
			if(mask == "texture_only"){
				m_ogreEntity->setVisibilityFlags(Common::Data::ONLY_TEXTURE_MASK);
			}else if (mask == "no_texture"){
				m_ogreEntity->setVisibilityFlags(Common::Data::NON_TEXTURE_MASK);
			}
		}

		m_ogreNode = m_ogreScene->getRootSceneNode()->createChildSceneNode(m_entity->getName() + "_node");

		if(entityInfo->hasAttribute("SelectionMask")){
			std::string mask = entityInfo->getStringAttribute("SelectionMask");
			if( mask == "Planet")
				m_ogreEntity->setQueryFlags(Common::Data::PLANET_MASK);
			else if(mask == "Star")
				m_ogreEntity->setQueryFlags(Common::Data::STAR_MASK);
			else if(mask == "System")
				m_ogreEntity->setQueryFlags(Common::Data::SYSTEM_MASK);
		}else{
			m_ogreEntity->setQueryFlags(Common::Data::NULL_MASK);
		}

		m_ogreNode->attachObject(m_ogreEntity);
		//m_ogreNode->attachObject(m_ogrePrimitive);
		//m_ogrePrimitive->setVisible(false);

		if ((entityInfo->hasAttribute("static")) && (entityInfo->getBoolAttribute("static")))
			m_entity->getScene()->addStaticOgreNode(m_ogreNode);
		else
			m_entity->getScene()->addDynamicOgreNode(m_ogreNode);

		Logic::CTransform* comp = static_cast<Logic::CTransform*>(m_entity->getComponentByName("CTransform"));

		m_ogreNode->setPosition(comp->getTransform().getTrans());
		m_ogreNode->setOrientation(comp->getTransform().extractQuaternion());

		if(entityInfo->hasAttribute("scale"))
		{
			m_scale = entityInfo->getFloatAttribute("scale");
			m_ogreNode->setScale(Ogre::Real(m_scale),Ogre::Real(m_scale),Ogre::Real(m_scale));
		}

		if(entityInfo->hasAttribute("extra"))
		{
			Ogre::Any data(Ogre::String(entityInfo->getStringAttribute("extra")));
			m_ogreNode->setUserAny(data);
		}

		if(entityInfo->hasAttribute("full_glow"))
		{
			// full glow alpha glow
			Ogre::Entity* alphaFullGlowEntity = m_ogreEntity->clone(m_ogreEntity->getName() + "_alphaGlow"); 
			alphaFullGlowEntity->setRenderQueueGroup(RENDER_QUEUE_FULL_GLOW_ALPHA_GLOW); 
			alphaFullGlowEntity->setMaterialName("cg/alpha_glow");
			alphaFullGlowEntity->setVisibilityFlags(Common::Data::NON_TEXTURE_MASK);
			Ogre::SceneNode* alphaFullGlowNode = m_ogreNode->createChildSceneNode(m_ogreNode->getName() + "FullGlowAlpha"); 
			if(entityInfo->hasAttribute("glow_scale")){
				float tmp(entityInfo->getFloatAttribute("glow_scale"));
				alphaFullGlowNode->setScale(tmp,tmp,tmp);
			}else
				alphaFullGlowNode->setScale(1.5,1.5,1.5);
			alphaFullGlowNode->attachObject(alphaFullGlowEntity); 
			alphaFullGlowEntity->setVisible(entityInfo->getBoolAttribute("full_glow"));

			// full glow alpha glow
			Ogre::Entity* glowFullGlowEntity = m_ogreEntity->clone(m_ogreEntity->getName() + "_fullglow"); 
			glowFullGlowEntity->setRenderQueueGroup(RENDER_QUEUE_FULL_GLOW_GLOW); 
			glowFullGlowEntity->setMaterialName("cg/no_depth_check_glow"); 
			glowFullGlowEntity->setVisibilityFlags(Common::Data::NON_TEXTURE_MASK);
			Ogre::SceneNode* glowFullGlowNode = m_ogreNode->createChildSceneNode(m_ogreNode->getName() + "FullGlowGlow"); 
			if(entityInfo->hasAttribute("glow_scale")){
				float tmp(entityInfo->getFloatAttribute("glow_scale"));
				glowFullGlowNode->setScale(tmp,tmp,tmp);
			}else
				glowFullGlowNode->setScale(1.5,1.5,1.5);

			glowFullGlowNode->attachObject(glowFullGlowEntity);
			glowFullGlowEntity->setVisible(entityInfo->getBoolAttribute("full_glow"));

			alphaFullGlowEntity->setQueryFlags(Common::Data::NULL_MASK);
			glowFullGlowEntity->setQueryFlags(Common::Data::NULL_MASK);

			alphaFullGlowNode->setVisible(false);

			if(entityInfo->hasAttribute("green_glow")){
				// full glow alpha glow
				Ogre::Entity* alphaFullGreenGlowEntity = m_ogreEntity->clone(m_ogreEntity->getName() + "_alphaGreenGlow"); 
				alphaFullGreenGlowEntity->setRenderQueueGroup(RENDER_QUEUE_FULL_GLOW_ALPHA_GLOW); 
				alphaFullGreenGlowEntity->setMaterialName("cg/alpha_greenglow");
				alphaFullGreenGlowEntity->setVisibilityFlags(Common::Data::NON_TEXTURE_MASK);
				Ogre::SceneNode* alphaFullGreenGlowNode = m_ogreNode->createChildSceneNode(m_ogreNode->getName() + "FullGreenGlowAlpha"); 
				if(entityInfo->hasAttribute("glow_scale")){
					float tmp(entityInfo->getFloatAttribute("glow_scale"));
					alphaFullGreenGlowNode->setScale(tmp,tmp,tmp);
				}else
					alphaFullGreenGlowNode->setScale(1.5,1.5,1.5);
				alphaFullGreenGlowNode->attachObject(alphaFullGlowEntity); 
				alphaFullGreenGlowEntity->setVisible(entityInfo->getBoolAttribute("green_glow"));

				// full glow alpha glow
				Ogre::Entity* glowFullGreenGlowEntity = m_ogreEntity->clone(m_ogreEntity->getName() + "_fullgreenglow"); 
				glowFullGlowEntity->setRenderQueueGroup(RENDER_QUEUE_FULL_GLOW_GLOW); 
				glowFullGlowEntity->setMaterialName("cg/no_depth_check_greenglow"); 
				glowFullGlowEntity->setVisibilityFlags(Common::Data::NON_TEXTURE_MASK);
				Ogre::SceneNode* glowFullGreenGlowNode = m_ogreNode->createChildSceneNode(m_ogreNode->getName() + "FullGreenGlowGlow"); 
				if(entityInfo->hasAttribute("glow_scale")){
					float tmp(entityInfo->getFloatAttribute("glow_scale"));
					glowFullGreenGlowNode->setScale(tmp,tmp,tmp);
				}else
					glowFullGreenGlowNode->setScale(1.5,1.5,1.5);

				glowFullGreenGlowNode->attachObject(glowFullGlowEntity);
				glowFullGreenGlowEntity->setVisible(entityInfo->getBoolAttribute("green_glow"));

				alphaFullGreenGlowEntity->setQueryFlags(Common::Data::NULL_MASK);
				glowFullGreenGlowEntity->setQueryFlags(Common::Data::NULL_MASK);

				alphaFullGlowNode->setVisible(false);
			}

		}else if(entityInfo->hasAttribute("glow")){

			m_ogreEntity->setRenderQueueGroup(RENDER_QUEUE_OUTLINE_GLOW_OBJECTS);

			// outline glow entity actual glow
			Ogre::Entity* actualOutlineGlowEntity = m_ogreEntity->clone(m_ogreEntity->getName() + "_glow");
			actualOutlineGlowEntity->setRenderQueueGroup(RENDER_QUEUE_OUTLINE_GLOW_GLOWS);
			actualOutlineGlowEntity->setMaterialName("cg/glow");
			actualOutlineGlowEntity->setVisibilityFlags(Common::Data::NON_TEXTURE_MASK);
			Ogre::SceneNode* actualOutlineGlowNode = m_ogreNode->createChildSceneNode(m_ogreNode->getName() + "OutlineGlow");
			if(entityInfo->hasAttribute("glow_scale")){
				float tmp(entityInfo->getFloatAttribute("glow_scale"));
				actualOutlineGlowNode->setScale(tmp,tmp,tmp);
			}else
				actualOutlineGlowNode->setScale(1.5,1.5,1.5);
			actualOutlineGlowEntity->setVisible(entityInfo->getBoolAttribute("glow"));
			actualOutlineGlowNode->attachObject(actualOutlineGlowEntity);

			actualOutlineGlowEntity->setQueryFlags(Common::Data::NULL_MASK);

			actualOutlineGlowNode->setVisible(false);

			if(entityInfo->hasAttribute("greenglow")){
				
				m_ogreEntity->setRenderQueueGroup(RENDER_QUEUE_OUTLINE_GLOW_OBJECTS);

				// outline glow entity actual glow
				Ogre::Entity* actualOutlineGreenGlowEntity = m_ogreEntity->clone(m_ogreEntity->getName() + "_greenglow");
				actualOutlineGreenGlowEntity->setRenderQueueGroup(RENDER_QUEUE_OUTLINE_GLOW_GLOWS);
				actualOutlineGreenGlowEntity->setMaterialName("cg/greenglow");
				actualOutlineGreenGlowEntity->setVisibilityFlags(Common::Data::NON_TEXTURE_MASK);
				Ogre::SceneNode* actualOutlineGreenGlowNode = m_ogreNode->createChildSceneNode(m_ogreNode->getName() + "OutlineGreenGlow");
				if(entityInfo->hasAttribute("glow_scale")){
					float tmp(entityInfo->getFloatAttribute("glow_scale"));
					actualOutlineGreenGlowNode->setScale(tmp,tmp,tmp);
				}else
					actualOutlineGreenGlowNode->setScale(1.5,1.5,1.5);
				actualOutlineGreenGlowEntity->setVisible(entityInfo->getBoolAttribute("greenglow"));
				actualOutlineGreenGlowNode->attachObject(actualOutlineGreenGlowEntity);

				actualOutlineGreenGlowEntity->setQueryFlags(Common::Data::NULL_MASK);

				actualOutlineGreenGlowNode->setVisible(false);

			}

		}

		return m_ogreEntity;
	}

	bool CGraphics::activate()
	{
		if(!IComponent::activate()) return false;

		if(m_ogreEntity) m_ogreEntity->setVisible(true);

		return true;
	}

	void CGraphics::deactivate()
	{
		IComponent::deactivate();
		if(m_ogreEntity) m_ogreEntity->setVisible(false);
	}
	/*
	void CGraphics::switchOgreEntities()
	{
		//m_ogreNode->detachAllObjects();
		if(m_isOgrePrimitive){
			//m_ogreNode->attachObject(m_ogreEntity);
			m_ogreEntity->setVisible(true);
			m_ogrePrimitive->setVisible(false);
			m_isOgrePrimitive = false;
		} else {
			//m_ogreNode->attachObject(m_ogrePrimitive);
			m_ogreEntity->setVisible(false);
			m_ogrePrimitive->setVisible(true);
			m_isOgrePrimitive = true;
		}
	}
	*/
}

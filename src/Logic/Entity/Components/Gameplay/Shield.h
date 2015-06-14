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

#ifndef __LOGIC_SHIELD_H
#define __LOGIC_SHIELD_H

#include "../Component.h"

namespace Logic
{
	namespace Component
	{
        class CShield : public IComponent
        {
            DEC_FACTORY(CShield);
        public:
            CShield() : IComponent(), m_resistance(0.0f), m_value(nullptr), m_regenVal(nullptr) 
            { }

            ~CShield();

            bool spawn(CEntity* entity, CScene* scene, const Map::CMapEntity* entityInfo);

            void tick(unsigned int);

            /*
            factor: 1, 2, 3... increasse the damage absorbed by the shield
            damage: amount of damage given by a weapon
            */
            void decreaseShield(const unsigned& damage, const unsigned& factor = 1);

            void increaseShield(const unsigned& value) { *m_value += value; }
            void regenShield() { *m_value += (*m_maxShield * 0.01 * (*m_regenVal)); }

            void destroyShield() { *m_value = 0; }

            unsigned getValue() const { return *m_value; }

            const unsigned& getMaxShield() { return *m_maxShield; }

            bool isFull() { return (*m_value == *m_maxShield); }

            bool m_player;

        private:
            float     m_resistance; // percentage of damage absorbed. Must be between 0..1
            unsigned* m_value; //"life" of the shield
            unsigned* m_maxShield; // min value 100
            unsigned* m_regenVal;
            CEntity*  m_subEntity;

            void createShield(CEntity*, CScene*);
        };

        REG_FACTORY(CShield);
    }
}

#endif
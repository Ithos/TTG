#include "Shoots.h"

#include "Logic/Entity/Entity.h"
#include "Logic/Entity/Components/Movement/Controller.h"

using namespace Common::Input::Action;

namespace GUI
{
	namespace Controller
	{
		namespace Command
		{
            void CPrimaryShoot::execute() {}
			void CPrimaryShoot::execute(Logic::CEntity *entity) {}

			void CPrimaryShoot::execute(TKeyMouseAction action, Logic::CEntity *entity)
			{
				static_cast<Logic::Component::CController*>(entity->getComponentByName("CController"))
                    ->primaryShoot(action == Common::Input::Action::KEY_PRESSED);
			}

            //---------- 2º shoot
            void CSecondaryShoot::execute() {}

            void CSecondaryShoot::execute(Logic::CEntity* entity) { }

            void CSecondaryShoot::execute(TKeyMouseAction action,  Logic::CEntity *entity)
            {
                static_cast<Logic::Component::CController*>(entity->getComponentByName("CController"))
                    ->secondaryShoot(action == Common::Input::Action::KEY_PRESSED);
            }
		}
	}
}
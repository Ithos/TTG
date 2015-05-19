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

#ifndef __COMMON_INPUT_INPUT_H
#define __COMMON_INPUT_INPUT_H

#include <OISMouse.h>
#include <OISKeyboard.h>

namespace Common
{
	namespace Input
	{
		namespace Button
		{
			enum TMouseButton
			{
				UNASSIGNED = 0xFFFFFFFF,
				LEFT       = OIS::MB_Left,
				RIGHT      = OIS::MB_Right,
				MIDDLE     = OIS::MB_Middle,
				BUTTON_3   = OIS::MB_Button3,
				BUTTON_4   = OIS::MB_Button4,
				BUTTON_5   = OIS::MB_Button5,
				BUTTON_6   = OIS::MB_Button6,
				BUTTON_7   = OIS::MB_Button7
			};
		}

		namespace Key
		{
			enum TKeyID
			{
				UNASSIGNED    = OIS::KC_UNASSIGNED,
				ESCAPE        = OIS::KC_ESCAPE,
				NUMBER1       = OIS::KC_1,
				NUMBER2       = OIS::KC_2,
				NUMBER3       = OIS::KC_3,
				NUMBER4       = OIS::KC_4,
				NUMBER5       = OIS::KC_5,
				NUMBER6       = OIS::KC_6,
				NUMBER7       = OIS::KC_7,
				NUMBER8       = OIS::KC_8,
				NUMBER9       = OIS::KC_9,
				NUMBER0       = OIS::KC_0,
				MINUS         = OIS::KC_MINUS,    // - on main keyboard
				EQUALS        = OIS::KC_EQUALS,
				BACK          = OIS::KC_BACK,    // backspace
				TAB           = OIS::KC_TAB,
				Q             = OIS::KC_Q,
				W             = OIS::KC_W,
				E             = OIS::KC_E,
				R             = OIS::KC_R,
				T             = OIS::KC_T,
				Y             = OIS::KC_Y,
				U             = OIS::KC_U,
				I             = OIS::KC_I,
				O             = OIS::KC_O,
				P             = OIS::KC_P,
				LBRACKET      = OIS::KC_LBRACKET,
				RBRACKET      = OIS::KC_RBRACKET,
				RETURN        = OIS::KC_RETURN,    // Enter on main keyboard
				LCONTROL      = OIS::KC_LCONTROL,
				A             = OIS::KC_A,
				S             = OIS::KC_S,
				D             = OIS::KC_D,
				F             = OIS::KC_F,
				G             = OIS::KC_G,
				H             = OIS::KC_H,
				J             = OIS::KC_J,
				K             = OIS::KC_K,
				L             = OIS::KC_L,
				NGRAVE        = OIS::KC_SEMICOLON,    // Ñ
				APOSTROPHE    = OIS::KC_APOSTROPHE,
				GRAVE         = OIS::KC_GRAVE,    // accent
				LSHIFT        = OIS::KC_LSHIFT,
				BACKSLASH     = OIS::KC_BACKSLASH,
				Z             = OIS::KC_Z,
				X             = OIS::KC_X,
				C             = OIS::KC_C,
				V             = OIS::KC_V,
				B             = OIS::KC_B,
				N             = OIS::KC_N,
				M             = OIS::KC_M,
				COMMA         = OIS::KC_COMMA,
				PERIOD        = OIS::KC_PERIOD,    // . on main keyboard
				SLASH         = OIS::KC_SLASH,    // / on main keyboard
				RSHIFT        = OIS::KC_RSHIFT,
				MULTIPLY      = OIS::KC_MULTIPLY,    // * on numeric keypad
				LALT          = OIS::KC_LMENU,    // left Alt
				SPACE         = OIS::KC_SPACE,
				CAPITAL       = OIS::KC_CAPITAL,
				F1            = OIS::KC_F1,
				F2            = OIS::KC_F2,
				F3            = OIS::KC_F3,
				F4            = OIS::KC_F4,
				F5            = OIS::KC_F5,
				F6            = OIS::KC_F6,
				F7            = OIS::KC_F7,
				F8            = OIS::KC_F8,
				F9            = OIS::KC_F9,
				F10           = OIS::KC_F10,
				NUMLOCK       = OIS::KC_NUMLOCK,
				SCROLL        = OIS::KC_SCROLL,    // Scroll Lock
				NUMPAD7       = OIS::KC_NUMPAD7,
				NUMPAD8       = OIS::KC_NUMPAD8,
				NUMPAD9       = OIS::KC_NUMPAD9,
				SUBTRACT      = OIS::KC_SUBTRACT,    // - on numeric keypad
				NUMPAD4       = OIS::KC_NUMPAD4,
				NUMPAD5       = OIS::KC_NUMPAD5,
				NUMPAD6       = OIS::KC_NUMPAD6,
				ADD           = OIS::KC_ADD,    // + on numeric keypad
				NUMPAD1       = OIS::KC_NUMPAD1,
				NUMPAD2       = OIS::KC_NUMPAD2,
				NUMPAD3       = OIS::KC_NUMPAD3,
				NUMPAD0       = OIS::KC_NUMPAD0,
				DECIMAL       = OIS::KC_DECIMAL,    // . on numeric keypad
				OEM_102       = OIS::KC_OEM_102,    // < > | on UK/Germany keyboards
				F11           = OIS::KC_F11,
				F12           = OIS::KC_F12,
				NUMPADEQUALS  = OIS::KC_NUMPADEQUALS,    // = on numeric keypad (NEC PC98)
				PREVTRACK     = OIS::KC_PREVTRACK,    // Previous Track (OIS::KC_CIRCUMFLEX on Japanese keyboard)
				AT            = OIS::KC_AT,    //                     (NEC PC98)
				COLON         = OIS::KC_COLON,    //                     (NEC PC98)
				UNDERLINE     = OIS::KC_UNDERLINE,    //                     (NEC PC98)
				STOP          = OIS::KC_STOP,    //                     (NEC PC98)
				NEXTTRACK     = OIS::KC_NEXTTRACK,    // Next Track
				NUMPADENTER   = OIS::KC_NUMPADENTER,    // Enter on numeric keypad
				RCONTROL      = OIS::KC_RCONTROL,
				MUTE          = OIS::KC_MUTE,    // Mute
				CALCULATOR    = OIS::KC_CALCULATOR,    // Calculator
				PLAYPAUSE     = OIS::KC_PLAYPAUSE,    // Play / Pause
				MEDIASTOP     = OIS::KC_MEDIASTOP,    // Media Stop
				VOLUMEDOWN    = OIS::KC_VOLUMEDOWN,    // Volume -
				VOLUMEUP      = OIS::KC_VOLUMEUP,    // Volume +
				WEBHOME       = OIS::KC_WEBHOME,    // Web home
				NUMPADCOMMA   = OIS::KC_NUMPADCOMMA,    // , on numeric keypad (NEC PC98)
				DIVIDE        = OIS::KC_DIVIDE,    // / on numeric keypad
				SYSRQ         = OIS::KC_SYSRQ,
				RALT          = OIS::KC_RMENU,    // right Alt
				PAUSE         = OIS::KC_PAUSE,    // Pause
				HOME          = OIS::KC_HOME,    // Home on arrow keypad
				UPARROW       = OIS::KC_UP,    // UpArrow on arrow keypad
				PGUP          = OIS::KC_PGUP,    // PgUp on arrow keypad
				LEFTARROW     = OIS::KC_LEFT,    // LeftArrow on arrow keypad
				RIGHTARROW    = OIS::KC_RIGHT,    // RightArrow on arrow keypad
				END           = OIS::KC_END,    // End on arrow keypad
				DOWNARROW     = OIS::KC_DOWN,    // DownArrow on arrow keypad
				PGDOWN        = OIS::KC_PGDOWN,    // PgDn on arrow keypad
				INSERT        = OIS::KC_INSERT,    // Insert on arrow keypad
				DEL           = OIS::KC_DELETE,    // Delete on arrow keypad
				LWIN          = OIS::KC_LWIN,    // Left Windows key
				RWIN          = OIS::KC_RWIN,    // Right Windows key
				APPS          = OIS::KC_APPS,    // AppMenu key
				POWER         = OIS::KC_POWER,    // System Power
				SLEEP         = OIS::KC_SLEEP,    // System Sleep
				WAKE          = OIS::KC_WAKE,    // System Wake
				WEBSEARCH     = OIS::KC_WEBSEARCH,    // Web Search
				WEBFAVORITES  = OIS::KC_WEBFAVORITES,    // Web Favorites
				WEBREFRESH    = OIS::KC_WEBREFRESH,    // Web Refresh
				WEBSTOP       = OIS::KC_WEBSTOP,    // Web Stop
				WEBFORWARD    = OIS::KC_WEBFORWARD,    // Web Forward
				WEBBACK       = OIS::KC_WEBBACK,    // Web Back
				MYCOMPUTER    = OIS::KC_MYCOMPUTER,    // My Computer
				MAIL          = OIS::KC_MAIL,    // Mail
				MEDIASELECT   = OIS::KC_MEDIASELECT     // Media Select
			};
		}

		namespace Modifier
		{
			enum TModifier
			{
				SHIFT  = OIS::Keyboard::Shift,
				CTRL   = OIS::Keyboard::Ctrl,
				ALT    = OIS::Keyboard::Alt
			};
		}

		struct TKey
		{
			TKey(const unsigned int text,const Key::TKeyID id)
			{
				this->text = text; this->id = id;
			}
			unsigned int text;
			Key::TKeyID id;
		};

		namespace Action
		{
			enum TKeyMouseAction
			{
				UNASSIGNED = 0xFFFFFFFF,
				KEY_PRESSED = 0,
				KEY_RELEASED,
				MOUSE_PRESSED,
				MOUSE_RELEASED,
				MOUSE_MOVED
			};
		}
	}
}

#endif
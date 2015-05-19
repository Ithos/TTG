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

#ifndef __LOGIC_COMPONENTFACTORY_H
#define __LOGIC_COMPONENTFACTORY_H

#include <string>
#include <map>

namespace Logic
{
	namespace Component
	{
		class IComponent;
	}
}

namespace Logic
{

	template <typename T>
	class CFactory
	{
	protected:
		typedef T (*FunctionPtr)();
	public:
		CFactory(){}
		virtual ~CFactory();

		void add(FunctionPtr func, const std::string &name);
		bool has(const std::string &name) const;
		T create(const std::string &name) const;
	protected:
		typedef std::pair<std::string, FunctionPtr> TStringFunctionPtrPair;
		typedef std::map<std::string, FunctionPtr> TFunctionPtrMap;

		TFunctionPtrMap m_table;
	private:
	};

	//template <typename T>
	//inline CFactory<T>::CFactory(){}

	template <typename T>
	inline CFactory<T>::~CFactory()
	{
		TFunctionPtrMap::const_iterator it = m_table.begin();
		TFunctionPtrMap::const_iterator it2;

		while( it != m_table.end()){
			it2 = it;
			it++;
			m_table.erase(it2);
		}
	}

	template <typename T>
	inline void CFactory<T>::add(FunctionPtr func, const std::string &name)
	{
		TStringFunctionPtrPair element(name,func);
		m_table.insert(element);
	}

	template <typename T>
	inline bool CFactory<T>::has(const std::string &name) const
	{
		return m_table.count(name) > 0;
	}

	template <typename T>
	inline T CFactory<T>::create(const std::string &name) const
	{
		if(has(name)){
			TFunctionPtrMap::const_iterator it = m_table.find(name);
			if(it != m_table.end()){
				return m_table.find(name)->second();
			}
		}

		throw new std::exception("Requested function does not exist.");

	}

	class CComponentFactory : public CFactory<Component::IComponent*>
	{
	public:
		static CComponentFactory* getInstance();
	protected:
	private:
		CComponentFactory();
		~CComponentFactory();

		static CComponentFactory* m_instance;
	};
}



#endif
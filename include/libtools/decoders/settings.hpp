
/*

    This file is part of libtools.

    Foobar is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

*/

/**
  * THIS FILE IS PART OF LIBTOOLS 
  * SECURITY LEVEL : 8 (CRITICAL)  
  * VISIBILITY     : PROTECTED
  * © COPYDOWN™ LAMOGUI ALL RIGHTS RESERVED 
  *
  * FILE         : settings.hpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : 1.1 olol
  * DEPENDENCIES : config.h
  *                variant.hpp
  * ROLE         : Definie un clone de QSettings pour lire des fichiers ini
  */


#ifndef LIBTOOLS_SETTINGS_HPP
#define LIBTOOLS_SETTINGS_HPP

#include <fstream>
#include <map>
#include <iostream>
#include <libtools/public/config.h>
#include <libtools/core/variant.hpp>

LIBTOOLS_PRIVATE bool GetSettingsFor(const string_t& key, bool defValue);
LIBTOOLS_PRIVATE unsigned int GetSettingsFor(const string_t& key, unsigned int defValue);
LIBTOOLS_PRIVATE int GetSettingsFor(const string_t&, int defValue);
LIBTOOLS_PRIVATE float GetSettingsFor(const string_t& key, float defValue);
LIBTOOLS_PRIVATE string_t GetSettingsFor(const string_t& key, const string_t& defValue);

LIBTOOLS_PRIVATE void SetSettings(string_t key,const Variant &yourvalue);

class LIBTOOLS_PRIVATE Settings 
{
	public :
		bool loadFile(const char* filename);
		void setValue(const string_t& key,const Variant &yourvalue);
		Variant value(const string_t& key);
	   static Settings& getInstance();
	
	private :
		Settings& operator= (const Settings&){ return *this; }
		Settings (const Settings&){}
		Settings();
		~Settings();
	
		static Settings _instance;
		void saveFile();
		
		std::map<string_t,Variant> _map;
		std::string _filename;
};

#endif

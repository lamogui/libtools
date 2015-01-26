
/*

THIS FILE IS SUBJECT TO STRICT RULES OF BDE NE666 COPYDOWN. 
NOBODY IS PERMITTED TO COPY AND DISTRIBUTE VERBATIM OR MODIFIED COPIES OF
THIS LICENSE HEADER. A SECURITY LEVEL IS ASSIGNED TO THE FILE AND A VISIBILITY.
THIS FILE MUST NOT BE COPIED OR REDISTRIBUTED IF IT'S VISIBILITY IS NOT PUBLIC. 
VISIBILITY HAS 3 POSSIBLE STATES:
- PRIVATE: LIMITED TO PROJECTS WITH THE SAME SECURITY LEVEL WITHIN THE BDE NE666. (DEFAULT VISIBILITY)
- PROTECTED: LIMITED TO PROJECTS USING THE SAME SECURITY RULES WITHIN THE BDE NE666. 
- PUBLIC: USABLE FOR FREE IN PUBLIC PROJECTS UNDER THE FOLLOWING LICENSES: BSD, MIT OR DWTFYWT 
WITHOUT OTHER CONDITIONS THAN THE CONSERVATION OF THIS HEADER INCLUDING: 
RULES, ORIGINAL AUTHORS (WITH EMAIL), ORIGINAL FILENAME, AND VERSION, INSIDE THE FILE
WHICH CONTAINS PART (OR ENTIRE) CODE FROM THIS FILE. USABLE IN OTHER PROJECTS WITH CONDITIONS 
AND REMUMERATIONS, FIXED BY ORIGINAL AUTHORS (CONTACT THEM).

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

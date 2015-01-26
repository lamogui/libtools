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
  * VISIBILITY     : PRIVATE
  * © COPYDOWN™ LAMOGUI ALL RIGHTS RESERVED 
  *
  * FILE         : settings.cpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : 1.1 olol
  * DEPENDENCIES : settings.hpp
  * ROLE         : Definie un clone de QSettings pour lire des fichiers ini
  */

#include <libtools/core/settings.hpp>

Settings Settings::_instance = Settings();

Settings::Settings() {}

Settings::~Settings() {}


Settings& Settings::getInstance()
{
	return _instance;
}

void Settings::saveFile()
{
	if (_filename.empty())
	{
		std::cerr << "failled to save the file no filename" << std::endl;
		return; 
	}
	std::ofstream strm(_filename.c_str(), std::ofstream::binary);
	if (!strm.is_open())
	{
		std::cerr << "cannot write on file : "  << _filename << std::endl;
		return; 
	}
	string_t head;
	for (std::map<string_t,Variant>::iterator it = _map.begin(); it != _map.end(); it++)
	{
		string_t newHead;
    std::size_t find_ret=it->first.find('/');
		if (string_t_find_success(find_ret))
		{
			newHead = it->first;
			newHead.erase(find_ret,string_t_size(newHead));
			newHead = '[' + newHead + ']';
		}
		else 
			newHead = "[General]";
			
		if (head != newHead && it == _map.begin()) {
			head = newHead; strm << string_t_to_std(head) << "\r\n\r\n";
    }
		else if (head != newHead) {
			head = newHead; strm << "\r\n" << string_t_to_std(head) << "\r\n";  
    }
		string_t copyfit = it->first;
		copyfit.erase(0,copyfit.find('/') + 1);
		strm << string_t_to_std(copyfit) << " = ";
		if (string_t_find_success(it->second.toString().find(' ')))
			strm << "\"" << string_t_to_std(it->second.toString()) << "\";\r\n";
		else
			strm << string_t_to_std(it->second.toString()) << ";\r\n";
	}
}

bool Settings::loadFile(const char* filename)
{
	if (filename == NULL || filename[0] == '\0')
	{
		std::cerr << "falled to read the file : no filename" << std::endl;
		return false; 
	}
	_filename = filename;
	std::ifstream strm(filename, std::ifstream::binary);
	if (!strm.is_open())
	{
		std::cerr << "cannot open file : "  << filename << std::endl;
		return false; 
	}
	
	string_t section = "General", currentKey;
  std::string fileString, currentVariant;
	bool ignoreSpaces = true,isInSection = false, isInKey = true, isInValue = false;
	
	while (!strm.eof() && !strm.bad())
	{
		char currentChar;
		strm.get(currentChar);
		fileString += currentChar;
	}
	strm.close();
	for (unsigned int i = 0; i < fileString.size(); i++)
	{
		switch (fileString[i]) 
		{
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f':
			case 'g':
			case 'h':
			case 'i':
			case 'j':
			case 'k':
			case 'l':
			case 'm':
			case 'n':
			case 'o':
			case 'p':
			case 'q':
			case 'r':
			case 's':
			case 't':
			case 'u':
			case 'v':
			case 'w':
			case 'x':
			case 'y':
			case 'z':
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':
			case 'G':
			case 'H':
			case 'I':
			case 'J':
			case 'K':
			case 'L':
			case 'M':
			case 'N':
			case 'O':
			case 'P':
			case 'Q':
			case 'R':
			case 'S':
			case 'T':
			case 'U':
			case 'V':
			case 'W':
			case 'X':
			case 'Y':
			case 'Z':
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '-':
			case '_':
			case '\'':
			case '/':
			case '\\':
			case ',':
			case '.':
			case '!':
			case '?':
			case '(':
			case ')':
				if (isInSection)
					section += fileString[i];
				else if (isInKey)
					currentKey += fileString[i];
				else if (isInValue)
					currentVariant += fileString[i];
				break;
			case ' ':
				if (ignoreSpaces)
					break;
				if (isInSection)
					section += fileString[i];
				else if (isInKey)
					currentKey += fileString[i];
				else if (isInValue)
					currentVariant += fileString[i];
				break;
			case '[':
				if (i == 0 || (i > 0 && fileString[i-1] == '\n')) 
				{
					ignoreSpaces = true;
					isInValue = false;
					isInKey = false;
					isInSection = true;
					if (!string_t_empty(currentKey) && 
              !string_t_empty(section) && 
              !currentVariant.empty())
						_map[section + '/' + currentKey] = Variant(currentVariant);
					section = "";
					currentKey = "";
					currentVariant = "";
				}
				else 
				{
					if (isInSection)
						section += fileString[i];
					else if (isInKey)
						currentKey += fileString[i];
					else if (isInValue)
						currentVariant += fileString[i];
				}
				break;
			case ']':
				if (isInSection) {
					isInValue = false;
					isInKey = true;
					isInSection = false;
				}
				else
				{
					if (isInSection)
						section += fileString[i];
					else if (isInKey)
						currentKey += fileString[i];
					else if (isInValue)
						currentVariant += fileString[i];
				}
				break;
			case '\"':
				if (!isInValue)
					break;
				if (ignoreSpaces)
					ignoreSpaces = false;
				else
					ignoreSpaces = true;
				break;
			case '=':
				if (isInKey)
				{
					isInValue = true;
					isInKey = false;
					isInSection = false;
				}
				break;
			case ';':
				if (isInValue)
				{
					isInValue = false;
					isInKey = true;
					isInSection = false;
					if (!string_t_empty(currentKey)
           && !string_t_empty(section) 
           && !currentVariant.empty())
						_map[section + '/' + currentKey] = Variant(currentVariant);
					currentKey = "";
					currentVariant = "";
				}
				break;
			default:
				break;
		}
	}
	return true;
}

void Settings::setValue(const string_t& key, const Variant &yourvalue)
{
	if (string_t_find_failed(key.find('/')))
		_map["General/" + key] = yourvalue;
  else
    _map[key] = yourvalue;
	saveFile();
}

Variant Settings::value(const string_t& key)
{
	if (_map.find(key) != _map.end())
		return _map.find(key)->second;
	else if (_map.find("General/" + key) != _map.end())
		return _map.find("General/" + key)->second;
	return Variant();
}

bool GetSettingsFor(const string_t& key, bool defValue)
{
  bool defaultValue=true;
  bool v = Settings::getInstance().value(key).toBool(&defaultValue);
  if (defaultValue)
  {
    Settings::getInstance().setValue(key,Variant(defValue));
    return defValue;
  }
	return v;
}

unsigned int GetSettingsFor(const string_t& key, unsigned int defValue)
{
  bool defaultValue=true;
  unsigned int v = Settings::getInstance().value(key).toUInt(&defaultValue);
  if (defaultValue)
  {
    Settings::getInstance().setValue(key,Variant(defValue));
    return defValue;
  }
	return v;
}

int GetSettingsFor(const string_t& key, int defValue)
{
  bool defaultValue=true;
  int v = Settings::getInstance().value(key).toInt(&defaultValue);
  if (defaultValue)
  {
    Settings::getInstance().setValue(key,Variant(defValue));
    return defValue;
  }
	return v;
}

float GetSettingsFor(const string_t& key, float defValue)
{
  bool defaultValue=true;
  float v = Settings::getInstance().value(key).toFloat(&defaultValue);
  if (defaultValue)
  {
    Settings::getInstance().setValue(key,Variant(defValue));
    return defValue;
  }
	return v;
}

string_t GetSettingsFor(const string_t& key, const string_t& defValue)
{
  string_t v = Settings::getInstance().value(key).toString();
  if (string_t_empty(v))
  {
    Settings::getInstance().setValue(key,Variant(defValue));
    return defValue;
  }
	return v;
}

void SetSettings(const string_t& key,const Variant &yourvalue)
{
	Settings::getInstance().setValue(key,yourvalue);
}

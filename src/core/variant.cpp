/*

THIS FILE IS SUBJECT TO STRICT RULES OF BDE NE777 COPYDOWN. 
NOBODY IS PERMITTED TO COPY AND DISTRIBUTE VERBATIM OR MODIFIED COPIES OF
THIS LICENSE HEADER. A SECURITY LEVEL IS ASSIGNED TO THE FILE AND A VISIBILITY.
THIS FILE MUST NOT BE COPIED OR REDISTRIBUTED IF IT'S VISIBILITY IS NOT PUBLIC. 
VISIBILITY HAS 3 POSSIBLE STATES:
- PRIVATE: LIMITED TO PROJECTS WITH THE SAME SECURITY LEVEL WITHIN THE BDE NE777. (DEFAULT VISIBILITY)
- PROTECTED: LIMITED TO PROJECTS USING THE SAME SECURITY RULES WITHIN THE BDE NE777. 
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
  * FILE         : variant.cpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : 1.1 olol
  * DEPENDENCIES : variant.hpp
  * ROLE         : Un équivalent à QVariant plus simple
  */


#include <libtools/core/variant.hpp>
#include <sstream>

Variant::Variant() : _str() {}

Variant::Variant(bool value) : _str()
{
	if (value)
		_str = "true";
	else
		_str = "false";
}

Variant::Variant(int value) : _str()
{
		std::stringstream strm;
		strm << value;
		_str = strm.str();
}

Variant::Variant(unsigned int value) : _str()
{
		std::stringstream strm;
		strm << value;
		_str = strm.str();
}

Variant::Variant(float value) : _str()
{
		std::stringstream strm;
		strm << value;
		_str = strm.str();
}

Variant::Variant(const string_t& str) : _str(str) {}

Variant::Variant(const char* str) : _str(str) {}

Variant::Variant(char* str) : _str(str) {}

Variant::Variant(const Variant& copy) : _str(copy._str) {}

Variant& Variant::operator= (const Variant& copy)
{
		_str = copy._str;
		return *this;
}

Variant& Variant::operator= (const int& number)
{
		std::stringstream strm;
		strm << number;
		_str = strm.str();
		return *this;
}

Variant& Variant::operator= (const unsigned int &number)
{
		std::stringstream strm;
		strm << number;
		_str = strm.str();
		return *this;
}

Variant& Variant::operator= (const float& number)
{
		std::stringstream strm;
		strm << number;
		_str = strm.str();
		return *this;
}

Variant& Variant::operator= (const string_t& str)
{
	_str = str;
	return *this;
}

Variant::~Variant() {}
		
bool Variant::toBool(bool *isDefaultValue) const
{
  std::string str = string_t_to_std(_str);

	if (str.empty() || str[0] == ' ')
	{
		if (isDefaultValue != NULL)
			*isDefaultValue = true;
		return false;
	}
	bool containNumber = true;
	bool containZero = true;
	for (unsigned int i = 0 ; i < str.size(); i++)
	{
		if (containZero && (str[i] != '0' || str[i] != '-' || str[i] != '.' || str[i] != ',' || str[i] != ' '))
			containZero = false;
		if (containNumber && ((str[i] >= '0' && str[i] <= '9') ||  str[i] != '-' || str[i] != '.' || str[i] != ',' || str[i] != ' '))
			containNumber = false;
		if (i < str.size() - 3 && (str[i] == 't' || str[i] == 'T') && (str[i+1] == 'r' || str[i+1] == 'R') && (str[i+2] == 'u' || str[i+2] == 'U') && (str[i+3] == 'e' || str[i+3] == 'E'))
		{
			if (isDefaultValue != NULL)
				*isDefaultValue = false;
			return true;
		}
		else if (i < str.size() - 4 && (str[i] == 'f' || str[i] == 'F') && (str[i+1] == 'a' || str[i+1] == 'A') && (str[i+2] == 'l' || str[i+2] == 'L') && (str[i+3] == 's' || str[i+3] == 'S') && (str[i+4] == 'e' || str[i+4] == 'E'))
		{
			if (isDefaultValue != NULL)
				*isDefaultValue = false;
			return false;
		}
	}
	if (containZero)
	{
		if (isDefaultValue != NULL)
			*isDefaultValue = false;
		return false;
	}
	if (containNumber)
	{
		if (isDefaultValue != NULL)
			*isDefaultValue = false;
		return true;
	}
	
	if (isDefaultValue != NULL)
		*isDefaultValue = true;
	return false;
}

int Variant::toInt(bool *isDefaultValue) const
{
  std::string str = string_t_to_std(_str);
	int value = 0;
	bool invert = false;
	if (isDefaultValue != NULL)
		*isDefaultValue = true;
	for (unsigned int i = 0 ; i < str.size(); i++)
	{
		if (str[i] == '-' && value == 0)
			invert = true;
		
		switch (str[i])
		{
			case '0':
				value *= 10;
				if (isDefaultValue != NULL)
					*isDefaultValue = false;
				break;
			case '1':
				value *= 10;
				value += 1;
				if (isDefaultValue != NULL)
					*isDefaultValue = false;
				break;
			case '2':
				value *= 10;
				value += 2;
				if (isDefaultValue != NULL)
					*isDefaultValue = false;
				break;
			case '3':
				value *= 10;
				value += 3;
				if (isDefaultValue != NULL)
					*isDefaultValue = false;
				break;	
			case '4':
				value *= 10;
				value += 4;
				if (isDefaultValue != NULL)
					*isDefaultValue = false;
				break;	
			case '5':
				value *= 10;
				value += 5;
				if (isDefaultValue != NULL)
					*isDefaultValue = false;
				break;	
			case '6':
				value *= 10;
				value += 6;
				if (isDefaultValue != NULL)
					*isDefaultValue = false;
				break;	
			case '7':
				value *= 10;
				value += 7;
				if (isDefaultValue != NULL)
					*isDefaultValue = false;
				break;	
			case '8':
				value *= 10;
				value += 8;
				if (isDefaultValue != NULL)
					*isDefaultValue = false;
				break;
			case '9':
				value *= 10;
				value += 9;
				if (isDefaultValue != NULL)
					*isDefaultValue = false;
				break;	
			case '.':
      case ',':
				i = static_cast<unsigned int>(str.size());
				break;
			
			default:
				break;
		}
	}
	if (invert)
		value *= -1;
	return value;
}

unsigned int Variant::toUInt(bool *isDefaultValue) const
{
  std::string str = string_t_to_std(_str);
	unsigned int value = 0;
	if (isDefaultValue != NULL)
		*isDefaultValue = true;
	for (unsigned int i = 0 ; i < str.size(); i++)
	{
		switch (str[i])
		{
			case '0':
				value *= 10;
				if (isDefaultValue != NULL)
					*isDefaultValue = false;
				break;
			case '1':
				value *= 10;
				value += 1;
				if (isDefaultValue != NULL)
					*isDefaultValue = false;
				break;
			case '2':
				value *= 10;
				value += 2;
				if (isDefaultValue != NULL)
					*isDefaultValue = false;
				break;
			case '3':
				value *= 10;
				value += 3;
				if (isDefaultValue != NULL)
					*isDefaultValue = false;
				break;	
			case '4':
				value *= 10;
				value += 4;
				if (isDefaultValue != NULL)
					*isDefaultValue = false;
				break;	
			case '5':
				value *= 10;
				value += 5;
				if (isDefaultValue != NULL)
					*isDefaultValue = false;
				break;	
			case '6':
				value *= 10;
				value += 6;
				if (isDefaultValue != NULL)
					*isDefaultValue = false;
				break;	
			case '7':
				value *= 10;
				value += 7;
				if (isDefaultValue != NULL)
					*isDefaultValue = false;
				break;	
			case '8':
				value *= 10;
				value += 8;
				if (isDefaultValue != NULL)
					*isDefaultValue = false;
				break;
			case '9':
				value *= 10;
				value += 9;
				if (isDefaultValue != NULL)
					*isDefaultValue = false;
				break;	
			case '.':
				i = static_cast<unsigned int>(str.size());
				break;
			case ',':
				i = static_cast<unsigned int>(str.size());
				break;
			default:
				break;
		}
	}
	return value;
}

float Variant::toFloat(bool *isDefaultValue) const
{
  std::string str = string_t_to_std(_str);
	float value = 0;
	int x = 0, y = static_cast<unsigned int>(str.size());
	bool invert = false;
	if (isDefaultValue != NULL)
		*isDefaultValue = true;
	for (unsigned int i = 0 ; i < str.size(); i++)
	{
		if (str[i] == '-' && value > 0)
			invert = true;

		switch (str[i])
		{
			case '0':
				value *= 10;
				x++;
				if (isDefaultValue != NULL)
					*isDefaultValue = false;
				break;
			case '1':
				value *= 10;
				value += 1;
				x++;
				if (isDefaultValue != NULL)
					*isDefaultValue = false;
				break;
			case '2':
				value *= 10;
				value += 2;
				x++;
				if (isDefaultValue != NULL)
					*isDefaultValue = false;
				break;
			case '3':
				value *= 10;
				value += 3;
				x++;
				if (isDefaultValue != NULL)
					*isDefaultValue = false;
				break;	
			case '4':
				value *= 10;
				value += 4;
				x++;
				if (isDefaultValue != NULL)
					*isDefaultValue = false;
				break;	
			case '5':
				value *= 10;
				value += 5;
				x++;
				if (isDefaultValue != NULL)
					*isDefaultValue = false;
				break;	
			case '6':
				value *= 10;
				value += 6;
				x++;
				if (isDefaultValue != NULL)
					*isDefaultValue = false;
				break;	
			case '7':
				value *= 10;
				value += 7;
				x++;
				if (isDefaultValue != NULL)
					*isDefaultValue = false;
				break;	
			case '8':
				value *= 10;
				value += 8;
				x++;
				if (isDefaultValue != NULL)
					*isDefaultValue = false;
				break;
			case '9':
				value *= 10;
				value += 9;
				x++;
				if (isDefaultValue != NULL)
					*isDefaultValue = false;
				break;	
			case '.':
				y = x;
				break;
			case ',':
				y = x;
				break;
			default:
				break;
		}
	}
	for (int i = 0; i < (x - y) ;i++)
		value /= 10;
	
	if (invert)
		value *= -1;
	return value;
}

string_t Variant::toString() const
{
	return _str;
}

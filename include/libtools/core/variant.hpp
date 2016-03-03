

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
  * FILE         : variant.hpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : 1.1 olol
  * DEPENDENCIES : config.h
  * ROLE         : Un équivalent à QVariant plus simple
  */


#ifndef LIBTOOLS_VARIANT_HPP
#define LIBTOOLS_VARIANT_HPP

#include <libtools/public/config.h>

class LIBTOOLS_PRIVATE Variant
{
	public:
		Variant();
		Variant(bool value);
		Variant(int value);
		Variant(unsigned int value);
		Variant(float value);
		Variant(const string_t& str);
		Variant(const char* str);
		Variant(char* str);
		Variant(const Variant& copy);
		Variant& operator= (const Variant& copy);
		Variant& operator= (const int& number);
		Variant& operator= (const unsigned int &number);
		Variant& operator= (const float& number);
		Variant& operator= (const string_t& str);
		virtual ~Variant();
		
		bool toBool(bool *isDefaultValue = NULL) const;
		int toInt(bool *isDefaultValue = NULL) const;
		unsigned int toUInt(bool *isDefaultValue = NULL) const;
		float toFloat(bool *isDefaultValue = NULL) const;
		string_t toString() const;
		
	private:
		string_t _str;

};

#endif

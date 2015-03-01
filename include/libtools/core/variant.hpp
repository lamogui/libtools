

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

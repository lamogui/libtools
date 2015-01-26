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
  * FILE         : file.hpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : 1.7 olol
  * DEPENDENCIES : file.hpp
  * ROLE         : Classes utiles pour manipuler facilement des 
  *                fichiers
  */
  
#include <libtools/core/file.hpp>


AbstractFileParser::AbstractFileParser() 
{

}

AbstractFileParser::~AbstractFileParser()
{

}

unsigned int AbstractFileParser::write_to_file(FILE* file) const
{
  unsigned int target_size=byte_size();
  unsigned int real_size;
  if (!target_size) return 0;
  unsigned char* buffer = (unsigned char*) malloc(target_size);
  if (real_size=write_to_buffer(buffer,target_size))
  {
    if (real_size!=fwrite((void*)buffer,1,real_size,file))
    {
      printf("fwrite error !\n"); 
      free((void*)buffer);
      return 0;
    }
  }
  else 
  {
    free((void*)buffer);
    return 0;
  }
  free((void*)buffer);
  return real_size;
}

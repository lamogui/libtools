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
  * FILE         : production.hpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : V1.0
  * DEPENDENCIES : config.h
  *                signal.hpp
  */
  
#ifndef LIBTOOLS_PRODUCTION_HPP
#define LIBTOOLS_PRODUCTION_HPP

#include <libtools/public/config.h>
#include <libtools/core/signal.hpp>
#include <map>


class Production
{
  public:
    virtual ~Production() {}
    
    typedef int32_t RoleId;
    typedef int32_t GenreId;
    
    virtual string_t name()=0; //Title, name of the production
    virtual string_t author()=0; //Main author, alone or presented with '&' in the string
    //Return true if the production have a author list (= if implemented)
    inline virtual bool authors(std::multimap<RoleId,string_t>& list) {
      (void)list;
      return false;
    }
    inline virtual string_t group() { //Whatever (demo,music)group/label/producer
      return "";
    }
    virtual GenreId genre()=0;    
};


class MusicProduction : public Production, public AbstractStereoSignalSource
{
  public:
    virtual ~MusicProduction() {} 
    virtual string_t name()=0; //Title, name of the production
    virtual string_t author()=0; //Main author, alone or presented with '&' in the string
    virtual Production::GenreId genre()=0;    
    virtual unsigned int fetch(Signal& outleft, Signal& outright) = 0;
    
    virtual bool ended() = 0; //can fetch ?
    virtual void rewind() = 0; //restart the production
    virtual double length() = 0; //Length of the music in seconds
  
};

#endif
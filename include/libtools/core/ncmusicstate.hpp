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
  * THIS FILE IS PART OF NOISECONTROLLERS 
  * SECURITY LEVEL : 8 (CRITICAL)  
  * VISIBILITY     : PROTECTED
  * © COPYDOWN LAMOGUI ALL RIGHTS RESERVED 
  *
  * FILE         : ncmusicstate.hpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : V2.0 olol
  */


#ifndef NOISECONTROLLERS_NCMUSICSTATE_HPP
#define NOISECONTROLLERS_NCMUSICSTATE_HPP

#include <libtools/public/config.h>

class NCMusicPower
{
  public:
    NCMusicPower();
    
  
    typedef enum {
      pause,
      chorus,
      drop
    } state;
    
    static string_t toStr(state p);
    
    
    void reset();
    
    inline bool newState() const
    {
      return _newState;
    }
    
    inline state currentState() const
    {
      return _currentState;
    }
    
    void feed(int16_t noisel11a128,
              int16_t noisem11a128,
              int16_t noiseh11a128,
              int16_t bassl11a128,
              int16_t bassm11a128,
              int16_t bassh11a128);
    
  private:
    void _registerNewState(state n);
  
    bool _newState;
    state _currentState;
    state _previousState;
    
};


#endif

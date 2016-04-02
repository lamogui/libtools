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
  * VISIBILITY     : PRIVATE
  * © COPYDOWN LAMOGUI ALL RIGHTS RESERVED
  *
  * FILE         : ncmusicstate.cpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : V2.0 olol
  */
  
#include <libtools/core/ncmusicstate.hpp>

string_t NCMusicPower::toStr(state p)
{
  switch (p)
  {
    case pause:
      return "Pause";
    case chorus:
      return "Chorus";
    case drop:
      return "Drop !";
    default:
      return "error";
  }
}


NCMusicPower::NCMusicPower()
{
  reset();
}

void NCMusicPower::reset()
{
  _newState=false;
  _currentState=pause;
  _previousState=pause;
}

void NCMusicPower::_registerNewState(state n)
{
  _previousState=_currentState;
  _currentState=n;
  _newState=true;
}

void NCMusicPower::feed(int16_t noisel11a128,
                        int16_t noisem11a128,
                        int16_t noiseh11a128,
                        int16_t bassl11a128,
                        int16_t bassm11a128,
                        int16_t bassh11a128)
{
  _newState=false;
  state now=_currentState;
  
    //std::cerr << "current " << toStr(now).toAnsiString() << std::endl;
  if (bassh11a128  > 2500)
  {
    //std::cerr << "drop" << std::endl;
    now=drop;
  }
  if ((noiseh11a128 < 200 && bassh11a128 < 2000 ))
  {
    //std::cerr << "pause" << std::endl;
    now=pause;
  }
  if (noisem11a128 > 400 || noiseh11a128 > 2000)
  {
    //std::cerr << "chorus" << std::endl;
    now=chorus;
  }

  /*if ((noisem11a128 > 1000 && bassh11a128 < 2000 ) ||
      (noisel11a128 > 500) )
  {
    now=chorus;
  }
  if ((noisel11a128 > 1500 && bassl11a128 > 1500) ||
      (noiseh11a128 > 4500 && bassh11a128 > 4500) ||
      (bassh11a128 > 6000))
  {
    now=drop;
  }*/
  
  if (now != _currentState)
  {

    //std::cerr << "register" << std::endl;
    _registerNewState(now);
  }
}


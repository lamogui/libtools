

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
  * VISIBILITY     : PUBLIC
  * © COPYDOWN™ LAMOGUI ALL RIGHTS RESERVED 
  *
  * FILE         : singleinstance.hpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  *                Microsoft (http://support.microsoft.com/kb/243953/fr)
  * VERSION      : 1.0
  * DEPENDENCIES : config.h
  */




#ifndef LIBTOOLS_SINGLEINSTANCE_HPP
#define LIBTOOLS_SINGLEINSTANCE_HPP

#include <libtools/public/config.h>

#ifdef LIBTOOLS_WINDOWS
#include <windows.h> 

//from http://support.microsoft.com/kb/243953/fr

//This code is from Q243953 in case you lose the article and wonder
//where this code came from.
class CLimitSingleInstance
{
protected:
  DWORD  m_dwLastError;
  HANDLE m_hMutex;

public:
  CLimitSingleInstance(const string_t& name)
  {
    //Make sure that you use a name that is unique for this application otherwise
    //two apps may think they are the same if they are using same name for
    //3rd parm to CreateMutex
    m_hMutex = CreateMutexA(NULL, FALSE, string_t_to_std(name).c_str()); //do early
    m_dwLastError = GetLastError(); //save for use later...
  }
   
  ~CLimitSingleInstance() 
  {
    if (m_hMutex)  //Do not forget to close handles.
    {
       CloseHandle(m_hMutex); //Do as late as possible.
       m_hMutex = NULL; //Good habit to be in.
    }
  }

  BOOL isAnotherInstanceRunning() 
  {
    return (ERROR_ALREADY_EXISTS == m_dwLastError);
  }
};
#endif
#endif
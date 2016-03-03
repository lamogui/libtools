

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

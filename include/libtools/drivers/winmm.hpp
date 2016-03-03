
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
  * FILE         : winmm.hpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : V2.0 
  * DEPENDENCIES : config.h
  *                audiodriver.hpp
  * ROLE         : Contient le wrapper pour winmm
  */

#ifndef LIBTOOLS_WINMM_HPP
#define LIBTOOLS_WINMM_HPP

#include <libtools/public/config.h>
#include <libtools/core/audiodriver.hpp>

#if defined(LIBTOOLS_WINDOWS)

#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>

class LIBTOOLS_PRIVATE WinmmDriver : public AudioDriver
{
  public:
    WinmmDriver(unsigned int rate);
    virtual ~WinmmDriver();

    //Push a signal block into driver buffers
    virtual bool pushStereoSignal(const Signal& left,const Signal& right);  
    
    //Return samples which can be pushed into the stream (at least an estimation)
    virtual unsigned int getBufferedSamplesCount();
    
    virtual bool start();
    
    virtual void stop();
    
    virtual inline bool working() { return _working; }
    
  protected:
    bool init(unsigned int rate);
    void free();
  
    /*Win32 end block callback*/
    static void CALLBACK _waveOutProc(HWAVEOUT, UINT, DWORD_PTR, DWORD_PTR, DWORD_PTR);
    WAVEHDR* _waveBlocks;
    HWAVEOUT _hWaveOut;
    unsigned int _nbBlocks;
    CRITICAL_SECTION _waveCriticalSection;
    volatile unsigned int _waveFreeBlockCount;
    unsigned int _waveCurrentBlock;
    unsigned int _rate;
	  HANDLE _event;
    bool _working;
};

#endif

#endif

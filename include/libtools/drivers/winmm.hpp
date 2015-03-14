
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
AND RENUMERATIONS, FIXED BY ORIGINAL AUTHORS (CONTACT THEM).

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
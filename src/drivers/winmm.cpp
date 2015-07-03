
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
  * FILE         : winmm.cpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : V2.0 
  * DEPENDENCIES : config.h
  *                winmm.hpp
  * ROLE         : Contient le wrapper pour winmm
  */

#include <libtools/drivers/winmm.hpp>
#include <libtools/decoders/settings.hpp>

#define MULTIPLIER_16 0.8*32768

WinmmDriver::WinmmDriver(unsigned int rate):
_waveBlocks(0),
_hWaveOut(0),
_nbBlocks(0),
_waveFreeBlockCount(0),
_waveCurrentBlock(0),
_rate(rate),
_event(0),
_working(false)
{
  _working=init(rate);
}

WinmmDriver::~WinmmDriver()
{
  this->free();
}

bool WinmmDriver::init(unsigned int rate)
{
  if (_hWaveOut || _event) {
    std::cerr << "WinMM already initialized !" << std::endl;
    return true;
  }
  WAVEFORMATEX wfx;
  wfx.nSamplesPerSec = rate; /* sample rate */
  wfx.wBitsPerSample = 16; /* sample size */
  wfx.nChannels= 2; /* channels*/
  wfx.cbSize = 0; /* size of _extra_ info */
  wfx.wFormatTag = WAVE_FORMAT_PCM;
  wfx.nBlockAlign = (wfx.wBitsPerSample * wfx.nChannels) >> 3;
  wfx.nAvgBytesPerSec = wfx.nBlockAlign * wfx.nSamplesPerSec;

  _event = CreateEvent(0,FALSE,TRUE,0);
  if (!_event)
  {
	  std::cerr << "Fatal error : cannot create Event ! " << std::endl;
	  return false;
  }

  if(waveOutOpen(
          &_hWaveOut, 
          WAVE_MAPPER, 
          &wfx, 
          (DWORD_PTR)&_waveOutProc, 
          (DWORD_PTR) this, 
          CALLBACK_FUNCTION | WAVE_ALLOWSYNC
          ) != MMSYSERR_NOERROR)
    {
      std::cerr << "Failed to initialize WINMM driver !" << std::endl;
      return false;
    }
  InitializeCriticalSection(&_waveCriticalSection);
  std::cout << "WINMM Initialized with sample rate " << rate << "Hz  (" << wfx.nAvgBytesPerSec << " Bytes/sec)" << std::endl;

  unsigned char* buffer;
  _nbBlocks=GetSettingsFor("WINMM/Blocks",3);
  if (_nbBlocks < 3) _nbBlocks = 3;
  _waveFreeBlockCount=_nbBlocks;
  _waveCurrentBlock=0;
  DWORD totalBufferSize = (Signal::size*4 + sizeof(WAVEHDR)) * _nbBlocks;
  /*
   * allocate memory for the entire set in one go
   */
  buffer = (unsigned char*) HeapAlloc(GetProcessHeap(), 
                                      HEAP_ZERO_MEMORY, 
                                      totalBufferSize); 
  _waveBlocks = (WAVEHDR*)buffer;
  buffer += sizeof(WAVEHDR) * _nbBlocks;
  for(unsigned int i = 0; i < _nbBlocks; i++) {
    _waveBlocks[i].dwBufferLength = Signal::size*4;
    _waveBlocks[i].lpData = (LPSTR)buffer;
    buffer += Signal::size*4;
  }
  return true;
}

void WinmmDriver::free()
{
  if (_waveBlocks) {
    while (1) {
      EnterCriticalSection(&_waveCriticalSection); //Mutex Locking
      if (_waveFreeBlockCount==_nbBlocks) {
        break;
      }
      LeaveCriticalSection(&_waveCriticalSection);
      Sleep(10);
    }
    for(unsigned int i = 0; i < _nbBlocks; i++) {
      if(_waveBlocks[i].dwFlags & WHDR_PREPARED)
        waveOutUnprepareHeader(_hWaveOut, &_waveBlocks[i], sizeof(WAVEHDR));
    }
    HeapFree(GetProcessHeap(), 0, _waveBlocks);
    _waveBlocks=0;
    
  }
  if (_hWaveOut){
    waveOutClose(_hWaveOut);
    _hWaveOut=0;
    DeleteCriticalSection(&_waveCriticalSection); //Delete mutex
  }

  if (_event)
	CloseHandle(_event);
  _event=0;
  _nbBlocks=0;
  _waveFreeBlockCount=0;
  _waveCurrentBlock=0;
}

unsigned int WinmmDriver::getBufferedSamplesCount()
{

  EnterCriticalSection(&_waveCriticalSection); //Mutex Locking
  unsigned int r=(_nbBlocks - _waveFreeBlockCount)*Signal::size;
  LeaveCriticalSection(&_waveCriticalSection);
  return r;
}

bool WinmmDriver::pushStereoSignal(const Signal& left,const Signal& right)
{
  EnterCriticalSection(&_waveCriticalSection); //Mutex Locking

  if (!_waveFreeBlockCount) {
    LeaveCriticalSection(&_waveCriticalSection);
	  WaitForSingleObject(_event,20);
	  EnterCriticalSection(&_waveCriticalSection);
	  unsigned int c= _waveFreeBlockCount;
	  LeaveCriticalSection(&_waveCriticalSection);
	  if (!c) return false;
  }

  _waveFreeBlockCount--;
  LeaveCriticalSection(&_waveCriticalSection);
  
  WAVEHDR* current = &_waveBlocks[_waveCurrentBlock];
  if(current->dwFlags & WHDR_PREPARED) 
      waveOutUnprepareHeader(_hWaveOut, current, sizeof(WAVEHDR)); 
      
  
  short* p = (short*)current->lpData;
  
  for (unsigned int i=0; i < Signal::size; i++)
  {
    *p++ = (short)(left.samples[i]*MULTIPLIER_16);
    *p++ = (short)(right.samples[i]*MULTIPLIER_16);
  }
  
  current->dwBufferLength = Signal::size*4;
  
  waveOutPrepareHeader(_hWaveOut, current, sizeof(WAVEHDR));
  waveOutWrite(_hWaveOut, current, sizeof(WAVEHDR));
  
  
  _waveCurrentBlock++;
  _waveCurrentBlock %= _nbBlocks;
  return true;
} 

void CALLBACK WinmmDriver::_waveOutProc(HWAVEOUT, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR, DWORD_PTR)
{
  WinmmDriver* a = (WinmmDriver*)dwInstance;
  if(uMsg != WOM_DONE)
    return;
  EnterCriticalSection(&(a->_waveCriticalSection)); //Mutex Locking
  a->_waveFreeBlockCount++;
  LeaveCriticalSection(&(a->_waveCriticalSection));
  SetEvent(a->_event);
}

bool WinmmDriver::start()
{
  _working=init(_rate);
  return _working;
}

void WinmmDriver::stop()
{
  this->free();
}

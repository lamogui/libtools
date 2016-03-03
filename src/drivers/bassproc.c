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
  * VISIBILITY     : PRIVATE
  * © COPYDOWN™ LAMOGUI ALL RIGHTS RESERVED 
  *
  * FILE         : bassproc.c
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : 1.2 
  * DEPENDENCIES : bassproc.h
  */
  

#define BASSPROC_IMPLEMENT
#include <libtools/drivers/bassproc.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(LIBTOOLS_WINDOWS) && !defined(BASS_H)

BASS_INIT_PROC* BASS_Init=0;
BASS_ERRORGETCODE_PROC* BASS_ErrorGetCode=0;
BASS_FREE_PROC* BASS_Free=0;
BASS_MUSICLOAD_PROC* BASS_MusicLoad=0;
BASS_MUSICFREE_PROC* BASS_MusicFree=0;
BASS_STREAMFREE_PROC* BASS_StreamFree=0;
BASS_STREAMCREATEFILE_PROC* BASS_StreamCreateFile=0;
BASS_STREAMCREATEURL_PROC* BASS_StreamCreateURL=0;
BASS_CHANNELGETINFO_PROC* BASS_ChannelGetInfo=0;
BASS_CHANNELGETDATA_PROC* BASS_ChannelGetData=0;
BASS_CHANNELSETPOSITION_PROC* BASS_ChannelSetPosition=0;
BASS_CHANNELGETPOSITION_PROC* BASS_ChannelGetPosition=0;
BASS_CHANNELGETLENGTH_PROC* BASS_ChannelGetLength=0;
BASS_CHANNELBYTES2SECONDS_PROC* BASS_ChannelBytes2Seconds=0;
BASS_CHANNELSECONDS2BYTES_PROC* BASS_ChannelSeconds2Bytes=0;
BASS_CHANNELGETTAGS_PROC* BASS_ChannelGetTags=0;


static HMODULE bass_dll=0;
int load_bass_procs(const char* bassdllname)
{
  int loaded=0;
  if (!bass_dll) {
    bass_dll=LoadLibrary(bassdllname);
  }
  if (!bass_dll) return -1;
  if (BASS_Init=(BASS_INIT_PROC*)GetProcAddress(bass_dll,"BASS_Init")) loaded++;
  if (BASS_ErrorGetCode=(BASS_ERRORGETCODE_PROC*)GetProcAddress(bass_dll,"BASS_ErrorGetCode")) loaded++;
  if (BASS_Free=(BASS_FREE_PROC*)GetProcAddress(bass_dll,"BASS_Free")) loaded++;
  if (BASS_MusicLoad=(BASS_MUSICLOAD_PROC*)GetProcAddress(bass_dll,"BASS_MusicLoad")) loaded++;
  if (BASS_MusicFree=(BASS_MUSICFREE_PROC*)GetProcAddress(bass_dll,"BASS_MusicFree")) loaded++;
  if (BASS_StreamFree=(BASS_STREAMFREE_PROC*)GetProcAddress(bass_dll,"BASS_StreamFree")) loaded++;
  if (BASS_StreamCreateFile=(BASS_STREAMCREATEFILE_PROC*)GetProcAddress(bass_dll,"BASS_StreamCreateFile")) loaded++;
  if (BASS_StreamCreateURL=(BASS_STREAMCREATEURL_PROC*)GetProcAddress(bass_dll,"BASS_StreamCreateURL")) loaded++;
  if (BASS_ChannelGetInfo=(BASS_CHANNELGETINFO_PROC*)GetProcAddress(bass_dll,"BASS_ChannelGetInfo")) loaded++;
  if (BASS_ChannelGetData=(BASS_CHANNELGETDATA_PROC*)GetProcAddress(bass_dll,"BASS_ChannelGetData")) loaded++;
  if (BASS_ChannelSetPosition=(BASS_CHANNELSETPOSITION_PROC*)GetProcAddress(bass_dll,"BASS_ChannelSetPosition")) loaded++;
  if (BASS_ChannelGetPosition=(BASS_CHANNELGETPOSITION_PROC*)GetProcAddress(bass_dll,"BASS_ChannelGetPosition")) loaded++;
  if (BASS_ChannelGetLength=(BASS_CHANNELGETLENGTH_PROC*)GetProcAddress(bass_dll,"BASS_ChannelGetLength")) loaded++;
  if (BASS_ChannelBytes2Seconds=(BASS_CHANNELBYTES2SECONDS_PROC*)GetProcAddress(bass_dll,"BASS_ChannelBytes2Seconds")) loaded++;
  if (BASS_ChannelSeconds2Bytes=(BASS_CHANNELSECONDS2BYTES_PROC*)GetProcAddress(bass_dll,"BASS_ChannelSeconds2Bytes")) loaded++;
  if (BASS_ChannelGetTags=(BASS_CHANNELGETTAGS_PROC*)GetProcAddress(bass_dll,"BASS_ChannelGetTags")) loaded++;
  return loaded;
}

BASS_FLAC_STREAMCREATEURL_PROC* BASS_FLAC_StreamCreateURL=0;
BASS_FLAC_STREAMCREATEFILE_PROC* BASS_FLAC_StreamCreateFile=0;

static HMODULE bassflac_dll=0;
int load_bassflac_procs(const char* bassflacdllname)
{
  int loaded=0;
  if (!bassflac_dll) {
    bassflac_dll=LoadLibrary(bassflacdllname);
  }
  if (!bassflac_dll) return -1;
  if (BASS_FLAC_StreamCreateURL=(BASS_FLAC_STREAMCREATEURL_PROC*)GetProcAddress(bassflac_dll,"BASS_FLAC_StreamCreateURL")) loaded++;
  if (BASS_FLAC_StreamCreateFile=(BASS_FLAC_STREAMCREATEFILE_PROC*)GetProcAddress(bassflac_dll,"BASS_FLAC_StreamCreateFile")) loaded++;
  return loaded;
}

#endif //defined(LIBTOOLS_WINDOWS) && !defined(BASS_H)

static int bass_initialized=0;

int BASS_EnsureBassInit(int device, DWORD freq, DWORD flags)
{
#if defined(LIBTOOLS_WINDOWS) && !defined(BASS_H)
  if (!BASS_Init) {
    return 0;
  }
#endif

  if (!bass_initialized)
  {
    if (!BASS_Init(device,freq,flags,0,0)){
    } else {
      bass_initialized=1;
    }
  } else {
    //TODO
    /*BASS_INFO infos;
    if ((BASS_GetDevice()==device || device==-1) &&
        BASS_GetInfo(&infos) && infos.freq==freq)
    {

    } else {*/
      BASS_EnsureBassFree();
      return BASS_EnsureBassInit(device,freq,flags);
    //}
  }
  return bass_initialized;
}

int BASS_EnsureBassContext()
{
  return bass_initialized;
}


void BASS_EnsureBassFree()
{
  if (bass_initialized) {
#if defined(LIBTOOLS_WINDOWS) && !defined(BASS_H)
    if (BASS_Free)
#endif
      BASS_Free();
    bass_initialized=0;
  }
}

#if defined(LIBTOOLS_WINDOWS) && !defined(BASSASIO_H)

BASS_ASIO_GETDEVICEINFO_PROC* BASS_ASIO_GetDeviceInfo=0;
BASS_ASIO_INIT_PROC* BASS_ASIO_Init=0;
BASS_ASIO_GETINFO_PROC* BASS_ASIO_GetInfo=0;
BASS_ASIO_SETRATE_PROC* BASS_ASIO_SetRate=0;
BASS_ASIO_GETRATE_PROC* BASS_ASIO_GetRate=0;
BASS_ASIO_CHANNELGETINFO_PROC* BASS_ASIO_ChannelGetInfo=0;
BASS_ASIO_CHANNELENABLE_PROC* BASS_ASIO_ChannelEnable=0;
BASS_ASIO_CHANNELJOIN_PROC* BASS_ASIO_ChannelJoin=0;
BASS_ASIO_CHANNELSETFORMAT_PROC* BASS_ASIO_ChannelSetFormat=0;
BASS_ASIO_START_PROC* BASS_ASIO_Start=0;
BASS_ASIO_STOP_PROC* BASS_ASIO_Stop=0;
BASS_ASIO_CHANNELRESET_PROC* BASS_ASIO_ChannelReset=0;
BASS_ASIO_ISSTARTED_PROC* BASS_ASIO_IsStarted=0;

static HMODULE bassasio_dll=0;
int load_bassasio_procs(const char* bassasiodllname)
{
  int loaded=0;
  if (!bassasio_dll) {
    bassasio_dll=LoadLibrary(bassasiodllname);
  }
  if (!bassasio_dll) return -1;
  if (BASS_ASIO_Init=(BASS_ASIO_INIT_PROC*)GetProcAddress(bassasio_dll,"BASS_ASIO_Init")) loaded++;
  if (BASS_ASIO_GetInfo=(BASS_ASIO_GETINFO_PROC*)GetProcAddress(bassasio_dll,"BASS_ASIO_GetInfo")) loaded++ ;
  if (BASS_ASIO_SetRate=(BASS_ASIO_SETRATE_PROC*)GetProcAddress(bassasio_dll,"BASS_ASIO_SetRate")) loaded++ ;
  if (BASS_ASIO_GetRate=(BASS_ASIO_GETRATE_PROC*)GetProcAddress(bassasio_dll,"BASS_ASIO_GetRate")) loaded++ ;
  if (BASS_ASIO_ChannelGetInfo=(BASS_ASIO_CHANNELGETINFO_PROC*)GetProcAddress(bassasio_dll,"BASS_ASIO_ChannelGetInfo")) loaded++ ;
  if (BASS_ASIO_ChannelEnable=(BASS_ASIO_CHANNELENABLE_PROC*)GetProcAddress(bassasio_dll,"BASS_ASIO_ChannelEnable")) loaded++ ;
  if (BASS_ASIO_ChannelJoin=(BASS_ASIO_CHANNELJOIN_PROC*)GetProcAddress(bassasio_dll,"BASS_ASIO_ChannelJoin")) loaded++ ;
  if (BASS_ASIO_ChannelSetFormat=(BASS_ASIO_CHANNELSETFORMAT_PROC*)GetProcAddress(bassasio_dll,"BASS_ASIO_ChannelSetFormat")) loaded++ ;
  if (BASS_ASIO_Start=(BASS_ASIO_START_PROC*)GetProcAddress(bassasio_dll,"BASS_ASIO_Start")) loaded++ ;
  if (BASS_ASIO_Stop=(BASS_ASIO_STOP_PROC*)GetProcAddress(bassasio_dll,"BASS_ASIO_Stop")) loaded++ ;
  if (BASS_ASIO_ChannelReset=(BASS_ASIO_CHANNELRESET_PROC*)GetProcAddress(bassasio_dll,"BASS_ASIO_ChannelReset")) loaded++ ;
  if (BASS_ASIO_IsStarted=(BASS_ASIO_ISSTARTED_PROC*)GetProcAddress(bassasio_dll,"BASS_ASIO_IsStarted")) loaded++;
  return loaded;
}

#endif //defined(LIBTOOLS_WINDOWS) && !defined(BASSASIO_H)

#ifdef __cplusplus
}
#endif

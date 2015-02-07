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
  * FILE         : bassproc.c
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : 1.0 
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

#ifdef __cplusplus
}
#endif
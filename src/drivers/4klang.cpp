
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
  * THIS FILE IS PART OF
  * SECURITY LEVEL : 8
  * VISIBILITY     : PRIVATE
  * © COPYDOWN™ LAMOGUI ALL RIGHTS RESERVED
  *
  * FILE         : 4klang.cpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : 1.0
  */

#include <libtools/public/config.h>
#include <libtools/drivers/4klang.hpp>
#include <iostream>

_4klangProduction::_4klangProduction(
    _4klang_render_proc proc,
    uint32_t nbSamples,
    int* notes,
    float* env,
    uint32_t rate) :
  _renderProc(proc),
  _noteBuffer(notes),
  _enveloppeBuffer(env),
  _buffer(new float[nbSamples*2]),
  _rate(rate),
  _nbSamples(nbSamples),
  _bufferPos(0),
  _ended(false)
#ifdef SFML_THREAD_HPP
  , _thread(proc,(void*)_buffer)
#endif
{
#ifdef SFML_THREAD_HPP
  //std::cout << "Launching rendering 4klang thread" << std::endl;
  _thread.launch();
#else
  _renderProc((void*)_buffer);
#endif
  rewind();
}

_4klangProduction::~_4klangProduction()
{
#ifdef SFML_THREAD_HPP
  _thread.wait();
#endif
  delete[] _buffer;
}

unsigned int _4klangProduction::fetch(Signal& outleft, Signal& outright)
{
  outleft.reset();
  outright.reset();

  if (!_ended && _nbSamples*2 > _bufferPos)
  {
    unsigned int k=0;
    unsigned int size=Signal::size;
    if (2*size > 2*_nbSamples - _bufferPos)
    {
      size=_nbSamples-(_bufferPos/2);
      _ended=true;
    }
    for (;k < size; k++)
    {
      outleft.samples[k]=_buffer[_bufferPos++];
      outright.samples[k]=_buffer[_bufferPos++];
    }
    return size;
  } else {
    _ended=true;
  }
  return 0;
}

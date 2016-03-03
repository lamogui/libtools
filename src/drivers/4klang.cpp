
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

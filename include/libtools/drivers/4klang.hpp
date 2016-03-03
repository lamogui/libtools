
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
  * VISIBILITY     : Private
  * © COPYDOWN™ LAMOGUI ALL RIGHTS RESERVED
  *
  * FILE         : 4klang.hpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : 1.0
  */

#ifndef LIBTOOLS_DRIVER_4KLANG_HPP
#define LIBTOOLS_DRIVER_4KLANG_HPP

#include <libtools/public/config.h>
#include <libtools/core/production.hpp>


extern "C" {
  typedef void (__stdcall *_4klang_render_proc) (void*);
}

class _4klangProduction : public MusicProduction
{
  public:
    _4klangProduction(
        _4klang_render_proc proc,
        uint32_t nbSamples, //number of samples of the production
        int* notes=0,
        float* env=0,
        uint32_t rate=44100); //If for mystery reason this is not...
    virtual ~_4klangProduction();


    inline virtual double length() const
    {
      return (double)_nbSamples/(double)_rate;
    }


    inline virtual bool ended() const
    {
      return _ended;
    }

    inline virtual void rewind()
    {
      _bufferPos=0;
      if (!_nbSamples)
      {
        _ended=true;
      }
      else
      {
        _ended=false;
      }
    }

    virtual unsigned int fetch(Signal& outleft, Signal& outright);


  protected:
    _4klang_render_proc _renderProc;
    int* _noteBuffer;
    float* _enveloppeBuffer;
    float* _buffer; //Song buffer
    uint32_t _rate;
    uint32_t _nbSamples; //size in "2.*float" of the buffer
    uint32_t _bufferPos; //Position in "float" into the buffer
    bool _ended;         //Fetch reached the end...

#ifdef SFML_THREAD_HPP
    sf::Thread _thread;
#endif
};

#endif

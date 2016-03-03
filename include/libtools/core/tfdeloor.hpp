
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
  * FILE         : tfdeloor.hpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : V2.0 olol
  * DEPENDENCIES : config.h
  *                note.hpp
  *                decoder.hpp
  */


#ifndef LIBTOOLS_TFDELOOR_HPP
#define LIBTOOLS_TFDELOOR_HPP

#include <libtools/public/config.h>
#include <libtools/core/note.hpp>
#include <libtools/core/decoder.hpp>



#define TFDeLoor_SIZE 3528

class LIBTOOLS_PRIVATE TFDeLoor
{
  public:
    TFDeLoor();
    virtual ~TFDeLoor();

    //push signal and compute when have enough samples
    void pushSignal(const Signal &s);
    
    //Direct immediate compute from samples (with truncate or zero padding...)
    void compute(const sample* s, unsigned int size);
    void compute(const Signal &s);

    //Compute module if needed !
    void computeModule();

    inline sample* getModule() {
      return _module;
    }

  private:
    sample _input[TFDeLoor_SIZE];
    sample _real[NB_NOTES];
    sample _imaginary[NB_NOTES];
    sample _module[NB_NOTES];
    sample _deloorFactor[NB_NOTES][TFDeLoor_SIZE];
    sample _deloorFactorI[NB_NOTES][TFDeLoor_SIZE];
    unsigned int _bufferNeeded[NB_NOTES];
};


#if defined(SFML_THREAD_HPP)

class LIBTOOLS_PRIVATE TFDeLoorPrecomputer
{
  public:
    TFDeLoorPrecomputer();
    virtual ~TFDeLoorPrecomputer();

    bool start(MusicDecoder* decoder);

    inline sample* getFrame(unsigned int frame) {
      if (frame<_frames && _table)
      {
        return _table + NB_NOTES*frame;
      }
      return 0;
    }

    inline sample* nextFrame() {
      return getFrame(_progress++);
    }

    unsigned int frames() { 
      //sf::Lock lock(_mutex);
      return _frames; 
    }
    unsigned int progress() { 
      //sf::Lock lock(_mutex);
      return _progress; 
    }

    inline bool ready() {
       return !_onAnalyse;
    }

    inline void finnish() {
      _thread.wait();
      _mutex.lock();
      _progress=0;
      _onAnalyse=false;
      _mutex.unlock();
    }

    

  private:
    void _compute();
    bool _onAnalyse;
    sf::Thread _thread;
    mutex_t _mutex;
    unsigned int _frames;
    unsigned int _progress;
    MusicDecoder* _decoder;
    sample* _table;

};

#endif //sf::Thread dependancy

#endif


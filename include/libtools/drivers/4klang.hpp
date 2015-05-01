
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

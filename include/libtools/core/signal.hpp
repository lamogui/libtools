﻿
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
  * FILE         : signal.hpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : V2.1 olol
  * DEPENDENCIES : config.h
  * ROLE         : Déclare une classe gérant une unité de son (classe Signal) 
  *                Ainsi que des modeles abstrait pour le générer ce son...
  */

#ifndef LIBTOOLS_SIGNAL_HPP
#define LIBTOOLS_SIGNAL_HPP

#include <libtools/public/config.h>
#include <vector>
#include <cstdlib> //old malloc
#include <cstring> //memset
#include <cmath>
#include <set>

typedef float sample; //type pour un echantillon

class LIBTOOLS_PRIVATE Signal
{
  public:
    Signal();
    Signal(const Signal& s);
    virtual ~Signal(); 

    //Signal take care of allocation of this
    sample* samples;

    void mix(const Signal*, unsigned int n=1);
    void add(const Signal*, unsigned int n=1);
    void addOffset(sample offset);
    void scale(sample scale);
    void constant(sample constant);
    void saturate(sample min, sample max);
    double energy() const;
    double fenergy(double f) const;

    inline double power() const {
      return energy()/(double)Signal::size;
    }

    inline double fpower(double f) const {
      return fenergy(f)/(double)Signal::size;
    }

    sample level();
    void normalize(sample coeff);
    void normalize();
    
    //Assume signal is normalized
    void glslize();

    void tfd(Signal& out_tfd);
    
      inline void reset()
      {
        memset((void*) samples,0,Signal::byteSize);
      }


    Signal& operator=(const Signal&);

    //Constants for all signals
    volatile static unsigned int frequency; //Fe
    volatile static double refreshRate;      //Le but c'est d'avoir 50 ici ! le plus possible sera le mieux
    volatile static unsigned int size;
    volatile static unsigned int byteSize;
    volatile static bool isPow2;

    // /!\ Signal::refreshRate n'est qu'une valeur indicative !


    //be sure to lock ALL instances of Signals before do theses
    static void globalConfigurationFromFramerate( unsigned int f,  float r);
    static void globalConfigurationFromPow2(unsigned int f,  unsigned short pow2);

  private:
    void _reset();
    static std::set<Signal*> _instances;

};

class LIBTOOLS_PRIVATE AbstractSignalModifier
{
  public:
    virtual ~AbstractSignalModifier() {}
    virtual void step(Signal* inout) = 0;
};

class LIBTOOLS_PRIVATE AbstractStereoSignalModifier
{
  public:
    virtual ~AbstractStereoSignalModifier() {}
    virtual void step(Signal* ioleft,Signal* ioright) = 0;
};

class LIBTOOLS_PRIVATE AbstractStereoSignalMixer
{
  public:
    virtual ~AbstractStereoSignalMixer() {}
    virtual void step(const Signal& inleft, const Signal& inright, Signal& outleft,  Signal& outright) = 0;
};

class LIBTOOLS_PRIVATE AbstractSignalSource
{
  public:
    virtual ~AbstractSignalSource() {}
    virtual void fetch(Signal& out) = 0;
};

class LIBTOOLS_PRIVATE AbstractStereoSignalSource
{
  public:
    virtual ~AbstractStereoSignalSource() {}
    virtual unsigned int fetch(Signal& outleft, Signal& outright) = 0;
};


//TODO refact this
class LIBTOOLS_PRIVATE AbstractSignalGenerator : public AbstractSignalModifier
{
  public:
    AbstractSignalGenerator() : _time(0) {}
    virtual ~AbstractSignalGenerator() {}

    virtual void step(Signal* output) = 0;

    inline Signal* generate()
    {
      this->step(&_output);
      return &_output;
    }

    virtual inline void resetTime()
    {
      _time=0;
    }

  protected:
    Signal _output;
    unsigned int _time; //in (1/fe) secondes
};





#endif

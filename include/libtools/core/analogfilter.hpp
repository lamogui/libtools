
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
  * FILE         : analogfilter.hpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  *                Kwon-Young Choi (kwonyoungchoi93@gmail.com)
  * VERSION      : 1.7 olol
  * DEPENDENCIES : signal.hpp
  *                config.h
  */
  
#ifndef LIBTOOLS_ANALOGFILTER_HPP
#define LIBTOOLS_ANALOGFILTER_HPP

#include <libtools/public/config.h>
#include <libtools/core/signal.hpp>


class LIBTOOLS_PRIVATE AnalogFilter : public AbstractSignalModifier
{
  public:
    AnalogFilter();
    AnalogFilter(float f, float m=0.5f);
    virtual ~AnalogFilter();
    virtual void step(Signal* inout) = 0;
    
    //set constant frequency
    void setFrequency(float f);
    //set constant resonance
    void setResonance(float m);
    
    inline Signal& getFrequency()
    {
      return _frequency;
    }
    
    inline Signal& getResonance()
    {
      return _resonance;
    }

  protected:
    Signal _frequency;
    Signal _resonance;
    
};

class LIBTOOLS_PRIVATE AnalogLowPassFilter : public AnalogFilter
{
  public:
    AnalogLowPassFilter();
    AnalogLowPassFilter(float f, float m=0.5f);
    virtual ~AnalogLowPassFilter();
    virtual void step(Signal* inout);
    
  private:
    sample _y_1; //y(n-1)
};

class LIBTOOLS_PRIVATE AnalogLowPassFilter2 : public AnalogFilter
{
  public:
    AnalogLowPassFilter2();
    AnalogLowPassFilter2(float f, float m=0.5f);
    virtual ~AnalogLowPassFilter2();
    virtual void step(Signal* inout);
    
  private:
    sample _y_1; //y(n-1)
    sample _y_2; //y(n-2)
};

class LIBTOOLS_PRIVATE AnalogBandPassFilter2 : public AnalogFilter
{
  public:
    AnalogBandPassFilter2();
    AnalogBandPassFilter2(float f, float m=0.5f);
    virtual ~AnalogBandPassFilter2();
    virtual void step(Signal* inout);
    
  private:
    sample _y_1; //y(n-1)
    sample _y_2; //y(n-2)
    sample _x_1; //x(n-1)
};

class LIBTOOLS_PRIVATE AnalogHighPassFilter : public AnalogFilter
{
  public:
    AnalogHighPassFilter();
    AnalogHighPassFilter(float f, float m=0.5f);
    virtual ~AnalogHighPassFilter();
    virtual void step(Signal* inout);

  private:
    sample _y_1; //y(n-1)
    sample _x_1; //x(n-1)
};

class LIBTOOLS_PRIVATE AnalogHighPassFilter2 : public AnalogFilter
{
  public:
    AnalogHighPassFilter2();
    AnalogHighPassFilter2(float f, float m=0.5f);
    virtual ~AnalogHighPassFilter2();
    virtual void step(Signal* inout);

  private:
    sample _y_1; //y(n-1)
    sample _y_2; //y(n-2)
    sample _x_1; //x(n-1)
    sample _x_2; //x(n-2)
};

class LIBTOOLS_PRIVATE AnalogRejector : public AnalogFilter
{
  public:
    AnalogRejector();
    AnalogRejector(float f, float m=0.5f);
    virtual ~AnalogRejector();
    virtual void step(Signal*  inout);
  private:
    sample _y_1; //y(n-1)
    sample _y_2; //y(n-2)
    sample _x_1; //x(n-1)
    sample _x_2; //x(n-2)

};

#endif

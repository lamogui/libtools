
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

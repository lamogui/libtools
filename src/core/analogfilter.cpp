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
  * SECURITY LEVEL  : 8 (CRITICAL)  
  * VISIBILIT       : PRIVATE
  * © COPYDOWN™ LAMOGUI ALL RIGHTS RESERVED 
  *
  * FILE         : analogfilter.cpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  *                Kwon-Young Choi (kwonyoungchoi93@gmail.com)
  * VERSION      : 1.7 olol
  * DEPENDENCIES : analogfilter.hpp
  *            
  */
  
#include <libtools/core/analogfilter.hpp>

AnalogFilter::AnalogFilter()
{
}

AnalogFilter::AnalogFilter(float f, float m)
{
  setFrequency(f);
  setResonance(m);
}

AnalogFilter::~AnalogFilter()
{
}

void AnalogFilter::setFrequency(float f)
{
  _frequency.constant(f);
}

void AnalogFilter::setResonance(float m)
{
  _resonance.constant(m);
}

AnalogLowPassFilter::AnalogLowPassFilter() : AnalogFilter(), _y_1(0) {}
AnalogLowPassFilter::AnalogLowPassFilter(float f, float m) : AnalogFilter(f,m), _y_1(0) {}
AnalogLowPassFilter::~AnalogLowPassFilter() {}
void AnalogLowPassFilter::step(Signal* inout)
{
  sample* samples = inout->samples;
  sample* f = getFrequency().samples;
  const float pi_2 = 3.1415f*2.f;
  const float te = 1.f/(float)Signal::frequency;
  for (unsigned int i=0;i < Signal::size;i++)
  {
    const float w0=f[i]*pi_2;
    const float w0te=w0*te;
    const float d=1.f+w0te;
    const float a0=w0te/d;
    const float b1=1.f/d;
    
    _y_1 = samples[i] = a0*samples[i]+b1*_y_1;
  }
}

AnalogLowPassFilter2::AnalogLowPassFilter2() : AnalogFilter(), _y_1(0), _y_2(0) {}
AnalogLowPassFilter2::AnalogLowPassFilter2(float f, float m) : AnalogFilter(f,m), _y_1(0), _y_2(0){}
AnalogLowPassFilter2::~AnalogLowPassFilter2() {}
void AnalogLowPassFilter2::step(Signal* inout)
{
  sample* samples = inout->samples;
  sample* f = getFrequency().samples;
  sample* m = getResonance().samples;
  const float pi_2 = 3.1415f*2.f;
  const float te = 1.f/(float)Signal::frequency;
  const float _2te=te*2.f;
  for (unsigned int i=0; i<Signal::size;i++)
  {
    const float w0=f[i]*pi_2;
    const float w0te=w0*te;
    const float w02te=w0*_2te;
    const float w02te2=w0te*w0te;
    const float _2mw0te=m[i]*w02te;
    float a=w02te2+_2mw0te+1.f;
    a=1.f/a;
    const float b=_2mw0te+2.f;
    samples[i]=a*(_y_1*b-_y_2+w02te2*samples[i]);
    _y_2=_y_1;
    _y_1=samples[i];
  }
}

AnalogBandPassFilter2::AnalogBandPassFilter2() : AnalogFilter(), _y_1(0), _y_2(0), _x_1(0) {}
AnalogBandPassFilter2::AnalogBandPassFilter2(float f, float m) : AnalogFilter(f,m), _y_1(0), _y_2(0), _x_1(0) {}
AnalogBandPassFilter2::~AnalogBandPassFilter2() {}
void AnalogBandPassFilter2::step(Signal* inout)
{
  sample* samples = inout->samples;
  sample* f = getFrequency().samples;
  sample* m = getResonance().samples;
  const float pi_2 = 3.1415f*2.f;
  const float te = 1.f/(float)Signal::frequency;
  for (unsigned int i=0;i < Signal::size;i++)
  {
    const float w0=f[i]*pi_2;
    const float w0te=w0*te;
    const float mw0te=w0te*2.f*m[i];
    const float w0te2=w0te*w0te;
    const float d=1.f+mw0te+w0te2;
    const float a0=mw0te/d;
    const float b1=(2.f + mw0te)/d;
    const float b2=1.f/d;
    const float save_x_1 = samples[i];
    const float k=1.f/0.7f;
    samples[i] = k*a0*samples[i]-k*a0*_x_1+b1*_y_1-b2*_y_2;
    _y_2 = _y_1;
    _y_1 = samples[i];
    _x_1 = save_x_1;
  }
}

AnalogHighPassFilter::AnalogHighPassFilter() : AnalogFilter(), _y_1(0), _x_1(0) {}
AnalogHighPassFilter::AnalogHighPassFilter(float f, float m) : AnalogFilter(f, m), _y_1(0), _x_1(0) {}
AnalogHighPassFilter::~AnalogHighPassFilter() {}
void AnalogHighPassFilter::step(Signal* inout)
{
  sample* samples = inout->samples;
  sample* f = getFrequency().samples;
  const float pi_2 = 3.1415f*2.f;
  const float te = 1.f/(float)Signal::frequency;
  for (unsigned int i=0; i<Signal::size;i++)
  {
    const float w0=f[i]*pi_2;
    const float w0te=w0*te;
    float d=1.f+w0te;
    d=1.f/d;
    const float save_x_1=samples[i];
    samples[i]=_y_1*d+samples[i]*d-_x_1*d;
    _y_1= samples[i];
    _x_1=save_x_1;
  }
}

AnalogHighPassFilter2::AnalogHighPassFilter2() : AnalogFilter(), _y_1(0), _y_2(0), _x_1(0), _x_2(0) {}
AnalogHighPassFilter2::AnalogHighPassFilter2(float f, float m) : AnalogFilter(f, m), _y_1(0), _y_2(0), _x_1(0), _x_2(0) {}
AnalogHighPassFilter2::~AnalogHighPassFilter2() {}
void AnalogHighPassFilter2::step(Signal* inout)
{
  sample* samples = inout->samples;
  sample* f = getFrequency().samples;
  sample* m = getResonance().samples;
  const float pi_2 = 3.1415f*2.f;
  const float te = 1.f/(float)Signal::frequency;
  const float _2te=te*2.f;
  for (unsigned int i=0; i<Signal::size;i++)
  {
    const float w0=f[i]*pi_2;
    const float w0te=w0*te;
    const float w02te=w0*_2te;
    const float w02te2=w0te*w0te;
    const float _2mw0te=m[i]*w02te;
    float a=w02te2+_2mw0te+1.f;
    a=1.f/a;
    const float b=_2mw0te+2.f;
    const float _2_x_1=2*_x_1;
    const float save_x_1=samples[i];
    samples[i]=a*(_y_1*b-_y_2+samples[i]-_2_x_1+_x_2);
    _y_2=_y_1;
    _y_1=samples[i];
    _x_2=_x_1;
    _x_1=save_x_1;
  }
}

AnalogRejector::AnalogRejector() : AnalogFilter(), _y_1(0), _y_2(0), _x_1(0), _x_2(0) {}
AnalogRejector::AnalogRejector(float f, float m) : AnalogFilter(f, m), _y_1(0), _y_2(0), _x_1(0), _x_2(0) {}
AnalogRejector::~AnalogRejector() {}
void AnalogRejector::step(Signal* inout) {
  sample* samples = inout->samples;
  sample* f = getFrequency().samples;
  sample* m = getResonance().samples;
  const float te = 1.f/(float)Signal::frequency;
  for (unsigned int i=0; i<Signal::size; i++)
  {
    const float w0te=f[i]*te;
    const float w02te2=w0te*w0te;
    const float mw0te=m[i]*w0te;
    const float a=1.f/(w02te2+2.f*mw0te+1.f);
    const float b=2.f*mw0te+2.f;
    const float c=w02te2+1.f;
    const float save_x_1=samples[i];
    samples[i]=a*(_y_1*b-_y_2+samples[i]*c-2.f*_x_1+_x_2);
    _y_2=_y_1;
    _y_1=samples[i];
    _x_2=_x_1;
    _x_1=save_x_1;
  }
}



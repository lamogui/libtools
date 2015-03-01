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
  * VISIBILITY     : PRIVATE
  * © COPYDOWN™ LAMOGUI ALL RIGHTS RESERVED 
  *
  * FILE         : signal.hpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : V2.1 olol
  * DEPENDENCIES : fft.hpp
  *                signal.hpp
  * ROLE         : Déclare une classe gérant une unité de son (classe Signal) 
  *                Ainsi que des modeles abstrait pour le générer ce son...
  */
  

#include <libtools/core/signal.hpp>
#include <libtools/core/fft.hpp>
  
volatile unsigned int Signal::frequency=44100;
volatile unsigned int Signal::size=1024;
volatile float Signal::refreshRate=Signal::frequency/(float)Signal::size;
volatile unsigned int Signal::byteSize=sizeof(sample)*Signal::size;
volatile bool Signal::isPow2=true;

std::set<Signal*> Signal::_instances;

Signal::Signal() :
samples(0)
{
  const unsigned int size = Signal::byteSize;
  samples=(sample*) malloc(size);
  memset((void*) samples,0,size);
  _instances.insert(this);
}

Signal::Signal(const Signal& s) :
samples(0)
{
  const unsigned int size = Signal::byteSize;
  samples=(sample*) malloc(size);
  memcpy((void*) samples,(void*) s.samples,size);
  _instances.insert(this);
}

Signal::~Signal()
{
  if (samples)
    free((void*) samples);
  _instances.erase(this);
}

void Signal::mix(const Signal* s, unsigned int n)
{ 
  const unsigned int size = Signal::size;
  for (unsigned int k=0; k < n; k++)
  {
    for (unsigned int i=0; i < size;i++)
    {
      samples[i] *= s[k].samples[i];
    }
  }
}

void Signal::add(const Signal* s, unsigned int n)
{
  const unsigned int size = Signal::size;
  for (unsigned int k=0; k < n; k++)
  {
    for (unsigned int i=0; i < size;i++)
    {
      samples[i] += s[k].samples[i];
    }
  }
}

void Signal::addOffset(sample offset)
{
  const unsigned int size = Signal::size;
  for (unsigned int i=0; i < size;i++)
  {
    samples[i] += offset;
  }
}

void Signal::scale(sample scale)
{
  const unsigned int size = Signal::size;
  for (unsigned int i=0; i < size;i++)
  {
    samples[i] *= scale;
  }
}

void Signal::constant(sample constant)
{
  const unsigned int size = Signal::size;
  for (unsigned int i=0; i < size;i++)
  {
    samples[i] = constant;
  }
}

Signal& Signal::operator=(const Signal& s)
{
  memcpy((void*) samples,(void*) s.samples,Signal::byteSize);
  return *this;
}

void Signal::_reset()
{
  const unsigned int size = Signal::byteSize;
  samples=(sample*) realloc((void*)samples,size);
  memset((void*) samples,0,size);
}

void Signal::globalConfigurationFromFramerate(unsigned int f,  float r)
{
  if (f>=8000)
    Signal::frequency = f;

  if (r > 0)
    Signal::refreshRate = r; 
  Signal::size = (unsigned int) (Signal::frequency/Signal::refreshRate);
  Signal::byteSize = sizeof(sample)*Signal::size;
  Signal::isPow2 = false;
  for (std::set<Signal*>::iterator it = _instances.begin(); it != _instances.end() ;it++)
  {
    (*it)->_reset();
  }
}

void Signal::globalConfigurationFromPow2(unsigned int f,  unsigned short pow2)
{
  if (f>=8000)
    Signal::frequency = f;

  pow2 = FFT::getSupPow2(pow2);
  Signal::size = 1 << pow2;
  Signal::refreshRate = Signal::frequency/(float)Signal::size;
  Signal::byteSize = sizeof(sample)*Signal::size;
  Signal::isPow2 = true;
  for (std::set<Signal*>::iterator it = _instances.begin(); it != _instances.end() ;it++)
  {
    (*it)->_reset();
  }
}

void Signal::saturate(sample min, sample max)
{
  const unsigned int size = Signal::size;
  for (unsigned int i=0; i < size;i++)
  {
    if (samples[i] > max) samples[i] = max;
    else if (samples[i] < min) samples[i] = min;
  }
}

sample Signal::energy()
{
  sample energy=0;
  const unsigned int size = Signal::size;
  for (unsigned int i=0; i < size;i++)
  {
    energy += samples[i]*samples[i];
  }
  return energy;
}

void Signal::tfd(Signal& out_tfd)
{
  out_tfd.reset();
  const unsigned int size = Signal::size;
  for (unsigned int k=0; k < size;k++)
  {
    for (unsigned int n=0; n < size;n++)
    {
      double a=2*3.14159*k*n/(double)(size);
      double cos_a=cos(a);
      double sin_a=sin(a);
      out_tfd.samples[k] += (sample) (abs(samples[n])*(sqrt(abs(cos_a*cos_a+sin_a*sin_a))));  
    }
  }
}

sample Signal::level(){
  sample lvl=0;
  const unsigned int size = Signal::size;
  for (unsigned int k=0; k < size;k++)
  {
    const sample fs = fabs(samples[k]);
    if (fs > lvl) lvl = fs;
  }
  return lvl;
}

void Signal::glslize(){
  const unsigned int size = Signal::size;
  for (unsigned int k=0; k < size;k++)
  {
    samples[k]=(samples[k]*0.5f) + 0.5f;
  }
}


void Signal::normalize() {
  scale(1.f/(level()+0.001f));
}

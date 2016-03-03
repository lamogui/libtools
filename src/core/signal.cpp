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
volatile double Signal::refreshRate=(double)Signal::frequency/(double)Signal::size;
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
  Signal::refreshRate = (double)Signal::frequency/(double)Signal::size;
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

double Signal::energy() const
{
  double energy=0;
  const unsigned int size = Signal::size;
  for (unsigned int i=0; i < size;i++)
  {
    energy += samples[i]*samples[i];
  }
  return energy;
}

double Signal::fenergy(double f) const
{
  double real=0.0;
  double im=0.0;
  const unsigned int size = Signal::size;
  const double rate=Signal::frequency;
  for (unsigned int i=0; i < size;i++)
  {
    real+=cos(2.0*3.14159265359*f*(double)i/rate)*samples[i];
    im-=sin(2.0*3.14159265359*f*(double)i/rate)*samples[i];
  }
  return sqrt(real*real + im*im);
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

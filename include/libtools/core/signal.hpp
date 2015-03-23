
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
    volatile static float refreshRate;      //Le but c'est d'avoir 50 ici ! le plus possible sera le mieux
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

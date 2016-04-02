
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
  * THIS FILE IS PART OF NOISECONTROLLERS 
  * SECURITY LEVEL : 8 (CRITICAL)  
  * VISIBILITY     : PRIVATE
  * © COPYDOWN LAMOGUI ALL RIGHTS RESERVED 
  *
  * FILE         : ncprecomputer.hpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : 1.7 olol
  */
  
#ifndef NOISECONTROLLERS_NCPRECOMPUTER_HPP
#define NOISECONTROLLERS_NCPRECOMPUTER_HPP

#include <libtools/public/config.h>
#include <libtools/core/production.hpp>
#include <libtools/core/signal.hpp>
#include <libtools/core/fft.hpp>
#include <libtools/core/visualsignal.hpp>
#include <libtools/core/decoder.hpp>


//#define NDEBUG //when not bugged anymore
#include <assert.h>

#ifdef SFML_SYSTEM_HPP

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#else
#error TODO Implement QT/C++11 version of this
#endif


#include <vector>



template <typename T>
struct ncprecomputervalueframe {
    T brut; //brut (unfiltered)

    T l11; //low median filtered
    T m11; //median filtered
    T h11; //high median filtered

    T l11a11; //low median + average filtered
    T m11a11; //median + average filtered
    T h11a11; //high + average median filtered

    T l11a128;
    T m11a128;
    T h11a128;
};

typedef ncprecomputervalueframe<float> ncprecomputervalueframef;
typedef ncprecomputervalueframe<int16_t> ncprecomputervalueframe16;

typedef struct {
  ncprecomputervalueframe16 noise;
  ncprecomputervalueframe16 clean;
  ncprecomputervalueframef bass;
  ncprecomputervalueframef power;
  //ncprecomputervalueframef level;
} ncprecomputerframe;

template <typename T>
class NCPrecomputerValueTable
{
  public:

    NCPrecomputerValueTable()
    {
      clear();
    }

    //assume index not out of range
    void getValueFrame(
        ncprecomputervalueframe<T>& frame,
        unsigned int index) const
    {
      assert(_ready);
      frame.brut=_brut[index];

      frame.l11=_l11[index];
      frame.m11=_m11[index];
      frame.h11=_h11[index];

      frame.l11a11=_l11a11[index];
      frame.m11a11=_m11a11[index];
      frame.h11a11=_h11a11[index];

      frame.l11a128=_l11a128[index];
      frame.m11a128=_m11a128[index];
      frame.h11a128=_h11a128[index];
    }

    void clear()
    {
      _brut.clear();
      _l11.clear();
      _m11.clear();
      _h11.clear();
      _l11a11.clear();
      _m11a11.clear();
      _h11a11.clear();
      _l11a128.clear();
      _m11a128.clear();
      _h11a128.clear();
      _ready=false;
    }

    void feed(T value)
    {
      assert(!_ready);
      _brut.push_back(value);
    }

    void reserve(unsigned int n)
    {
      _brut.reserve(n);
      _l11.reserve(n);
      _m11.reserve(n);
      _h11.reserve(n);
      _l11a11.reserve(n);
      _m11a11.reserve(n);
      _h11a11.reserve(n);
      _l11a128.reserve(n);
      _m11a128.reserve(n);
      _h11a128.reserve(n);
    }


    float computeHighTempo() const;
    float computeMedTempo() const;
    float computeLowTempo() const;

    bool isReady() const
    {
      return _ready;
    }

    void finalize();

  private:
    bool _ready;

    std::vector<T> _brut;

    std::vector<T> _l11;
    std::vector<T> _m11;
    std::vector<T> _h11;

    std::vector<T> _l11a11;
    std::vector<T> _m11a11;
    std::vector<T> _h11a11;

    std::vector<T> _l11a128;
    std::vector<T> _m11a128;
    std::vector<T> _h11a128;
};

typedef NCPrecomputerValueTable<float> NCPrecomputerValueTablef;
typedef NCPrecomputerValueTable<int16_t> NCPrecomputerValueTable16;


class NCPrecomputer : public MusicProduction
{
  public:
    NCPrecomputer();
    ~NCPrecomputer();
    
    
    //Launch precomputation on seperate thread
    void launch(MusicProduction& decoder, const string_t &f="");

    void reset(); //Reset precomputer state 
    bool process(MusicProduction& decoder); //Decode the entire production and process computation


    inline bool threadReady() { return _endOfThread; }
    inline bool valid() { return (_fullyDecoded && _fullyComputed);}
    
    inline string_t name() { return _name; }
    inline string_t author() { return _artist; }
    inline Production::GenreId genre() {return _genre;}
    
    unsigned int fetch(Signal& outleft, Signal& outright);
    
    bool ended() const { return (_currentFrame >= _frames); }
    inline void rewind() { _currentFrame=0; }
    inline double length() const { return (double)_frames/(double)Signal::refreshRate; }
    
    inline unsigned int frames() { return _frames; }
    inline unsigned int currentFrame() { return _currentFrame; }
    
    bool getFrame(ncprecomputerframe& frame, unsigned int i);
    inline bool getCurrentFrame(ncprecomputerframe& frame) {
      return getFrame(frame, _currentFrame);
    }
    
    template<typename T> 
    static void medianFilter(const std::vector<T>& brut, 
                             std::vector<T>& low,
                             std::vector<T>& med,
                             std::vector<T>& high,
                             const unsigned int samples); 
    template<typename T>
    static void averageFilter(const std::vector<T>& brut,
                              std::vector<T>& aver,
                              const unsigned int samples);

    template<typename T>
    static float computeTempo(const std::vector<T>& brut,
                              const std::vector<T>& filtered,
                              const std::vector<T>& more_filtered);
    
    inline float processTime()
    {
      return _precomputationTime;
    }

    inline void wait()
    {
      _thread.wait();
    }

    inline float progress() const
    {
      return _computationprogress;
    }

  protected:
    //Pour la classe fille
    virtual void additionalSignalComputation(
        const Signal& left,
        const Signal& right);
    virtual void additionalFinalize();
    virtual void additionalReset();

  private:

    //For parallel processing
    void _thread_func();

    sf::Thread _thread;

  protected:
    std::vector<sample> _left;
    std::vector<sample> _right;

    NCPrecomputerValueTable16 _noise;
    NCPrecomputerValueTable16 _clean;
    NCPrecomputerValueTablef _bass;
    NCPrecomputerValueTablef _power;
    //NCPrecomputerValueTablef _level;

  private:
    volatile bool _fullyDecoded;
    volatile bool _fullyComputed;
    volatile bool _endOfThread;
    
    volatile unsigned int _frames;
    volatile unsigned int _currentFrame;

  protected:
    const unsigned int _maxframes;

  private:
    MusicProduction* _decoder;
    string_t _fileToOpen;

    sf::Clock _chrono;
    float _precomputationTime;
    volatile float _computationprogress;

    string_t _name;
    string_t _artist;
    Production::GenreId _genre;
};


template<typename T>
float NCPrecomputerValueTable<T>::computeHighTempo() const
{
  return NCPrecomputer::computeTempo(_brut,_h11a11,_h11a128);
}


template<typename T>
float NCPrecomputerValueTable<T>::computeMedTempo() const
{
  return NCPrecomputer::computeTempo(_brut,_m11a11,_m11a128);
}


template<typename T>
float NCPrecomputerValueTable<T>::computeLowTempo() const
{
  return NCPrecomputer::computeTempo(_brut,_l11a11,_l11a128);
}

#endif



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
  * FILE         : note.hpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : V2.0 olol
  * DEPENDENCIES : config.h
  * ROLE         : Déclare les classes permettant de 
  *                manipuler des données rythmiques en 
  *                lien avec un morceau et un instrument
  */


#ifndef LIBTOOLS_NOTE_HPP
#define LIBTOOLS_NOTE_HPP

#include <libtools/public/config.h>

//a music note
class LIBTOOLS_PRIVATE Note
{
  public:
    Note(unsigned int st, unsigned char id, float v=1.0);
    Note(const Note& c);
    virtual ~Note();
    
    Note& operator=(const Note&);

    inline unsigned char id()
    {
      return _id;
    }
	
	inline void setId(unsigned char i)
	{
	  _id=i;
	}
	
    inline float velocity()
    {
      return _velocity;
    }
    
	inline void setVelocity(float v)
	{
	  _velocity=v;
	}
	
	inline unsigned int start()
	{
	  return _start;
	}
	
	inline void setStart(unsigned int s)
	{
	  _start=s;
	}
	
	inline unsigned int length()
	{
	  return _length;
	}
	
	inline void setLength(unsigned int l)
	{
	  _length=l;
	}
	
    
    inline float frequency() { return (float) noteFrequency[_id]; }
    
    static inline float getFrequencyFromID(unsigned char id)
    {
      return (float) noteFrequency[id];
    }
	
    static bool isBlack(unsigned char id);
  
  private:
    static double noteFrequency[256];    
    unsigned int _start; //time where the note is appeared
    unsigned int _length; //note duration
	  float _velocity;
	  unsigned char _id;

};

#define NOTE_BEGIN 0
#define LA_1   0
#define SIb_1  1
#define SI_1   2
#define DO_1   3
#define REb_1  4
#define RE_1   5
#define MIb_1  6
#define MI_1   7
#define FA_1   8
#define SOLb_1 9
#define SOL_1  10
#define LAb_1  11

#define LA_2   12
#define SIb_2  13
#define SI_2   14
#define DO_2   15
#define REb_2  16
#define RE_2   17
#define MIb_2  18
#define MI_2   19
#define FA_2   20
#define SOLb_2 21
#define SOL_2  22
#define LAb_2  23

#define LA_3   24
#define SIb_3  25
#define SI_3   26
#define DO_3   27
#define REb_3  28
#define RE_3   29
#define MIb_3  30
#define MI_3   31
#define FA_3   32
#define SOLb_3 33
#define SOL_3  34
#define LAb_3  35

#define LA_4   36
#define SIb_4  37
#define SI_4   38
#define DO_4   39
#define REb_4  40
#define RE_4   41
#define MIb_4  42
#define MI_4   43
#define FA_4   44
#define SOLb_4 45
#define SOL_4  46
#define LAb_4  47

#define LA_5   48
#define SIb_5  49
#define SI_5   50
#define DO_5   51
#define REb_5  52
#define RE_5   53
#define MIb_5  54
#define MI_5   55
#define FA_5   56
#define SOLb_5 57
#define SOL_5  58
#define LAb_5  59

#define LA_6   60
#define SIb_6  61
#define SI_6   62
#define DO_6   63
#define REb_6  64
#define RE_6   65
#define MIb_6  66
#define MI_6   67
#define FA_6   68
#define SOLb_6 69
#define SOL_6  70
#define LAb_6  71

#define LA_7   72
#define SIb_7  73
#define SI_7   74
#define DO_7   75
#define REb_7  76
#define RE_7   77
#define MIb_7  78
#define MI_7   79
#define FA_7   80
#define SOLb_7 81
#define SOL_7  82
#define LAb_7  83

#define LA_8   84
#define SIb_8  85
#define SI_8   86
#define DO_8   87
#define NOTE_END 88
#define NB_NOTES (NOTE_END-NOTE_BEGIN)

#define NOT_A_NOTE 255

#endif

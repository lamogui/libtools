
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
  * SECURITY LEVEL : 8 (CRITICAL)  
  * VISIBILITY     : PROTECTED
  * © COPYDOWN™ LAMOGUI ALL RIGHTS RESERVED 
  *
  * FILE         : peak.hpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : V2.0 olol
  * DEPENDENCIES : config.h
  *                note.hpp
  *                signal.hpp
  */



#ifndef LIBTOOLS_PEAK_HPP
#define LIBTOOLS_PEAK_HPP

#include <libtools/public/config.h>
#include <vector>
#include <libtools/core/signal.hpp>
#include <libtools/core/note.hpp>

class LIBTOOLS_PUBLIC TFDeLoorPeak 
{
  public:
    sample amp;
    sample max_amp;
    sample l;
    sample r;
    unsigned int frames;
    unsigned char note_id;
    unsigned char begin_note_id;
    unsigned char width;
    unsigned char begin_width;
    bool updated;
    bool alive;
    bool oscillate;
};

class LIBTOOLS_PRIVATE TFDeLoorPeakManager
{
  public:
    TFDeLoorPeakManager();
    ~TFDeLoorPeakManager();

    void reset();


    int rouge;
    int vert;
    int bleu;
    int jaune;

    void update(bool create, bool kill, sample* l, sample* r=0);
    unsigned int mountains_position;

    TFDeLoorPeak* peaks;
    sample previouspeaksl[NB_NOTES];
    sample previouspeaksr[NB_NOTES];

  private:
    void _unupdate();
    void _clean();
};  

#endif 
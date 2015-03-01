

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
  * FILE         : peak.cpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : V2.0 olol
  * DEPENDENCIES : peak.hpp
  */

  
#include <libtools/core/peak.hpp>


TFDeLoorPeakManager::TFDeLoorPeakManager()
{
  peaks = new TFDeLoorPeak[NB_NOTES];
  reset();

  rouge=0;
  vert=0;
  bleu=0;
  jaune=0;
}

TFDeLoorPeakManager::~TFDeLoorPeakManager()
{
  delete[] peaks;
}

void TFDeLoorPeakManager::reset()
{
  for (unsigned int n=0; n < NB_NOTES; n++){
      previouspeaksl[n] = 0;
      previouspeaksr[n] = 0;
      peaks[n].alive=false;
  }
}

void TFDeLoorPeakManager::update(bool create, bool kill, sample* l, sample* r){
  
  if (!l) return;

  _unupdate();

  sample previous=0;
  sample peak_max=0;
  unsigned int begin=0;
  unsigned int max=0;
  const sample seuil = 0.025;
  mountains_position=0;
  

  for (unsigned int n=0; n < NB_NOTES; n++){
    if (l[n] > peak_max) //monté
    {
      peak_max = l[n];
      max=n;
    }
    else if (l[n] > previous) //fin de descente PEAK Detecté
    {
      
      //Y avait t'il un pic a côté ? avant
      if (max != NB_NOTES-1 && peaks[max+1].alive) //Swipe Vert
      {
        peaks[max]=peaks[max+1];
        peaks[max+1].alive=false;
        if (max == peaks[max].begin_note_id) peaks[max].oscillate=true;
        
      }
      else if (max && peaks[max-1].alive) //Swipe Bleu
      {
        peaks[max]=peaks[max-1];
        peaks[max-1].alive=false;
        if (max == peaks[max].begin_note_id) peaks[max].oscillate=true;
      }

      if (peaks[max].alive) //Le PEAK existe ? alors mettons le à jour
      {
        peaks[max].amp=peak_max;
        peaks[max].frames++; 
        peaks[max].width=n-begin;
        peaks[max].note_id=max;
        peaks[max].updated=true;
        if (peaks[max].note_id > peaks[max].begin_note_id)
          bleu += peaks[max].note_id - peaks[max].begin_note_id + 1;
        else if (peaks[max].note_id < peaks[max].begin_note_id) {
          vert += peaks[max].begin_note_id - peaks[max].note_id + 1;
          if (max > FA_5) bleu += peaks[max].begin_note_id - peaks[max].note_id + 1;
        }
        if (peaks[max].oscillate) jaune += (int)(peaks[max].max_amp*5.f) + 3;

        //Le peak augmente (attack) !
        if (peak_max > peaks[max].max_amp) {
          peaks[max].max_amp=peak_max;
          if (peaks[max].frames < 10) bleu += (int)(peaks[max].max_amp*5.f) + 3;
        }

        //le peak descend (Release) !
        if (kill && peak_max*3.f < peaks[max].max_amp) {
          peaks[max].alive = false;
        }

      }
      //
      else if (/*peak_max > seuil||*/ (create && 2.f*previouspeaksl[max] < peak_max  && previouspeaksl[max] > 0.0001 && peak_max > seuil) || peak_max > 0.25 ) {
        peaks[max].alive = true;
        peaks[max].max_amp=peak_max;
        peaks[max].amp=peak_max;
        peaks[max].l=peak_max;
        peaks[max].r=0;
        peaks[max].begin_width=n-begin;
        peaks[max].width=n-begin;
        peaks[max].begin_note_id=max;
        peaks[max].note_id=max;
        peaks[max].frames=0;
        peaks[max].updated=true;
        peaks[max].oscillate=false;
      }

      if (n-begin > 4 && peak_max > 0.2 && n < DO_5)
         mountains_position=n;

      //Pret pour la detection d'un nouveau PEAK
      begin=n;
      max=n;
      peak_max=l[n];
    }
    previous = l[n];
  }

  //rouge+=mountains_position;


  for (unsigned int n=0; n < NB_NOTES; n++) {
    previouspeaksl[n]=l[n];
  }

  if (kill)
    _clean();
}


void TFDeLoorPeakManager::_unupdate()
{
  rouge=0;
  vert=0;
  bleu=0;
  jaune=0;
  for (unsigned int n=0; n < NB_NOTES; n++){
      peaks[n].updated=false;
  }
}

void TFDeLoorPeakManager::_clean() {
  for (unsigned int n=0; n < mountains_position; n++){
    if (peaks[n].alive) {
      if (!peaks[n].updated) peaks[n].alive=false;
      else rouge += (peaks[n].width)*(int)(peaks[n].amp*5.f) + 1;
    }
  }
  sample max=0;
  for (unsigned int n=mountains_position; n < NB_NOTES; n++){
    if (peaks[n].alive) {
      if (!peaks[n].updated) peaks[n].alive=false;
    }
  }

  if ( mountains_position > FA_2)
  {

    for (unsigned int n=mountains_position; n < NB_NOTES; n++){
      if (peaks[n].alive) {
        if (peaks[n].amp < 0.3*max) peaks[n].alive=false;
        else if ( peaks[n].amp < 0.03) peaks[n].alive=false;
        else {
          if (peaks[n].frames > 4) vert += (int)(peaks[n].amp*5.f) + 2;
        }
      }
    }
  }
  else
  {
    for (unsigned int n=mountains_position; n < NB_NOTES; n++){
      if (peaks[n].alive) {
        if (peaks[n].amp < 0.3*max) peaks[n].alive=false;
        else {
          if (n >= FA_5) bleu += (int)(peaks[n].amp*5.f) + 2;
          else if (peaks[n].frames > 4) vert += (int)(peaks[n].amp*5.f) + 2;
        }
      }
    }
  }
  
}
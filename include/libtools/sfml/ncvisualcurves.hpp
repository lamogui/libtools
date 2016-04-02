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
  * VISIBILITY     : PROTECTED
  * © COPYDOWN LAMOGUI ALL RIGHTS RESERVED 
  *
  * FILE         : ncvisualcurves.hpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : V2.0 olol
  */


#ifndef NOISECONTROLLERS_NCVISUALCURVES_HPP
#define NOISECONTROLLERS_NCVISUALCURVES_HPP

#include <libtools/public/config.h>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <map>
#include <libtools/core/ncprecomputer.hpp>
#include <libtools/sfml/interface.hpp>
#include <libtools/core/circularpurger.hpp>
#include <libtools/sfml/glslrender.hpp>
#include <libtools/sfml/sampler1d.hpp>
#include <libtools/shaders/shaders.h>


class NCVisualCurves : public Interface
{
  public:
  
    typedef struct {
      sf::Color color;
      circularpurger<float>* buffer;
      GLSLRender render;
      Sampler1D sampler;
      sf::RectangleShape bar;
      string_t name;
      int16_t value;
    } tracker;
    
  
    NCVisualCurves(
      const sf::Vector2f& size,
      const sf::Font&,
      const string_t& title="NCCurves");
    virtual ~NCVisualCurves();

    
    void addTracker(const string_t name, tracker& t);
    virtual void update();
    

  private:
    void _computeText();
    void _place();
  
    string_t _title;
    float _maxwidth;
    sf::Text _text;
    std::map<string_t, tracker*> _trackers;

};


class NCPrecomputerValueCurves : public NCVisualCurves
{
  public:
    NCPrecomputerValueCurves(const sf::Vector2f& size,
                             const sf::Font&font,
                             const string_t& title);

    void feed(const ncprecomputervalueframe16& frame, float diviser);
    void feed(const ncprecomputervalueframef& frame, int16_t multiplier);

  private:

    void _createBuffers();
    void _setupColors();
    void _addTrackers(const string_t base);

    NCVisualCurves::tracker _brut;

    /*
    NCVisualCurves::tracker _l11;
    NCVisualCurves::tracker _m11;
    NCVisualCurves::tracker _h11;
    */
    NCVisualCurves::tracker _l11a11;
    NCVisualCurves::tracker _m11a11;
    NCVisualCurves::tracker _h11a11;

    NCVisualCurves::tracker _l11a128;
    NCVisualCurves::tracker _m11a128;
    NCVisualCurves::tracker _h11a128;


};


#endif

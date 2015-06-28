

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
  * FILE         : window.hpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : V1.0 olol
  * DEPENDENCIES : config.h
  *                interface.hpp
  * ROLE         : Defini une fenetre qui utilise des interfaces
  */


#ifndef LIBTOOLS_WINDOW_HPP
#define LIBTOOLS_WINDOW_HPP
#include <libtools/public/config.h>
#include <libtools/sfml/interface.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <vector>
#include <queue>

#ifdef LIBTOOLS_WINDOWS
#include <windows.h>
#endif


class NEWindow : public sf::RenderWindow
{
  public:
    NEWindow(sf::VideoMode mode,
             const sf::String& title,
             const sf::Font&,
             const sf::ContextSettings& settings = sf::ContextSettings());
    virtual ~NEWindow();



    inline void setTitleCharacterSize(unsigned int s)
    {
      _title.setCharacterSize(s);
    }

    //NEWindow NEVER clean or deletes interfaces registred
    //Order Up to Down
    inline void registerInterface(Interface& i) {
      _interfaces.push_back(&i);
    }


    //NEWindow NEVER clean or deletes MouseCatcher registred
    inline void registerMouseCatcher(MouseCatcher& m) {
      _mouseCatchers.push_back(&m);

    }

    inline const sf::View getFullView() const {
      return _fullView;
    }

    inline const sf::Vector2f& clientSize() const {
      return _clientSize;
    }

    void setBackgroundTexture(const std::string& name,
                              const sf::Vector2i center);


    //Arrange interfaces disposal
    void arrange();
    bool useEvent(const sf::Event& event);

    bool checkInterrupt();

    void drawContent();
    inline void removeInterfaces() {
      _interfaces.clear();
    }

    void minimize();

#ifdef LIBTOOLS_WINDOWS
  void setZPosition(HWND insert_after);
  void setTransparency(uint8_t alpha);
  void setMaskColor(const sf::Color& ref);
#endif

    void destroy();
    void setMinSize(const sf::Vector2u size);
    void setMaxSize(const sf::Vector2u size);

    const sf::Vector2u& maxSize() const {
      return _maxSize;
    }

    const sf::Vector2u& minSize() const{
      return _minSize;
    }

  protected:

    virtual void onCreate();
    virtual void onResize();

    ///Paramètres de la fenêtre
    //Taille minimale et maximale de la fenetre
    sf::Vector2u _maxSize;
    sf::Vector2u _minSize;
    //Tailles des bordures
    float _borderSizeUp;
    float _borderSizeDown;
    float _borderSizeRight;
    float _borderSizeLeft;
    //Taille alloué pour les interfaces
    sf::Vector2f _clientSize;
    sf::Vector2f _viewportMin;
    sf::Vector2f _viewportMax;


    //Variables de gestion de l'état de la fenêtre
    bool _onMoveWin;
    bool _onResizeWin;
    int _onClose;
    int _onMinimize;

    //previous positions...
    sf::Vector2i _previousWinPos;
    sf::Vector2i _previousMousePos;
    sf::Vector2u _previousWinSize;


#ifdef LIBTOOLS_WINDOWS
    //Buttons
    Button _closeButton;
    Button _minimizeButton;

    //Resize Triangle
    sf::ConvexShape _resizeTriangle;
#endif
    //Windows Title
    sf::Text _title;

    //Interface manager
    Interface* _currentInterfaceCatcher;
    std::vector<Interface*> _interfaces;

    //Mouse Catcher Manager
    MouseCatcher* _currentMouseCatcher;
    std::vector<MouseCatcher*> _mouseCatchers;

    //windows FULL VIEW
    sf::View _fullView;

    //Background image
    sf::Sprite _backSprite;
    sf::Texture _backTexture;
    sf::Vector2i _backCenter;

  public:
    //Files dropped
    std::queue<string_t> filesDropped;



#ifdef LIBTOOLS_WINDOWS
    static bool hasUnicodeSupport();
    static LRESULT CALLBACK neWinProc(HWND hwnd,
                                  UINT uMsg,
                                  WPARAM wParam,
                                  LPARAM lParam);
    static void registerClass();
    static void forwardMessages();
    static bool sendFilepathsW(const wchar_t *win_name , int argc,wchar_t** argv);

    static void setCurrentDirToExecutablePath();

  protected:
    static std::map<HWND,NEWindow*> _wins;
  private:
    bool _cleanExit;
#endif
};

#endif

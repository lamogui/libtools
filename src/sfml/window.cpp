

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
  * FILE         : window.cpp
  * AUTHORS      : Julien De Loor (julien.deloor@gmail.com)
  * VERSION      : V1.0 olol
  * DEPENDENCIES : window.hpp
  * ROLE         : Defini une fenetre qui utilise des interfaces
  */



#ifdef _MSC_VER
#pragma warning( disable : 4244 ) //"Possible perte de données"
#endif

#include <iostream>
#include <libtools/sfml/window.hpp>
#include <assert.h>

#ifdef LIBTOOLS_WINDOWS
#include <windows.h>

std::map<HWND,NEWindow*> NEWindow::_wins;
#endif

NEWindow::NEWindow(sf::VideoMode mode,
                   const sf::String& title,
                   const sf::Font& font,
                   const sf::ContextSettings& settings) :
#ifdef LIBTOOLS_WINDOWS
//sf::RenderWindow(mode,title,0,settings),
sf::RenderWindow(),
#else
sf::RenderWindow(mode,title,sf::Style::Default,settings),
#endif
_maxSize(-1,-1),
_minSize(400,200),
_borderSizeUp(40),
_borderSizeDown(20),
_borderSizeRight(20),
_borderSizeLeft(20),
_clientSize(mode.width-_borderSizeRight-_borderSizeLeft,
            mode.height-_borderSizeUp-_borderSizeDown),
_viewportMin((float)_borderSizeLeft/(float)mode.width,
             (float)_borderSizeUp/(float)mode.width),
_viewportMax((float)_clientSize.x/(float)mode.width,
             (float)_clientSize.y/(float)mode.width),
_onMoveWin(false),
_onResizeWin(false),
_onClose(0),
_onMinimize(0),
#ifdef LIBTOOLS_WINDOWS
_closeButton(sf::Vector2f(_borderSizeRight+_borderSizeLeft,
                          _borderSizeUp*0.5f),"X",font),
_minimizeButton(sf::Vector2f((_borderSizeRight+_borderSizeLeft)*2.f/3.f,
                              _borderSizeUp*.5f),"-",font),
#endif
_title(title,font,11),
_currentInterfaceCatcher(NULL),
_currentMouseCatcher(NULL),
_fullView(sf::FloatRect(0,0,mode.width,mode.height)),
_backSprite(),
_backTexture(),
_backCenter(0,0),
_cleanExit(false)
{
#ifdef LIBTOOLS_WINDOWS


  //Close Button params
  _closeButton.linkTo(&_onClose,ButtonMode::on);
  _closeButton.setOutlineThickness(0);
  _closeButton.setClickedColor(sf::Color(200,42,42,128));
  _closeButton.setIdleColor(sf::Color(142,42,42,128));

  //minimize Button params
  _minimizeButton.linkTo(&_onMinimize,ButtonMode::on);
  _minimizeButton.setOutlineThickness(0);



  //ResizeTriangle
  _resizeTriangle.setPointCount(3);
  _resizeTriangle.setPoint(0, sf::Vector2f(15, 0));
  _resizeTriangle.setPoint(1, sf::Vector2f(15, 15));
  _resizeTriangle.setPoint(2, sf::Vector2f(0, 15));
  _resizeTriangle.setOrigin(0,0);
  _resizeTriangle.setFillColor(sf::Color(75,75,75,255));

  registerMouseCatcher(_closeButton);
  registerMouseCatcher(_minimizeButton);


    // Compute position and size
    HDC screenDC = GetDC(NULL);
    int left   = (GetDeviceCaps(screenDC, HORZRES) - static_cast<int>(mode.width))  / 2;
    int top    = (GetDeviceCaps(screenDC, VERTRES) - static_cast<int>(mode.height)) / 2;
    int width  = mode.width;
    int height = mode.height;
    ReleaseDC(NULL, screenDC);

    // Choose the window style according to the Style parameter
    sf::Uint32 style=0;
    DWORD win32Style = WS_VISIBLE | WS_POPUP;
    RECT rectangle = {0, 0, width, height};
    AdjustWindowRect(&rectangle, win32Style, false);
    width  = rectangle.right - rectangle.left;
    height = rectangle.bottom - rectangle.top;

    HWND hWnd=0;
    // Create the window
    //if (hasUnicodeSupport())
    {
        hWnd = CreateWindowExW(WS_EX_ACCEPTFILES,L"NEWin98", title.toWideString().c_str(), win32Style, left, top, width, height, NULL, NULL, GetModuleHandle(NULL), 0);
    }
    /*else
    {
        hWnd = CreateWindowExA(WS_EX_ACCEPTFILES,"NEWin98", title.toAnsiString().c_str(), win32Style, left, top, width, height, NULL, NULL, GetModuleHandle(NULL), 0);
    }*/
    NEWindow::_wins.insert(std::pair<HWND,NEWindow*>(hWnd,this)); //[hWnd]=this;

    create(hWnd,settings);

    // By default, the OS limits the size of the window the the desktop size,
    // we have to resize it after creation to apply the real size
    setSize(sf::Vector2u(mode.width, mode.height));

    assert(hWnd==getSystemHandle());


#endif

  arrange();


}

NEWindow::~NEWindow(){
  #ifdef LIBTOOLS_WINDOWS
  assert(_cleanExit);
  #endif
}

void NEWindow::destroy()
{
#ifdef LIBTOOLS_WINDOWS
  HWND save = getSystemHandle();
#endif

  close(); //Destroy SFML window

#ifdef LIBTOOLS_WINDOWS
  _cleanExit=true;
  _wins.erase(save);
  DestroyWindow(save); //Effective destroy
#endif
}

void NEWindow::onCreate()
{

}

void NEWindow::onResize()
{
  sf::Vector2u newSize(getSize());

  bool valid=true;
  if (newSize.x > _maxSize.x)
  {
     newSize.x = _maxSize.x;
     valid=false;
  }
  else if (newSize.x < _minSize.x)
  {
     newSize.x=_minSize.x;
     valid=false;
  }
  if (newSize.y > _maxSize.y)
  {
     newSize.y = _maxSize.y;
     valid=false;
  }
  else if (newSize.y < _minSize.y)
  {
     newSize.y=_minSize.y;
     valid=false;
  }
  if (valid)
  {
    arrange();
  }
  else
  {
    setSize(newSize);
  }
}


void NEWindow::setMinSize(const sf::Vector2u size)
{
   _minSize=size;
   onResize();
}

void NEWindow::setMaxSize(const sf::Vector2u size)
{
   _maxSize=size;
   onResize();
}

bool NEWindow::useEvent(const sf::Event& event)
{
  switch (event.type)
  {
    case sf::Event::Closed: {
      close();
      return true;
                            }
    case sf::Event::Resized:
      return true;

    ///Gestion de la capture de la souris pour les Mouse Catchers
    case sf::Event::MouseButtonPressed:
      if (event.mouseButton.button == sf::Mouse::Left)
      {
        const sf::Vector2i mousePosition(event.mouseButton.x,
                                         event.mouseButton.y);
        _previousMousePos=mousePosition;
        _previousWinPos=getPosition();
        _previousWinSize=getSize();
        if (!_currentMouseCatcher) //Priorité aux interfaces
        {
          for (unsigned int i =0; i < _interfaces.size(); i++)
          {
            if (!_interfaces[i]->onIt(event.mouseButton.x,
                                      event.mouseButton.y,
                                      getSize().x,
                                      getSize().y
                                      )) continue;

            sf::Vector2f v = mapPixelToCoords(mousePosition,
                                              _interfaces[i]->getView());
            if ((_currentMouseCatcher = _interfaces[i]->onMousePress(v.x,v.y)))
            {
              _currentInterfaceCatcher = _interfaces[i];
              return true;
            }
          }
        }
        sf::Vector2f v = mapPixelToCoords(mousePosition,_fullView);
        if (!_currentMouseCatcher) //ensuite élèments de la fenetre
        {
          for (unsigned int i =0; i < _mouseCatchers.size(); i++)
          {
            if (_mouseCatchers[i]->onMousePress(v.x,v.y))
            {
              _currentMouseCatcher=_mouseCatchers[i];
              _currentInterfaceCatcher = 0;
              return true;
            }
          }
        }
#ifdef LIBTOOLS_WINDOWS
        if (!_currentMouseCatcher) //enfins éléments spéciaux
        {
          if (mousePosition.x > getSize().x - _borderSizeRight &&
              mousePosition.y > getSize().y - _borderSizeDown)
          {
            _onResizeWin=true;
            _resizeTriangle.setFillColor(sf::Color(142,142,142,255));

          }
          else
            _onMoveWin=true;
          return true;
        }
#endif
      }
      break;
    case sf::Event::MouseButtonReleased:
      if (event.mouseButton.button == sf::Mouse::Left)
      {
        const sf::Vector2i mousePosition(event.mouseButton.x,
                                         event.mouseButton.y);
        if (_currentMouseCatcher && _currentInterfaceCatcher)
        {
          sf::Vector2f v = mapPixelToCoords(mousePosition,
                                       _currentInterfaceCatcher->getView());
          _currentMouseCatcher->onMouseRelease(v.x,v.y);
          _currentMouseCatcher=NULL;
          _currentInterfaceCatcher=NULL;
        }
        else if (_currentMouseCatcher)
        {
          sf::Vector2f v = mapPixelToCoords(mousePosition,_fullView);
          _currentMouseCatcher->onMouseRelease(v.x,v.y);
          _currentMouseCatcher=NULL;
          _currentInterfaceCatcher=NULL;
        }
#ifdef LIBTOOLS_WINDOWS
        else if (_onResizeWin)
        {
          _onResizeWin=false;
          _resizeTriangle.setFillColor(sf::Color(75,75,75,255));
        }
        else if (_onMoveWin)
        {
          _onMoveWin=false;
        }
#endif
        else return false;
        checkInterrupt();
        return true;
      }
      break;
    case sf::Event::MouseMoved:
      {
        const sf::Vector2i mousePosition(event.mouseMove.x,
                                         event.mouseMove.y);
        if (_currentMouseCatcher && _currentInterfaceCatcher)
        {
          sf::Vector2f v = mapPixelToCoords(mousePosition,
                                         _currentInterfaceCatcher->getView());
          _currentMouseCatcher->onMouseMove(v.x,v.y);
        }
        else if (_currentMouseCatcher)
        {
          sf::Vector2f v = mapPixelToCoords(mousePosition,_fullView);
          _currentMouseCatcher->onMouseMove(v.x,v.y);
        }
#ifdef LIBTOOLS_WINDOWS
        else if (_onMoveWin)
        {
          setPosition(mousePosition-_previousMousePos+_previousWinPos);
          _previousWinPos=getPosition();
        }
        else if (_onResizeWin)
        {
          if (mousePosition.x > 0 && mousePosition.y > 0)
          {
            sf::Vector2u newSize(mousePosition.x-_previousMousePos.x + _previousWinSize.x,
                                 mousePosition.y-_previousMousePos.y + _previousWinSize.y);


            if (newSize.x > _maxSize.x)
            {
               newSize.x = _maxSize.x;
            }
            else if (newSize.x < _minSize.x)
            {
               newSize.x=_minSize.x;
            }
            if (newSize.y > _maxSize.y)
            {
               newSize.y = _maxSize.y;
            }
            else if (newSize.y < _minSize.y)
            {
               newSize.y=_minSize.y;
            }
            setSize(newSize);

          }
        }
#endif
        else return false;
        checkInterrupt();
        return true;
      }

    case sf::Event::KeyPressed:
      break;
    case sf::Event::KeyReleased:
      if (event.key.code == sf::Keyboard::Escape)
      {
         close();
         return true;
      }
      break;
    default:
      break;
  }

  return false;
}

void NEWindow::arrange() {
  _title.setPosition(_borderSizeLeft,5.f);

  _clientSize.x=getSize().x-_borderSizeLeft-_borderSizeRight;
  _clientSize.y=getSize().y-_borderSizeUp-_borderSizeDown;
  _fullView = sf::View(sf::FloatRect(0,0,getSize().x,getSize().y));

  _viewportMin.x=_borderSizeLeft/(float)getSize().x;
  _viewportMin.y=_borderSizeUp/(float)getSize().y;
  _viewportMax.x=_clientSize.x/(float)getSize().x;
  _viewportMax.y=_clientSize.y/(float)getSize().y;
#ifdef LIBTOOLS_WINDOWS
  _resizeTriangle.setPosition(_clientSize.x+_borderSizeLeft,
                              _clientSize.y+_borderSizeUp);
  _closeButton.setPosition(_clientSize.x-5,5);
  _minimizeButton.setPosition(_closeButton.getPosition().x-_minimizeButton.getSize().x-5,5);
#endif

   _backSprite.setPosition(-(int)_backCenter.x+(int)_backCenter.x*(int)getSize().x/((int)_backTexture.getSize().x+1),
                           (int)_backCenter.y*(int)getSize().y/(float)(_backTexture.getSize().y+1)-(int)_backCenter.y);

  // Toute la place est disponible
  sf::Vector2u idealSize(0,0);
  for (unsigned int k=0;k<_interfaces.size();k++)
    idealSize+=_interfaces[k]->getIdealSize();

  /*if (0) { //Cas zoom automatique
    if (_clientSize.y >= idealSize.y) //Plus de place que necessaire
    {
      float y1=0;
      for (unsigned int i=0;i<_interfaces.size();i++)
      {
        float sfac=_clientSize.y/(float)idealSize.y;
        _interfaces[i]->setViewSize(_clientSize.x/sfac,_interfaces[i]->getIdealSize().y);
        float y2 = sfac*_interfaces[i]->getIdealSize().y/(float)_clientSize.y;
        _interfaces[i]->setViewport(sf::FloatRect(_viewportMin.x,
                                        _viewportMin.y+y1*_viewportMax.y,
                                        _viewportMax.x,
                                        y2*_viewportMax.y));
        y1+=y2;
      }
    }
    else //on alloue proportionellement à la place dispo
    {
      float y1=0;
      for (unsigned int i=0;i<_interfaces.size();i++)
      {
        _interfaces[i]->setViewSize(_clientSize.x,
                                    _interfaces[i]->getIdealSize().y*_clientSize.y/(float)idealSize.y);
        float y2 = _interfaces[i]->getIdealSize().y/(float)idealSize.y;
        _interfaces[i]->setViewport(sf::FloatRect(_viewportMin.x,
                                                  _viewportMin.y+y1*_viewportMax.y,
                                                  _viewportMax.x,
                                                  y2*_viewportMax.y));
        y1+=y2;
      }
    }
  } else */{ //Cas pas de zoom et on commence en bas
    if (_clientSize.y >= idealSize.y) //Plus de place que necessaire
    {
      float y1=1.f-(float)idealSize.y/(float)_clientSize.y;
      for (unsigned int i=0;i<_interfaces.size();i++)
      {
          float sfac=1.f;//_clientSize.y/(float)idealSize.y;
          _interfaces[i]->setViewSize(_clientSize.x/sfac,_interfaces[i]->getIdealSize().y);
          float y2 = sfac*_interfaces[i]->getIdealSize().y/(float)_clientSize.y;
          _interfaces[i]->setViewport(sf::FloatRect(_viewportMin.x,
                                     _viewportMin.y+y1*_viewportMax.y,
                                     _viewportMax.x,
                                     y2*_viewportMax.y));
          y1+=y2;
      }
    }
    else //on alloue proportionellement à la place dispo
    {
      float y1=0;
      for (unsigned int i=0;i<_interfaces.size();i++)
      {
        _interfaces[i]->setViewSize(_clientSize.x,
                                    _interfaces[i]->getIdealSize().y*_clientSize.y/(float)idealSize.y);
        float y2 = _interfaces[i]->getIdealSize().y/(float)idealSize.y;
        _interfaces[i]->setViewport(sf::FloatRect(_viewportMin.x,
                                                  _viewportMin.y+y1*_viewportMax.y,
                                                  _viewportMax.x,
                                                  y2*_viewportMax.y));
        y1+=y2;
      }
    }
  }
}

void NEWindow::drawContent()
{
  setView(_fullView);
  draw(_backSprite);
#ifdef LIBTOOLS_WINDOWS
  draw(_title);
  draw(_resizeTriangle);
#endif
  for (unsigned int i =0; i < _mouseCatchers.size(); i++)
  {
    draw(*(_mouseCatchers[i]));
  }

  //On dessine Les interfaces
  for (unsigned int i =0; i < _interfaces.size(); i++)
  {
    setView(_interfaces[i]->getView());
    draw(*(_interfaces[i]));
  }
}

bool NEWindow::checkInterrupt()
{
  if (_onMinimize)
  {
    minimize();
    _onMinimize=false;
    return true;
  }
  if (_onClose) {
    close();
    _onClose=false;
    return true;
  }
  return false;
}

void NEWindow::minimize()
{
#ifdef LIBTOOLS_WINDOWS
  ShowWindow(getSystemHandle(),SW_MINIMIZE);
#endif
}


void NEWindow::setBackgroundTexture(const std::string& name,
                                    const sf::Vector2i center)
{
   _backCenter=center;
   _backTexture.loadFromFile(name);
   _backSprite.setTexture(_backTexture,true);
   _backSprite.setPosition(-(int)_backCenter.x+(int)_backCenter.x*(int)getSize().x/((int)_backTexture.getSize().x+1),
                           (int)_backCenter.y*(int)getSize().y/(float)(_backTexture.getSize().y+1)-(int)_backCenter.y);
}



#ifdef LIBTOOLS_WINDOWS

void NEWindow::setZPosition(HWND insert_after)
{
  SetWindowPos(this->getSystemHandle(), insert_after, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
}

void NEWindow::setTransparency(uint8_t alpha)
{
  COLORREF previous_colorref=0;
  DWORD previous_styleflag;
  DWORD previous_layerflag=0;
  HWND hWnd=this->getSystemHandle();

  GetLayeredWindowAttributes(hWnd,&previous_colorref,NULL,&previous_layerflag);
  bool unicode=hasUnicodeSupport();
  if (unicode)
  {
    previous_styleflag=GetWindowLongW(hWnd,GWL_EXSTYLE);
  }
  else
  {
    previous_styleflag=GetWindowLongA(hWnd, GWL_EXSTYLE);
  }

  if (alpha==255 && !previous_layerflag)
  {
    if (unicode)
    {
      SetWindowLongW(hWnd, GWL_EXSTYLE, previous_styleflag & ~WS_EX_LAYERED);
    }
    else
    {
      SetWindowLongA(hWnd, GWL_EXSTYLE, previous_styleflag & ~WS_EX_LAYERED);
    }
  }
  else
  {
    if ((previous_styleflag & WS_EX_LAYERED) == 0)
    {
      if (unicode)
      {
        SetWindowLongW(hWnd, GWL_EXSTYLE, previous_styleflag | WS_EX_LAYERED);
      }
      else
      {
        SetWindowLongA(hWnd, GWL_EXSTYLE, previous_styleflag | WS_EX_LAYERED);
      }
    }
    SetLayeredWindowAttributes(hWnd,
                               previous_colorref,
                               alpha,
                               previous_layerflag | LWA_ALPHA);
  }
}


void NEWindow::setMaskColor(const sf::Color& ref)
{
  bool desactivate=ref.a!=255;
  COLORREF colorref=RGB(ref.r,ref.g,ref.b);
  DWORD previous_styleflag;
  DWORD previous_layerflag=0;
  BYTE previous_alpha=0;
  HWND hWnd=this->getSystemHandle();

  GetLayeredWindowAttributes(hWnd,NULL,&previous_alpha,&previous_layerflag);
  bool unicode=hasUnicodeSupport();
  if (unicode)
  {
    previous_styleflag=GetWindowLongW(hWnd,GWL_EXSTYLE);
  }
  else
  {
    previous_styleflag=GetWindowLongA(hWnd, GWL_EXSTYLE);
  }

  if (desactivate && !previous_layerflag)
  {
    if (unicode)
    {
      SetWindowLongW(hWnd, GWL_EXSTYLE, previous_styleflag & ~WS_EX_LAYERED);
    }
    else
    {
      SetWindowLongA(hWnd, GWL_EXSTYLE, previous_styleflag & ~WS_EX_LAYERED);
    }
  }
  else
  {
    if (previous_styleflag & WS_EX_LAYERED == 0)
    {
      if (unicode)
      {
        SetWindowLongW(hWnd, GWL_EXSTYLE, previous_styleflag | WS_EX_LAYERED);
      }
      else
      {
        SetWindowLongA(hWnd, GWL_EXSTYLE, previous_styleflag | WS_EX_LAYERED);
      }
    }
    if (desactivate)
    {
      SetLayeredWindowAttributes(hWnd,
                                 0,
                                 previous_alpha,
                                 previous_layerflag & ~LWA_COLORKEY);
    }
    else
    {
      SetLayeredWindowAttributes(hWnd,
                                 colorref,
                                 previous_alpha,
                                 previous_layerflag | LWA_COLORKEY);
      std::cout<<"setLayerATtribut" << std::endl;
    }
  }
}

bool NEWindow::hasUnicodeSupport()
{
    OSVERSIONINFO version;
    ZeroMemory(&version, sizeof(version));
    version.dwOSVersionInfoSize = sizeof(version);

    if (GetVersionEx(&version))
    {
        return version.dwPlatformId == VER_PLATFORM_WIN32_NT;
    }
    else
    {
        return false;
    }
}


LRESULT CALLBACK NEWindow::neWinProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
  std::map<HWND,NEWindow*>::iterator it=NEWindow::_wins.find(hwnd);
  if (it != NEWindow::_wins.end())
  {
    NEWindow* win=it->second;
    if (uMsg==WM_DROPFILES)
    {
      //MessageBoxA(0,"Drop receive","debug",MB_OK);
      HDROP hDropInfo = (HDROP) wParam;
      const unsigned int files = DragQueryFileW(hDropInfo,0xFFFFFFFF,0,0);
      if (files)
      {
        for (unsigned int i=0; i < files; i++)
        {
           WCHAR filename[MAX_PATH];
           unsigned int len=DragQueryFileW(hDropInfo,i,filename,MAX_PATH);
           string_t s = filename;
           win->filesDropped.push(s);
        }
        //free((void*)filename);
        return TRUE;
      }
      return FALSE;
     }
     else if (uMsg==WM_COPYDATA)
     {
       COPYDATASTRUCT* cds = (COPYDATASTRUCT*) lParam;
       string_t current = (wchar_t*)cds->lpData;
       win->filesDropped.push(current);
       return TRUE;
     }
   }
   return DefWindowProcW(hwnd, uMsg, wParam, lParam);

   /*static const bool hasUnicode = hasUnicodeSupport();
   return hasUnicode ? DefWindowProcW(hwnd, uMsg, wParam, lParam) :
                       DefWindowProcA(hwnd, uMsg, wParam, lParam);*/
}

void NEWindow::registerClass()
{
  //if (hasUnicodeSupport())
  {
        WNDCLASSW windowClass;
        windowClass.style         = 0;
        windowClass.lpfnWndProc   = &NEWindow::neWinProc;
        windowClass.cbClsExtra    = 0;
        windowClass.cbWndExtra    = 0;
        windowClass.hInstance     = GetModuleHandle(NULL);
        windowClass.hIcon         = NULL;
        windowClass.hCursor       = 0;
        windowClass.hbrBackground = 0;
        windowClass.lpszMenuName  = NULL;
        windowClass.lpszClassName = L"NEWin98";
        RegisterClassW(&windowClass);
   }
  /*else
  {
      WNDCLASSA windowClass;
      windowClass.style         = 0;
      windowClass.lpfnWndProc   = &NEWindow::lamoguiWinProc;
      windowClass.cbClsExtra    = 0;
      windowClass.cbWndExtra    = 0;
      windowClass.hInstance     = GetModuleHandle(NULL);
      windowClass.hIcon         = NULL;
      windowClass.hCursor       = 0;
      windowClass.hbrBackground = 0;
      windowClass.lpszMenuName  = NULL;
      windowClass.lpszClassName ="NEWin98";
      RegisterClassA(&windowClass);
  }*/

}

void NEWindow::forwardMessages()
{
  MSG message;
  while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
  {
    TranslateMessage(&message);
    DispatchMessage(&message);
  }
}


bool NEWindow::sendFilepathsW(const wchar_t* win_name , int argc, wchar_t **argv)
{
  HWND win = FindWindowW(L"NEWin98", win_name);
  if (win) {
    SetForegroundWindow(win);

    // Command line is not empty. Send the
    // command line in a WM_COPYDATA message.
    for (unsigned int i=0; i < argc;i++){
      COPYDATASTRUCT cds;
      size_t len=wcslen(argv[i]);
      cds.cbData = (len+1)*sizeof(wchar_t);
      wchar_t* data=(wchar_t*) malloc(cds.cbData);
      memcpy((void*) data, (void*) argv[i], cds.cbData);
      cds.lpData = (void*) data;
      SendMessageW(win, WM_COPYDATA, 0, (LPARAM)&cds);
    }
    return true;
  }
  return false;
}


void NEWindow::setCurrentDirToExecutablePath()
{
  HMODULE hModule = GetModuleHandleW(NULL);
  WCHAR path[MAX_PATH];
  GetModuleFileNameW(hModule, path, MAX_PATH);
  unsigned int k=0;
  while (path[k++]);
  unsigned int i=k-1;
  while (i && path[i] != L'/' && path[i] != L'\\')
     path[i--]=0;
  SetCurrentDirectoryW(path);
}

#endif //LIBTOOLS_WINDOW

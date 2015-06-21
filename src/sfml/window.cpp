

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
#ifdef LIBTOOLS_WINDOWS
_closeButton(sf::Vector2f(_borderSizeRight+_borderSizeLeft,
                          _borderSizeUp*0.5f),"X",font),
#endif
_title(title,font,11),
_currentInterfaceCatcher(NULL),
_currentMouseCatcher(NULL),
_fullView(sf::FloatRect(0,0,mode.width,mode.height)),
_backSprite(),
_backTexture(),
_backCenter(0,0)         
{
#ifdef LIBTOOLS_WINDOWS
  

  //Close Button params
  _closeButton.linkTo(&_onClose,ButtonMode::on);
  _closeButton.setOutlineThickness(0);
  _closeButton.setClickedColor(sf::Color(142,42,42,255));
  _closeButton.setIdleColor(sf::Color(100,42,42,255));

  
  //ResizeTriangle
  _resizeTriangle.setPointCount(3);
  _resizeTriangle.setPoint(0, sf::Vector2f(15, 0));
  _resizeTriangle.setPoint(1, sf::Vector2f(15, 15));
  _resizeTriangle.setPoint(2, sf::Vector2f(0, 15));
  _resizeTriangle.setOrigin(0,0);
  _resizeTriangle.setFillColor(sf::Color(75,75,75,255));

  registerMouseCatcher(_closeButton);

  
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
    if (hasUnicodeSupport())
    {
        hWnd = CreateWindowExW(WS_EX_ACCEPTFILES,L"NEWin98", title.toWideString().c_str(), win32Style, left, top, width, height, NULL, NULL, GetModuleHandle(NULL), 0);
    }
    else
    {
        hWnd = CreateWindowExA(WS_EX_ACCEPTFILES,"NEWin98", title.toAnsiString().c_str(), win32Style, left, top, width, height, NULL, NULL, GetModuleHandle(NULL), 0);
    }

    create(hWnd,settings);

    // By default, the OS limits the size of the window the the desktop size,
    // we have to resize it after creation to apply the real size
    setSize(sf::Vector2u(mode.width, mode.height));

    _wins[getSystemHandle()]=this;

#endif
 
  arrange();
  
  
}

NEWindow::~NEWindow(){
 
  /*#ifdef LIBTOOLS_WINDOWS
  sf::WindowHandle save = getSystemHandle();
  close();
  _wins.erase(save);
  DestroyWindow(save);
  #endif*/
}

bool NEWindow::useEvent(const sf::Event& event)
{
  switch (event.type)
  {
    case sf::Event::Closed: {
      sf::WindowHandle save = getSystemHandle();
      close();
#ifdef LIBTOOLS_WINDOWS
      _wins.erase(save);
      DestroyWindow(save);
#endif
      return true;
                            }
    case sf::Event::Resized:
      arrange();
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
            if (newSize.x < 400) newSize.x = 400;
            if (newSize.y < 200) newSize.y = 200;
            setSize(newSize);
            arrange();
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
          sf::WindowHandle save = getSystemHandle();
          close();
#ifdef LIBTOOLS_WINDOWS
          _wins.erase(save);
          DestroyWindow(save);
#endif
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
#endif

   _backSprite.setPosition(-(int)_backCenter.x+(int)_backCenter.x*(int)getSize().x/((int)_backTexture.getSize().x+1),
                           (int)_backCenter.y*(int)getSize().y/(float)(_backTexture.getSize().y+1)-(int)_backCenter.y);

  // Toute la place est disponible 
  sf::Vector2u idealSize(0,0);
  for (unsigned int k=0;k<_interfaces.size();k++)
    idealSize+=_interfaces[k]->getIdealSize();
  
  if (0) { //Cas zoom automatique
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
  } else { //Cas pas de zoom et on commence en bas
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
  if (_onClose) {
    close();
    _onClose=false;
    return true;
  }
  return false;
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


LRESULT CALLBACK NEWindow::lamoguiWinProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
  NEWindow* win=NEWindow::_wins[hwnd];
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
			  DragQueryFileW(hDropInfo,i,filename,MAX_PATH);
        string_t s = filename;
			  win->filesDropped.push(s);
		  }
      return TRUE;
	  }
    return FALSE;
   }
   else if (uMsg==WM_COPYDATA)
   {
     
      COPYDATASTRUCT* cds = (COPYDATASTRUCT*) lParam;
      char* c= (char*)cds->lpData;
      string_t current;
      unsigned int i=0;
      while (*c && i < cds->cbData) {
        if (*c == ';')
        {
          win->filesDropped.push(current);
          current="";
        }
        else
          current+=*c;
        c++;
        i++;
      }
      /*debug << "Received " << cds->cbData  << " bytes" <<  std::endl;
      debug << "Received " << (char*)cds->lpData <<  std::endl;*/
      return TRUE;
   }
   static const bool hasUnicode = hasUnicodeSupport();
   return hasUnicode ? DefWindowProcW(hwnd, uMsg, wParam, lParam) :
                       DefWindowProcA(hwnd, uMsg, wParam, lParam);
}

void NEWindow::registerClass()
{
  if (hasUnicodeSupport())
  {
        WNDCLASSW windowClass;
        windowClass.style         = 0;
        windowClass.lpfnWndProc   = &NEWindow::lamoguiWinProc;
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
  else
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
  }

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

#endif //LIBTOOLS_WINDOW

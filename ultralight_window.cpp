#include "ultralight_hb.h"
#include <AppCore/Window.h>
#include "ultralight_window_listener.cpp"

using namespace ultralight;
DEFINE_GETCLASSID(WINDOW)

/*
    DATA bOnClose
    DATA bOnResize
    CONSTRUCTOR Create(monitor, width, height, fullscreen, window_flags)
    ACCESS width()
    ACCESS height()
    ACCESS is_fullscreen()
    ACCESS scale()
    ASSIGN title()
    ASSIGN cursor()
    METHOD Close()
    METHOD DeviceToPixels(val)
    METHOD PixelsToDevice(val)
    //METHOD DrawSurface(x, y, surface)
*/
class HBWindowListener;

void SetupWindow(PHB_ITEM pItem,Window* window) {
    HBWindowListener* listener = new HBWindowListener(pItem);
    window->set_listener(listener);
}

Monitor* hb_parMonitor(int);
HB_FUNC( ULTRALIGHT_WINDOW_CREATE ) {
	Monitor *mon = hb_parMonitor(1);
    RefPtr<Window> window = Window::Create(mon,hb_parni(2), hb_parni(3),hb_parldef(4,0)!=0,hb_parnidef(5,0));
    initUltralightObj(window.get(), getWINDOWClassId());
   	PHB_ITEM pSelf = hb_stackSelfItem();
    SetupWindow(pSelf, window.get());
}

HB_FUNC( ULTRALIGHT_WINDOW_WIDTH ) {
    Window* window = (Window*)hb_selfUltralight();
    hb_retni(window->width());
}

HB_FUNC( ULTRALIGHT_WINDOW_HEIGHT ) {
    Window* window = (Window*)hb_selfUltralight();
    hb_retni(window->height());
}

HB_FUNC( ULTRALIGHT_WINDOW_IS_FULLSCREEN ) {
    Window* window = (Window*)hb_selfUltralight();
    hb_retl(window->is_fullscreen()? HB_TRUE : HB_FALSE);
}

HB_FUNC( ULTRALIGHT_WINDOW_SCALE ) {
    Window* window = (Window*)hb_selfUltralight();
    hb_retnd(window->scale());
}

HB_FUNC( ULTRALIGHT_WINDOW__TITLE ) {
    Window* window = (Window*)hb_selfUltralight();
    window->SetTitle(hb_parc(1));
    hb_ret();
}

HB_FUNC( ULTRALIGHT_WINDOW__CURSOR ) {
    Window* window = (Window*)hb_selfUltralight();
//#ifdef _WIN32
//    window->SetCursor((Cursor)0);
//#endif
    window->SetCursor((Cursor)hb_parni(1));
    hb_ret();
}

HB_FUNC( ULTRALIGHT_WINDOW_CLOSE ) {
    Window* window = (Window*)hb_selfUltralight();
    window->Close();
    hb_ret();
}

HB_FUNC( ULTRALIGHT_WINDOW_DEVICETOPIXELS ) {
    Window* window = (Window*)hb_selfUltralight();
    hb_retni(window->DeviceToPixels(hb_parni(1)));
}

HB_FUNC( ULTRALIGHT_WINDOW_PIXELSTODEVICE ) {
    Window* window = (Window*)hb_selfUltralight();
    hb_retni(window->PixelsToDevice(hb_parni(1)));
}

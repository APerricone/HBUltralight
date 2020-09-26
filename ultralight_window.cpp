#include "ultralight_hb.h"
#include "hb_smartApi.h"
#include <AppCore/Window.h>

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

Monitor* hb_parMonitor(int);
HB_FUNC( ULTRALIGHT_WINDOW_CREATE ) {
	Monitor *mon = hb_parMonitor(1);
    RefPtr<Window> window = Window::Create(mon,hb_parni(2), hb_parni(3),hb_parldef(4,0)!=0,hb_parnidef(5,0));
    initUltralightObj(window.get(), getWINDOWClassId());
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

class HBWindowListener : public WindowListener {
public:
    PHB_ITEM pOnResize;
    PHB_ITEM pOnClose;

    HBWindowListener() : pOnClose(0), pOnResize(0) { }

    void OnClose() {
        if(!pOnClose) return;
        hb_evalBlock(pOnClose, NULL );
    }

    void OnResize(uint32_t width, uint32_t height) {
        if(!pOnResize) return;
        hb_evalBlock(pOnResize, SmartItem((int)width), SmartItem((int)height), NULL );
    }

};

void ManageWindowCodeBlockGetSet(PHB_ITEM HBWindowListener::*pOnMember) {
    Window* window = (Window*)hb_selfUltralight();
    HBWindowListener* listener = (HBWindowListener*)window->listener();
    if(hb_pcount()==0) {
        if(listener) {
            hb_itemCopy(hb_stackReturnItem(), listener->*pOnMember);
            return;
        }
    } else {
        if(HB_ISEVALITEM(1)) {
            if(!listener) {
                listener = new HBWindowListener();
                window->set_listener(listener);
            }
            listener->*pOnMember = hb_itemNew(hb_param(1, HB_IT_EVALITEM));
        }
    }
    hb_ret();
}

HB_FUNC( ULTRALIGHT_WINDOW_BONCLOSE ) { ManageWindowCodeBlockGetSet(&HBWindowListener::pOnClose); }
HB_FUNC( ULTRALIGHT_WINDOW_BONRESIZE ) { ManageWindowCodeBlockGetSet(&HBWindowListener::pOnResize); }

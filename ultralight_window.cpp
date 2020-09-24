#include "ultralight_hb.h"
#include <AppCore/Window.h>

using namespace ultralight;

HB_SIZE ibOnCloseIdx = 0;
HB_SIZE ibOnResizeIdx = 0;

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

class HBWindowListener : public WindowListener {
private:
    PHB_ITEM pItem;
public:
    HBWindowListener(PHB_ITEM pItem) : pItem(pItem) {
    }

    PHB_ITEM getItem() const { return pItem; }

    void OnClose() {
        if(ibOnCloseIdx==0) {
            ibOnCloseIdx = hb_clsGetVarIndex(getWINDOWClassId(),hb_dynsymGet("bOnClose"));
        }
        PHB_ITEM pCallback = hb_itemArrayGet(pItem, ibOnCloseIdx);
        if(!HB_IS_EVALITEM( pCallback )) return;
        hb_evalBlock(pCallback, pItem, NULL );
    }

    void OnResize(uint32_t width, uint32_t height) {
        if(ibOnResizeIdx==0) {
            ibOnResizeIdx = hb_clsGetVarIndex(getWINDOWClassId(),hb_dynsymGet("bOnResize"));
        }
        PHB_ITEM pCallback = hb_itemArrayGet(pItem, ibOnResizeIdx);
        if(!HB_IS_EVALITEM( pCallback )) return;
        PHB_ITEM pWidth = hb_itemPutNI(0,width);
        PHB_ITEM pHeight = hb_itemPutNI(0,height);
        hb_evalBlock(pCallback, pItem, pWidth, pHeight, NULL );
        hb_itemRelease(pWidth);
        hb_itemRelease(pHeight);
    }

};

void SetupWindow(PHB_ITEM pItem,const  RefPtr<Window>& window) {
    HBWindowListener* listener = new HBWindowListener(pItem);
    window->set_listener(listener);
}

void hb_retWindow(ultralight::Window* pObj) {
    HBWindowListener* listener = (HBWindowListener*)pObj->listener();
    if(listener) {
        hb_itemCopy(hb_stackReturnItem(), listener->getItem());
    }
    initUltralightObj(pObj, getWINDOWClassId());
    SetupWindow(hb_stackReturnItem(), pObj);
}

HB_FUNC( ULTRALIGHT_WINDOW_CREATE ) {
	Monitor *mon = (Monitor*)hb_parUltralight(1);
    RefPtr<Window> window = Window::Create(mon,hb_parni(2), hb_parni(3),hb_parldef(4,0)!=0,hb_parnidef(5,0));
    initUltralightObj(window.get(), getWINDOWClassId());
   	PHB_ITEM pSelf = hb_stackSelfItem();
    SetupWindow(pSelf, window);
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

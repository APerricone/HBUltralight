#include "ultralight_hb.h"
OBJDATA ultralight_window;

/*
    //DATA bOnClose
    // DATA OnResize

    CONSTRUCTOR Create(monitor, width, height, fullscreen, window_flags)
    METHOD width()
    METHOD height()
    //ACCESS is_fullscreen()
    //ACCESS scale()
    METHOD SetTile(cTile)
    //METHOD SetCursor(oCursor)
    //METHOD Close()

    //METHOD DeviceToPixels(nVal)
    //METHOD PixelsToDevice(nVal)
*/

HB_FUNC( ULTRALIGHT_WINDOW_CREATE ) {
	PHB_ITEM pSelf;
	ULMonitor mon = PARAM_MONITOR(1);
	ULWindow win = ulCreateWindow(
        mon, hb_parni(2), hb_parni(3), 
        hb_parldef(4,0)!=0,hb_parnidef(5,0));
	setupOBJDATA("ULTRALIGHT_WINDOW",&ultralight_window);
    hb_clsAssociate(  ultralight_window.classId );
    pSelf = hb_stackReturnItem();
    hb_itemArrayPut(pSelf, ultralight_window.ptrObj, hb_itemPutPtr(0, win)); 
}

HB_FUNC( ULTRALIGHT_WINDOW_WIDTH ) {
    hb_retni(ulWindowGetWidth(SELF_WINDOW()));
}

HB_FUNC( ULTRALIGHT_WINDOW_HEIGHT ) {
    hb_retni(ulWindowGetHeight(SELF_WINDOW()));
}

HB_FUNC( ULTRALIGHT_WINDOW_SETTILE ) {
    ulWindowSetTitle(SELF_WINDOW(),hb_parc(1));
    hb_ret();
}


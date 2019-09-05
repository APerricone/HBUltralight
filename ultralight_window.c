#include "ultralight_hb.h"
OBJDATA ultralight_window;

/*
    //DATA bOnClose
    // DATA OnResize

    CONSTRUCTOR Create(monitor, width, height, fullscreen, window_flags)
    METHOD width()
    METHOD ù()
    //ACCESS is_fullscreen()
    //ACCESS scale()
    METHOD SetTile(cTile)
    METHOD SetCursor(nCursor)
    //METHOD Close()

    //METHOD DeviceToPixels(nVal)
    //METHOD PixelsToDevice(nVal)
*/
HB_SIZE ptr_bOnClose = 0;
HB_SIZE ptr_bOnResize = 0;

void hbOnCloseCallback(void* user_data);
void hbOnResizeCallback(void* user_data, int width, int height);

HB_FUNC( ULTRALIGHT_WINDOW_CREATE ) {
	PHB_ITEM pSelf, pTemp;
	ULMonitor mon = PARAM_MONITOR(1);
	ULWindow win = ulCreateWindow(
        mon, hb_parni(2), hb_parni(3), 
        hb_parldef(4,0)!=0,hb_parnidef(5,0));
	setupOBJDATA("ULTRALIGHT_WINDOW",&ultralight_window);
    hb_clsAssociate(  ultralight_window.classId );
    pSelf = hb_stackReturnItem();
    hb_itemArrayPut(pSelf, ultralight_window.ptrObj, hb_itemPutPtr(0, win)); 
    if(ptr_bOnClose==0) {
        ptr_bOnClose      = hb_clsGetVarIndex(ultralight_window.classId,hb_dynsymGet("bOnClose"));
        ptr_bOnResize     = hb_clsGetVarIndex(ultralight_window.classId,hb_dynsymGet("bOnResize"));
    }
    pTemp = hb_itemNew(pSelf);
    ulWindowSetCloseCallback(win,hbOnCloseCallback,pTemp);
    ulWindowSetResizeCallback(win,hbOnResizeCallback,pTemp);
}

HB_FUNC( ULTRALIGHT_WINDOW_WIDTH ) {
    hb_retni(ulWindowGetWidth(SELF_WINDOW()));
}

HB_FUNC( ULTRALIGHT_WINDOW_HEIGHT ) {
    hb_retni(ulWindowGetHeight(SELF_WINDOW()));
}

HB_FUNC( ULTRALIGHT_WINDOW_SCALE ) {
    hb_retnd(ulWindowGetScale(SELF_WINDOW()));
}

HB_FUNC( ULTRALIGHT_WINDOW_SETTILE ) {
    ulWindowSetTitle(SELF_WINDOW(),hb_parc(1));
    hb_ret();
}

HB_FUNC( ULTRALIGHT_WINDOW_SETCURSOR ) {
#ifdef _WIN32
    ulWindowSetCursor(SELF_WINDOW(),(ULCursor)0);
#endif
    ulWindowSetCursor(SELF_WINDOW(),(ULCursor)hb_parni(1));
    hb_ret();
}




void hbOnCloseCallback(void* user_data) {
    PHB_ITEM pCallback = hb_itemArrayGet((PHB_ITEM)user_data, ptr_bOnClose);
    if(!HB_IS_EVALITEM( pCallback )) return;
    hb_evalBlock(pCallback, (PHB_ITEM)user_data, NULL );
}

void hbOnResizeCallback(void* user_data, int width, int height) {
    PHB_ITEM pCallback = hb_itemArrayGet((PHB_ITEM)user_data, ptr_bOnResize);
    if(!HB_IS_EVALITEM( pCallback )) return;
    hb_evalBlock(pCallback, (PHB_ITEM)user_data, hb_itemPutNI(0,width),hb_itemPutNI(0,height), NULL );
}

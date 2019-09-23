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
    METHOD SetTitle(cTile)
    METHOD SetCursor(nCursor)
    //METHOD Close()

    //METHOD DeviceToPixels(nVal)
    //METHOD PixelsToDevice(nVal)
*/
HB_SIZE ptr_bOnClose = 0;
HB_SIZE ptr_bOnResize = 0;

void hbOnCloseCallback(void* user_data);
void hbOnResizeCallback(void* user_data, int width, int height);

HB_FUNC_EXTERN( ULTRALIGHT_WINDOW );
static void SetupWindow(ULWindow window) {
	if(ultralight_window.classId == 0) {
		ultralight_window.classId = hb_clsFindClass("ULTRALIGHT_WINDOW", NULL);
	    if(ultralight_window.classId == 0) {
		    HB_FUNC_EXEC(ULTRALIGHT_WINDOW);
		    ultralight_window.classId = hb_clsFindClass("ULTRALIGHT_WINDOW", NULL);
	    }
		ultralight_window.ptrObj = hb_clsGetVarIndex(ultralight_window.classId,hb_dynsymGet("pObj"));

        ptr_bOnClose      = hb_clsGetVarIndex(ultralight_window.classId,hb_dynsymGet("bOnClose"));
        ptr_bOnResize     = hb_clsGetVarIndex(ultralight_window.classId,hb_dynsymGet("bOnResize"));
    }
    hb_clsAssociate( ultralight_window.classId );
    hb_itemArrayPut(hb_stackReturnItem(), ultralight_window.ptrObj, hb_itemPutPtr(0, window));
    ulWindowSetCloseCallback(window,hbOnCloseCallback,window);
    ulWindowSetResizeCallback(window,hbOnResizeCallback,window);
}

PHB_ITEM itemFromWindow(ULWindow window) {
    hb_vmPushSymbol( hb_dynsymFindSymbol("ULTRALIGHT_GETHB") );
    hb_vmPushNil();
    hb_vmPushPointer(window);
    hb_vmDo( 1 );
    //HB_FUNC_EXEC(ULTRALIGHT_GETHB);
    return hb_stackReturnItem();
}

void hb_retWindow(ULWindow window) {
    hb_vmPushSymbol( hb_dynsymFindSymbol("ULTRALIGHT_GETHB") );
    hb_vmPushNil();
    hb_vmPushPointer(window);
    hb_vmDo( 1 );
    //HB_FUNC_EXEC(ULTRALIGHT_GETHB);
    if( hb_itemType(hb_stackReturnItem())!=HB_IT_NIL ) {
        return;
    }
    SetupWindow(window);
    hb_vmPushSymbol( hb_dynsymFindSymbol("ULTRALIGHT_SETHB") );
    hb_vmPushNil();
    hb_vmPushPointer(window);
    hb_vmPush(hb_stackReturnItem());
    ///hb_vmPushPointer(window);
    hb_vmDo( 2 );

    //HB_FUNC_EXEC(ULTRALIGHT_SETHB);
}

HB_FUNC( ULTRALIGHT_WINDOW_CREATE ) {
	ULMonitor mon = PARAM_MONITOR(1);
	ULWindow win = ulCreateWindow(
        mon, hb_parni(2), hb_parni(3), 
        hb_parldef(4,0)!=0,hb_parnidef(5,0));
    hb_retWindow(win);
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

HB_FUNC( ULTRALIGHT_WINDOW_SETTITLE ) {
    ulWindowSetTitle(SELF_WINDOW(),hb_parc(1));
    hb_ret();
}

HB_FUNC( ULTRALIGHT_WINDOW_SETCURSOR ) {
    ULCursor newCursor = (ULCursor)hb_parni(1);
#ifdef _WIN32
    ulWindowSetCursor(SELF_WINDOW(),(ULCursor)0);
#endif
    ulWindowSetCursor(SELF_WINDOW(),newCursor);
    hb_ret();
}




void hbOnCloseCallback(void* user_data) {
    PHB_ITEM pCaller = itemFromWindow((ULWindow)user_data);
    PHB_ITEM pCallback = hb_itemArrayGet(pCaller, ptr_bOnClose);
    if(!HB_IS_EVALITEM( pCallback )) return;
    hb_evalBlock(pCallback, pCaller, NULL );
}

void hbOnResizeCallback(void* user_data, int width, int height) {
    PHB_ITEM pCaller = itemFromWindow((ULWindow)user_data);
    PHB_ITEM pCallback = hb_itemArrayGet(pCaller, ptr_bOnResize);
    if(!HB_IS_EVALITEM( pCallback )) return;
    hb_evalBlock(pCallback, pCaller, hb_itemPutNI(0,width),hb_itemPutNI(0,height), NULL );
}

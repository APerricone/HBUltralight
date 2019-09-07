#include "ultralight_hb.h"
OBJDATA ultralight_overlay;

/*
    CONSTRUCTOR Create(window, width, height, x,y)
    METHOD view()
    //METHOD width() 
    //METHOD height()
    //METHOD x()
    //METHOD y()
    //METHOD hidden() SETGET
    //METHOD focus() SETGET
    //METHOD moveTo(x,y)
    METHOD Resize(width,height)  
*/

HB_FUNC( ULTRALIGHT_OVERLAY_CREATE ) {
    PHB_ITEM pSelf;
	ULOverlay overlay = ulCreateOverlay(
        PARAM_WINDOW(1), hb_parni(2), hb_parni(3), 
        hb_parnidef(4,0),hb_parnidef(5,0));
	setupOBJDATA("ULTRALIGHT_OVERLAY", &ultralight_overlay);
    hb_clsAssociate(  ultralight_overlay.classId );
    pSelf = hb_stackReturnItem();
    hb_itemArrayPut(pSelf, ultralight_overlay.ptrObj, hb_itemPutPtr(0, overlay)); 
}

HB_FUNC_EXTERN( ULTRALIGHT_VIEW );
HB_FUNC( ULTRALIGHT_OVERLAY_VIEW ) {
    PHB_ITEM pRet;
    ULView view = ulOverlayGetView(SELF_OVERLAY());
    HB_FUNC_EXEC(ULTRALIGHT_VIEW);
    setupOBJDATA("ULTRALIGHT_VIEW", &ultralight_view);
    hb_clsAssociate(  ultralight_view.classId );
    pRet = hb_stackReturnItem();
    SetupView(pRet,view);
}

HB_FUNC( ULTRALIGHT_OVERLAY_WIDTH ) {
    hb_retni(ulOverlayGetWidth(SELF_OVERLAY()));
}
HB_FUNC( ULTRALIGHT_OVERLAY_HEIGHT ) {
    hb_retni(ulOverlayGetHeight(SELF_OVERLAY()));

}
HB_FUNC( ULTRALIGHT_OVERLAY_X ) {
    hb_retni(ulOverlayGetX(SELF_OVERLAY()));
}
HB_FUNC( ULTRALIGHT_OVERLAY_Y ) {
    hb_retni(ulOverlayGetY(SELF_OVERLAY()));
}

HB_FUNC( ULTRALIGHT_OVERLAY_HIDDEN ) {
    if(hb_pcount()>0) {
        if(hb_parl(1)) 
            ulOverlayHide(SELF_OVERLAY());
        else
            ulOverlayShow(SELF_OVERLAY());
        hb_retl(hb_parl(1));
    } else {
        hb_retl(ulOverlayIsHidden(SELF_OVERLAY()));
    }
}
HB_FUNC( ULTRALIGHT_OVERLAY_FOCUS ) {
    if(hb_pcount()>0) {
        if(hb_parl(1)) 
            ulOverlayFocus(SELF_OVERLAY());
        else
            ulOverlayUnfocus(SELF_OVERLAY());
        hb_retl(hb_parl(1));
    } else {
        hb_retl(ulOverlayHasFocus(SELF_OVERLAY()));
    }
}

HB_FUNC( ULTRALIGHT_OVERLAY_MOVETO ) {
    ulOverlayMoveTo(SELF_OVERLAY(),hb_parni(1),hb_parni(2));
    hb_ret();
}

HB_FUNC( ULTRALIGHT_OVERLAY_RESIZE ) {
    ulOverlayResize(SELF_OVERLAY(),hb_parni(1),hb_parni(2));
    hb_ret();
}


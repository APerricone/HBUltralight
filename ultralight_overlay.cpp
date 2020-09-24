#include "ultralight_hb.h"
#include <AppCore/Overlay.h>
using namespace ultralight;

DEFINE_GETCLASSID(OVERLAY)

/*
    CONSTRUCTOR Create(window, width, height, x,y)
    ACCESS view()
    ACCESS width()
    ACCESS height()
    ACCESS x()
    ACCESS y()
    METHOD hidden() SETGET
    METHOD focus() SETGET
    METHOD moveTo(x,y)
    METHOD Resize(width,height)
    ACCESS needsRepaint()
*/

HB_FUNC( ULTRALIGHT_OVERLAY_CREATE ) {
 	Window *win = (Window*)hb_parUltralight(1);
    View *view = (View*)hb_parUltralight(2);
    Overlay* overlay;
    if(view)
        overlay = (Overlay::Create(*win,*view,hb_parni(3), hb_parni(4))).ptr();
    else
        overlay = (Overlay::Create(*win,hb_parni(2), hb_parni(3),hb_parni(4), hb_parni(5))).ptr();

    initUltralightObj(overlay, getOVERLAYClassId());
}

FORWARD_GETCLASSID(VIEW)
HB_FUNC( ULTRALIGHT_OVERLAY_VIEW ) {
    Overlay* overlay = (Overlay*)hb_selfUltralight();
    hb_retUltralight((RefCounted*)overlay->view().ptr(),getVIEWClassId());
}

HB_FUNC( ULTRALIGHT_OVERLAY_WIDTH ) {
    Overlay* overlay = (Overlay*)hb_selfUltralight();
    hb_retni(overlay->width());
}

HB_FUNC( ULTRALIGHT_OVERLAY_HEIGHT ) {
    Overlay* overlay = (Overlay*)hb_selfUltralight();
    hb_retni(overlay->height());
}

HB_FUNC( ULTRALIGHT_OVERLAY_X ) {
    Overlay* overlay = (Overlay*)hb_selfUltralight();
    hb_retni(overlay->x());
}

HB_FUNC( ULTRALIGHT_OVERLAY_Y ) {
    Overlay* overlay = (Overlay*)hb_selfUltralight();
    hb_retni(overlay->y());
}

HB_FUNC( ULTRALIGHT_OVERLAY_HIDDEN ) {
    Overlay* overlay = (Overlay*)hb_selfUltralight();
    if(hb_pcount()>0) {
        if(hb_parl(1))
            overlay->Hide();
        else
            overlay->Show();
        hb_retl(hb_parl(1));
    } else {
        hb_retl(overlay->is_hidden()? HB_TRUE : HB_FALSE);
    }
}

HB_FUNC( ULTRALIGHT_OVERLAY_HAS_FOCUS ) {
    Overlay* overlay = (Overlay*)hb_selfUltralight();
    if(hb_pcount()>0) {
        if(hb_parl(1))
            overlay->Focus();
        else
            overlay->Unfocus();
        hb_retl(hb_parl(1));
    } else {
        hb_retl(overlay->has_focus());
    }
}

HB_FUNC( ULTRALIGHT_OVERLAY_MOVETO ) {
    Overlay* overlay = (Overlay*)hb_selfUltralight();
    overlay->MoveTo(hb_parni(1),hb_parni(2));
    hb_ret();
}

HB_FUNC( ULTRALIGHT_OVERLAY_RESIZE ) {
    Overlay* overlay = (Overlay*)hb_selfUltralight();
    overlay->Resize(hb_parni(1),hb_parni(2));
    hb_ret();
}

HB_FUNC( ULTRALIGHT_OVERLAY_NEEDSREPAINT ) {
    Overlay* overlay = (Overlay*)hb_selfUltralight();
    hb_retl(overlay->NeedsRepaint()? HB_TRUE : HB_FALSE);
}

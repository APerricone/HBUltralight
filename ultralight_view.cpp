#include "ultralight_hb.h"
#include <ultralight/View.h>
#include <ultralight/Listener.h>
#include "ultralight_view_listener.cpp"

using namespace ultralight;
DEFINE_GETCLASSID(VIEW)

/*
    DATA bOnChangeTitle // (oView,cTitle)
    DATA bOnChangeURL //(oView, cUrl)
    DATA bOnChangeTooltip //(oView, cTooltip)
    DATA bOnChangeCursor //(oView, iCursor) // see defines ulCursor_*
    DATA bOnAddConsoleMessage //(oView,iSource,iLevel,cMessage,iLine_number,iColumn_number,cSource_id)
    DATA bOnCreateChildView // (oView,cOpener_url,cTarget_url,lIs_popup,aPopup_rect {left, top, right, bottom}) -> new oView
    DATA bOnBeginLoading //(oView, iFrame_id, lIs_main_frame, cUrl)
    DATA bOnFinishLoading //(oView, iFrame_id, lIs_main_frame, cUurl)
    DATA bOnFailLoading //(oView, iFrame_id, lIs_main_frame, cUrl, cDescription, cError_domain,iError_code)
    DATA bOnWindowObjectReady //(oView,iFrame_id,lIs_main_frame,cUrl)
    DATA bOnDOMReady //(oView, iFrame_id,lIs_main_frame,cUrl)
    DATA bOnUpdateHistory //(oView
    ACCESS url()
    ACCESS title()
    ACCESS width()
    ACCESS height()
    ACCESS is_loading()
    //METHOD  render_target()
    //METHOD surface();
    METHOD  LoadHTML(html,url,add_to_history)
    METHOD LoadURL(url)
    METHOD Resize(width,height)
    //METHOD LockJSContext();
    METHOD EvaluateScript(script, exception)
    ACCESS CanGoBack()
    ACCESS CanGoForward()
    METHOD GoBack()
    METHOD GoForward()
    METHOD GoToHistoryOffset(offset)
    METHOD Reload()
    METHOD Stop()
    METHOD hasFocus() SETGET
    ACCESS HasInputFocus()
    //METHOD FireKeyEvent(evt)
    //METHOD FireMouseEvent(evt)
    //METHOD FireScrollEvent(evt)
    METHOD needs_paint() SETGET
    METHOD inspector()
*/

void SetupView(PHB_ITEM pItem, View* view) {
    HBViewListener* listener = new HBViewListener(pItem);
    view->set_load_listener((LoadListener*)listener);
    view->set_view_listener((ViewListener*)listener);
}

HB_FUNC( ULTRALIGHT_VIEW_URL ) {
    View* view = (View*)hb_selfUltralight();
    hb_retULString(view->url());
}

HB_FUNC( ULTRALIGHT_VIEW_TITLE ) {
    View* view = (View*)hb_selfUltralight();
    hb_retULString(view->title());
}

HB_FUNC( ULTRALIGHT_VIEW_WIDTH ) {
    View* view = (View*)hb_selfUltralight();
    hb_retni(view->width());
}

HB_FUNC( ULTRALIGHT_VIEW_HEIGHT ) {
    View* view = (View*)hb_selfUltralight();
    hb_retni(view->height());
}

HB_FUNC( ULTRALIGHT_VIEW_IS_LOADING ) {
    View* view = (View*)hb_selfUltralight();
    hb_retl(view->is_loading()? HB_TRUE : HB_FALSE);
}

//HB_FUNC( ULTRALIGHT_VIEW_RENDER_TARGET ) {}
//HB_FUNC( ULTRALIGHT_VIEW_SURFACE ) {}

HB_FUNC( ULTRALIGHT_VIEW_LOADHTML ) { // (cHtml,cUrl,lAdd_to_history)
    View* view = (View*)hb_selfUltralight();
    view->LoadHTML(hb_parc(1),
        HB_ISCHAR(2)? hb_parc(2) : "",
        hb_parldef(3,false));
}

HB_FUNC( ULTRALIGHT_VIEW_LOADURL ) {
    View* view = (View*)hb_selfUltralight();
    view->LoadURL(hb_parc(1));
}

HB_FUNC( ULTRALIGHT_VIEW_RESIZE ) {
    View* view = (View*)hb_selfUltralight();
    view->Resize(hb_parnidef(1,view->width()),hb_parnidef(2,view->height()));
}

//HB_FUNC( ULTRALIGHT_VIEW_LOCKJSCONTEXT ) {
//    View* view = (View*)hb_selfUltralight();
//    view->LockJSContext
//}

HB_FUNC( ULTRALIGHT_VIEW_EVALUATESCRIPT ) {
    View* view = (View*)hb_selfUltralight();
    String exp;
    String ret = view->EvaluateScript(hb_parc(1),&exp);
    PHB_ITEM pItem = hb_stackItemFromBase( 2 );

    if( HB_IS_BYREF( pItem ) ) {
        hb_itemPutULString(pItem, exp);
    }
    hb_retULString(ret);
}

HB_FUNC( ULTRALIGHT_VIEW_CANGOBACK ) {
    View* view = (View*)hb_selfUltralight();
    hb_retl(view->CanGoBack()? HB_TRUE : HB_FALSE);
}

HB_FUNC( ULTRALIGHT_VIEW_CANGOFORWARD ) {
    View* view = (View*)hb_selfUltralight();
    hb_retl(view->CanGoForward()? HB_TRUE : HB_FALSE);
}

HB_FUNC( ULTRALIGHT_VIEW_GOBACK ) {
    View* view = (View*)hb_selfUltralight();
    view->GoBack();
}

HB_FUNC( ULTRALIGHT_VIEW_GOFORWARD ) {
    View* view = (View*)hb_selfUltralight();
    view->GoForward();
}

HB_FUNC( ULTRALIGHT_VIEW_GOTOHISTORYOFFSET ) {
    View* view = (View*)hb_selfUltralight();
    view->GoToHistoryOffset(hb_parni(1));
}

HB_FUNC( ULTRALIGHT_VIEW_RELOAD ) {
    View* view = (View*)hb_selfUltralight();
    view->Reload();
}

HB_FUNC( ULTRALIGHT_VIEW_STOP ) {
    View* view = (View*)hb_selfUltralight();
    view->Stop();
}

HB_FUNC( ULTRALIGHT_VIEW_HASFOCUS ) {
    View* view = (View*)hb_selfUltralight();
    if(hb_pcount()>0) {
        if(hb_parl(1))
            view->Focus();
        else
            view->Unfocus();
        hb_retl(hb_parl(1));
    } else {
        hb_retl(view->HasFocus());
    }
}

HB_FUNC( ULTRALIGHT_VIEW_HASINPUTFOCUS ) {
    View* view = (View*)hb_selfUltralight();
    hb_retl(view->HasFocus()? HB_TRUE : HB_FALSE);
}
//HB_FUNC( ULTRALIGHT_VIEW_FIREKEYEVENT ) {}
//HB_FUNC( ULTRALIGHT_VIEW_FIREMOUSEEVENT ) {}
//HB_FUNC( ULTRALIGHT_VIEW_FIRESCROLLEVENT ) {}
HB_FUNC( ULTRALIGHT_VIEW_NEEDS_PAINT ) {
    View* view = (View*)hb_selfUltralight();
    hb_retl(view->needs_paint()? HB_TRUE : HB_FALSE);
}

HB_FUNC( ULTRALIGHT_VIEW_INSPECTOR ) {
    View* view = (View*)hb_selfUltralight();
    hb_retUltralight(view->inspector().get(), getVIEWClassId());
}

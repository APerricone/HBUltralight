#include "ultralight_hb.h"
#include <ultralight/View.h>
#include <ultralight/Listener.h>
#include "hb_smartApi.h"

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

class HBViewListener : public ViewListener, public LoadListener {
public:

    /// View Listener
    PHB_ITEM pOnChangeTitle;
    virtual void OnChangeTitle(View* caller,
                                const String& title) {
        if(!pOnChangeTitle) return;
        hb_evalBlock(pOnChangeTitle, SmartItem(caller, getVIEWClassId()), SmartItem(title), NULL );
    }

    PHB_ITEM pOnChangeURL;
    virtual void OnChangeURL(View* caller,
                            const String& url) {
        if(!pOnChangeURL) return;
        hb_evalBlock(pOnChangeURL, SmartItem(caller, getVIEWClassId()), SmartItem(url), NULL );
    }

    PHB_ITEM pOnChangeTooltip;
    virtual void OnChangeTooltip(View* caller, const String& tooltip) {
        if(!pOnChangeTooltip) return;
        hb_evalBlock(pOnChangeTooltip, SmartItem(caller, getVIEWClassId()), SmartItem(tooltip), NULL );
    }

    PHB_ITEM pOnChangeCursor;
    virtual void OnChangeCursor(View* caller,
                                Cursor cursor) {
        if(!pOnChangeCursor) return;
        hb_evalBlock(pOnChangeCursor, SmartItem(caller, getVIEWClassId()), SmartItem((int)cursor), NULL );
    }

    PHB_ITEM pOnAddConsoleMessage;
    virtual void OnAddConsoleMessage(View* caller,
                                   MessageSource source,
                                   MessageLevel level,
                                   const String& message,
                                   uint32_t line_number,
                                   uint32_t column_number,
                                   const String& source_id) {
        if(!pOnAddConsoleMessage) return;
        hb_evalBlock(pOnAddConsoleMessage,
            SmartItem(caller, getVIEWClassId()), SmartItem(source), SmartItem(level),
            SmartItem(message), SmartItem((int)line_number),
            SmartItem((int)column_number), SmartItem(source_id), NULL );
    }

    PHB_ITEM pOnCreateChildView;
    virtual RefPtr<View> OnCreateChildView(View* caller,
                                         const String& opener_url,
                                         const String& target_url,
                                         bool is_popup,
                                         const IntRect& popup_rect) {
        if(!pOnCreateChildView) return nullptr;
        hb_evalBlock(pOnCreateChildView, SmartItem(caller, getVIEWClassId()), SmartItem(opener_url),
                                         SmartItem(target_url), SmartItem(is_popup),
                                         SmartItem(popup_rect), NULL );
        RefPtr<View> retView = (View*)hb_selfUltralight(hb_stackReturnItem());
        return retView;
    }

    /// Load Listener
    PHB_ITEM pOnBeginLoading;
    virtual void OnBeginLoading(View* caller,
                              uint64_t frame_id,
                              bool is_main_frame,
                              const String& url) {
        if(!pOnBeginLoading) return;
        hb_evalBlock(pOnBeginLoading, SmartItem(caller, getVIEWClassId()), SmartItem((HB_LONGLONG)frame_id),
            SmartItem(is_main_frame), SmartItem(url), NULL );
    }

    PHB_ITEM pOnFinishLoading;
    virtual void OnFinishLoading(View* caller,
                               uint64_t frame_id,
                               bool is_main_frame,
                               const String& url) {
        if(!pOnFinishLoading) return;
        hb_evalBlock(pOnFinishLoading, SmartItem(caller, getVIEWClassId()), SmartItem((HB_LONGLONG)frame_id),
            SmartItem(is_main_frame), SmartItem(url), NULL );
    }

    PHB_ITEM pOnFailLoading;
    virtual void OnFailLoading(View* caller,
                             uint64_t frame_id,
                             bool is_main_frame,
                             const String& url,
                             const String& description,
                             const String& error_domain,
                             int error_code) {
        if(!pOnFailLoading) return;
        hb_evalBlock(pOnFailLoading, SmartItem(caller, getVIEWClassId()), SmartItem((HB_LONGLONG)frame_id),
                SmartItem(is_main_frame), SmartItem(url), SmartItem(description),
                SmartItem(error_domain), SmartItem(error_code), NULL );
    }

    PHB_ITEM pOnWindowObjectReady;
    virtual void OnWindowObjectReady(View* caller,
                                   uint64_t frame_id,
                                   bool is_main_frame,
                                   const String& url) {
        if(!pOnWindowObjectReady) return;
        hb_evalBlock(pOnWindowObjectReady, SmartItem(caller, getVIEWClassId()), SmartItem((HB_LONGLONG)frame_id),
            SmartItem(is_main_frame), SmartItem(url), NULL );
    }

    PHB_ITEM pOnDOMReady;
    virtual void OnDOMReady(View* caller,
                          uint64_t frame_id,
                          bool is_main_frame,
                          const String& url) {
        if(!pOnDOMReady) return;
        hb_evalBlock(pOnDOMReady, SmartItem(caller, getVIEWClassId()), SmartItem((HB_LONGLONG)frame_id),
            SmartItem(is_main_frame), SmartItem(url), NULL );
    }

    PHB_ITEM pOnUpdateHistory;
    virtual void OnUpdateHistory(View* caller) {
        if(!pOnUpdateHistory) return;
        hb_evalBlock(pOnUpdateHistory, SmartItem(caller, getVIEWClassId()), NULL );
    }

    HBViewListener() :
        pOnChangeTitle(0), pOnChangeURL(0), pOnChangeTooltip(0), pOnChangeCursor(0),
        pOnAddConsoleMessage(0), pOnCreateChildView(0), pOnBeginLoading(0), pOnFinishLoading(0),
        pOnFailLoading(0), pOnWindowObjectReady(0), pOnDOMReady(0), pOnUpdateHistory(0) {};
};

void ManageViewCodeBlockGetSet(PHB_ITEM HBViewListener::*pOnMember) {
    View* view = (View*)hb_selfUltralight();
    HBViewListener* listener = (HBViewListener*)view->view_listener();
    if(hb_pcount()==0) {
        if(listener) {
            hb_itemCopy(hb_stackReturnItem(), listener->*pOnMember);
            return;
        }
    } else {
        if(HB_ISEVALITEM(1)) {
            if(!listener) {
                listener = new HBViewListener();
                view->set_view_listener(listener);
                view->set_load_listener(listener);
            }
            listener->*pOnMember = hb_itemNew(hb_param(1, HB_IT_EVALITEM));
        }
    }
    hb_ret();
}

HB_FUNC( ULTRALIGHT_VIEW_BONCHANGETITLE ) { ManageViewCodeBlockGetSet(&HBViewListener::pOnChangeTitle); }
HB_FUNC( ULTRALIGHT_VIEW_BONCHANGEURL ) { ManageViewCodeBlockGetSet(&HBViewListener::pOnChangeURL); }
HB_FUNC( ULTRALIGHT_VIEW_BONCHANGETOOLTIP ) { ManageViewCodeBlockGetSet(&HBViewListener::pOnChangeTooltip); }
HB_FUNC( ULTRALIGHT_VIEW_BONCHANGECURSOR ) { ManageViewCodeBlockGetSet(&HBViewListener::pOnChangeCursor); }
HB_FUNC( ULTRALIGHT_VIEW_BONADDCONSOLEMESSAGE ) { ManageViewCodeBlockGetSet(&HBViewListener::pOnAddConsoleMessage); }
HB_FUNC( ULTRALIGHT_VIEW_BONCREATECHILDVIEW ) { ManageViewCodeBlockGetSet(&HBViewListener::pOnCreateChildView); }
HB_FUNC( ULTRALIGHT_VIEW_BONBEGINLOADING ) { ManageViewCodeBlockGetSet(&HBViewListener::pOnBeginLoading); }
HB_FUNC( ULTRALIGHT_VIEW_BONFINISHLOADING ) { ManageViewCodeBlockGetSet(&HBViewListener::pOnFinishLoading); }
HB_FUNC( ULTRALIGHT_VIEW_BONFAILLOADING ) { ManageViewCodeBlockGetSet(&HBViewListener::pOnFailLoading); }
HB_FUNC( ULTRALIGHT_VIEW_BONWINDOWOBJECTREADY ) { ManageViewCodeBlockGetSet(&HBViewListener::pOnWindowObjectReady); }
HB_FUNC( ULTRALIGHT_VIEW_BONDOMREADY ) { ManageViewCodeBlockGetSet(&HBViewListener::pOnDOMReady); }
HB_FUNC( ULTRALIGHT_VIEW_BONUPDATEHISTORY ) { ManageViewCodeBlockGetSet(&HBViewListener::pOnUpdateHistory); }

#include "ultralight_hb.h"
#include <ultralight/View.h>
#include <ultralight/Listener.h>

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
    virtual void OnChangeTitle(ultralight::View* caller,
                                const String& title) {
        if(!pOnChangeTitle) return;
        PHB_ITEM pCaller = hb_itemUltralight(caller, getVIEWClassId());
        PHB_ITEM pTitle = hb_itemPutULString(0,title);
        hb_evalBlock(pOnChangeTitle, pCaller, pTitle, NULL );
        hb_itemRelease(pCaller);
        hb_itemRelease(pTitle);
    }

    PHB_ITEM pOnChangeURL;
    virtual void OnChangeURL(ultralight::View* caller,
                            const String& url) {
        if(!pOnChangeURL) return;
        PHB_ITEM pCaller = hb_itemUltralight(caller, getVIEWClassId());
        PHB_ITEM pUrl = hb_itemPutULString(0,url);
        hb_evalBlock(pOnChangeURL, pCaller, pUrl, NULL );
        hb_itemRelease(pCaller);
        hb_itemRelease(pUrl);
    }

    PHB_ITEM pOnChangeTooltip;
    virtual void OnChangeTooltip(ultralight::View* caller,
                               const String& tooltip) {
        if(!pOnChangeTooltip) return;
        PHB_ITEM pCaller = hb_itemUltralight(caller, getVIEWClassId());
        PHB_ITEM pTooltip = hb_itemPutULString(0,tooltip);
        hb_evalBlock(pOnChangeTooltip, pCaller, pTooltip, NULL );
        hb_itemRelease(pCaller);
        hb_itemRelease(pTooltip);
    }

    PHB_ITEM pOnChangeCursor;
    virtual void OnChangeCursor(ultralight::View* caller,
                                Cursor cursor) {
        if(!pOnChangeCursor) return;
        PHB_ITEM pCaller = hb_itemUltralight(caller, getVIEWClassId());
        PHB_ITEM pCursor = hb_itemPutNI(0,(int)cursor);
        hb_evalBlock(pOnChangeCursor, pCaller, pCursor, NULL );
        hb_itemRelease(pCaller);
        hb_itemRelease(pCursor);
    }

    PHB_ITEM pOnAddConsoleMessage;
    virtual void OnAddConsoleMessage(ultralight::View* caller,
                                   MessageSource source,
                                   MessageLevel level,
                                   const String& message,
                                   uint32_t line_number,
                                   uint32_t column_number,
                                   const String& source_id) {
        if(!pOnAddConsoleMessage) return;
        PHB_ITEM pCaller = hb_itemUltralight(caller, getVIEWClassId());
        PHB_ITEM pSource = hb_itemPutNI(0,(int)source);
        PHB_ITEM pLevel = hb_itemPutNI(0,(int)level);
        PHB_ITEM pMessage = hb_itemPutULString(0,message);
        PHB_ITEM pLineNumber = hb_itemPutNI(0,line_number);
        PHB_ITEM pColumnNumber = hb_itemPutNI(0,column_number);
        PHB_ITEM pSourceId = hb_itemPutULString(0,source_id);
        hb_evalBlock(pOnAddConsoleMessage, pCaller, pSource, pLevel, pMessage, pLineNumber, pColumnNumber, pSourceId, NULL );
        hb_itemRelease(pCaller);
        hb_itemRelease(pSource);
        hb_itemRelease(pLevel);
        hb_itemRelease(pMessage);
        hb_itemRelease(pLineNumber);
        hb_itemRelease(pColumnNumber);
        hb_itemRelease(pSourceId);
    }

    PHB_ITEM pOnCreateChildView;
    virtual RefPtr<View> OnCreateChildView(ultralight::View* caller,
                                         const String& opener_url,
                                         const String& target_url,
                                         bool is_popup,
                                         const IntRect& popup_rect) {
        if(!pOnCreateChildView) return nullptr;
        PHB_ITEM pCaller = hb_itemUltralight(caller, getVIEWClassId());
        PHB_ITEM pOpenerURL = hb_itemPutULString(0,opener_url);
        PHB_ITEM pTargetURL = hb_itemPutULString(0,target_url);
        PHB_ITEM pIsPopup = hb_itemPutL(0,is_popup? HB_TRUE : HB_FALSE);
        PHB_ITEM pRect = hb_itemArrayNew(4); //{left, top, right, bottom}
        hb_arraySetNI(pRect,1, popup_rect.left);
        hb_arraySetNI(pRect,2, popup_rect.top);
        hb_arraySetNI(pRect,3, popup_rect.right);
        hb_arraySetNI(pRect,4, popup_rect.bottom);
        hb_evalBlock(pOnCreateChildView, pCaller, pOpenerURL, pTargetURL, pIsPopup, pRect, NULL );
        hb_itemRelease(pCaller);
        hb_itemRelease(pOpenerURL);
        hb_itemRelease(pTargetURL);
        hb_itemRelease(pIsPopup);
        hb_itemRelease(pRect);
        RefPtr<View> retView = (View*)hb_selfUltralight(hb_stackReturnItem());
        return retView;
    }

    /// Load Listener
    PHB_ITEM pOnBeginLoading;
    virtual void OnBeginLoading(ultralight::View* caller,
                              uint64_t frame_id,
                              bool is_main_frame,
                              const String& url) {
        if(!pOnBeginLoading) return;
        PHB_ITEM pCaller = hb_itemUltralight(caller, getVIEWClassId());
        PHB_ITEM pFrameID = hb_itemPutNLL(0,frame_id);
        PHB_ITEM pIsMain = hb_itemPutL(0,is_main_frame);
        PHB_ITEM pUrl = hb_itemPutULString(0, url);
        hb_evalBlock(pOnBeginLoading, pCaller, pFrameID, pIsMain, pUrl, NULL );
        hb_itemRelease( pCaller );
        hb_itemRelease( pFrameID );
        hb_itemRelease( pIsMain );
        hb_itemRelease( pUrl );
    }

    PHB_ITEM pOnFinishLoading;
    virtual void OnFinishLoading(ultralight::View* caller,
                               uint64_t frame_id,
                               bool is_main_frame,
                               const String& url) {
        if(!pOnFinishLoading) return;
        PHB_ITEM pCaller = hb_itemUltralight(caller, getVIEWClassId());
        PHB_ITEM pFrameID = hb_itemPutNLL(0,frame_id);
        PHB_ITEM pIsMain = hb_itemPutL(0,is_main_frame);
        PHB_ITEM pUrl = hb_itemPutULString(0, url);
        hb_evalBlock(pOnFinishLoading, pCaller, pFrameID, pIsMain, pUrl, NULL );
        hb_itemRelease( pCaller );
        hb_itemRelease( pFrameID );
        hb_itemRelease( pIsMain );
        hb_itemRelease( pUrl );
    }

    PHB_ITEM pOnFailLoading;
    virtual void OnFailLoading(ultralight::View* caller,
                             uint64_t frame_id,
                             bool is_main_frame,
                             const String& url,
                             const String& description,
                             const String& error_domain,
                             int error_code) {
        if(!pOnFailLoading) return;
        PHB_ITEM pCaller = hb_itemUltralight(caller, getVIEWClassId());
        PHB_ITEM pFrameID = hb_itemPutNLL(0,frame_id);
        PHB_ITEM pIsMain = hb_itemPutL(0,is_main_frame);
        PHB_ITEM pUrl = hb_itemPutULString(0, url);
        PHB_ITEM pDescription = hb_itemPutULString(0, description);
        PHB_ITEM pErrorDomain = hb_itemPutULString(0, error_domain);
        PHB_ITEM pErrorCode = hb_itemPutNI(0, error_code);
        hb_evalBlock(pOnFailLoading, pCaller, pFrameID, pIsMain, pUrl, pDescription, pErrorDomain, pErrorCode, NULL );
        hb_itemRelease( pCaller );
        hb_itemRelease( pFrameID );
        hb_itemRelease( pIsMain );
        hb_itemRelease( pUrl );
        hb_itemRelease( pDescription );
        hb_itemRelease( pErrorDomain );
        hb_itemRelease( pErrorCode );
    }

    PHB_ITEM pOnWindowObjectReady;
    virtual void OnWindowObjectReady(ultralight::View* caller,
                                   uint64_t frame_id,
                                   bool is_main_frame,
                                   const String& url) {
        if(!pOnWindowObjectReady) return;
        PHB_ITEM pCaller = hb_itemUltralight(caller, getVIEWClassId());
        PHB_ITEM pFrameID = hb_itemPutNLL(0,frame_id);
        PHB_ITEM pIsMain = hb_itemPutL(0,is_main_frame);
        PHB_ITEM pUrl = hb_itemPutULString(0, url);
        hb_evalBlock(pOnWindowObjectReady, pCaller, pFrameID, pIsMain, pUrl, NULL );
        hb_itemRelease( pCaller );
        hb_itemRelease( pFrameID );
        hb_itemRelease( pIsMain );
        hb_itemRelease( pUrl );
    }

    PHB_ITEM pOnDOMReady;
    virtual void OnDOMReady(ultralight::View* caller,
                          uint64_t frame_id,
                          bool is_main_frame,
                          const String& url) {
        if(!pOnDOMReady) return;
        PHB_ITEM pCaller = hb_itemUltralight(caller, getVIEWClassId());
        PHB_ITEM pFrameID = hb_itemPutNLL(0,frame_id);
        PHB_ITEM pIsMain = hb_itemPutL(0,is_main_frame);
        PHB_ITEM pUrl = hb_itemPutULString(0, url);
        hb_evalBlock(pOnDOMReady, pCaller, pFrameID, pIsMain, pUrl, NULL );
        hb_itemRelease( pCaller );
        hb_itemRelease( pFrameID );
        hb_itemRelease( pIsMain );
        hb_itemRelease( pUrl );
    }

    PHB_ITEM pOnUpdateHistory;
    virtual void OnUpdateHistory(ultralight::View* caller) {
        if(!pOnUpdateHistory) return;
        PHB_ITEM pCaller = hb_itemUltralight(caller, getVIEWClassId());
        hb_evalBlock(pOnUpdateHistory, pCaller, NULL );
        hb_itemRelease( pCaller );
    }

    HBViewListener() :
        pOnChangeTitle(0), pOnChangeURL(0), pOnChangeTooltip(0), pOnChangeCursor(0),
        pOnAddConsoleMessage(0), pOnCreateChildView(0), pOnBeginLoading(0), pOnFinishLoading(0),
        pOnFailLoading(0), pOnWindowObjectReady(0), pOnDOMReady(0), pOnUpdateHistory(0) {};
};

void ManageBlockGetSet(PHB_ITEM HBViewListener::*pOnMember) {
    View* view = (View*)hb_selfUltralight();
    HBViewListener* listener = (HBViewListener*)view->view_listener();
    if(hb_pcount()>0) {
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
            listener->*pOnMember = hb_param(1, HB_IT_EVALITEM);
        }
    }
    hb_ret();
}

HB_FUNC( ULTRALIGHT_VIEW_BONCHANGETITLE ) { ManageBlockGetSet(&HBViewListener::pOnChangeTitle); }
HB_FUNC( ULTRALIGHT_VIEW_BONCHANGEURL ) { ManageBlockGetSet(&HBViewListener::pOnChangeURL); }
HB_FUNC( ULTRALIGHT_VIEW_BONCHANGETOOLTIP ) { ManageBlockGetSet(&HBViewListener::pOnChangeTooltip); }
HB_FUNC( ULTRALIGHT_VIEW_BONCHANGECURSOR ) { ManageBlockGetSet(&HBViewListener::pOnChangeCursor); }
HB_FUNC( ULTRALIGHT_VIEW_BONADDCONSOLEMESSAGE ) { ManageBlockGetSet(&HBViewListener::pOnAddConsoleMessage); }
HB_FUNC( ULTRALIGHT_VIEW_BONCREATECHILDVIEW ) { ManageBlockGetSet(&HBViewListener::pOnCreateChildView); }
HB_FUNC( ULTRALIGHT_VIEW_BONBEGINLOADING ) { ManageBlockGetSet(&HBViewListener::pOnBeginLoading); }
HB_FUNC( ULTRALIGHT_VIEW_BONFINISHLOADING ) { ManageBlockGetSet(&HBViewListener::pOnFinishLoading); }
HB_FUNC( ULTRALIGHT_VIEW_BONFAILLOADING ) { ManageBlockGetSet(&HBViewListener::pOnFailLoading); }
HB_FUNC( ULTRALIGHT_VIEW_BONWINDOWOBJECTREADY ) { ManageBlockGetSet(&HBViewListener::pOnWindowObjectReady); }
HB_FUNC( ULTRALIGHT_VIEW_BONDOMREADY ) { ManageBlockGetSet(&HBViewListener::pOnDOMReady); }
HB_FUNC( ULTRALIGHT_VIEW_BONUPDATEHISTORY ) { ManageBlockGetSet(&HBViewListener::pOnUpdateHistory); }

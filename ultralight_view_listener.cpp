#include "ultralight_hb.h"
#include <ultralight/Listener.h>

using namespace ultralight;
FORWARD_GETCLASSID(VIEW);

HB_SIZE ibOnChangeTitle = 0;
HB_SIZE ibOnChangeURL = 0;
HB_SIZE ibOnChangeTooltip = 0;
HB_SIZE ibOnChangeCursor = 0;
HB_SIZE ibOnAddConsoleMessage = 0;
HB_SIZE ibOnCreateChildView = 0;
HB_SIZE ibOnBeginLoading = 0;
HB_SIZE ibOnFinishLoading = 0;
HB_SIZE ibOnFailLoading = 0;
HB_SIZE ibOnWindowObjectReady = 0;
HB_SIZE ibOnDOMReady = 0;
HB_SIZE ibOnUpdateHistory = 0;

#define CHECKIDX(idx)  \
    if(i##idx==0) i##idx = hb_clsGetVarIndex(getVIEWClassId(),hb_dynsymGet(#idx)); \
    PHB_ITEM pCallback = hb_itemArrayGet(pItem, i##idx); \
    if(!HB_IS_EVALITEM( pCallback )) return

class HBViewListener : public ViewListener, LoadListener {
private:
    PHB_ITEM pItem;
public:
    HBViewListener(PHB_ITEM pItem) : pItem(pItem) { }

    PHB_ITEM getItem() const { return pItem; }

    /// View Listener
    virtual void OnChangeTitle(ultralight::View* caller,
                                const String& title) {
        CHECKIDX(bOnChangeTitle);
        PHB_ITEM pTitle = hb_itemPutULString(0,title);
        hb_evalBlock(pCallback, pItem, pTitle, NULL );
        hb_itemRelease(pTitle);
    }

    virtual void OnChangeURL(ultralight::View* caller,
                            const String& url) {
        CHECKIDX(bOnChangeURL);
        PHB_ITEM pUrl = hb_itemPutULString(0,url);
        hb_evalBlock(pCallback, pItem, pUrl, NULL );
        hb_itemRelease(pUrl);
    }

    virtual void OnChangeTooltip(ultralight::View* caller,
                               const String& tooltip) {
        CHECKIDX(bOnChangeTooltip);
        PHB_ITEM pTooltip = hb_itemPutULString(0,tooltip);
        hb_evalBlock(pCallback, pItem, pTooltip, NULL );
        hb_itemRelease(pTooltip);
    }

    virtual void OnChangeCursor(ultralight::View* caller,
                                Cursor cursor) {
        CHECKIDX(bOnChangeCursor);
        PHB_ITEM pCursor = hb_itemPutNI(0,(int)cursor);
        hb_evalBlock(pCallback, pItem, pCursor, NULL );
        hb_itemRelease(pCursor);
    }

    virtual void OnAddConsoleMessage(ultralight::View* caller,
                                   MessageSource source,
                                   MessageLevel level,
                                   const String& message,
                                   uint32_t line_number,
                                   uint32_t column_number,
                                   const String& source_id) {
        CHECKIDX(bOnAddConsoleMessage);
        PHB_ITEM pSource = hb_itemPutNI(0,(int)source);
        PHB_ITEM pLevel = hb_itemPutNI(0,(int)level);
        PHB_ITEM pMessage = hb_itemPutULString(0,message);
        PHB_ITEM pLineNumber = hb_itemPutNI(0,line_number);
        PHB_ITEM pColumnNumber = hb_itemPutNI(0,column_number);
        PHB_ITEM pSourceId = hb_itemPutULString(0,source_id);
        hb_evalBlock(pCallback, pItem, pSource, pLevel, pMessage, pLineNumber, pColumnNumber, pSourceId, NULL );
        hb_itemRelease(pSource);
        hb_itemRelease(pLevel);
        hb_itemRelease(pMessage);
        hb_itemRelease(pLineNumber);
        hb_itemRelease(pColumnNumber);
        hb_itemRelease(pSourceId);
    }

    virtual RefPtr<View> OnCreateChildView(ultralight::View* caller,
                                         const String& opener_url,
                                         const String& target_url,
                                         bool is_popup,
                                         const IntRect& popup_rect) {
        CHECKIDX(bOnCreateChildView) nullptr;
        PHB_ITEM pOpenerURL = hb_itemPutULString(0,opener_url);
        PHB_ITEM pTargetURL = hb_itemPutULString(0,target_url);
        PHB_ITEM pIsPopup = hb_itemPutL(0,is_popup? HB_TRUE : HB_FALSE);
        PHB_ITEM pRect = hb_itemArrayNew(4); //{left, top, right, bottom}
        hb_arraySetNI(pRect,1, popup_rect.left);
        hb_arraySetNI(pRect,2, popup_rect.top);
        hb_arraySetNI(pRect,3, popup_rect.right);
        hb_arraySetNI(pRect,4, popup_rect.bottom);
        hb_evalBlock(pCallback, pItem, pOpenerURL, pTargetURL, pIsPopup, pRect, NULL );
        hb_itemRelease(pOpenerURL);
        hb_itemRelease(pTargetURL);
        hb_itemRelease(pIsPopup);
        hb_itemRelease(pRect);
        RefPtr<View> retView = (View*)hb_selfUltralight(hb_stackReturnItem());
        return retView;
    }
    /// Load Listener
    virtual void OnBeginLoading(ultralight::View* caller,
                              uint64_t frame_id,
                              bool is_main_frame,
                              const String& url) {
        CHECKIDX(bOnBeginLoading);
        PHB_ITEM pFrameID = hb_itemPutNLL(0,frame_id);
        PHB_ITEM pIsMain = hb_itemPutL(0,is_main_frame);
        PHB_ITEM pUrl = hb_itemPutULString(0, url);
        hb_evalBlock(pCallback, pItem, pFrameID, pIsMain, pUrl, NULL );
        hb_itemRelease( pFrameID );
        hb_itemRelease( pIsMain );
        hb_itemRelease( pUrl );


    }

    virtual void OnFinishLoading(ultralight::View* caller,
                               uint64_t frame_id,
                               bool is_main_frame,
                               const String& url) {
        CHECKIDX(bOnFinishLoading);
        PHB_ITEM pFrameID = hb_itemPutNLL(0,frame_id);
        PHB_ITEM pIsMain = hb_itemPutL(0,is_main_frame);
        PHB_ITEM pUrl = hb_itemPutULString(0, url);
        hb_evalBlock(pCallback, pItem, pFrameID, pIsMain, pUrl, NULL );
        hb_itemRelease( pFrameID );
        hb_itemRelease( pIsMain );
        hb_itemRelease( pUrl );
    }

    virtual void OnFailLoading(ultralight::View* caller,
                             uint64_t frame_id,
                             bool is_main_frame,
                             const String& url,
                             const String& description,
                             const String& error_domain,
                             int error_code) {
        CHECKIDX(bOnFailLoading);
        PHB_ITEM pFrameID = hb_itemPutNLL(0,frame_id);
        PHB_ITEM pIsMain = hb_itemPutL(0,is_main_frame);
        PHB_ITEM pUrl = hb_itemPutULString(0, url);
        PHB_ITEM pDescription = hb_itemPutULString(0, description);
        PHB_ITEM pErrorDomain = hb_itemPutULString(0, error_domain);
        PHB_ITEM pErrorCode = hb_itemPutNI(0, error_code);
        hb_evalBlock(pCallback, pItem, pFrameID, pIsMain, pUrl, pDescription, pErrorDomain, pErrorCode, NULL );
        hb_itemRelease( pFrameID );
        hb_itemRelease( pIsMain );
        hb_itemRelease( pUrl );
        hb_itemRelease( pDescription );
        hb_itemRelease( pErrorDomain );
        hb_itemRelease( pErrorCode );
    }

    virtual void OnWindowObjectReady(ultralight::View* caller,
                                   uint64_t frame_id,
                                   bool is_main_frame,
                                   const String& url) {
        CHECKIDX(bOnWindowObjectReady);
        PHB_ITEM pFrameID = hb_itemPutNLL(0,frame_id);
        PHB_ITEM pIsMain = hb_itemPutL(0,is_main_frame);
        PHB_ITEM pUrl = hb_itemPutULString(0, url);
        hb_evalBlock(pCallback, pItem, pFrameID, pIsMain, pUrl, NULL );
        hb_itemRelease( pFrameID );
        hb_itemRelease( pIsMain );
        hb_itemRelease( pUrl );
    }

    virtual void OnDOMReady(ultralight::View* caller,
                          uint64_t frame_id,
                          bool is_main_frame,
                          const String& url) {
        CHECKIDX(bOnDOMReady);
        PHB_ITEM pFrameID = hb_itemPutNLL(0,frame_id);
        PHB_ITEM pIsMain = hb_itemPutL(0,is_main_frame);
        PHB_ITEM pUrl = hb_itemPutULString(0, url);
        hb_evalBlock(pCallback, pItem, pFrameID, pIsMain, pUrl, NULL );
        hb_itemRelease( pFrameID );
        hb_itemRelease( pIsMain );
        hb_itemRelease( pUrl );
    }

    virtual void OnUpdateHistory(ultralight::View* caller) {
        CHECKIDX(bOnUpdateHistory);
        hb_evalBlock(pCallback, pItem, NULL );
    }

};
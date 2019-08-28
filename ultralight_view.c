#include "ultralight_hb.h"
OBJDATA ultralight_view;

/*
    //METHOD url()
    //METHOD title()
    //ACCESS is_loading() 
    //METHOD render_target()
    //METHOD is_bitmap_dirty()
    //METHOD bitmap()
    METHOD LoadHTML(cHTML)
    //METHOD LoadURL(cURL)
    //METHOD Resize(width,height)  
    //METHOD js_context()  
    //METHOD EvaluateScript(cScript)
    //METHOD CanGoBack()
    //METHOD CanGoForward()
    //METHOD GoBack()
    //METHOD GoForward()
    //METHOD GoToHistoryOffset(offset)
    //METHOD Reload()
    //METHOD Stop()
    //METHOD FireKeyEvent(evt)
    //METHOD FireMouseEvent(evt)
    //METHOD FireScrollEvent( evt)
    //METHOD view_listener() SETGET
    //METHOD load_listener() SETGET
    //METHOD needs_paint() SETGET
*/

HB_SIZE ptr_bOnChangeTitle = 0;
HB_SIZE ptr_bOnChangeURL = 0;
HB_SIZE ptr_bOnChangeTooltip = 0;
HB_SIZE ptr_bOnChangeCursor = 0;
HB_SIZE ptr_bOnAddConsoleMessage = 0;
HB_SIZE ptr_bOnBeginLoading = 0;
HB_SIZE ptr_bOnFinishLoading = 0;
HB_SIZE ptr_bOnUpdateHistory = 0;
HB_SIZE ptr_bOnDOMReady = 0;

static void hbChangeTitleCallback (void* user_data, ULView caller, ULString title);

void SetupView(PHB_ITEM pRet,ULView view) {
    if(ptr_bOnChangeTitle==0) {
        ptr_bOnChangeTitle      = hb_clsGetVarIndex(ultralight_view.classId,hb_dynsymGet("bOnChangeTitle"));
        ptr_bOnChangeURL        = hb_clsGetVarIndex(ultralight_view.classId,hb_dynsymGet("bOnChangeURL"));
        ptr_bOnChangeTooltip    = hb_clsGetVarIndex(ultralight_view.classId,hb_dynsymGet("bOnChangeTooltip"));
        ptr_bOnChangeCursor     = hb_clsGetVarIndex(ultralight_view.classId,hb_dynsymGet("bOnChangeCursor"));
        ptr_bOnAddConsoleMessage= hb_clsGetVarIndex(ultralight_view.classId,hb_dynsymGet("bOnAddConsoleMessage"));
        ptr_bOnBeginLoading     = hb_clsGetVarIndex(ultralight_view.classId,hb_dynsymGet("bOnBeginLoading"));
        ptr_bOnFinishLoading    = hb_clsGetVarIndex(ultralight_view.classId,hb_dynsymGet("bOnFinishLoading"));
        ptr_bOnUpdateHistory    = hb_clsGetVarIndex(ultralight_view.classId,hb_dynsymGet("bOnUpdateHistory"));
        ptr_bOnDOMReady         = hb_clsGetVarIndex(ultralight_view.classId,hb_dynsymGet("bOnDOMReady"));
    }
    hb_itemArrayPut(pRet, ultralight_view.ptrObj, hb_itemPutPtr(0, view));
    ulViewSetChangeTitleCallback(view,hbChangeTitleCallback,pRet);
}

HB_FUNC( ULTRALIGHT_VIEW_LOADHTML ) {
	ULView view = SELF_VIEW();
    ulViewLoadHTML(view,ulCreateString(hb_parc(1)));
    hb_ret();
}

void hbChangeTitleCallback (void* pItem, ULView caller, ULString title) {
    PHB_ITEM pCallback = hb_itemArrayGet(pItem, ptr_bOnChangeTitle);
    (caller);
    if(HB_ITEM_TYPE( pCallback ) != HB_IT_BLOCK) return;
    hb_evalBlock(pCallback, pItem, 
        hb_itemPutStrLenU16( 0, HB_CDP_ENDIAN_NATIVE,ulStringGetData(title), ulStringGetLength(title) ), NULL );
}
#include "ultralight_hb.h"
OBJDATA ultralight_view;

/*
    //METHOD url()
    //METHOD title()
    //ACCESS is_loading() 
    //METHOD render_target()
    //METHOD is_bitmap_dirty()
    METHOD bitmap()
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

static void hbChangeTitleCallback(void* user_data, ULView caller, ULString title);
static void hbOnChangeURLCallback(void* user_data, ULView caller, ULString url);
static void hbOnChangeTooltipCallback(void* user_data, ULView caller, ULString tooltip);
static void hbOnChangeCursorCallback(void* user_data, ULView caller, ULCursor cursor);
static void hbOnAddConsoleMessageCallback(void* user_data, ULView caller, ULMessageSource source, ULMessageLevel level,ULString message, unsigned int line_number,unsigned int column_number,ULString source_id);
static void hbOnBeginLoadingCallback(void* user_data, ULView caller);
static void hbOnFinishLoadingCallback(void* user_data, ULView caller);
static void hbOnUpdateHistoryCallback(void* user_data, ULView caller);
static void hbOnDOMReadyCallback(void* user_data, ULView caller);

void SetupView(PHB_ITEM pRet,ULView view) {
    PHB_ITEM pTemp = hb_itemNew(0);
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
    hb_itemCopy(pTemp,pRet);
    ulViewSetChangeTitleCallback(view,hbChangeTitleCallback,pTemp);
    ulViewSetChangeURLCallback(view,hbOnChangeURLCallback,pTemp);
    ulViewSetChangeTooltipCallback(view,hbOnChangeTooltipCallback,pTemp);
    ulViewSetChangeCursorCallback(view,hbOnChangeCursorCallback,pTemp);
    ulViewSetAddConsoleMessageCallback(view,hbOnAddConsoleMessageCallback,pTemp);
    ulViewSetBeginLoadingCallback(view,hbOnBeginLoadingCallback,pTemp);
    ulViewSetFinishLoadingCallback(view,hbOnFinishLoadingCallback,pTemp);
    ulViewSetUpdateHistoryCallback(view,hbOnUpdateHistoryCallback,pTemp);
    ulViewSetDOMReadyCallback(view,hbOnDOMReadyCallback,pTemp);
}

HB_FUNC( ULTRALIGHT_VIEW_LOADHTML ) {
	ULView view = SELF_VIEW();
    ulViewLoadHTML(view,ulCreateString(hb_parc(1)));
    hb_ret();
}

HB_FUNC_EXTERN( ULTRALIGHT_BITMAP );
HB_FUNC( ULTRALIGHT_VIEW_BITMAP ) {
    PHB_ITEM pRet;
	ULView view = SELF_VIEW();
    ULBitmap bitmap = ulViewGetBitmap(view);
    HB_FUNC_EXEC(ULTRALIGHT_BITMAP);
    setupOBJDATA("ULTRALIGHT_BITMAP", &ultralight_bitmap);
    hb_clsAssociate(  ultralight_bitmap.classId );
    pRet = hb_stackReturnItem();
    hb_itemArrayPut(pRet, ultralight_bitmap.ptrObj, hb_itemPutPtr(0, bitmap)); 
}

HB_FUNC( ULTRALIGHT_VIEW_JS_CONTEXT ) {
	ULView view = SELF_VIEW();
    JSContextRef ctx = ulViewGetJSContext(view);
    hb_retptr((void*)ctx);
}

void hbChangeTitleCallback (void* user_data, ULView caller, ULString title) {
    HB_SYMBOL_UNUSED(caller);
    PHB_ITEM pCallback = hb_itemArrayGet((PHB_ITEM)user_data, ptr_bOnChangeTitle);
    if(!HB_IS_EVALITEM( pCallback )) return;
    hb_evalBlock(pCallback, (PHB_ITEM)user_data, 
        hb_itemPutStrLenU16( 0, HB_CDP_ENDIAN_NATIVE,ulStringGetData(title), ulStringGetLength(title) ), NULL );
}

void hbOnChangeURLCallback(void* user_data, ULView caller, ULString url) {
    HB_SYMBOL_UNUSED(caller);
    PHB_ITEM pCallback = hb_itemArrayGet((PHB_ITEM)user_data, ptr_bOnChangeURL);
    if(!HB_IS_EVALITEM( pCallback )) return;
    hb_evalBlock(pCallback, (PHB_ITEM)user_data, 
        hb_itemPutStrLenU16( 0, HB_CDP_ENDIAN_NATIVE,ulStringGetData(url), ulStringGetLength(url) ), NULL );
}

void hbOnChangeTooltipCallback(void* user_data, ULView caller, ULString tooltip) {
    HB_SYMBOL_UNUSED(caller);
    PHB_ITEM pCallback = hb_itemArrayGet((PHB_ITEM)user_data, ptr_bOnChangeTooltip);
    if(!HB_IS_EVALITEM( pCallback )) return;
    hb_evalBlock(pCallback, (PHB_ITEM)user_data, 
        hb_itemPutStrLenU16( 0, HB_CDP_ENDIAN_NATIVE,ulStringGetData(tooltip), ulStringGetLength(tooltip) ), NULL );
}

void hbOnChangeCursorCallback(void* user_data, ULView caller, ULCursor cursor) {
    HB_SYMBOL_UNUSED(caller);
    PHB_ITEM pCallback = hb_itemArrayGet((PHB_ITEM)user_data, ptr_bOnChangeCursor);
    if(!HB_IS_EVALITEM( pCallback )) return;
    hb_evalBlock(pCallback, (PHB_ITEM)user_data, hb_itemPutNI(0, (int)cursor),  NULL );
}

void hbOnAddConsoleMessageCallback(void* user_data, ULView caller, ULMessageSource source, ULMessageLevel level,
    ULString message, unsigned int line_number,unsigned int column_number,ULString source_id) {
    HB_SYMBOL_UNUSED(caller);
    PHB_ITEM pCallback = hb_itemArrayGet((PHB_ITEM)user_data, ptr_bOnAddConsoleMessage);
    if(!HB_IS_EVALITEM( pCallback )) return;
    hb_evalBlock(pCallback, (PHB_ITEM)user_data, hb_itemPutNI(0, (int)source), hb_itemPutNI(0, (int)level), 
        hb_itemPutStrLenU16( 0, HB_CDP_ENDIAN_NATIVE,ulStringGetData(message), ulStringGetLength(message) ),
        hb_itemPutNI(0, (int)line_number), hb_itemPutNI(0, (int)column_number), 
        hb_itemPutStrLenU16( 0, HB_CDP_ENDIAN_NATIVE,ulStringGetData(source_id), ulStringGetLength(source_id) ), NULL );   
}

void hbOnBeginLoadingCallback(void* user_data, ULView caller) {
    HB_SYMBOL_UNUSED(caller);
    PHB_ITEM pCallback = hb_itemArrayGet((PHB_ITEM)user_data, ptr_bOnBeginLoading);
    if(!HB_IS_EVALITEM( pCallback )) return;
    hb_evalBlock(pCallback, (PHB_ITEM)user_data, NULL );
}

void hbOnFinishLoadingCallback(void* user_data, ULView caller) {
    HB_SYMBOL_UNUSED(caller);
    PHB_ITEM pCallback = hb_itemArrayGet((PHB_ITEM)user_data, ptr_bOnFinishLoading);
    if(!HB_IS_EVALITEM( pCallback )) return;
    hb_evalBlock(pCallback, (PHB_ITEM)user_data, NULL );
}

void hbOnUpdateHistoryCallback(void* user_data, ULView caller) {
    HB_SYMBOL_UNUSED(caller);
    PHB_ITEM pCallback = hb_itemArrayGet((PHB_ITEM)user_data, ptr_bOnUpdateHistory);
    if(!HB_IS_EVALITEM( pCallback )) return;
    hb_evalBlock(pCallback, (PHB_ITEM)user_data, NULL );
}

void hbOnDOMReadyCallback(void* user_data, ULView caller) {
    HB_SYMBOL_UNUSED(caller);
	//int type = HB_ITEM_TYPE(user_data);
	//int claId = hb_objGetClass(user_data);
    PHB_ITEM pCallback = hb_itemArrayGet((PHB_ITEM)user_data, ptr_bOnDOMReady);
	//type = HB_ITEM_TYPE(pCallback);

    if(!HB_IS_EVALITEM( pCallback )) return;
    hb_evalBlock(pCallback, (PHB_ITEM)user_data, NULL );
}

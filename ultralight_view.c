#include "ultralight_hb.h"
OBJDATA ultralight_view;

/*
    //METHOD url()
    //METHOD title()
    ACCESS is_loading() 
    //METHOD render_target()
    //METHOD is_bitmap_dirty()
    METHOD bitmap()
    METHOD LoadHTML(cHTML)
    METHOD LoadURL(cURL)
    //METHOD Resize(width,height)  
    METHOD js_context()  
    //METHOD EvaluateScript(cScript)
    METHOD CanGoBack()
    METHOD CanGoForward()
    METHOD GoBack()
    METHOD GoForward()
    //METHOD GoToHistoryOffset(offset)
    METHOD Reload()
    METHOD Stop()
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

HB_FUNC_EXTERN( ULTRALIGHT_VIEW );
static void SetupView(ULView view) {
	if(ultralight_view.classId == 0) {
		ultralight_view.classId = hb_clsFindClass("ULTRALIGHT_VIEW", NULL);
	    if(ultralight_view.classId == 0) {
		    HB_FUNC_EXEC(ULTRALIGHT_VIEW);
		    ultralight_view.classId = hb_clsFindClass("ULTRALIGHT_VIEW", NULL);
	    }
		ultralight_view.ptrObj = hb_clsGetVarIndex(ultralight_view.classId,hb_dynsymGet("pObj"));

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
    hb_clsAssociate( ultralight_view.classId );
    hb_itemArrayPut(hb_stackReturnItem(), ultralight_view.ptrObj, hb_itemPutPtr(0, view));
    ulViewSetChangeTitleCallback(view,hbChangeTitleCallback,0);
    ulViewSetChangeURLCallback(view,hbOnChangeURLCallback,0);
    ulViewSetChangeTooltipCallback(view,hbOnChangeTooltipCallback,0);
    ulViewSetChangeCursorCallback(view,hbOnChangeCursorCallback,00);
    ulViewSetAddConsoleMessageCallback(view,hbOnAddConsoleMessageCallback,0);
    ulViewSetBeginLoadingCallback(view,hbOnBeginLoadingCallback,0);
    ulViewSetFinishLoadingCallback(view,hbOnFinishLoadingCallback,0);
    ulViewSetUpdateHistoryCallback(view,hbOnUpdateHistoryCallback,0);
    ulViewSetDOMReadyCallback(view,hbOnDOMReadyCallback,0);
}

PHB_ITEM viewHash;
PHB_ITEM itemFromView(ULView view) {
    HB_SIZE pos;
    if(!viewHash) viewHash = hb_hashNew(0);
    PHB_ITEM ptrItem = hb_itemPutPtr(0,view);
    if(hb_hashScan(viewHash, ptrItem, &pos)) {
        hb_itemRelease(ptrItem);
        return hb_hashGetValueAt(viewHash,pos);
    }
    hb_itemRelease(ptrItem);
    return 0;
}

void hb_retView(ULView view) {
    PHB_ITEM pItem = itemFromView(view);
    if(pItem) {
        hb_itemCopy(hb_stackReturnItem(), pItem);
        return;
    }
    SetupView(view); //it is called only here, but it is so big!
    PHB_ITEM ptrItem = hb_itemPutPtr(0,view);
    PHB_ITEM dest = hb_itemNew(hb_stackReturnItem());
    hb_hashAdd(viewHash,ptrItem,dest);
    hb_itemRelease(dest);
}


HB_FUNC( ULTRALIGHT_VIEW_LOADHTML ) {
	ULView view = SELF_VIEW();
    ULString html = ulCreateString(hb_parc(1));
    ulViewLoadHTML(view,html);
    ulDestroyString(html);
    hb_ret();
}

HB_FUNC( ULTRALIGHT_VIEW_LOADURL ) {
	ULView view = SELF_VIEW();
    ULString url = ulCreateString(hb_parc(1));
    ulViewLoadURL(view,url);
    ulDestroyString(url);
    hb_ret();
}

HB_FUNC( ULTRALIGHT_VIEW_RESIZE ) {
    ulViewResize(SELF_VIEW(),hb_parni(1),hb_parni(2));
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

HB_FUNC( ULTRALIGHT_VIEW_URL ) {
	ULView view = SELF_VIEW();
    /// @note Don't destroy the returned string, it is owned by the View.
    ULString url = ulViewGetURL(view);
    hb_itemPutStrLenU16(hb_stackReturnItem(),HB_CDP_ENDIAN_NATIVE,
        ulStringGetData(url), ulStringGetLength(url) );
}
HB_FUNC( ULTRALIGHT_VIEW_TITLE ) {
	ULView view = SELF_VIEW();
    /// @note Don't destroy the returned string, it is owned by the View.
    ULString title = ulViewGetTitle(view);
    hb_itemPutStrLenU16(hb_stackReturnItem(),HB_CDP_ENDIAN_NATIVE,
        ulStringGetData(title), ulStringGetLength(title) );
}

HB_FUNC( ULTRALIGHT_VIEW_IS_LOADING ) {
	ULView view = SELF_VIEW();
    hb_retl(ulViewIsLoading(view));
}
HB_FUNC( ULTRALIGHT_VIEW_CANGOBACK ) {
	ULView view = SELF_VIEW();
    hb_retl(ulViewCanGoBack(view));
}
HB_FUNC( ULTRALIGHT_VIEW_CANGOFORWARD ) {
	ULView view = SELF_VIEW();
    hb_retl(ulViewCanGoForward(view));
}

HB_FUNC( ULTRALIGHT_VIEW_GOBACK ) {
	ULView view = SELF_VIEW();
    ulViewGoBack(view);
    hb_ret();
}

HB_FUNC( ULTRALIGHT_VIEW_GOFORWARD ) {
	ULView view = SELF_VIEW();
    ulViewGoForward(view);
    hb_ret();
}
HB_FUNC( ULTRALIGHT_VIEW_RELOAD ) {
	ULView view = SELF_VIEW();
    ulViewReload(view);
    hb_ret();
}
HB_FUNC( ULTRALIGHT_VIEW_STOP ) {
	ULView view = SELF_VIEW();
    ulViewStop(view);
    hb_ret();
}


void hbChangeTitleCallback (void* user_data, ULView caller, ULString title) {
    PHB_ITEM pItemView = itemFromView(caller);
    PHB_ITEM pCallback = hb_itemArrayGet((PHB_ITEM)pItemView, ptr_bOnChangeTitle);
    HB_SYMBOL_UNUSED(user_data);
    if(!HB_IS_EVALITEM( pCallback )) return;
    PHB_ITEM pTitle = hb_itemPutStrLenU16( 0, HB_CDP_ENDIAN_NATIVE,ulStringGetData(title), ulStringGetLength(title) );
    hb_evalBlock(pCallback, pItemView, pTitle, NULL );
    hb_itemRelease(pTitle);
}

void hbOnChangeURLCallback(void* user_data, ULView caller, ULString url) {
    PHB_ITEM pItemView = itemFromView(caller);
    PHB_ITEM pCallback = hb_itemArrayGet((PHB_ITEM)itemFromView(caller), ptr_bOnChangeURL);
    HB_SYMBOL_UNUSED(user_data);
    if(!HB_IS_EVALITEM( pCallback )) return;
    PHB_ITEM pUrl = hb_itemPutStrLenU16( 0, HB_CDP_ENDIAN_NATIVE,ulStringGetData(url), ulStringGetLength(url) );
    hb_evalBlock(pCallback, pItemView, pUrl, NULL );
    hb_itemRelease(pUrl);
}

void hbOnChangeTooltipCallback(void* user_data, ULView caller, ULString tooltip) {
    PHB_ITEM pItemView = itemFromView(caller);
    PHB_ITEM pCallback = hb_itemArrayGet(pItemView, ptr_bOnChangeTooltip);
    HB_SYMBOL_UNUSED(user_data);
    if(!HB_IS_EVALITEM( pCallback )) return;
    PHB_ITEM pTooltip = hb_itemPutStrLenU16( 0, HB_CDP_ENDIAN_NATIVE,ulStringGetData(tooltip), ulStringGetLength(tooltip) );
    hb_evalBlock(pCallback, pItemView, pTooltip, NULL );
    hb_itemRelease(pTooltip);
}

void hbOnChangeCursorCallback(void* user_data, ULView caller, ULCursor cursor) {
    PHB_ITEM pItemView = itemFromView(caller);    
    PHB_ITEM pCallback = hb_itemArrayGet(pItemView, ptr_bOnChangeCursor);
    HB_SYMBOL_UNUSED(user_data);
    if(!HB_IS_EVALITEM( pCallback )) return;
    hb_evalBlock(pCallback, pItemView, hb_itemPutNI(0, (int)cursor),  NULL );
}

void hbOnAddConsoleMessageCallback(void* user_data, ULView caller, ULMessageSource source, ULMessageLevel level,
    ULString message, unsigned int line_number,unsigned int column_number,ULString source_id) {
    PHB_ITEM pItemView = itemFromView(caller);
    PHB_ITEM pCallback = hb_itemArrayGet(pItemView, ptr_bOnAddConsoleMessage);
    HB_SYMBOL_UNUSED(user_data);
    if(!HB_IS_EVALITEM( pCallback )) return;
    PHB_ITEM pMessage = hb_itemPutStrLenU16( 0, HB_CDP_ENDIAN_NATIVE,ulStringGetData(message), ulStringGetLength(message) );
    PHB_ITEM pSourceId = hb_itemPutStrLenU16( 0, HB_CDP_ENDIAN_NATIVE,ulStringGetData(source_id), ulStringGetLength(source_id) );
    hb_evalBlock(pCallback, pItemView, hb_itemPutNI(0, (int)source), hb_itemPutNI(0, (int)level), 
        pMessage, hb_itemPutNI(0, (int)line_number), hb_itemPutNI(0, (int)column_number), 
        pSourceId, NULL );   
    hb_itemRelease(pMessage);
    hb_itemRelease(pSourceId);
}

void hbOnBeginLoadingCallback(void* user_data, ULView caller) {
    PHB_ITEM pItemView = itemFromView(caller);
    PHB_ITEM pCallback = hb_itemArrayGet(pItemView, ptr_bOnBeginLoading);
    HB_SYMBOL_UNUSED(user_data);
    if(!HB_IS_EVALITEM( pCallback )) return;
    hb_evalBlock(pCallback, pItemView, NULL );
}

void hbOnFinishLoadingCallback(void* user_data, ULView caller) {
    PHB_ITEM pItemView = itemFromView(caller);
    PHB_ITEM pCallback = hb_itemArrayGet(pItemView, ptr_bOnFinishLoading);
    HB_SYMBOL_UNUSED(user_data);
    if(!HB_IS_EVALITEM( pCallback )) return;
    hb_evalBlock(pCallback, pItemView, NULL );
}

void hbOnUpdateHistoryCallback(void* user_data, ULView caller) {
    PHB_ITEM pItemView = itemFromView(caller);
    PHB_ITEM pCallback = hb_itemArrayGet(pItemView, ptr_bOnUpdateHistory);
    HB_SYMBOL_UNUSED(user_data);
    if(!HB_IS_EVALITEM( pCallback )) return;
    hb_evalBlock(pCallback, pItemView, NULL );
}

void hbOnDOMReadyCallback(void* user_data, ULView caller) {
    PHB_ITEM pItemView = itemFromView(caller);
	//int type = HB_ITEM_TYPE(user_data);
	//int claId = hb_objGetClass(user_data);
    PHB_ITEM pCallback = hb_itemArrayGet(pItemView, ptr_bOnDOMReady);
	//type = HB_ITEM_TYPE(pCallback);

    HB_SYMBOL_UNUSED(user_data);
    if(!HB_IS_EVALITEM( pCallback )) return;
    hb_evalBlock(pCallback, pItemView, NULL );
}


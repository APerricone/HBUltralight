#include "ultralight_hb.h"
OBJDATA ultralight_app;

/*  CONSTRUCTOR Create()
    CONSTRUCTOR instance()
    METHOD window() SETGET
    //METHOD listener() SETGET
    //METHOD is_running() 
    METHOD main_monitor()
    METHOD renderer()
    METHOD run()
    //METHOD quit()
*/

void setupOBJDATA(const char* className,OBJDATA* dest)
{
	if(dest->classId == 0)
   	{
		dest->classId = hb_clsFindClass(className, NULL);
		dest->ptrObj = hb_clsGetVarIndex(dest->classId,hb_dynsymGet("pObj"));
   	}
}

void* hb_parvptr_obj(int n,int delta) {
	PHB_ITEM pItem = hb_param(n, HB_IT_OBJECT);
	if(pItem!=0)
		return hb_itemGetPtr( hb_itemArrayGet(pItem,delta) );
	return 0;
}

void hbOnUpdate(void* user_data);
PHB_ITEM appInstance;
HB_FUNC( ULTRALIGHT_APP_CREATE ) {
	PHB_ITEM pSelf;
	ULApp app = ulCreateApp(ulCreateConfig());
    setupOBJDATA("ULTRALIGHT_APP",&ultralight_app);
    hb_clsAssociate( ultralight_app.classId );
   	pSelf = hb_stackReturnItem();
    hb_itemArrayPut(pSelf, ultralight_app.ptrObj, hb_itemPutPtr(0, app)); 
    ulAppSetUpdateCallback(app,hbOnUpdate,0);
    appInstance = hb_itemNew(pSelf);
}

HB_FUNC( ULTRALIGHT_APP_INSTANCE ) {
    hb_itemCopy(hb_stackReturnItem(), appInstance);
}

void hbOnUpdate (void* user) {
    HB_SYMBOL_UNUSED(user);
    hb_idleState();
}

HB_FUNC_EXTERN( ULTRALIGHT_RENDERER );
HB_FUNC( ULTRALIGHT_APP_RENDERER ) {
    PHB_ITEM pRet;
    ULRenderer ren = ulAppGetRenderer(SELF_APP());
    HB_FUNC_EXEC(ULTRALIGHT_RENDERER);
    setupOBJDATA("ULTRALIGHT_RENDERER",&ultralight_renderer);
    hb_clsAssociate(  ultralight_renderer.classId );
    pRet = hb_stackReturnItem();
    hb_itemArrayPut(pRet, ultralight_renderer.ptrObj, hb_itemPutPtr(0, ren)); 
}

HB_FUNC_EXTERN( ULTRALIGHT_MONITOR );
HB_FUNC( ULTRALIGHT_APP_MAIN_MONITOR ) {
    PHB_ITEM pRet;
    ULMonitor mon = ulAppGetMainMonitor(SELF_APP());
    HB_FUNC_EXEC(ULTRALIGHT_MONITOR);
    setupOBJDATA("ULTRALIGHT_MONITOR",&ultralight_monitor);
    hb_clsAssociate(  ultralight_monitor.classId );
    pRet = hb_stackReturnItem();
    hb_itemArrayPut(pRet, ultralight_monitor.ptrObj, hb_itemPutPtr(0, mon)); 
}

HB_FUNC( ULTRALIGHT_APP_WINDOW ) {
    ULApp app = SELF_APP();
    ULWindow win;
    PHB_ITEM pRet;
    if(hb_pcount()>0) {
        ulAppSetWindow(app,PARAM_WINDOW(1));
        hb_ret();
    } else {
        win = ulAppGetWindow(app);
        hb_clsAssociate(  ultralight_window.classId );
        pRet = hb_stackReturnItem();
        hb_itemArrayPut(pRet, ultralight_window.ptrObj, hb_itemPutPtr(0, win)); 
    }
}

HB_FUNC( ULTRALIGHT_APP_RUN ) {
    ulAppRun(SELF_APP());
    hb_ret();
}

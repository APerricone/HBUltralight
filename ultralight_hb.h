#include <hbapi.h>
#include <hbapicls.h>
#include <hbapiitm.h>
#include <hbstack.h>
#include <hbapistr.h>

#include <windows.h>
#include <ultralight/CAPI.h>
#include <AppCore/CAPI.h>

typedef struct _OBJDATA
{
   HB_USHORT classId;
   HB_SIZE ptrObj;
} OBJDATA;
extern OBJDATA ultralight_app, ultralight_window,ultralight_monitor, ultralight_overlay, ultralight_view;

void setupOBJDATA(const char* className,OBJDATA* dest);

void* hb_parvptr_obj(int n,int delta);

#define SELF_APP()       (ULApp)    hb_itemGetPtr( hb_itemArrayGet(hb_stackSelfItem(),ultralight_app.ptrObj) )
#define SELF_WINDOW()    (ULWindow) hb_itemGetPtr( hb_itemArrayGet(hb_stackSelfItem(),ultralight_window.ptrObj) )
#define SELF_MONITOR()   (ULMonitor)hb_itemGetPtr( hb_itemArrayGet(hb_stackSelfItem(),ultralight_monitor.ptrObj) )
#define SELF_OVERLAY()   (ULOverlay)hb_itemGetPtr( hb_itemArrayGet(hb_stackSelfItem(),ultralight_overlay.ptrObj) )
#define SELF_VIEW()      (ULView)   hb_itemGetPtr( hb_itemArrayGet(hb_stackSelfItem(),ultralight_view.ptrObj) )
#define PARAM_WINDOW(n)  (ULWindow) hb_parvptr_obj(n, ultralight_window.ptrObj)
#define PARAM_MONITOR(n) (ULMonitor)hb_parvptr_obj(n, ultralight_monitor.ptrObj)
#define PARAM_OVERLAY(n) (ULOverlay)hb_parvptr_obj(n, ultralight_overlay.ptrObj)
#define PARAM_VIEW(n)    (ULView)   hb_parvptr_obj(n, ultralight_view.ptrObj)

void SetupView(PHB_ITEM pRet,ULView view);
#include <hbapi.h>
#include <hbapicls.h>
#include <hbapiitm.h>
#include <hbstack.h>
#include <hbapistr.h>
#include <hbapierr.h>
#include <hbvm.h>

#ifdef _WINDOWS_
#include <windows.h>
#endif
#include <Ultralight/CAPI.h>
#include <AppCore/CAPI.h>

typedef struct _OBJDATA
{
   HB_USHORT classId;
   HB_SIZE ptrObj;
} OBJDATA;
extern OBJDATA ultralight_app, ultralight_renderer, ultralight_window,ultralight_monitor, ultralight_overlay, ultralight_view, ultralight_bitmap;

void setupOBJDATA(const char* className,OBJDATA* dest);
void* hb_parvptr_obj(int n,HB_SIZE delta);

#define SELF_APP()       (ULApp)    hb_arrayGetPtr(hb_stackSelfItem(),ultralight_app.ptrObj)
#define SELF_RENDERER()  (ULRenderer)hb_arrayGetPtr(hb_stackSelfItem(),ultralight_renderer.ptrObj)
#define SELF_WINDOW()    (ULWindow) hb_arrayGetPtr(hb_stackSelfItem(),ultralight_window.ptrObj)
#define SELF_MONITOR()   (ULMonitor)hb_arrayGetPtr(hb_stackSelfItem(),ultralight_monitor.ptrObj)
#define SELF_OVERLAY()   (ULOverlay)hb_arrayGetPtr(hb_stackSelfItem(),ultralight_overlay.ptrObj)
#define SELF_VIEW()      (ULView)   hb_arrayGetPtr(hb_stackSelfItem(),ultralight_view.ptrObj)
#define SELF_BITMAP()    (ULBitmap) hb_arrayGetPtr(hb_stackSelfItem(),ultralight_bitmap.ptrObj)
#define PARAM_WINDOW(n)  (ULWindow) hb_parvptr_obj(n, ultralight_window.ptrObj)
#define PARAM_MONITOR(n) (ULMonitor)hb_parvptr_obj(n, ultralight_monitor.ptrObj)
#define PARAM_OVERLAY(n) (ULOverlay)hb_parvptr_obj(n, ultralight_overlay.ptrObj)
#define PARAM_VIEW(n)    (ULView)   hb_parvptr_obj(n, ultralight_view.ptrObj)

//void SetupView(PHB_ITEM pRet,ULView view);

void hb_retView(ULView);
void hb_retWindow(ULWindow);

HB_FUNC_EXTERN( ULTRALIGHT_GETHB );
HB_FUNC_EXTERN( ULTRALIGHT_SETHB );

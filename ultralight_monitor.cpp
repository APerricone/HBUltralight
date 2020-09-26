#include "ultralight_hb.h"
#include <AppCore/Monitor.h>

using namespace ultralight;
DEFINE_GETCLASSID(MONITOR)

/*
    ACCESS scale()
    ACCESS width()
    ACCESS height()
*/

HB_SIZE iObjIdxMonitor = 0;

HB_SIZE getObjIdxMonitor() {
    if(iObjIdxMonitor) return iObjIdxMonitor;
    iObjIdxMonitor = hb_clsGetVarIndex(getMONITORClassId(),hb_dynsymGet("pObj"));
    return iObjIdxMonitor;
}

void hb_retMonitor(Monitor* monitor) {
    hb_clsAssociate( getMONITORClassId() );
   	PHB_ITEM pSelf = hb_stackReturnItem();
    hb_arraySetPtr(pSelf, getObjIdxMonitor(), monitor);
}

Monitor* hb_parMonitor(int n) {
    PHB_ITEM pItem = hb_param(n, HB_IT_OBJECT);
    if(!pItem) return 0;
    return (Monitor*)hb_arrayGetPtr(pItem,getObjIdxMonitor());
}

HB_FUNC( ULTRALIGHT_MONITOR_WIDTH ) {
    PHB_ITEM pSelf = hb_stackSelfItem();
    if(!HB_IS_OBJECT(pSelf)) return;
    Monitor* monitor = (Monitor*)hb_arrayGetPtr(pSelf,getObjIdxMonitor());
    hb_retni(monitor->width());
}

HB_FUNC( ULTRALIGHT_MONITOR_HEIGHT ) {
    PHB_ITEM pSelf = hb_stackSelfItem();
    if(!HB_IS_OBJECT(pSelf)) return;
    Monitor* monitor = (Monitor*)hb_arrayGetPtr(pSelf,getObjIdxMonitor());
    hb_retni(monitor->height());
}

HB_FUNC( ULTRALIGHT_MONITOR_SCALE ) {
    PHB_ITEM pSelf = hb_stackSelfItem();
    if(!HB_IS_OBJECT(pSelf)) return;
    Monitor* monitor = (Monitor*)hb_arrayGetPtr(pSelf,getObjIdxMonitor());
    hb_retnd(monitor->scale());
}

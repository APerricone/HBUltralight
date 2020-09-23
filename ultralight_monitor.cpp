#include "ultralight_hb.h"
#include <AppCore/Monitor.h>

using namespace ultralight;
HB_USHORT monitorClassId = 0;

/*
    ACCESS scale()
    ACCESS width()
    ACCESS height()
*/

HB_FUNC_EXTERN(ULTRALIGHT_MONITOR);
HB_USHORT getMonitorClassId() {
    if(monitorClassId!=0) return monitorClassId;
	monitorClassId = hb_clsFindClass("ULTRALIGHT_MONITOR", NULL);
    if(monitorClassId!=0) return monitorClassId;
    HB_FUNC_EXEC(ULTRALIGHT_MONITOR);
    monitorClassId = hb_clsFindClass("ULTRALIGHT_MONITOR", NULL);
    return monitorClassId;
}

HB_FUNC( ULTRALIGHT_MONITOR_WIDTH ) {
    Monitor* monitor = (Monitor*)hb_selfUltralight();
    hb_retni(monitor->width());
}

HB_FUNC( ULTRALIGHT_MONITOR_HEIGHT ) {
    Monitor* monitor = (Monitor*)hb_selfUltralight();
    hb_retni(monitor->height());
}

HB_FUNC( ULTRALIGHT_MONITOR_SCALE ) {
    Monitor* monitor = (Monitor*)hb_selfUltralight();
    hb_retnd(monitor->scale());
}

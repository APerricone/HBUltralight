#include "ultralight_hb.h"
#include <AppCore/Monitor.h>

using namespace ultralight;
DEFINE_GETCLASSID(MONITOR)

/*
    ACCESS scale()
    ACCESS width()
    ACCESS height()
*/

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

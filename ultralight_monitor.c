#include "ultralight_hb.h"
OBJDATA ultralight_monitor;

/*
    ACCESS scale() 
    ACCESS width() 
    ACCESS height() 
*/

HB_FUNC( ULTRALIGHT_MONITOR_WIDTH ) {
    hb_retni(ulMonitorGetWidth(SELF_MONITOR()));
}

HB_FUNC( ULTRALIGHT_MONITOR_HEIGHT ) {
    hb_retni(ulMonitorGetHeight(SELF_MONITOR()));
}

HB_FUNC( ULTRALIGHT_MONITOR_SCALE ) {
    hb_retnd(ulMonitorGetScale(SELF_MONITOR()));
}

#include "ultralight_hb.h"
#include "ultralight/Session.h"

using namespace ultralight;

DEFINE_GETCLASSID(SESSION)

/*
  METHOD is_persistent()
  METHOD name()
  METHOD id()
  METHOD disk_path()
*/

HB_FUNC( ULTRALIGHT_SESSION_IS_PERSISTENT ) {
    Session* session = (Session*)hb_selfUltralight();
    hb_retl(session->is_persistent()? HB_TRUE : HB_FALSE);
}

HB_FUNC( ULTRALIGHT_SESSION_NAME ) {
    Session* session = (Session*)hb_selfUltralight();
    hb_retULString( session->name() );
}

HB_FUNC( ULTRALIGHT_SESSION_ID ) {
    Session* session = (Session*)hb_selfUltralight();
    // WARNING: signed / unsigned
    // why not hb_retnull ???
    hb_retnll(session->id());
}

HB_FUNC( ULTRALIGHT_SESSION_DISK_PATH ) {
    Session* session = (Session*)hb_selfUltralight();
    hb_retULString( session->disk_path() );
}


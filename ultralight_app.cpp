#include "ultralight_hb.h"
#include <AppCore/App.h>
#include <AppCore/window.h>

HB_USHORT appClassId = 0;

using namespace ultralight;

/*
    //DATA bOnUpdate
    CONSTRUCTOR Create()
    CONSTRUCTOR instance()
    //ACCESS settings()
    METHOD window() SETGET
    ACCESS is_running()
    ACCESS main_monitor()
    ACCESS renderer()
    METHOD Run()
    METHOD Quit()
*/

class HBAppListener : public AppListener {
    virtual void OnUpdate() {
        hb_idleState();
    }
} s_appListener;

HB_FUNC( ULTRALIGHT_APP_CREATE ) {
    if(appClassId==0) {
		appClassId = hb_clsFindClass("ULTRALIGHT_APP", NULL);
    }
	RefPtr<App> app = App::Create();
    initUltralightObj(app.get(),appClassId);
    app->set_listener(&s_appListener);
}

HB_FUNC( ULTRALIGHT_APP_INSTANCE ) {
    hb_retUltralight(App::instance(), appClassId);
}

FORWARD_GETCLASSID(WINDOW);
HB_FUNC( ULTRALIGHT_APP_WINDOW ) {
    App* app = (App*)hb_selfUltralight();
    if(hb_pcount()>0) {
        app->set_window(*((Window*)hb_parUltralight(1)));
        hb_ret();
    } else {
        Window* wnd = app->window().get();
        hb_retUltralight((RefCounted*)wnd,getWINDOWClassId());
    }
}


FORWARD_GETCLASSID(RENDERER);
HB_FUNC( ULTRALIGHT_APP_RENDERER ) {
    App* app = (App*)hb_selfUltralight();
    hb_retUltralight(app->renderer().ptr(),getRENDERERClassId());
}

FORWARD_GETCLASSID(MONITOR);
void hb_retMonitor(Monitor*);
HB_FUNC( ULTRALIGHT_APP_MAIN_MONITOR ) {
    App* app = (App*)hb_selfUltralight();
    hb_retMonitor(app->main_monitor());
}

HB_FUNC( ULTRALIGHT_APP_RUN ) {
    App* app = (App*)hb_selfUltralight();
    app->Run();
    hb_ret();
}

HB_FUNC( ULTRALIGHT_APP_IS_RUNNING ) {
    App* app = (App*)hb_selfUltralight();
    hb_retl(app->is_running());
}

HB_FUNC( ULTRALIGHT_APP_QUIT ) {
    App* app = (App*)hb_selfUltralight();
    app->Quit();
    hb_ret();
}

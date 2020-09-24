#include "ultralight_hb.h"
#include "ultralight/Renderer.h"

using namespace ultralight;

DEFINE_GETCLASSID(RENDERER)

/*
    CONSTRUCTOR Create()
    METHOD CreateSession(is_persistent,name)
    METHOD default_session();
    METHOD CreateView(nWidth,nHeight,lransparent,session)
    METHOD Update()
    METHOD Render()
    METHOD PurgeMemory()
    METHOD  LogMemoryUsage();

*/

HB_FUNC( ULTRALIGHT_RENDERER_CREATE ) {
    initUltralightObj((Renderer::Create()).ptr(), getRENDERERClassId());
}

FORWARD_GETCLASSID(SESSION);
HB_FUNC( ULTRALIGHT_RENDERER_CREATESESSION ) { //(is_persistent,name)
    Renderer* renderer = (Renderer*)hb_selfUltralight();
    hb_retUltralight((renderer->CreateSession(hb_parnl(1), hb_parc(2))).ptr(), getSESSIONClassId());
}

HB_FUNC( ULTRALIGHT_RENDERER_DEFAULT_SESSION ) {
    Renderer* renderer = (Renderer*)hb_selfUltralight();
    hb_retUltralight((renderer->default_session()).ptr(), getSESSIONClassId());
}

FORWARD_GETCLASSID(VIEW);
HB_FUNC( ULTRALIGHT_RENDERER_CREATEVIEW ) { //(nWidth,nHeight,lransparent)
    Renderer* renderer = (Renderer*)hb_selfUltralight();
    RefPtr<View> view = (renderer->CreateView(hb_parni(1), hb_parni(2), hb_parl(3), nullptr));
    hb_retUltralight(view.get(), getVIEWClassId());
}

HB_FUNC( ULTRALIGHT_RENDERER_UPDATE ) { //()
    Renderer* renderer = (Renderer*)hb_selfUltralight();
    renderer->Update();
    hb_idleState();
}

HB_FUNC( ULTRALIGHT_RENDERER_RENDER ) { //()
    Renderer* renderer = (Renderer*)hb_selfUltralight();
    renderer->Render();
}
HB_FUNC( ULTRALIGHT_RENDERER_PURGEMEMORY ) { //()
    Renderer* renderer = (Renderer*)hb_selfUltralight();
    renderer->PurgeMemory();
}

HB_FUNC( ULTRALIGHT_RENDERER_LOGMEMORYUSAGE ) { //()
    Renderer* renderer = (Renderer*)hb_selfUltralight();
    renderer->LogMemoryUsage();
}

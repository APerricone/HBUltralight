#include "ultralight_hb.h"
OBJDATA ultralight_renderer;

/*
    CONSTRUCTOR Create()
    METHOD CreateView(nWidth,nHeight,lransparent)
    METHOD Update()
    METHOD Render()
    //METHOD PurgeMemory()
*/

HB_FUNC( ULTRALIGHT_RENDERER_CREATE ) {
	PHB_ITEM pSelf;
	ULRenderer renderer = ulCreateRenderer(ulCreateConfig());
    setupOBJDATA("ULTRALIGHT_RENDERER",&ultralight_renderer);
    hb_clsAssociate( ultralight_renderer.classId );
   	pSelf = hb_stackReturnItem();
    hb_itemArrayPut(pSelf, ultralight_renderer.ptrObj, hb_itemPutPtr(0, renderer)); 
}

HB_FUNC( ULTRALIGHT_RENDERER_CREATEVIEW ) { //(nWidth,nHeight,lransparent)
    ULView view = ulCreateView(SELF_RENDERER(), hb_parni(1), hb_parni(2), hb_parl(3));
    hb_retView(view);
}

HB_FUNC( ULTRALIGHT_RENDERER_UPDATE ) { //()
    ulUpdate(SELF_RENDERER());
    hb_idleState();
}

HB_FUNC( ULTRALIGHT_RENDERER_RENDER ) { //()
    ulRender(SELF_RENDERER());
}
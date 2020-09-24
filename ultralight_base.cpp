#include "ultralight_hb.h"
#include <Ultralight/RefPtr.h>

namespace ultralight {
    class Window;
}
using namespace ultralight;

DEFINE_GETCLASSID(REFCOUNTED);
HB_SIZE iObjIdx = 0;

HB_SIZE getObjIdx() {
    if(iObjIdx) return iObjIdx;
    iObjIdx = hb_clsGetVarIndex(getREFCOUNTEDClassId(),hb_dynsymGet("pObj"));
    return iObjIdx;
}

void initUltralightObj(RefCounted* refCnt, HB_USHORT classId) {
    hb_clsAssociate(classId);
    PHB_ITEM pSelf = hb_stackSelfItem();
    hb_arraySetPtr(pSelf, getObjIdx(), refCnt);
    refCnt->AddRef();
}

HB_FUNC( ULTRALIGHT_REFCOUNTED_DELETE ) {
    PHB_ITEM pSelf = hb_stackSelfItem();
    RefCounted* refCnt = (RefCounted*)hb_arrayGetPtr(pSelf,getObjIdx());
    refCnt->Release();
}

RefCounted* hb_selfUltralight(PHB_ITEM pSelf) {
    if(!pSelf) pSelf = hb_stackSelfItem();
    return (RefCounted*)hb_arrayGetPtr(pSelf,getObjIdx());
}

RefCounted* hb_parUltralight(int n) {
	PHB_ITEM pItem = hb_param(n, HB_IT_OBJECT);
	if(pItem!=0) return (RefCounted*)hb_selfUltralight(pItem);
	return 0;
}

FORWARD_GETCLASSID(WINDOW);
void hb_retWindow(ultralight::Window* pObj);

void hb_retUltralight(RefCounted* refCnt, HB_USHORT classId) {
    // classes witch callback are special
    if(classId==getWINDOWClassId()) {
        hb_retWindow((Window*)refCnt);
        return;
    }
    hb_clsAssociate( classId );
   	PHB_ITEM pSelf = hb_stackReturnItem();
    hb_arraySetPtr(pSelf, iObjIdx, refCnt);
    refCnt->AddRef();
}

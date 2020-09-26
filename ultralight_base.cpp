#include "ultralight_hb.h"
#include <Ultralight/RefPtr.h>
#include <Ultralight/String.h>
#include <Ultralight/String16.h>

namespace ultralight {
    class Window;
    class View;
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
    if(!HB_IS_OBJECT(pSelf)) return 0;
    return (RefCounted*)hb_arrayGetPtr(pSelf,getObjIdx());
}

RefCounted* hb_parUltralight(int n) {
	PHB_ITEM pItem = hb_param(n, HB_IT_OBJECT);
	if(pItem!=0) return (RefCounted*)hb_selfUltralight(pItem);
	return 0;
}

FORWARD_GETCLASSID(WINDOW);
void hb_retWindow(ultralight::Window* pObj);
FORWARD_GETCLASSID(VIEW);
void hb_retView(ultralight::View* pObj);

void hb_retUltralight(RefCounted* refCnt, HB_USHORT classId) {
    // classes with callback are special
    if(classId==getWINDOWClassId()) {
        hb_retWindow((Window*)refCnt);
        return;
    }
    if(classId==getVIEWClassId()) {
        hb_retView((View*)refCnt);
        return;
    }
    hb_clsAssociate( classId );
   	PHB_ITEM pSelf = hb_stackReturnItem();
    hb_arraySetPtr(pSelf, iObjIdx, refCnt);
    refCnt->AddRef();
}


PHB_ITEM hb_itemPutULString(PHB_ITEM pItem,const ultralight::String& str) {
    const String16& str16 = str.utf16();
    return hb_itemPutStrLenU16(pItem, HB_CDP_ENDIAN_NATIVE,
        str16.data(), str16.length());
}

void hb_retULString(const ultralight::String& str) {
    hb_itemPutULString(hb_stackReturnItem(), str);
}

#include "ultralight_hb.h"
#include <Ultralight/RefPtr.h>

using namespace ultralight;

HB_USHORT refCountedClassId = 0;
HB_SIZE iObjIdx;

struct ultralight_cnt {
    RefCounted* ptr;
};

static HB_GARBAGE_FUNC( HBUltralightDestroy )
{
    ultralight_cnt * pStatePtr = ( ultralight_cnt * ) Cargo;

    if( pStatePtr->ptr )
    {
        if(pStatePtr->ptr->ref_count()==0) {
            hb_vmPushSymbol( hb_dynsymFindSymbol("ULTRALIGHT_DELHB") );
            hb_vmPushNil();
            hb_vmPushPointer(Cargo);
            hb_vmDo( 1 );
        }
        pStatePtr->ptr->Release();
        pStatePtr->ptr = 0;
    }
}

static const HB_GC_FUNCS s_gcHBULFuncs =
{
   HBUltralightDestroy,
   hb_gcDummyMark
};

HB_FUNC_EXTERN(ULTRALIGHT_REFCOUNTED);
void putHBUltralight(PHB_ITEM pSelf,RefCounted* refCnt ) {
	if(refCountedClassId == 0)
   	{
		refCountedClassId = hb_clsFindClass("ULTRALIGHT_REFCOUNTED", NULL);
	    if(refCountedClassId==0) {
		    HB_FUNC_EXEC(ULTRALIGHT_REFCOUNTED);
		    refCountedClassId = hb_clsFindClass("ULTRALIGHT_REFCOUNTED", NULL);
	    }
		iObjIdx = hb_clsGetVarIndex(refCountedClassId,hb_dynsymGet("pObj"));
   	}
    hb_vmPushSymbol( hb_dynsymFindSymbol("ULTRALIGHT_GETHB") );
    hb_vmPushNil();
    hb_vmPushPointer(refCnt);
    hb_vmDo( 1 );
    PHB_ITEM ppItem = hb_stackReturnItem();
    if(HB_IS_NIL(ppItem)) {
        ultralight_cnt* pCnt = (ultralight_cnt*) hb_gcAllocate( sizeof( ultralight_cnt ), &s_gcHBULFuncs );
        pCnt->ptr = refCnt;
        ppItem = hb_itemPutPtrGC( NULL, ( void * ) pCnt);
    }
    hb_itemArrayPut(pSelf, iObjIdx, ppItem);
}

RefCounted* hb_parUltralight(int n) {
	PHB_ITEM pItem = hb_param(n, HB_IT_OBJECT);
	if(pItem!=0)
		return (RefCounted*)hb_selfUltralight(pItem);
	return 0;
}

void hb_retUltralight(RefCounted* pObj, HB_USHORT classId) {
    hb_vmPushSymbol( hb_dynsymFindSymbol("ULTRALIGHT_GETHB") );
    hb_vmPushNil();
    hb_vmPushPointer(pObj);
    hb_vmDo( 1 );
    //HB_FUNC_EXEC(ULTRALIGHT_GETHB);
    if(hb_itemType(hb_stackReturnItem())!=HB_IT_NIL || classId == 0)
        return;
    hb_clsAssociate( classId );
   	PHB_ITEM pSelf = hb_stackReturnItem();
    putHBUltralight(pSelf,pObj);
}

RefCounted* hb_selfUltralight(PHB_ITEM pSelf) {
    if(!pSelf) pSelf = hb_stackSelfItem();
    if(refCountedClassId == 0) {
        return 0;
    }
    ultralight_cnt* pCnt = (ultralight_cnt*) hb_arrayGetPtrGC(pSelf, iObjIdx, &s_gcHBULFuncs);
    if(!pCnt) return 0;
    // add ref?
    return pCnt->ptr;
}

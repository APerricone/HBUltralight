#pragma once

#ifdef _MSC_VER
#pragma warning( disable : 4100 4201 )
#endif

#include <hbapi.h>
#include <hbapicls.h>
#include <hbapiitm.h>
#include <hbstack.h>
#include <hbapistr.h>
#include <hbapierr.h>
#include <hbvm.h>

#ifdef _WINDOWS_
#include <windows.h>
#endif

namespace ultralight {
    class RefCounted;
    class String;
}


// called from constructors methods
void initUltralightObj(ultralight::RefCounted* refCnt, HB_USHORT classId );
// get ultralight from an item or form self
ultralight::RefCounted* hb_selfUltralight(PHB_ITEM pSelf=0);
// get ultralight parameter
ultralight::RefCounted* hb_parUltralight(int n);
// create an item from an utralight object
PHB_ITEM hb_itemUltralight(ultralight::RefCounted* refCnt, HB_USHORT classId);
// Returns a ultralight object
void hb_retUltralight(ultralight::RefCounted* pObj,HB_USHORT classId);

PHB_ITEM hb_itemPutULString(PHB_ITEM pItem,const ultralight::String& str);
void hb_retULString(const ultralight::String& str);

#define FORWARD_GETCLASSID(objName) HB_USHORT get ## objName ## _ClassId();
#define GETCLASSID(objName) get ## objName ## _ClassId()

#define DEFINE_GETCLASSID(objName) \
    HB_FUNC_EXTERN(ULTRALIGHT_ ## objName); \
    HB_USHORT objName ## _ClassId = 0; \
    HB_USHORT get ## objName ## _ClassId() { \
        if(objName ## _ClassId) { return objName ## _ClassId; } \
	    objName ## _ClassId = hb_clsFindClass("ULTRALIGHT_" # objName, NULL); \
        if(objName ## _ClassId) { return objName ## _ClassId; } \
        HB_FUNC_EXEC(ULTRALIGHT_ ## objName); \
        objName ## _ClassId = hb_clsFindClass("ULTRALIGHT_"  # objName, NULL); \
        return objName ## _ClassId; \
    }

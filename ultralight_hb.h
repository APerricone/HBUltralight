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
//#include <Ultralight/CAPI.h>

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
// Returns a ultralight object
void hb_retUltralight(ultralight::RefCounted* pObj,HB_USHORT classId);

PHB_ITEM hb_itemPutULString(PHB_ITEM pItem,const ultralight::String& str);
void hb_retULString(const ultralight::String& str);

#define FORWARD_GETCLASSID(objName) HB_USHORT get ## objName ## ClassId();

#define DEFINE_GETCLASSID(objName) \
    HB_FUNC_EXTERN(ULTRALIGHT_ ## objName); \
    HB_USHORT objName ## ClassId = 0; \
    HB_USHORT get ## objName ## ClassId() { \
        if(objName ## ClassId) return objName ## ClassId; \
	    objName ## ClassId = hb_clsFindClass("ULTRALIGHT_ ## objName", NULL); \
        if(objName ## ClassId) return objName ## ClassId; \
        HB_FUNC_EXEC(ULTRALIGHT_ ## objName); \
        objName ## ClassId = hb_clsFindClass("ULTRALIGHT_ ## objName", NULL); \
        return objName ## ClassId; \
    }

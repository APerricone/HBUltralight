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
}


ultralight::RefCounted* hb_selfUltralight(PHB_ITEM pItem=0);
// put an ultralight pointer in a harbour class
void putHBUltralight(PHB_ITEM pSelf,ultralight::RefCounted* refCnt );
// get the pointer of ultralight
ultralight::RefCounted* hb_parUltralight(int n);
// return true if the stack return is setted, otherwise a new class must be created
void hb_retUltralight(ultralight::RefCounted* pObj,HB_USHORT classId=0);

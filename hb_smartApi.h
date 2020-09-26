#pragma once

#include "ultralight_hb.h"
#include <hbapiitm.h>
#include <Ultralight/Geometry.h>

namespace ultralight {
    class RefCounted;
    class String;
}

// C++ class to keep an item and release it
class SmartItem {
private:
    PHB_ITEM pItem;
public:
    ~SmartItem() {
        hb_itemRelease(pItem);
    }
    SmartItem(ultralight::RefCounted* prt,HB_USHORT classId) : pItem(hb_itemUltralight(prt, classId)) {};
    SmartItem(const ultralight::String& str) : pItem(hb_itemPutULString(0,str)) {};
    SmartItem(int v) : pItem(hb_itemPutNI(0,v)) {};
    SmartItem(bool v) : pItem(hb_itemPutL(0,v? HB_TRUE : HB_FALSE)) {};
    SmartItem(const ultralight::IntRect& rect) {
        pItem = hb_itemArrayNew(4); //{left, top, right, bottom}
        hb_arraySetNI(pItem,1, rect.left);
        hb_arraySetNI(pItem,2, rect.top);
        hb_arraySetNI(pItem,3, rect.right);
        hb_arraySetNI(pItem,4, rect.bottom);
    }
    SmartItem(HB_LONGLONG v) : pItem(hb_itemPutNLL(0,v)) {};

    operator PHB_ITEM() const { return pItem; }
};

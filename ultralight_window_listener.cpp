#include "ultralight_hb.h"
#include <AppCore/Window.h>

using namespace ultralight;
FORWARD_GETCLASSID(WINDOW)

HB_SIZE ibOnCloseIdx = 0;
HB_SIZE ibOnResizeIdx = 0;
class HBWindowListener : public WindowListener {
private:
    PHB_ITEM pItem;
public:
    HBWindowListener(PHB_ITEM pItem) : pItem(pItem) { }

    PHB_ITEM getItem() const { return pItem; }

    void OnClose() {
        if(ibOnCloseIdx==0) {
            ibOnCloseIdx = hb_clsGetVarIndex(getWINDOWClassId(),hb_dynsymGet("bOnClose"));
        }
        PHB_ITEM pCallback = hb_itemArrayGet(pItem, ibOnCloseIdx);
        if(!HB_IS_EVALITEM( pCallback )) return;
        hb_evalBlock(pCallback, pItem, NULL );
    }

    void OnResize(uint32_t width, uint32_t height) {
        if(ibOnResizeIdx==0) {
            ibOnResizeIdx = hb_clsGetVarIndex(getWINDOWClassId(),hb_dynsymGet("bOnResize"));
        }
        PHB_ITEM pCallback = hb_itemArrayGet(pItem, ibOnResizeIdx);
        if(!HB_IS_EVALITEM( pCallback )) return;
        PHB_ITEM pWidth = hb_itemPutNI(0,width);
        PHB_ITEM pHeight = hb_itemPutNI(0,height);
        hb_evalBlock(pCallback, pItem, pWidth, pHeight, NULL );
        hb_itemRelease(pWidth);
        hb_itemRelease(pHeight);
    }

};

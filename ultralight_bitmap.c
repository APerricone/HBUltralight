#include "ultralight_hb.h"
OBJDATA ultralight_bitmap;

/*
    //ACCESS scale() 
    //ACCESS width() 
    //ACCESS height() 
*/

HB_FUNC( ULTRALIGHT_BITMAP_WRITEPNG ) {
	ULBitmap bitmap = SELF_BITMAP();
    ulBitmapWritePNG(bitmap,hb_parc(1));
    hb_ret();
}

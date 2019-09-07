#include "ultralight.ch"

proc main()
    LOCAL app := ultralight_app():Create()
    LOCAL window := ultralight_window():Create(app:main_Monitor,300,300,.F.,;
        hb_bitOr(ulWindowFlags_Titled,ulWindowFlags_Resizable))
    LOCAL overlay 
    window:SetTitle("Tutorial 3 - Resize Me!")
    app:window := window
    overlay:=ultralight_overlay():Create(window,window:width(),window:height(),0,0)
    overlay:view():LoadHTML("<center>Hello World!</center>")
    window:bOnResize:={|wnd,w,h| HB_SYMBOL_UNUSED(wnd), overlay:Resize(w,h) }
    app:Run()

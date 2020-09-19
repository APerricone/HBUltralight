#include <hbclass.ch>
#include "ultralight.ch"

proc main()
    LOCAL app := ultralight_app():Create()
    LOCAL window := ultralight_window():Create(app:main_Monitor,300,300,.F.,ulWindowFlags_Titled)
    LOCAL overlay
    window:SetTitle("Basic App")
    app:window := window
    overlay:=ultralight_overlay():Create(window,window:width(),window:height(),0,0)
    overlay:view():LoadHTML("<center>Hello World!</center>")
    app:Run()

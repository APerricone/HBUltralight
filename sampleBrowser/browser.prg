#include <hbclass.ch>
#include "ultralight.ch"

// main.cpp
proc main()
    LOCAL b := Browser():New()
    b:Run()

//browser.h
class Browser 
    DATA app, window, ui

    CONSTRUCTOR New()
    METHOD Run() INLINE ::app:run()
endclass

//browser.cpp
METHOD New() CLASS Browser
    ::app := ultralight_app():Create()
    ::window := ultralight_window():Create(::app:main_Monitor,1024,768,.F.,;
        ulWindowFlags_Resizable + ulWindowFlags_Titled + ulWindowFlags_Maximizable )
    ::window:SetTitle("HBUltralight Sample - Browser")
    ::app:window := ::window
    ::ui := ui():New(::window)
return self

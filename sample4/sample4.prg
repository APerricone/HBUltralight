#include "ultralight.ch"

proc main()
    LOCAL app := ultralight_app():Create()
    LOCAL window := ultralight_window():Create(app:main_Monitor,300,300,.F.,ulWindowFlags_Titled)
    LOCAL overlay, cHTML, v
    window:SetTile("Tutorial 4 - JavaScript")
    app:window := window
    overlay:=ultralight_overlay():Create(window,window:width(),window:height(),0,0)
    v := overlay:view()
    v:bOnDOMReady = {|caller| OnDOMReady(caller) }
    #pragma __text |    cHTML+=%s+e"\r\n" |    v:LoadHTML(cHTML) |    cHtml:=""
  <html>
    <head>
      <style type="text/css">
        body { font-family: Arial; text-align: center; }
      </style>
    </head>
    <body>
      <button onclick="GetMessage();">Get the Secret Message!</button>
      <button onclick="GetMessage2();">Get the OTHER secret message!</button>
      <div id="message"></div>
    </body>
  </html>
    #pragma __endtext
    app:Run()

///
/// Our native JavaScript callback. This function will be called from
/// JavaScript by calling GetMessage(). We bind the callback within
/// the DOMReady callback defined below.
///
proc GetMessage(/*this,args*/)
    JSEval("document.getElementById('message').innerHTML='Ultralight rocks!';")

proc GetMessage2(/*this,args*/)
    JSEval("document.getElementById('message').innerHTML='Harbour rocks too!';")
  
proc OnDOMReady(caller)
    LOCAL global
    ///
    /// Set our View's JSContext as the one to use in subsequent JSHelper calls
    ///
    SetJSContext(caller:js_context())
    global := JSGlobalObject()
    // they are both ok
    global["GetMessage"] := @GetMessage()
    //global["GetMessage"] := {|this,args| GetMessage(this,args)})
    global["GetMessage2"] := @GetMessage2()

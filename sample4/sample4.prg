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
    #pragma __text | cHTML+=%s | v:LoadHTML(cHTML) | cHtml:=""
  <html>
    <head>
      <style type="text/css">
        body { font-family: Arial; text-align: center; }
      </style>
    </head>
    <body>
      <button onclick="GetMessage();">Get the Secret Message!</button>
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

proc Console(caller,nSource,nLevel,cMessage,nLine,nColumn,cSource)
  HB_SYMBOL_UNUSED(caller)
  switch nSource
    case ulMessageSource_JS
      ? "JS"
      exit
    otherwise
      ? nSource
  endswitch
  ?? " "
  switch nLevel
    case ulMessageLevel_Log
      ?? " Log"
      exit
    case ulMessageLevel_Warning
      ?? " Warning"
      exit
    case ulMessageLevel_Error
      ?? " Error"
      exit
    case ulMessageLevel_Debug
      ?? " Debug"
      exit    
    case ulMessageLevel_Info
      ?? " Info"
      exit
  endswitch
  ?? ":"+cMessage+" - "+cSource+"("+alltrim(str(nLine))+":"+alltrim(str(nColumn))+")"  

proc OnDOMReady(caller)
    LOCAL global
    ///
    /// Set our View's JSContext as the one to use in subsequent JSHelper calls
    ///
    SetJSContext(caller:js_context())
    global := JSGlobalObject()
    // they are all ok
    global["GetMessage"] := @GetMessage()
    //global["GetMessage"] := {|this,args| GetMessage(this,args)})
    //
    caller:bOnAddConsoleMessage = @Console()

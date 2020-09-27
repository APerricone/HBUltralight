#include "ultralight.ch"

///
///  Welcome to Sample 4!
///
///  In this sample we'll show how to integrate C++ code with JavaScript.
///
///  We will introduce the DOMReady event and use it to bind a C++ callback to
///  a JavaScript function on our page. Later, when that callback is triggered,
///  we will execute some JavaScript to update a hidden message on the page.
///
///  __JavaScriptCore and AppCore__
///
///  Ultralight integrates the full JavaScriptCore engine (the same JS engine
///  in Safari on macOS and iOS). The SDK includes full C API bindings to
///  JavaScriptCore which gives you a great deal of flexibility but can be a
///  little daunting for first-time users.
///
///  To help simplify things, AppCore provides a simple C++ wrapper over
///  JavaScriptCore (@see JSHelpers.h). We'll be using this wrapper for the
///  sake of convenience in this sample.
///
proc main()
    LOCAL overlay
    ///
    /// Create our main App instance.
    ///
    LOCAL app := ultralight_app():Create()
    ///
    /// Create our Window using default window flags.
    ///
    LOCAL window := ultralight_window():Create(app:main_monitor, 300, 300, .T., ulWindowFlags_Titled)
    ///
    /// Set our window title.
    ///
    window:title := "Ultralight Sample 4 - JavaScript"

    ///
    /// Bind our App's main window.
    ///
    /// @note This MUST be done before creating any overlays or calling App::Run
    ///
    app:window := window

    ///
    /// Create an Overlay using the same dimensions as our Window.
    ///
    overlay := ultralight_overlay():Create(window, window:width, window:height, 0, 0)

    ///
    /// Register our MyApp instance as a load listener so we can handle the
    /// View's OnDOMReady event below.
    ///
    overlay:view:bOnDOMReady := @OnDOMReady()
    //overlay_:view()->set_load_listener(this);

    ///
    /// Load a string of HTML (we're using a C++11 Raw String Literal)
    ///
    overlay:view:LoadHTML(htmlString())


    ///
    /// Run our main loop.
    ///
    app:Run()
return

proc OnDOMReady(view,frame_id,is_main_frame,url)
    LOCAL context, global
    HB_SYMBOL_UNUSED(frame_id)
    HB_SYMBOL_UNUSED(is_main_frame)
    HB_SYMBOL_UNUSED(url)
    ///
    /// Set our View's JSContext as the one to use in subsequent JSHelper calls
    ///
    context := view:LockJSContext()
    SetJSContext(context);

    ///
    /// Get the global object (this would be the "window" object in JS)
    ///
    global := JSGlobalObject()

    ///
    /// Bind MyApp::GetMessage to the JavaScript function named "GetMessage".
    ///
    /// You can get/set properties of JSObjects by using the [] operator with
    /// the following types as potential property values:
    ///  - JSValue
    ///      Represents a JavaScript value, eg String, Object, Function, etc.
    ///  - JSCallback
    ///      Typedef of std::function<void(const JSObject&, const JSArgs&)>)
    ///  - JSCallbackWithRetval
    ///      Typedef of std::function<JSValue(const JSObject&, const JSArgs&)>)
    ///
    /// We use the BindJSCallbackWithRetval macro to bind our C++ class member
    /// function to our JavaScript callback.
    ///
    global["GetMessage"] = @GetMessage()
return


///
/// Our native JavaScript callback. This function will be called from
/// JavaScript by calling GetMessage(). We bind the callback within
/// the DOMReady callback defined below.
///
func GetMessage(/*this,args*/)
///
/// Return our message to JavaScript as a JSValue.
///
return "Hello from C++!<br/>Ultralight rocks!"

func htmlString()
    local v
#pragma __text|v+=%s+hb_eol()|return v|v:=""
    <html>
    <head>
      <style type="text/css">
        * { -webkit-user-select: none; }
        body {
          font-family: -apple-system, 'Segoe UI', Ubuntu, Arial, sans-serif;
          text-align: center;
          background: linear-gradient(#FFF, #DDD);
          padding: 2em;
        }
        body.rainbow {
          background: linear-gradient(90deg, #ff2363, #fff175, #68ff9d,
                                             #45dce0, #6c6eff, #9e23ff, #ff3091);
          background-size: 1000% 1000%;
          animation: ScrollGradient 10s ease infinite;
        }
        @keyframes ScrollGradient {
          0%   { background-position:0% 50%; }
          50%  { background-position:100% 50%; }
          100% { background-position:0% 50%; }
        }
        #message {
          padding-top: 2em;
          color: white;
          font-weight: bold;
          font-size: 24px;
          text-shadow: 1px 1px rgba(0, 0, 0, 0.4);
        }
      </style>
      <script type="text/javascript">
      function HandleButton(evt) {
        // Call our C++ callback 'GetMessage'
        var message = GetMessage();

        // Display the result in our 'message' div element and apply the
        // rainbow effect to our document's body.
        document.getElementById('message').innerHTML = message;
        document.body.classList.add('rainbow');
      }
      </script>
    </head>
    <body>
      <button onclick="HandleButton(event);">Get the Secret Message!</button>
      <div id="message"></div>
    </body>
  </html>
ENDTEXT
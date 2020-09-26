#include "ultralight.ch"

///
///  Welcome to Sample 1!
///
///  In this sample we'll load a string of HTML and render it to a PNG.
///
///  Since we're rendering offscreen and don't need to create any windows or
///  handle any user input, we won't be using App::Create() and will instead be using
///  the Ultralight API directly with our own custom main loop.
///
///  Our main loop waits for the page to finish loading by subscribing to the
///  LoadListener interface then writes the rendering surface to a PNG on disk.
///

proc main()
    LOCAL renderer_, view_
    LOCAL done_ := .F.
    LOCAL bitmap_surface, bitmap
    ///
    /// Setup our config. The config can be used to customize various
    /// options within the renderer and WebCore module.
    ///
    /// Our config uses 2x DPI scale and "Arial" as the default font.
    ///
    LOCAL config := {=>}
    config:device_scale := 2.0
    config:font_family_standard := "Arial"

    ///
    /// We need to tell config where our resources are so it can load our
    /// bundled certificate chain and make HTTPS requests.
    ///
    config:resource_path := "./resources/"

    ///
    /// Make sure the GPU renderer is disabled so we can render to an offscreen
    /// pixel buffer surface.
    ///
    config:use_gpu_renderer := .F.

    ///
    /// Pass our configuration to the Platform singleton so that the library
    /// can use it.
    ///
    /// The Platform singleton can be used to define various platform-specific
    /// properties and handlers such as file system access, font loaders, and
    /// the gpu driver.
    ///
    ultralight_Platform():instance():set_config(config)

    ///
    /// Use AppCore's font loader singleton to load fonts from the OS.
    ///
    /// You could replace this with your own to provide your own fonts.
    ///
    ultralight_Platform():instance():set_font_loader(GetPlatformFontLoader())

    ///
    /// Use AppCore's file system singleton to load file:/// URLs from the OS.
    ///
    /// You could replace this with your own to provide your own file loader
    /// (useful if you need to bundle encrypted / compressed HTML assets).
    ///
    ultralight_Platform():instance():set_file_system(GetPlatformFileSystem("."))

    ///
    /// Register our MyApp instance as a logger so we can handle the
    /// library's LogMessage() event below in case we encounter an error.
    ///
    ultralight_Platform():instance():logger := {|level,msg| QOut(msg) }

    ///
    /// Create our Renderer (you should only create this once per application).
    ///
    /// The Renderer singleton maintains the lifetime of the library and
    /// is required before creating any Views. It should outlive any Views.
    ///
    /// You should set up the Platform singleton before creating this.
    ///
    renderer_ = ultralight_Renderer():Create()

    ///
    /// Create our View.
    ///
    /// Views are sized containers for loading and displaying web content.
    ///
    view_ = renderer_:CreateView(1600, 1600, .F., nil)

    ///
    /// Register our MyApp instance as a load listener so we can handle the
    /// View's OnFinishLoading event below.
    ///
    view_:bOnFinishLoading := {|caller,frame_id,is_main_frame,url|
        if is_main_frame
            QOut("Our page has loaded!")
            done_:=.T.
        endif
        return nil
    }

    ///
    /// Load a string of HTML into our View. (For code readability, the string
    /// is defined in the htmlString() function at the bottom of this file)
    ///
    /// **Note**:
    ///   This operation may not complete immediately-- we will call
    ///   Renderer::Update continuously and wait for the OnFinishLoading event
    ///   before rendering our View.
    ///
    /// Views can also load remote URLs, try replacing the code below with:
    ///
    ///    view_:LoadURL("https://en.wikipedia.org")
    ///
    view_:LoadHTML(htmlString())

    QOut("Starting Run(), waiting for page to load...")
    ///
    /// Continuously update our Renderer until are done flag is set to true.
    ///
    /// **Note**:
    ///   Calling Renderer::Update handles any pending network requests,
    ///   resource loads, and JavaScript timers.
    ///
    while .not. done_
        hb_idleSleep(10)
        renderer_:Update()
        renderer_:Render()
    enddo

    ///
    /// Get our View's rendering surface and cast it to BitmapSurface.
    ///
    /// BitmapSurface is the default Surface implementation, you can provide
    /// your own via Platform::set_surface_factory.
    ///
    bitmap_surface := view_:surface()

    ///
    /// Get the underlying bitmap.
    ///
    bitmap = bitmap_surface:bitmap()

    ///
    /// The renderer uses a BGRA pixel format but PNG expects RGBA format,
    /// let's convert the format by swapping Red and Blue channels.
    ///
    bitmap:SwapRedBlueChannels()

    ///
    /// Write our bitmap to a PNG in the current working directory.
    ///
    bitmap:WritePNG("result.png")

    QOut("Saved a render of our page to result.png.")
    QOut("Finished.")

FUNC htmlString()
    local v
#pragma __text|v+=%s+hb_eol()|return v|v:=""
<html>
<head>
  <style type="text/css">
    body {
      margin: 0;
      padding: 0;
      overflow: hidden;
      color: black;
      font-family: Arial;
      background: linear-gradient(-45deg, #acb4ff, #f5d4e2);
      display: flex;
      justify-content: center;
      align-items: center;
    }
    div {
      width: 350px;
      height: 350px;
      text-align: center;
      border-radius: 25px;
      background: linear-gradient(-45deg, #e5eaf9, #f9eaf6);
      box-shadow: 0 7px 18px -6px #8f8ae1;
    }
    h1 {
      padding: 1em;
    }
    p {
      background: white;
      padding: 2em;
      margin: 40px;
      border-radius: 25px;
    }
  </style>
</head>
<body>
  <div>
    <h1>Hello World!</h1>
    <p>Welcome to Ultralight!</p>
  </div>
</body>
</html>
ENDTEXT
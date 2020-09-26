#include <hbclass.ch>

class ultralight_refCounted
    DATA pObj PROTECTED

    //CONSTRUCTOR New(pObj)
    DESTRUCTOR Delete()

    METHOD COPY OPERATOR ":="
endclass

/// Main application class.
class ultralight_app inherit ultralight_refCounted
    // this callback is not defined because an internal version calls hb_idle
    //DATA bOnUpdate

    /// Create the App singleton.
    CONSTRUCTOR Create()

    /// Get the App singleton.
    CONSTRUCTOR instance()

    /// Get the settings this App was created with.
    //ACCESS settings()

    /// Set the main window. You must set this before calling Run.
    /// Get the main window.
    // It is the union of 2 methods: set_window and get_window
    METHOD window() SETGET

    /// Whether or not the App is running.
    ACCESS is_running()

    /// Get the main monitor (this is never NULL).
    ACCESS main_monitor()

    /// Get the underlying Renderer instance.
    ACCESS renderer()

    /// Run the main loop.
    METHOD Run()

    /// Quit the application.
    METHOD Quit()

endclass

/// This singleton manages the lifetime of all Views (@see View) and
/// coordinates painting, network requests, and event dispatch.
class ultralight_renderer inherit ultralight_refCounted

    /// Create the Ultralight Renderer directly.
    ///
    /// Unlike App::Create(), this does not use any native windows for drawing
    /// and allows you to manage your own runloop and painting. This method is
    /// recommended for those wishing to integrate the library into a game.
    CONSTRUCTOR Create()

    /// Create a Session to store local data in (such as cookies, local storage,
    /// application cache, indexed db, etc).
    ///
    /// @note  A default, persistent Session is already created for you. You
    ///        only need to call this if you want to create private, in-memory
    ///        session or use a separate session for each View.
    ///
    /// @param  is_persistent  Whether or not to store the session on disk.
    ///                        Persistent sessions will be written to the path
    ///                        set in Config::cache_path
    ///
    /// @param  name  A unique name for this session, this will be used to
    ///               generate a unique disk path for persistent sessions.
    ///
    METHOD CreateSession(is_persistent,name)

    /// Get the default Session. This session is persistent (backed to disk) and
    /// has the name "default".
    ///
    METHOD default_session();

    /// Create a new View.
    ///
    /// @param  width   The initial width, in pixels.
    ///
    /// @param  height  The initial height, in pixels.
    ///
    /// @param  transparent  Whether or not the view background is transparent.
    ///
    /// @param  session  The session to store local data in. Pass a nullptr to
    ///                  use the default session.
    METHOD CreateView(nWidth,nHeight,lransparent,session)

    /// Update timers and dispatch internal callbacks. You should call this often
    /// from your main application loop.
    METHOD Update()

    /// Render all active views to their respective render-targets/surfaces.
    ///
    /// You should call this once per frame (usually in synchrony with the
    /// monitor's refresh rate).
    METHOD Render()

    /// Attempt to release as much memory as possible. Don't call this from any
    /// callbacks or driver code.
    ///
    METHOD PurgeMemory()

    /// Print detailed memory usage statistics to the log.
    /// (@see Platform:set_logger())
    METHOD  LogMemoryUsage();

endclass


/// Monitor class, represents a platform monitor. *** IT IS NOT REFCOUNTED
class ultralight_monitor
    DATA pObj PROTECTED
    /// Get the DPI scale (1.0 = 100%)
    ACCESS scale()
    /// Get the width of the monitor.
    ACCESS width()
    /// Get the height of the monitor.
    ACCESS height()
endclass

/// Window class, represents a platform window.
class ultralight_window inherit ultralight_refCounted
    /// Called when the Window is closed.
    DATA bOnClose
    /// Called when the Window is resized.
    /// @param  width   The new width (in pixels).
    /// @param  height  The new height (in pixels).
    DATA bOnResize

    /// Create a new Window.
    /// @param  monitor       The monitor to create the Window on.
    /// @param  width         The width (in device coordinates).
    /// @param  height        The height (in device coordinates).
    /// @param  fullscreen    Whether or not the window is fullscreen.
    /// @param  window_flags  Various window flags. (ulWindowFlags_* on ultralight.ch)
    CONSTRUCTOR Create(monitor, width, height, fullscreen, window_flags)
    /// Get the window width (in pixels).
    ACCESS width()
    /// Get the window height (in pixels).
    ACCESS height()
    /// Whether or not the window is fullscreen.
    ACCESS is_fullscreen()
    /// The DPI scale of the window.
    ACCESS scale()
    /// Set the window title.
    ASSIGN title()
    /// Set the cursor.
    ASSIGN cursor()
    /// Close the window.
    METHOD Close()

    /// Convert device coordinates to pixels using the current DPI scale.
    METHOD DeviceToPixels(val)

    /// Convert pixels to device coordinates using the current DPI scale.
    METHOD PixelsToDevice(val)

    /// Draw a surface directly to window, used only by CPU renderer
    //METHOD DrawSurface(x, y, surface)

endclass

/// Web-content overlay. Displays a web-page within an area of the main window.
class ultralight_overlay inherit ultralight_refCounted
    /// Create a new Overlay.
    /// @param  window  The window to create the Overlay in. (we currently only support one window per application)
    /// @param  width   The width in device coordinates.
    /// @param  height  The height in device coordinates.
    /// @param  x       The x-position (offset from the left of the Window), in
    ///                 pixels.
    /// @param  y       The y-position (offset from the top of the Window), in
    ///                 pixels.
    /// ** OR **
    /// @param  window  The window to create the Overlay in. (we currently only
    ///                 support one window per application)
    /// @param  view    The View to wrap (will use its width and height).
    /// @param  x       The x-position (offset from the left of the Window), in pixels.
    /// @param  y       The y-position (offset from the top of the Window), in pixels.
    CONSTRUCTOR Create(window, width, height, x,y)
    /// Get the underlying View.
    ACCESS view()
    /// Get the width (in device coordinates).
    ACCESS width()
    /// Get the height (in device coordinates).
    ACCESS height()
    /// Get the x-position (offset from the left of the Window), in device
    /// coordinates.
    ACCESS x()
    /// Get the y-position (offset from the top of the Window), in device
    /// coordinates.
    ACCESS y()
    /// Whether or not the overlay is hidden (not drawn)
    /// It is the union of 3 methods: is_hidden, Hide and Show
    METHOD hidden() SETGET
    /// Hide the overlay (will no longer be drawn)
    METHOD Hide() INLINE ::hidden := .T.
    /// Show the overlay.
    METHOD Show() INLINE ::hidden := .F.
    /// Whether or not this overlay has keyboard focus.
    /// It is the union of 3 methods: has_focus, Focus and Unfocus
    METHOD has_focus() SETGET
    /// Grant this overlay exclusive keyboard focus.
    METHOD Focus() INLINE ::has_focus := .T.
    /// Remove keyboard focus.
    METHOD Unfocus() INLINE ::has_focus := .F.

    /// Move the overlay to a new position (in pixels).
    METHOD moveTo(x,y)
    /// Resize the overlay (and underlying View), dimensions should be
    /// specified in pixels.
    METHOD Resize(width,height)
    /// Whether or not this Overlay needs repaint (either it has moved, resized,
    /// or the internal View needs repaint).
    ACCESS needsRepaint()
endclass

/// A Session stores local data such as cookies, local storage,
/// and application cache for one or more Views.
class ultralight_session inherit ultralight_refCounted
  /// Whether or not this session is written to disk.
  METHOD is_persistent()
  /// A unique name identifying this session.
  METHOD name()

  /// A unique numeric ID identifying this session.
  METHOD id()

  /// The disk path of this session (only valid for persistent sessions).
  METHOD disk_path()

endclass

/// The View class is used to load and display web content.
class ultralight_View inherit ultralight_refCounted
    /// *** ViewListener ***
    /// *** Interface for View-related events ***

    /// Called when the page title changes
    DATA bOnChangeTitle // (oView,cTitle)

    /// Called when the page URL changes
    DATA bOnChangeURL //(oView, cUrl)

    /// Called when the tooltip changes (usually as result of a mouse hover)
    DATA bOnChangeTooltip //(oView, cTooltip)

    /// Called when the mouse cursor changes
    DATA bOnChangeCursor //(oView, iCursor) // see defines ulCursor_*

    /// Called when a message is added to the console (useful for errors / debug)
    DATA bOnAddConsoleMessage //(oView,iSource,iLevel,cMessage,iLine_number,iColumn_number,cSource_id)
                              //  see defines ulMessageSource_* and ulMessageLevel_*

    /// Called when the page wants to create a new View.
    ///
    /// This is usually the result of a user clicking a link with target="_blank"
    /// or by JavaScript calling window.open(url).
    ///
    /// To allow creation of these new Views, you should create a new View in
    /// this callback (eg, Renderer:CreateView()), resize it to your container,
    /// and return it. You are responsible for displaying the returned View.
    DATA bOnCreateChildView // (oView,cOpener_url,cTarget_url,lIs_popup,aPopup_rect {left, top, right, bottom}) -> new oView

    /// *** LoadListener ***
    /// *** Interface for Load-related events ***

    /// Called when the page begins loading a new URL into a frame.
    DATA bOnBeginLoading //(oView, iFrame_id, lIs_main_frame, cUrl)

    /// Called when the page finishes loading a URL into a frame.
    DATA bOnFinishLoading //(oView, iFrame_id, lIs_main_frame, cUurl)

    /// Called when an error occurs while loading a URL into a frame.
    DATA bOnFailLoading //(oView, iFrame_id, lIs_main_frame, cUrl, cDescription, cError_domain,iError_code)

    /// Called when the JavaScript window object is reset for a new page load.
    /// If you need to make any JavaScript calls that are dependent on DOM elements
    /// or scripts on the page, use OnDOMReady instead.
    DATA bOnWindowObjectReady //(oView,iFrame_id,lIs_main_frame,cUrl)

    /// Called when all JavaScript has been parsed and the document is ready.
    DATA bOnDOMReady //(oView, iFrame_id,lIs_main_frame,cUrl)

    /// Called when the session history (back/forward state) is modified.
    DATA bOnUpdateHistory //(oView

    /// *** VIEW PART ***
    /// Get the URL of the current page loaded into this View, if any.
    ACCESS url()

    /// Get the title of the current page loaded into this View, if any.
    ACCESS title()

    /// Get the width of the View, in pixels.
    ACCESS width()

    /// Get the height of the View, in pixels.
    ACCESS height()

    /// Check if the main frame of the page is currently loading.
    ACCESS is_loading()

    /// Get the offscreen RenderTarget for the View.
    //METHOD  render_target()

    /// Get the offscreen Surface for the View (pixel-buffer container).
    //METHOD surface();

    /// Load a raw string of HTML, the View will navigate to it as a new page.
    /// @param  html  The raw HTML string to load.
    /// @param  url   An optional URL for this load
    /// @param  add_to_history  Whether or not this load should be added to the session's history (back/forward list).
    METHOD  LoadHTML(cHtml,cUrl,lAdd_to_history)

    /// Load a URL, the View will navigate to it as a new page.
    METHOD LoadURL(url)

    /// Resize View to a certain size.
    METHOD Resize(width,height)

    /// Acquire the page's JSContext for use with the JavaScriptCore API
    //METHOD LockJSContext();

    /// Helper function to evaluate a raw string of JavaScript and return the
    /// result as a String.
    /// @param  script     A string of JavaScript to evaluate in the main frame.
    /// @param  exception  (@)A string to store the exception in, if any. Pass a nullptr if you don't care about exceptions.
    /// @return  Returns the JavaScript result typecast to a String.
    METHOD EvaluateScript(script, exception)

    /// Whether or not we can navigate backwards in history
    ACCESS CanGoBack()

    /// Whether or not we can navigate forwards in history
    ACCESS CanGoForward()

    /// Navigate backwards in history
    METHOD GoBack()

    /// Navigate forwards in history
    METHOD GoForward()

    /// Navigate to an arbitrary offset in history
    METHOD GoToHistoryOffset(offset)

    /// Reload current page
    METHOD Reload()

    /// Stop all page loads
    METHOD Stop()

    /// Whether or not the View has focus.
    /// It is the union of 3 methods: hasFocus, Focus and Unfocus
    METHOD hasFocus() SETGET
    /// Grant this overlay exclusive keyboard focus.
    METHOD Focus() INLINE ::focus := .T.
    /// Remove keyboard focus.
    METHOD Unfocus() INLINE ::focus := .F.

    /// Whether or not the View has an input element with visible keyboard focus
    /// (indicated by a blinking caret).
    /// You can use this to decide whether or not the View should consume
    /// keyboard input events (useful in games with mixed UI and key handling).
    ACCESS HasInputFocus()

    /// Fire a keyboard event
    //METHOD FireKeyEvent(evt)

    /// Fire a mouse event
    //METHOD FireMouseEvent(evt)

    /// Fire a scroll event
    //METHOD FireScrollEvent(evt)

    /// Set a ViewListener to receive callbacks for View-related events.
    /// NEVER IMPLEMENTED SEE block DATA
    //virtual void set_view_listener(ViewListener* listener) = 0;
    /// Get the active ViewListener, if any
    /// NEVER IMPLEMENTED SEE block DATA
    //virtual ViewListener* view_listener() const = 0;

    /// Set a LoadListener to receive callbacks for Load-related events.
    /// NEVER IMPLEMENTED SEE block DATA
    //virtual void set_load_listener(LoadListener* listener) = 0;
    /// Get the active LoadListener, if any
    /// NEVER IMPLEMENTED SEE block DATA
    //virtual LoadListener* load_listener() const = 0;

    /// Set whether or not this View should be repainted during the next
    /// Whether or not this View should be repainted during the next call to Renderer::Render.
    /// It is the union of 3 methods: get_needs_paint and needs_paint
    METHOD needs_paint() SETGET

    /// Get the inspector for this View, this is useful for debugging and
    /// inspecting pages locally. This will only succeed if you have the
    /// inspector assets in your filesystem-- the inspector will look for
    /// file:///inspector/Main.html when it first loads.
    METHOD inspector()
endclass
/*
class ultralight_Bitmap
    DATA pObj HIDDEN

  ///
  /// Create a Bitmap with existing pixels and configuration.
  ///
  /// @param  width        The width in pixels.
  ///
  /// @param  height       The height in pixels.
  ///
  /// @param  format       The pixel format to use.
  ///
  /// @param  row_bytes    The number of bytes between each row (note that this
  ///                      value should be >= width * bytes_per_pixel).
  ///
  /// @param  pixels       Pointer to raw pixel buffer.
  //CONSTRUCTOR Create(nWidth, nHeight, ulFormat, nRowBytes, cPixels)

  ///
  /// Create a bitmap from a deep copy of another Bitmap.
  ///
  //CONSTRUCTOR Create(oBitmap);

  ///
  /// Get the width in pixels.
  ///
  //ACCESS width()

  ///
  /// Get the height in pixels.
  ///
  //ACCESS height()

  ///
  /// Get the bounds as an IntRect
  ///
  //ACCESS bounds() //=> {l,t,r,b}

  ///
  /// Get the pixel format.
  ///
  //ACCESS format()

  ///
  /// Get the number of bytes per pixel.
  ///
  //ACCESS bpp()

  ///
  /// Get the number of bytes between each row (this is always >= width * bpp)
  ///
  //ACCESS row_bytes()

  ///
  /// Get the size in bytes of the pixel buffer.
  ///
  //ACCESS size()

  ///
  /// Whether or not this Bitmap owns the pixel buffer and will destroy it
  /// at the end of its lifetime.
  ///
  //ACCESS owns_pixels()

  ///
  /// Lock the pixel buffer for reading/writing.
  ///
  /// @return  A pointer to the pixel buffer.
  ///
  //METHOD LockPixels()

  ///
  /// Unlock the pixel buffer.
  ///
  //METHOD UnlockPixels()

  ///
  /// Get the raw pixel buffer.
  ///
  /// @note  You should only call this if pixels are already locked.
  ///
  //virtual void* raw_pixels() = 0;

  ///
  /// Whether or not this Bitmap is empty (no pixels allocated).
  ///
  //ACCESS IsEmpty()

  ///
  /// Erase the Bitmap (set all pixels to 0).
  ///
  //METHOD Erase()

  ///
  /// Assign another bitmap to this one.
  ///
  /// @param  bitmap  The bitmap to copy from.
  ///
  //virtual void Set(Ref<Bitmap> bitmap) = 0;

  ///
  /// Draw another bitmap to this bitmap.
  ///
  /// @note  Formats do not need to match. Bitmap formats will be converted
  ///        to one another automatically. Note that when converting from
  ///        RGBA8 to A8, only the Red channel will be used.
  ///
  /// @param  src_rect    The source rectangle, relative to src bitmap.
  ///
  /// @param  dest_rect   The destination rectangle, relative to this bitmap.
  ///
  /// @param  src         The source bitmap.
  ///
  /// @param  pad_repeat  Whether or not we should pad the drawn bitmap by one
  ///                     pixel of repeated edge pixels from the source bitmap.
  ///
  /// @return  Whether or not the operation succeeded (this can fail if the
  ///          src_rect and/or dest_rect are invalid, or if their total
  ///          dimensions do not match).
  ///
  //METHOD DrawBitmap(src_rect,dest_rect, src, pad_repeat)
  ///
  /// Write this Bitmap out to a PNG image. (mainly used for Debug)
  ///
  /// @param  path  The filepath to write to (opened with fopen())
  ///
  /// @return  Whether or not the operation succeeded.
  ///
  METHOD WritePNG(cPath)
endclass
*/

#include <hbclass.ch>
static ULHash := {=>}

class ultralight_app
    DATA pObj HIDDEN
    /// Create the App singleton.
    CONSTRUCTOR Create()
    /// Get the App singleton.
    CONSTRUCTOR instance()
    /// Set the main window. You must set this before calling Run.
    METHOD window() SETGET

    //ACCESS is_running()
    /// Get the main monitor (this is never NULL).
    METHOD main_monitor()
    /// Get the underlying Renderer instance.
    METHOD renderer()
    /// Run the main loop.
    /// *NOTE* it calls hb_idle.
    METHOD run()
    //METHOD quit()
endclass

class ultralight_renderer
    DATA pObj HIDDEN
    ///
    /// Create the Renderer singleton. You should set up all your Platform config,
    /// file-system, and drivers before calling this function. @see Platform
    ///
    /// @note  You should only create one Renderer per application lifetime.
    CONSTRUCTOR Create()

    ///
    /// Create a new View.
    ///
    /// @param  width   The initial width, in device coordinates.
    ///
    /// @param  height  The initial height, in device coordinates.
    ///
    /// @param  transparent  Whether or not the view background is transparent.
    ///
    /// @note  The device coordinates are scaled to pixels by multiplying them
    ///        with the current DPI scale (@see Config::device_scale_hint) and
    ///        rounding to the nearest integer value.
    ///
    METHOD CreateView(nWidth,nHeight,lransparent)

    ///
    /// Update timers and dispatch internal callbacks. You should call this often
    /// from your main application loop.
    ///
    METHOD Update()

    ///
    /// Render all active views to display lists and dispatch calls to GPUDriver.
    ///
    /// @note  If you're using the default, offscreen GL driver, this updates the
    ///        internal bitmap of each View (@see View::bitmap).
    ///
    METHOD Render()

    ///
    /// Attempt to release as much memory as possible. Don't call this from any
    /// callbacks or driver code.
    ///
    //METHOD PurgeMemory()
endclass

class ultralight_monitor
    DATA pObj HIDDEN
    /// Get the DPI scale (1.0 = 100%)
    ACCESS scale()
    /// Get the width of the monitor.
    ACCESS width()
    /// Get the height of the monitor.
    ACCESS height()
endclass

/// Window class, represents a platform window.
class ultralight_window
    DATA pObj HIDDEN
    /// Called when the Window is closed.
    DATA bOnClose
    /// Called when the Window is resized.
    /// @param  width   The new width (in device coordinates).
    /// @param  height  The new height (in device coordinates).
    DATA bOnResize

    /// Create a new Window.
    ///
    /// @param  monitor       The monitor to create the Window on.
    ///
    /// @param  width         The width (in device coordinates).
    ///
    /// @param  height        The height (in device coordinates).
    ///
    /// @param  fullscreen    Whether or not the window is fullscreen.
    ///
    /// @param  window_flags  Various window flags. (ulWindowFlags_* on ultralight.ch)
    CONSTRUCTOR Create(monitor, width, height, fullscreen, window_flags)
    /// Get the window width (in device coordinates).
    ACCESS width()
    /// Get the window height (in device coordinates).
    ACCESS height()
    /// Whether or not the window is fullscreen.
    //ACCESS is_fullscreen()
    /// The DPI scale of the window.
    ACCESS scale()
    /// Set the window title.
    METHOD SetTitle(cTile)
    /// Set the cursor.
    METHOD SetCursor(nCursor)
    /// Close the window.
    //METHOD Close()

    /// Convert device coordinates to pixels using the current DPI scale.
    //METHOD DeviceToPixels(nVal)
    /// Convert pixels to device coordinates using the current DPI scale.
    //METHOD PixelsToDevice(nVal)
endclass

class ultralight_overlay
    DATA pObj HIDDEN
    ///
    /// Create a new Overlay.
    ///
    /// @param  window  The window to create the Overlay in. (we currently only
    ///                 support one window per application)
    ///
    /// @param  width   The width in device coordinates.
    ///
    /// @param  height  The height in device coordinates.
    ///
    /// @param  x       The x-position (offset from the left of the Window), in
    ///                 device coordinates.
    ///
    /// @param  y       The y-position (offset from the top of the Window), in
    ///                 device coordinates.
    ///
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
    /// Hide the overlay (will no longer be drawn)
    /// Show the overlay.
    METHOD hidden() SETGET
    METHOD Show() INLINE ::hidden := .F.
    METHOD Hide() INLINE ::hidden := .T.
    /// Whether or not this overlay has keyboard focus.
    /// Grant this overlay exclusive keyboard focus.
    /// Remove keyboard focus.
    METHOD focus() SETGET
    /// Move the overlay to a new position (in device coordinates).
    METHOD moveTo(x,y)
    /// Resize the overlay (and underlying View), dimensions should be
    /// specified in device coordinates.
    METHOD Resize(width,height)
endclass

/// A View is similar to a tab in a browser-- you load web content into
///	it and display it however you want. @see Renderer::CreateView
class ultralight_View
    DATA pObj HIDDEN

    /// Called when the page title changes
    /// @param caller this ultralight_view
    /// @param cTitle the new title
    DATA bOnChangeTitle
    /// Called when the page URL changes
    /// @param caller this ultralight_view
    /// @param cURL the new url
    DATA bOnChangeURL
    /// Called when the tooltip changes (usually as result of a mouse hover)
    /// @param caller this ultralight_view
    /// @param cTooltip the new tooltip
    DATA bOnChangeTooltip
    /// Called when the mouse cursor changes
    /// @param caller this ultralight_view
    /// @param nCursor ulCursor_*
    DATA bOnChangeCursor
    /// Called when a message is added to the console (useful for errors / debug)
    /// @param caller this ultralight_view
    /// @param nSource ulMessageSource_*
    /// @param nLevel ulMessageLevel_*
    /// @param cMessage
    /// @param nLine
    /// @param nColumn
    /// @param cSource
    DATA bOnAddConsoleMessage

    /// Called when the page begins loading new URL into main frame
    /// @param caller this ultralight_view
    DATA bOnBeginLoading
    /// Called when the page finishes loading URL into main frame
    /// @param caller this ultralight_view
    DATA bOnFinishLoading
    /// Called when the history (back/forward state) is modified
    /// @param caller this ultralight_view
    DATA bOnUpdateHistory
    /// Called when all JavaScript has been parsed and the document is ready.
    /// This is the best time to make any initial JavaScript calls to your page.
    DATA bOnDOMReady

    /// Get the URL of the current page loaded into this View, if any.
    ACCESS url()
    /// Get the title of the current page loaded into this View, if any.
    ACCESS title()
    /// Check if the main frame of the page is currently loading.
    ACCESS is_loading()
    /// Get the RenderTarget for the View.
    //METHOD render_target()
    /// Check if bitmap is dirty (has changed since last call to View::bitmap)
    //METHOD is_bitmap_dirty()
    /// Get the bitmap for the View (calling this resets the dirty state).
    //METHOD bitmap()
    /// Load a raw string of HTML, the View will navigate to it as a new page.
    METHOD LoadHTML(cHTML)
    /// Load a URL, the View will navigate to it as a new page.
    METHOD LoadURL(cURL)
    /// Resize View to a certain size.
    METHOD Resize(width,height)
    /// Get the page's JSContext for use with the JavaScriptCore API
    METHOD LockJSContext()
    /// Evaluate a raw string of JavaScript and return results as a native
    /// JavaScriptCore JSValueRef (@see <JavaScriptCore/JSValueRef.h>)
    //METHOD EvaluateScript(cScript)
    /// Whether or not we can navigate backwards in history
    METHOD CanGoBack()
    /// Whether or not we can navigate forwards in history
    METHOD CanGoForward()
    /// Navigate backwards in history
    METHOD GoBack()
    /// Navigate forwards in history
    METHOD GoForward()
    /// Navigate to an arbitrary offset in history
    //METHOD GoToHistoryOffset(offset)
    /// Reload current page
    METHOD Reload()
    /// Stop all page loads
    METHOD Stop()
    /// Fire a keyboard event
    //METHOD FireKeyEvent(evt)
    /// Fire a mouse event
    //METHOD FireMouseEvent(evt)
    /// Fire a scroll event
    //METHOD FireScrollEvent( evt)
    /// Set or get whether or not this View should be repainted during the next
    /// call to Renderer::Render
    //METHOD needs_paint() SETGET
endclass

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

func ultralight_getHB(ptr)
    if hb_HHasKey(ULHash,ptr)
        return ULHash[ptr]
    endif
retur nil

func ultralight_setHB(ptr,val)
    hb_HSet(ULHash,ptr,val)
return val



proc main()
    LOCAL renderer_ := ultralight_renderer():Create()
    LOCAL view_ := renderer_:CreateView(200, 200, .F.)
    LOCAL done_ :=.F.
    view_:bOnFinishLoading := {|| OnFinishLoading(renderer_,view_),done_:=.T. }
    view_:LoadHTML("<h1>Hello!</h1><p>Welcome to Ultralight!</p>")
    do while .not. done_
        renderer_:Update()
    enddo
  

proc OnFinishLoading(renderer_,view_)
    ///
    /// Render all Views (the default GPUDriver paints each View to an
    /// offscreen Bitmap which you can access via View::Bitmap)
    ///
    renderer_:Render();
    
    ///
    /// Get our View's bitmap and write it to a PNG.
    ///
    view_:bitmap():WritePNG("result.png");
    

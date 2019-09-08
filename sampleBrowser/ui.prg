#include <hbclass.ch>
#include "ultralight.ch"

#define UI_HEIGHT 79

class UI
    DATA window
    DATA overlay
    DATA active_tab_id, tabs
    DATA tab_id_counter INIT 1
    DATA ui_height, tab_height, scale
    DATA cur_cursor
    

    CONSTRUCTOR NEW(window)
    //~UI();

    ACCESS view() INLINE ::overlay:view
    ACCESS active_tab() INLINE iif(!Empty(::tabs) .and. hb_HHasKey(::tabs,::active_tab_id),::tabs[::active_tab_id],nil)

    METHOD CreateNewTab() 
    METHOD UpdateTabTitle(id, cTitle) INLINE ::updateTab:CallNoThis(id, ctitle, "")
    METHOD UpdateTabURL(id, cUrl) inline IIF(id=::active_tab_id,::SetUrl(cUrl),) 
    METHOD UpdateTabNavigation(id,is_loading,can_go_back,can_go_forward) 

    METHOD OnResize(width, height)

    METHOD SetLoading(is_loading) INLINE ::updateLoading:CallNoThis(is_loading)
    METHOD SetCanGoBack(can_go_back) INLINE ::updateBack:CallNoThis(can_go_back)
    METHOD SetCanGoForward(can_go_forward) INLINE ::updateForward:CallNoThis(can_go_forward)
    METHOD SetURL(cUrl) INLINE ::updateURL:CallNoThis(cUrl)
    METHOD SetCursor(nCursor) INLINE ultralight_app():Instance():window:SetCursor(nCursor)

    METHOD OnDOMReady(caller) 
    METHOD OnRequestTabClose(obj,args)
    METHOD OnActiveTabChange(obj,args)
    METHOD OnRequestChangeURL(obj,args)
    DATA updateBack
    DATA updateForward
    DATA updateLoading
    DATA updateURL
    DATA addTab
    DATA updateTab
    DATA closeTab

endclass 

METHOD New(window) CLASS UI
    LOCAL v
    ::window := window
    ::overlay := ultralight_overlay():Create(window, window:width, UI_HEIGHT, 0, 0)
    v:=::overlay:view
    v:LoadURL("file:///ui.html")
    v:bOnDOMReady := {|caller| ::OnDOMReady(caller)}
    window:bOnResize := {|width, height| ::OnResize(width,height) }
    v:bOnAddConsoleMessage = @Console()
    ::tabs := {=>}
return self

METHOD OnResize(width, height) CLASS UI
    LOCAL tab, tab_height := height - UI_HEIGHT
    ::overlay:Resize(width, UI_HEIGHT)
    for each tab in ::tabs
        tab:resize(width, tab_height)
    next
return nil  

METHOD OnDOMReady(caller) CLASS UI
  LOCAL global
  SetJSContext(caller:js_context())

  global := JSGlobalObject()
  ::updateBack = global["updateBack"]
  ::updateForward = global["updateForward"]
  ::updateLoading = global["updateLoading"]
  ::updateURL = global["updateURL"]
  ::addTab = global["addTab"]
  ::updateTab = global["updateTab"]
  ::closeTab = global["closeTab"]

  global["OnBack"] = {|| iif(!empty(::active_tab),::active_tab:view:GoBack(),) }
  global["OnForward"] = {|| iif(!empty(::active_tab),::active_tab:view:GoForward(),) }
  global["OnRefresh"] = {|| iif(!empty(::active_tab),::active_tab:view:Reload(),) }
  global["OnStop"] = {|| iif(empty(::active_tab),,::active_tab:view:Stop()) }
  global["OnRequestNewTab"] = {|| ::CreateNewTab() }
  global["OnRequestTabClose"] = {|obj,args| ::OnRequestTabClose(obj,args) }
  global["OnActiveTabChange"] = {|obj,args| ::OnActiveTabChange(obj,args) }
  global["OnRequestChangeURL"] = {|obj,args| ::OnRequestChangeURL(obj,args) }

  ::CreateNewTab();

return nil

METHOD OnRequestTabClose(obj,args) CLASS UI
    LOCAL id
    HB_SYMBOL_UNUSED(obj)
    if len(args) > 0
        id := args[1]
        IF valtype(id)=="C"
             id:=val(id)
        endif
        if .not. hb_HHasKey(::tabs,id)
            return nil
        endif
        if len(::tabs) == 1
            ultralight_app():Instance():Quit()
        endif
        if id != ::active_tab_id
            ::tabs[id]:reset()
            hb_HDel(::tabs,id)
        endif
    else
      ::active_tab:ready_to_close := .T.
    endif

    ::closeTab:CallNoThis(id)
return nil

METHOD OnActiveTabChange(obj, args)
    LOCAL id, tab, tab_view
    HB_SYMBOL_UNUSED(obj)
    if len(args) >0
        id := args[1]
        IF valtype(id)=="C"
             id:=val(id)
        endif
        if (id = ::active_tab_id)
            return nil
        endif
        if .not. hb_HHasKey(::tabs,id)
            return nil
        endif
        tab = ::active_tab
        if !empty(tab)
            tab:overlay():Hide()
            tab:overlay():Focus := .F.

       
            if tab:ready_to_close()
                ::tabs[id]:reset()
                hb_HDel(::tabs,::active_tab)
            endif
        endif
        ::active_tab_id = id
        tab = ::active_tab
        tab:overlay:Show()
        tab:overlay:Focus := .T.
      
        tab_view := tab:view()
        ::SetLoading(tab_view:is_loading())
        ::SetCanGoBack(tab_view:CanGoBack())
        ::SetCanGoForward(tab_view:CanGoForward())
        ::SetURL(tab_view:url)
    endif
return nil

METHOD OnRequestChangeURL(obj, args)
    LOCAL url
    HB_SYMBOL_UNUSED(obj)
    if len(args) == 1 .and. !empty(::tabs)
        url := args[0]
        ::active_tab:view:LoadURL(url)
    endif
return nil

METHOD CreateNewTab() CLASS UI
    LOCAL id := ::tab_id_counter++
    ::tabs[id] := Tab():New(Self, Len(::tabs), ::window:width, ::window:height - UI_HEIGHT, 0, UI_HEIGHT)
    ::tabs[id]:view:LoadURL("file:///new_tab_page.html")
    ::active_tab_id := id
  
    ::addTab:CallNoThis(id, "New Tab", "")
return nil  

METHOD UpdateTabNavigation(id,is_loading,can_go_back,can_go_forward) CLASS UI
    ? "UpdateTabNavigation", id,is_loading,can_go_back,can_go_forward
    IF valtype(id)=="C"
         id:=val(id)
    endif
    if id == ::active_tab_id
        ::SetLoading(is_loading)
        ::SetCanGoBack(can_go_back)
        ::SetCanGoForward(can_go_forward)
    endif
return nil


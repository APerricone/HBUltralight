#include <hbclass.ch>
#include "ultralight.ch"

class Tab
    DATA ready_to_close INIT .F.
    DATA overlay, id, ui

    CONSTRUCTOR New(ui,id,width,height,x,y)

    ACCESS view() INLINE ::overlay:view()

    METHOD Resize(width, height) INLINE ::overlay:resize(width, height)
endclass

METHOD new(ui,id,width,height,x,y) CLASS Tab
    LOCAL view, commonBlock
    ::ui := ui
    ::id := id
    ::overlay = ultralight_overlay():Create(ui:window, width, height, x, y)
    view := ::overlay:view
    view:bOnChangeTitle := {|c,title| HB_SYMBOL_UNUSED(c), ui:UpdateTabTitle(::id,title) }
    view:bOnChangeURL := {|c,url| HB_SYMBOL_UNUSED(c), ui:UpdateTabURL(::id,url) }
    view:bOnChangeCursor := {|c,cur| HB_SYMBOL_UNUSED(c), iif(id==ui:active_tab_id,ui:cursor := cur,) }
    commonBlock := {|caller| ui:UpdateTabNavigation(id, caller:is_loading(), caller:CanGoBack(), caller:CanGoForward()) }
    view:bOnBeginLoading := commonBlock
    view:bOnFinishLoading := commonBlock
    view:bOnUpdateHistory := commonBlock

    view:bOnAddConsoleMessage = @Console()
return self

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

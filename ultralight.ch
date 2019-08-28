
#define  ulWindowFlags_Borderless  1 //1 << 0
#define  ulWindowFlags_Titled      2 //1 << 1
#define  ulWindowFlags_Resizable   4 //1 << 2
#define  ulWindowFlags_Maximizable 8 //1 << 3

#define ulMessageSource_XML             0
#define ulMessageSource_JS              1
#define ulMessageSource_Network         2
#define ulMessageSource_ConsoleAPI      3    
#define ulMessageSource_Storage         4
#define ulMessageSource_AppCache        5    
#define ulMessageSource_Rendering       6    
#define ulMessageSource_CSS             7
#define ulMessageSource_Security        8    
#define ulMessageSource_ContentBlocker  9          
#define ulMessageSource_Other           10

/// MessageLevel types ultralight_View:bOnAddConsoleMessage
#define ulMessageLevel_Log      1
#define ulMessageLevel_Warning  2
#define ulMessageLevel_Error    3
#define ulMessageLevel_Debug    4
#define ulMessageLevel_Info     5

/// Cursor types ultralight_View:bOnChangeCursor
#define ulCursor_Pointer                    0
#define ulCursor_Cross                      1
#define ulCursor_Hand                       2
#define ulCursor_IBeam                      3
#define ulCursor_Wait                       4
#define ulCursor_Help                       5
#define ulCursor_EastResize                 6
#define ulCursor_NorthResize                7
#define ulCursor_NorthEastResize            8
#define ulCursor_NorthWestResize            9
#define ulCursor_SouthResize                10
#define ulCursor_SouthEastResize            11
#define ulCursor_SouthWestResize            12
#define ulCursor_WestResize                 13
#define ulCursor_NorthSouthResize           14
#define ulCursor_EastWestResize             15
#define ulCursor_NorthEastSouthWestResize   16
#define ulCursor_NorthWestSouthEastResize   17
#define ulCursor_ColumnResize               18
#define ulCursor_RowResize                  19
#define ulCursor_MiddlePanning              20
#define ulCursor_EastPanning                21
#define ulCursor_NorthPanning               22
#define ulCursor_NorthEastPanning           23
#define ulCursor_NorthWestPanning           24
#define ulCursor_SouthPanning               25
#define ulCursor_SouthEastPanning           26
#define ulCursor_SouthWestPanning           27
#define ulCursor_WestPanning                28
#define ulCursor_Move                       29
#define ulCursor_VerticalText               30
#define ulCursor_Cell                       31
#define ulCursor_ContextMenu                32
#define ulCursor_Alias                      33
#define ulCursor_Progress                   34
#define ulCursor_NoDrop                     35
#define ulCursor_Copy                       36
#define ulCursor_None                       37
#define ulCursor_NotAllowed                 38
#define ulCursor_ZoomIn                     39
#define ulCursor_ZoomOut                    40
#define ulCursor_Grab                       41
#define ulCursor_Grabbing                   42
#define ulCursor_Custom                     43
mkdir release
::xcopy ultralight\include release\ultralight\include /E /I
xcopy ultralight\lib release\ultralight\lib /E /I
xcopy ultralight\bin release\ultralight\bin /E /I
copy hbultralight.hbc release
copy hbultralight.lib release
copy *.ch release
copy ultralight.prg release
copy ultralight_JS.prg release

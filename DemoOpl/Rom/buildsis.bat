@echo off
if exist "%epocroot%epoc32\release\thumb\urel\DemoOPL.sis" del "%epocroot%epoc32\release\thumb\urel\DemoOPL.sis"
makesis "%epocroot%DemoOPL\rom\DemoOPL (Thumb).pkg" "%epocroot%epoc32\release\thumb\urel\DemoOPL.sis"

if exist "%epocroot%epoc32\release\wins\udeb\DemoOPL.sis" del "%epocroot%epoc32\release\wins\udeb\DemoOPL.sis"
makesis "%epocroot%DemoOPL\rom\DemoOPL (WINS UDEB).pkg" "%epocroot%epoc32\release\wins\udeb\DemoOPL.sis"

if exist "%epocroot%epoc32\release\wins\urel\DemoOPL.sis" del "%epocroot%epoc32\release\wins\urel\DemoOPL.sis"
makesis "%epocroot%DemoOPL\rom\DemoOPL (WINS UREL).pkg" "%epocroot%epoc32\release\wins\urel\DemoOPL.sis"

rem
rem Now generate WINS stub files - NOT currently used
rem
rem if exist "%epocroot%epoc32\release\wins\udeb\z\system\install\DemoOPL.sis" del "%epocroot%epoc32\release\wins\udeb\z\system\install\DemoOPL.sis"
rem makesis -s "%epocroot%DemoOPL\rom\DemoOPL (WINS UDEB).pkg" "%epocroot%epoc32\release\wins\udeb\z\system\install\DemoOPL.sis"

rem if exist "%epocroot%epoc32\release\wins\urel\z\system\install\DemoOPL.sis" del "%epocroot%epoc32\release\wins\urel\z\system\install\DemoOPL.sis"
rem makesis -s "%epocroot%DemoOPL\rom\DemoOPL (WINS UREL).pkg" "%epocroot%epoc32\release\wins\urel\z\system\install\DemoOPL.sis"

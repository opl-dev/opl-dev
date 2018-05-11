@echo off
if exist "%epocroot%epoc32\release\thumb\urel\OPL.sis" del "%epocroot%epoc32\release\thumb\urel\OPL.sis"
makesis "%epocroot%oplr\rom\OPL (Thumb).pkg" "%epocroot%epoc32\release\thumb\urel\OPL.sis"

if exist "%epocroot%epoc32\release\wins\udeb\OPL.sis" del "%epocroot%epoc32\release\wins\udeb\OPL.sis"
makesis "%epocroot%oplr\rom\OPL (WINS UDEB).pkg" "%epocroot%epoc32\release\wins\udeb\OPL.sis"

if exist "%epocroot%epoc32\release\wins\urel\OPL.sis" del "%epocroot%epoc32\release\wins\urel\OPL.sis"
makesis "%epocroot%oplr\rom\OPL (WINS UREL).pkg" "%epocroot%epoc32\release\wins\urel\OPL.sis"

rem
rem Now generate WINS stub files
rem
if exist "%epocroot%epoc32\release\wins\udeb\z\system\install\OPL.sis" del "%epocroot%epoc32\release\wins\udeb\z\system\install\OPL.sis"
makesis -s "%epocroot%oplr\rom\OPL (WINS UDEB).pkg" "%epocroot%epoc32\release\wins\udeb\z\system\install\OPL.sis"

if exist "%epocroot%epoc32\release\wins\urel\z\system\install\OPL.sis" del "%epocroot%epoc32\release\wins\urel\z\system\install\OPL.sis"
makesis -s "%epocroot%oplr\rom\OPL (WINS UREL).pkg" "%epocroot%epoc32\release\wins\urel\z\system\install\OPL.sis"

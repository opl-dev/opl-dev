@echo off
if exist "%EPOCROOT%epoc32\release\thumb\urel\OPLDev.sis" del "%EPOCROOT%epoc32\release\thumb\urel\OPLDev.sis"
makesis "\opl\texted\rom\OPLDev (Thumb).pkg" "%EPOCROOT%epoc32\release\thumb\urel\OPLDev.sis"

if exist "%EPOCROOT%epoc32\release\wins\udeb\OPLDev.sis" del "%EPOCROOT%epoc32\release\wins\udeb\OPLDev.sis"
makesis "\opl\texted\rom\OPLDev (WINS UDEB).pkg" "%EPOCROOT%epoc32\release\wins\udeb\OPLDev.sis"

if exist "%EPOCROOT%epoc32\release\wins\urel\OPLDev.sis" del "%EPOCROOT%epoc32\release\wins\urel\OPLDev.sis"
makesis "\opl\texted\rom\OPLDev (WINS UREL).pkg" "%EPOCROOT%epoc32\release\wins\urel\OPLDev.sis"

rem
rem Now generate WINS stub files
rem
if exist "%EPOCROOT%epoc32\release\wins\udeb\z\system\install\OPLDev.sis" del "%EPOCROOT%epoc32\release\wins\udeb\z\system\install\OPLDev.sis"
makesis -s "\opl\texted\rom\OPLDev (WINS UDEB).pkg" "%EPOCROOT%epoc32\release\wins\udeb\z\system\install\OPLDev.sis"

if exist "%EPOCROOT%epoc32\release\wins\urel\z\system\install\OPLDev.sis" del "%EPOCROOT%epoc32\release\wins\urel\z\system\install\OPLDev.sis"
makesis -s "\opl\texted\rom\OPLDev (WINS UREL).pkg" "%EPOCROOT%epoc32\release\wins\urel\z\system\install\OPLDev.sis"

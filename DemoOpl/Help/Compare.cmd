rem @fc4bat %1 %2
@diff %1 %2
@if errorlevel==1 echo Error - Help HRH files do not match.
@if errorlevel==1 echo Help links may be broken unless code is recompiled.

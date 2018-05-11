@rem makezip.bat

@set _wrapper_name=OplTestS60
@pushd %EPOCROOT%epoc32\release\thumb\urel\
@del %_wrapper_name%_wrapper.zip > NUL 2>&1
@call zip %_wrapper_name%_wrapper.zip %_wrapper_name%.aif %_wrapper_name%_caption.rsc %_wrapper_name%.rsc %_wrapper_name%.app
@popd
@move %EPOCROOT%epoc32\release\thumb\urel\%_wrapper_name%_wrapper.zip .
@cd
@dir/b *.zip
@set _wrapper_name=

del *.obj
del *.exe

set include_path=
set include_path=%include_path% /I "./include/"
set include_path=%include_path% /I "./include/sb7"
set include_path=%include_path% /I "C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/um"
set include_path=%include_path% /I "C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/shared"
set include_path=%include_path% /I "C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/ucrt"
set include_path=%include_path% /I "C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/winrt"
set include_path=%include_path% /I "C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/cppwinrt"

set lib_path=
set lib_path=%lib_path% /link
set lib_path=%lib_path% /LIBPATH:"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.18362.0/ucrt/x86"
set lib_path=%lib_path% /LIBPATH:"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.18362.0/um/x86"

set output_path=/link /OUT:./engine.exe

set lib_files=
set lib_files=%lib_files% ./lib/glfw3.lib
set lib_files=%lib_files% ./lib/freeglutd.lib
set lib_files=%lib_files% ./lib/libglew32d.lib
set lib_files=%lib_files% opengl32.lib
set lib_files=%lib_files% kernel32.lib
set lib_files=%lib_files% user32.lib
set lib_files=%lib_files% gdi32.lib
set lib_files=%lib_files% winspool.lib
set lib_files=%lib_files% comdlg32.lib
set lib_files=%lib_files% advapi32.lib
set lib_files=%lib_files% shell32.lib
set lib_files=%lib_files% ole32.lib
set lib_files=%lib_files% oleaut32.lib
set lib_files=%lib_files% uuid.lib
set lib_files=%lib_files% odbc32.lib
set lib_files=%lib_files% odbccp32.lib
set lib_files=%lib_files% msvcrtd.lib

set src_files=./src/*.cpp ./src/sb7/*

cl %src_files% %include_path% %lib_path% %lib_files% %output_path%
start ./engine.exe

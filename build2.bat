rem del *.obj
del *.exe
del *.ilk
del *.pdb

set include_path=
set include_path=%include_path% /I "./include/"
set include_path=%include_path% /I "./include/sb7"
set include_path=%include_path% /I "D:/Python27_x86/include"
set include_path=%include_path% /I "C:/Program Files (x86)/Windows Kits/10/Include/10.0.17763.0/um"
set include_path=%include_path% /I "C:/Program Files (x86)/Windows Kits/10/Include/10.0.17763.0/shared"
set include_path=%include_path% /I "C:/Program Files (x86)/Windows Kits/10/Include/10.0.17763.0/ucrt"
set include_path=%include_path% /I "C:/Program Files (x86)/Windows Kits/10/Include/10.0.17763.0/winrt"
set include_path=%include_path% /I "C:/Program Files (x86)/Windows Kits/10/Include/10.0.17763.0/cppwinrt"
set include_path=%include_path% /I "C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/atlmfc/include"
set include_path=%include_path% /I "C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/include"

set lib_path=
set lib_path=%lib_path% /link
set lib_path=%lib_path% /LIBPATH:"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.17763.0/ucrt/x86"
set lib_path=%lib_path% /LIBPATH:"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.17763.0/um/x86"
set lib_path=%lib_path% /LIBPATH:"C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Tools/MSVC/14.16.27023/lib/x86"
set lib_path=%lib_path% /LIBPATH:"D:/Python27_x86/libs"

set output_path=/OUT:./engine.exe

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
REM set lib_files=%lib_files% python27.lib
REM set lib_files=%lib_files% _ctypes.lib
REM set lib_files=%lib_files% _msi.lib
REM set lib_files=%lib_files% _bsddb.lib
REM set lib_files=%lib_files% bz2.lib
REM set lib_files=%lib_files% pyexpat.lib
REM set lib_files=%lib_files% unicodedata.lib
REM set lib_files=%lib_files% winsound.lib
REM set lib_files=%lib_files% select.lib

set src_files=./src/*.cpp ./src/sb7/* ./src/model_loader/* ./src/GLObject/* ./src/utils/*
set cl_exe="C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Tools/MSVC/14.16.27023/bin/Hostx86/x86/cl.exe"
set cl_exe=%cl_exe% /Zi /EHsc

%cl_exe% %src_files% %include_path% %lib_path% %lib_files% %output_path%
start ./engine.exe

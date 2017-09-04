@echo off
echo ------ Running C++/WinRT compiler ------
setlocal
set compiler=..\..\..\cppwinrt\compiler\cppwinrt.exe
if not exist %compiler% (
set compiler=..\..\..\cppwinrt\compiler\cppwinrt.exe
)
%compiler% %* -verbose
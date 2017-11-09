@echo off
rem 使用方法：zipres XML路径 zip文件名
set PATH=%PATH%;"C:\Program Files (x86)\WinRAR"
pushd %1
if exist %2 del %2
ping -n 3 127.0.0.1>nul
call winrar a -r -afzip -tl %2
copy %2 ..\..\res
del %2
popd
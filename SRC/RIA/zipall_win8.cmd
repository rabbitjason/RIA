@echo off
cmd /c zipicowin8.cmd
copy /y XML\Win8���\*.xml XML\Win8���2
copy /y XML\Win8���\*.xml XML\Win8���3
rem copy /y XML\Win8���\*.xml XML\Win8���4
cmd /c "zipres.cmd XML\Win8��� res.zip"
cmd /c "zipres.cmd XML\Win8���2 res2.zip"
cmd /c "zipres.cmd XML\Win8���3 res3.zip"
rem cmd /c "zipres.cmd XML\Win8���4 res4.zip"

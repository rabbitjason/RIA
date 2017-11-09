@echo off
cmd /c zipicowin8.cmd
copy /y XML\Win8风格\*.xml XML\Win8风格2
copy /y XML\Win8风格\*.xml XML\Win8风格3
rem copy /y XML\Win8风格\*.xml XML\Win8风格4
cmd /c "zipres.cmd XML\Win8风格 res.zip"
cmd /c "zipres.cmd XML\Win8风格2 res2.zip"
cmd /c "zipres.cmd XML\Win8风格3 res3.zip"
rem cmd /c "zipres.cmd XML\Win8风格4 res4.zip"

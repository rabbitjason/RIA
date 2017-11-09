@echo off
cmd /c zipicosmart.cmd
copy /y XML\锤子风格\*.xml XML\锤子风格2
copy /y XML\锤子风格\*.xml XML\锤子风格3
rem copy /y XML\锤子风格\*.xml XML\锤子风格4
cmd /c "zipres.cmd XML\锤子风格 res.zip"
cmd /c "zipres.cmd XML\锤子风格2 res2.zip"
cmd /c "zipres.cmd XML\锤子风格3 res3.zip"
rem cmd /c "zipres.cmd XML\锤子风格4 res4.zip"

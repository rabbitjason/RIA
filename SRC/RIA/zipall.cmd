@echo off
cmd /c zipicosmart.cmd
copy /y XML\���ӷ��\*.xml XML\���ӷ��2
copy /y XML\���ӷ��\*.xml XML\���ӷ��3
rem copy /y XML\���ӷ��\*.xml XML\���ӷ��4
cmd /c "zipres.cmd XML\���ӷ�� res.zip"
cmd /c "zipres.cmd XML\���ӷ��2 res2.zip"
cmd /c "zipres.cmd XML\���ӷ��3 res3.zip"
rem cmd /c "zipres.cmd XML\���ӷ��4 res4.zip"

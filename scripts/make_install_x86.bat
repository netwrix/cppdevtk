REM x86, static, debug
xcopy .\..\include\*.hpp D:\cososys\x86\static\include /S /V /I /F /H /R /Y
xcopy .\..\include\*.h D:\cososys\x86\static\include /S /V /I /F /H /R /Y
xcopy .\..\build\windows\qt4\x86\static\debug\*.lib D:\cososys\x86\static\lib /S /V /I /F /H /R /Y
xcopy .\..\build\windows\qt4\x86\static\debug\*.prl D:\cososys\x86\static\lib /S /V /I /F /H /R /Y
REM xcopy .\..\build\windows\qt4\x86\static\debug\*.pdb D:\cososys\x86\static\lib /S /V /I /F /H /R /Y
xcopy .\..\build\windows\qt4\x86\static\debug\*.exe D:\cososys\x86\static\bin /S /V /I /F /H /R /Y
xcopy .\..\build\x86\qt5\static\debug\*.lib D:\cososys\x86\static\lib /S /V /I /F /H /R /Y
xcopy .\..\build\x86\qt5\static\debug\*.prl D:\cososys\x86\static\lib /S /V /I /F /H /R /Y
REM xcopy .\..\build\x86\qt5\static\debug\*.pdb D:\cososys\x86\static\lib /S /V /I /F /H /R /Y
xcopy .\..\build\x86\qt5\static\debug\*.exe D:\cososys\x86\static\bin /S /V /I /F /H /R /Y

REM x86, static, release
xcopy .\..\include\*.hpp D:\cososys\x86\static\include /S /V /I /F /H /R /Y
xcopy .\..\include\*.h D:\cososys\x86\static\include /S /V /I /F /H /R /Y
xcopy .\..\build\windows\qt4\x86\static\release\*.lib D:\cososys\x86\static\lib /S /V /I /F /H /R /Y
xcopy .\..\build\windows\qt4\x86\static\release\*.prl D:\cososys\x86\static\lib /S /V /I /F /H /R /Y
REM xcopy .\..\build\windows\qt4\x86\static\release\*.pdb D:\cososys\x86\static\lib /S /V /I /F /H /R /Y
xcopy .\..\build\windows\qt4\x86\static\release\*.exe D:\cososys\x86\static\bin /S /V /I /F /H /R /Y
xcopy .\..\build\x86\qt5\static\release\*.lib D:\cososys\x86\static\lib /S /V /I /F /H /R /Y
xcopy .\..\build\x86\qt5\static\release\*.prl D:\cososys\x86\static\lib /S /V /I /F /H /R /Y
REM xcopy .\..\build\x86\qt5\static\release\*.pdb D:\cososys\x86\static\lib /S /V /I /F /H /R /Y
xcopy .\..\build\x86\qt5\static\release\*.exe D:\cososys\x86\static\bin /S /V /I /F /H /R /Y

REM x86, shared, debug
xcopy .\..\include\*.hpp D:\cososys\x86\shared\include /S /V /I /F /H /R /Y
xcopy .\..\include\*.h D:\cososys\x86\shared\include /S /V /I /F /H /R /Y
xcopy .\..\build\x86\qt4\shared\debug\*.lib D:\cososys\x86\shared\lib /S /V /I /F /H /R /Y
xcopy .\..\build\x86\qt4\shared\debug\*.prl D:\cososys\x86\shared\lib /S /V /I /F /H /R /Y
xcopy .\..\build\x86\qt4\shared\debug\*.pdb D:\cososys\x86\shared\bin /S /V /I /F /H /R /Y
xcopy .\..\build\x86\qt4\shared\debug\*.dll D:\cososys\x86\shared\bin /S /V /I /F /H /R /Y
xcopy .\..\build\x86\qt4\shared\debug\*.exe D:\cososys\x86\shared\bin /S /V /I /F /H /R /Y
xcopy .\..\build\x86\qt5\shared\debug\*.lib D:\cososys\x86\shared\lib /S /V /I /F /H /R /Y
xcopy .\..\build\x86\qt5\shared\debug\*.prl D:\cososys\x86\shared\lib /S /V /I /F /H /R /Y
xcopy .\..\build\x86\qt5\shared\debug\*.pdb D:\cososys\x86\shared\bin /S /V /I /F /H /R /Y
xcopy .\..\build\x86\qt5\shared\debug\*.dll D:\cososys\x86\shared\bin /S /V /I /F /H /R /Y
xcopy .\..\build\x86\qt5\shared\debug\*.exe D:\cososys\x86\shared\bin /S /V /I /F /H /R /Y

REM x86, shared, release
xcopy .\..\include\*.hpp D:\cososys\x86\shared\include /S /V /I /F /H /R /Y
xcopy .\..\include\*.h D:\cososys\x86\shared\include /S /V /I /F /H /R /Y
xcopy .\..\build\x86\qt4\shared\release\*.lib D:\cososys\x86\shared\lib /S /V /I /F /H /R /Y
xcopy .\..\build\x86\qt4\shared\release\*.prl D:\cososys\x86\shared\lib /S /V /I /F /H /R /Y
REM xcopy .\..\build\x86\qt4\shared\release\*.pdb D:\cososys\x86\shared\bin /S /V /I /F /H /R /Y
xcopy .\..\build\x86\qt4\shared\release\*.dll D:\cososys\x86\shared\bin /S /V /I /F /H /R /Y
xcopy .\..\build\x86\qt4\shared\release\*.exe D:\cososys\x86\shared\bin /S /V /I /F /H /R /Y
xcopy .\..\build\x86\qt5\shared\release\*.lib D:\cososys\x86\shared\lib /S /V /I /F /H /R /Y
xcopy .\..\build\x86\qt5\shared\release\*.prl D:\cososys\x86\shared\lib /S /V /I /F /H /R /Y
REM xcopy .\..\build\x86\qt5\shared\release\*.pdb D:\cososys\x86\shared\bin /S /V /I /F /H /R /Y
xcopy .\..\build\x86\qt5\shared\release\*.dll D:\cososys\x86\shared\bin /S /V /I /F /H /R /Y
xcopy .\..\build\x86\qt5\shared\release\*.exe D:\cososys\x86\shared\bin /S /V /I /F /H /R /Y

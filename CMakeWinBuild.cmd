


set CMAKE_SYSTEM_NAME=Windows
set PROCESS_ARCHITECTUR=x64
set BUILD_DIR=build%CMAKE_SYSTEM_NAME%





set CMAKE_BUILD_TYPE=Debug
cd %BUILD_DIR%\%PROCESS_ARCHITECTUR%
cmake --build . --parallel --target install  -- /p:Configuration=%CMAKE_BUILD_TYPE%

if ERRORLEVEL 1 goto exit

robocopy lib ..\%PROCESS_ARCHITECTUR%%CMAKE_BUILD_TYPE%\lib  /purge /move
robocopy bin ..\%PROCESS_ARCHITECTUR%%CMAKE_BUILD_TYPE%\bin  /purge /move
robocopy ..\%PROCESS_ARCHITECTUR%%CMAKE_BUILD_TYPE%\bin ..\..\Server\bin\%PROCESS_ARCHITECTUR%%CMAKE_BUILD_TYPE% /move






set CMAKE_BUILD_TYPE=RelWithDebInfo
cmake --build . --parallel --target install -- /p:Configuration=%CMAKE_BUILD_TYPE% 

if ERRORLEVEL 1 goto exit

robocopy lib ..\%PROCESS_ARCHITECTUR%%CMAKE_BUILD_TYPE%\lib  /purge /move
robocopy bin ..\%PROCESS_ARCHITECTUR%%CMAKE_BUILD_TYPE%\bin  /purge /move
robocopy ..\%PROCESS_ARCHITECTUR%%CMAKE_BUILD_TYPE%\bin ..\..\Server\bin\%PROCESS_ARCHITECTUR%%CMAKE_BUILD_TYPE% /move



:exit


call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x86_amd64

xsd /classes /namespace:BR.Config /language:CS config.xsd /out:MonitorConfig

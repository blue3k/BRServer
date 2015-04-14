
call "C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat"

echo creating object bindings to schemas
xsd /c /namespace:xmldata ProtocolDef.xsd 

pause

// dllmain.h : Declaration of module class.

class CSRWMIPerfModule : public CAtlDllModuleT< CSRWMIPerfModule >
{
public :
	DECLARE_LIBID(LIBID_SRWMIPerfLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_SRWMIPERF, "{470169C2-5841-4EC9-8042-7A9E219F09C6}")
};

extern class CSRWMIPerfModule _Module;


//extern CComModule _Module;


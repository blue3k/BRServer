////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Brave base types - binary
//
////////////////////////////////////////////////////////////////////////////////



// Constructor
Binary::Binary( UINT32 uiMaxSize )
	: MaxSize(uiMaxSize)
	, Magic(0)
	, Version(0)
	, Size(0)
{
}

Binary::Binary( UINT32 uiMaxSize, UINT32 uiMagic, UINT32 uiVersion )
	: MaxSize(uiMaxSize)
	, Magic(uiMagic)
	, Version(uiVersion)
	, Size(0)
{
}

//// Conversion to BLOB
//Binary::operator BLOB()
//{
//	BLOB blob;
//	blob.cbSize = Size;
//	blob.pBlobData = (BYTE*)this;
//};

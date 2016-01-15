////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Utility library
//	
//
////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////
//
//	Random utility functions
//

// Generate new random number
UINT RandomGen::Rand( UINT iMax )
{
	return Rand()%(iMax+1);
}

// Generate new random number
INT RandomGen::Rand( INT iMin, INT iMax )
{
	if( iMax <= iMin )
		return iMin;
	return iMin + (INT)( Rand()%(iMax - iMin + 1) );
}



// Generate new random number
UINT RandomGenLinear::Rand( UINT iMax )
{
	return Rand()%iMax;
}

// Generate new random number
INT RandomGenLinear::Rand( INT iMin, INT iMax )
{
	if( iMax <= iMin )
		return iMin;
	return iMin + (INT)( Rand()%(iMax-iMin) );
}



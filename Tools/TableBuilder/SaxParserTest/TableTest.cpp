// TestApp.cpp : Defines the entry point for the console application.

//#include "stdafx.h"
//#include <gtest/gtest.h>
//#include "Common/StrUtil.h"
//#include "Common/StrFormat.h"
//#include "TableTest.h"

#include "XMLParserHandler.h"
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include <iostream>
#include "windows.h"
#include "ItemTbl.h"




//using ::testing::EmptyTestEventListener;
//using ::testing::InitGoogleTest;
//using ::testing::Test;
//using ::testing::TestCase;
//using ::testing::TestEventListeners;
//using ::testing::TestInfo;
//using ::testing::TestPartResult;
//using ::testing::UnitTest; 





namespace SRTestApp
{


class PrintParser : public SR::XMLParserHandler
{
public:



	virtual void StartElementNs( 
								const xmlChar * localname, 
								const xmlChar * prefix, 
								const xmlChar * URI, 
								int nb_namespaces, 
								const xmlChar ** namespaces, 
								int nb_attributes, 
								int nb_defaulted, 
								const xmlChar ** attributes )
	{
		printf( "startElementNs: name = '%s' \n", localname );
		for ( int indexNamespace = 0; indexNamespace < nb_namespaces; ++indexNamespace )
		{
			const xmlChar *prefix = namespaces[indexNamespace*2];
			const xmlChar *nsURI = namespaces[indexNamespace*2+1];
			printf( "  namespace: name='%s' uri=(%p)'%s'\n", prefix, nsURI, nsURI );
		}

		unsigned int index = 0;
		for ( int indexAttribute = 0; 
			indexAttribute < nb_attributes; 
			++indexAttribute, index += 5 )
		{
			const xmlChar *localname = attributes[index];
			const xmlChar *prefix = attributes[index+1];
			const xmlChar *nsURI = attributes[index+2];
			const xmlChar *valueBegin = attributes[index+3];
			const xmlChar *valueEnd = attributes[index+4];
			std::string value( (const char *)valueBegin, (const char *)valueEnd );

			printf( "  %sattribute: localname='%s', value='%s'\n",
					indexAttribute >= (nb_attributes - nb_defaulted) ? "defaulted " : "",
					localname,
					value.c_str() );
		}
	}


	virtual void EndElementNs( 
								const xmlChar * localname, 
								const xmlChar * prefix, 
								const xmlChar * URI )
	{
		printf( "endElementNs: name = '%s' \n", localname );
	}

	virtual void Error( 
						const char * msg, 
						... ) 
	{
		va_list args;
		va_start(args, msg);
		vprintf( msg, args );
		va_end(args);
	}


	/** Display and format a warning messages, callback.
		ctx:  an XML parser context
		msg:  the message to display/transmit
		...:  extra parameters for the message display
		*/
	virtual void Warning( 
						const char * msg, 
						... )
	{
		va_list args;
		va_start(args, msg);
		vprintf( msg, args );
		va_end(args);
	}
};


struct ItemList
{
	unsigned int ItemID;
	unsigned int NameID;
	std::string Memo;
};


class ItemParser : public SR::XMLParserHandler
{
public:
	enum ATT_ID
	{
		EATT_ItemID,
		EATT_NameID,
		EATT_Memo,
		EATT_MainCategory,
	};


private:

public:
	ItemParser()
	{
		AddElementHandler( "ItemList", reinterpret_cast<ElementStartHandler>(&ItemParser::StartItemList), reinterpret_cast<ElementEndHandler>(&ItemParser::EndItemList) );

		AddAttributeID( "ItemID", EATT_ItemID );
		AddAttributeID( "NameID", EATT_NameID );
		AddAttributeID( "Memo", EATT_Memo );
		AddAttributeID( "MainCategory", EATT_MainCategory );
	}



	void StartItemList( 
								const xmlChar * localname, 
								const xmlChar * prefix, 
								const xmlChar * URI, 
								int nb_namespaces, 
								const xmlChar ** namespaces, 
								int nb_attributes, 
								int nb_defaulted, 
								const xmlChar ** attributes )
	{
		ItemList *pNewItemList = new ItemList;

		unsigned int index = 0;
		for ( int indexAttribute = 0; 
			indexAttribute < nb_attributes; 
			++indexAttribute, index += 5 )
		{
			const xmlChar *attName = attributes[index];
			const xmlChar *prefix = attributes[index+1];
			const xmlChar *nsURI = attributes[index+2];
			const xmlChar *valueBegin = attributes[index+3];
			const xmlChar *valueEnd = attributes[index+4];
			std::string value( (const char *)valueBegin, (const char *)valueEnd );

			switch( GetAttributeID((char*)attName) )
			{
			case EATT_ItemID:
				pNewItemList->ItemID = atoi( value.c_str() );
				break;
			case EATT_NameID:
				pNewItemList->NameID = atoi( value.c_str() );
				break;
			case EATT_Memo:
				pNewItemList->Memo = value;
				break;
			case EATT_MainCategory:
				// mapping to enum
				// ...
				break;
			};

		}

		printf("ItemList ItemID:%0%, NameID:%1%, Memo:%2%", 
			pNewItemList->ItemID, pNewItemList->NameID, pNewItemList->Memo.c_str() );
	}


	void EndItemList( 
								const xmlChar * localname, 
								const xmlChar * prefix, 
								const xmlChar * URI )
	{
		// pNewItemList를 테이블에넣기
		printf("EndItemList");
	}


	virtual void Error( 
						const char * msg, 
						... ) 
	{
		va_list args;
		va_start(args, msg);
		vprintf( msg, args );
		va_end(args);
	}


	/** Display and format a warning messages, callback.
		ctx:  an XML parser context
		msg:  the message to display/transmit
		...:  extra parameters for the message display
		*/
	virtual void Warning( 
						const char * msg, 
						... )
	{
		va_list args;
		va_start(args, msg);
		vprintf( msg, args );
		va_end(args);
	}
};



//TEST_F(TableTest, LibXML)
//{
//
//	LIBXML_TEST_VERSION
//
//
//	//PrintParser parser;
//	ItemParser parser;
//	int result = xmlSAXUserParseFile( parser, &parser, "Data/Item.xml" );
//	if ( result != 0 )
//	{
//		defTrace( Trace::TRC_TRACE, "Failed to parse document.\n" );
//		return;
//	}
//
//	/*
//	* Cleanup function for the XML library.
//	*/
//	xmlCleanupParser();
//	/*
//	* this is to debug memory for regression tests
//	*/
//	xmlMemoryDump();
//
//
//}




}  // namespace SRTestApp

using namespace SRTestApp;

//HRESULT LoadTable( const char *strFileName )
//{
//	GameTbl::ItemParser parser;
//	int result = xmlSAXUserParseFile( parser, &parser, "Data/Item.xml" );
//	if ( result != 0 )
//	{
//		printf("Failed to parse document.\n");
//		return E_FAIL;
//	}
//
//	/*
//	* Cleanup function for the XML library.
//	*/
//	xmlCleanupParser();
//	/*
//	* this is to debug memory for regression tests
//	*/
//	xmlMemoryDump();
//
//	return S_OK;
//}

//void main()
//{
//	LIBXML_TEST_VERSION
//
//	LoadTable("Data/Item.xml");	
//}


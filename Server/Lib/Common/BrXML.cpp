////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : XML Parser Handler
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/BrXML.h"
#include "Common/Trace.h"



namespace BR {
namespace XML {

	// Constructor/Destructors
	DOMElement::DOMElement()
	{
	}

	DOMElement::DOMElement( const char* strTypeName )
		:m_TypeName(strTypeName)
	{
	}

	DOMElement::~DOMElement()
	{
		RemoveAllChildren();
	}

	// Get child element by name
	DOMElement* DOMElement::GetChildElement( const std::string& childName )
	{
		ChildrenList::iterator itFound = m_Children.find(childName);
		if( itFound != m_Children.end() )
			return itFound->second;
		return nullptr;
	}

	// Child management
	void DOMElement::AddChild( DOMElement *pChild )
	{
		if( pChild == nullptr || pChild->GetName().length() == 0 )
		{
			AssertRel(false);
			return;
		}

		m_Children.insert( std::make_pair(pChild->GetName(), pChild) );
	}

	void DOMElement::RemoveChild( DOMElement *pChild )
	{
		ChildrenList::iterator itFound = m_Children.find( pChild->GetName() );
		if( itFound != m_Children.end() )
		{
			m_Children.erase( itFound );
		}
	}

	void DOMElement::RemoveAllChildren()
	{
		std::for_each( m_Children.begin(), m_Children.end(), 
			[]( std::pair<const std::string,DOMElement*>& iter ) {
				DOMElement* pChild = iter.second;
				if( pChild != nullptr )
					delete pChild;
			}
		);
		m_Children.clear();
	}

	
	// for parsing
	bool DOMElement::SetAttributeValue( const std::string& name, const std::string& value )
	{
		if( name == "xmlns"
			|| name == "schemaLocation" )
		{
			return true;
		}
		else
		{
			defTrace( Trace::TRC_ERROR, "Invalid element:%0%, Att:%1%", GetName(), name );
			return false;
		}
	}
	


	// Register element creator
	HRESULT XMLParserHandler::RegisterElementCreator( const std::string& elementName, std::function<DOMElement*()> funcCreator )
	{
		if( m_CreatorMap.insert( std::make_pair( elementName, funcCreator ) ).second )
			return S_OK;

		return E_FAIL;
	}

	/** Display and format an error messages, callback.
		ctx:  an XML parser context
		msg:  the message to display/transmit
		...:  extra parameters for the message display
		*/
	void XMLParserHandler::Error( 
						const char * msg, 
						... )
	{
		char strBuff[1024];
		va_list args;
		va_start(args, msg);
		vsnprintf( strBuff, sizeof(strBuff), msg, args );
		va_end(args);

		defTrace( Trace::TRC_ERROR, "XML Parse Error : %0%", strBuff );
	}


	/** Display and format a warning messages, callback.
		ctx:  an XML parser context
		msg:  the message to display/transmit
		...:  extra parameters for the message display
		*/
	void XMLParserHandler::Warning( 
						const char * msg, 
						... )
	{
		char strBuff[1024];
		va_list args;
		va_start(args, msg);
		vsnprintf( strBuff, sizeof(strBuff), msg, args );
		va_end(args);

		defTrace( Trace::TRC_ERROR, "XML Parse Warning : %0%", strBuff );
	}

	/** SAX2 callback when an element start has been detected by the parser. It provides the namespace informations for the element, as well as the new namespace declarations on the element.
		ctx:  the user data (XML parser context)
		localname:  the local name of the element
		prefix:  the element namespace prefix if available
		URI:  the element namespace name if available
		nb_namespaces: number of namespace definitions on that node
		namespaces: pointer to the array of prefix/URI pairs namespace definitions
		nb_attributes: the number of attributes on that node
		nb_defaulted:  the number of defaulted attributes. The defaulted ones are at the end of the array
		attributes: pointer to the array of (localname/prefix/URI/value/end) attribute values.
		*/
	void XMLParserHandler::StartElementNs( 
								const xmlChar * localname, 
								const xmlChar * prefix, 
								const xmlChar * URI, 
								int nb_namespaces, 
								const xmlChar ** namespaces, 
								int nb_attributes, 
								int nb_defaulted, 
								const xmlChar ** attributes )
	{
		CreatorMap::iterator itFound = m_CreatorMap.find( std::string((char*)localname) );
		AssertRel( itFound != m_CreatorMap.end() );

		DOMElement* pNewElement = itFound->second();
		pNewElement->SetName( (char*)localname );

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

			// ignore schema
			if (StrUtil::StringCmpLwr((const char*)attName, -1, "noNamespaceSchemaLocation", -1) == 0)
				continue;

			if( !pNewElement->SetAttributeValue( (char*)attName, value ) )
			{
				defTrace( Trace::TRC_ERROR, "Invalid XML Element:%0%, Att:%1%", (const char*)localname, (const char*)attName );
			}
		}

		PushDOMElement( pNewElement );
	}


	/** SAX2 callback when an element end has been detected by the parser. It provides the namespace informations for the element.
		ctx:  the user data (XML parser context)
		localname:  the local name of the element
		prefix:  the element namespace prefix if available
		URI:  the element namespace name if available
		*/
	void XMLParserHandler::EndElementNs( 
								const xmlChar * localname, 
								const xmlChar * prefix, 
								const xmlChar * URI )
	{
		DOMElement* pElement = m_DOMStack.top();
		// leave last one for use
		if( pElement != nullptr )
		{
			pElement->OnLoadDone();
			PopDOMElement();
		}
	}


}; // namespace XML
}; // namespace BR


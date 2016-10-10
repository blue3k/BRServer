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



#pragma once


#include "Common/Typedefs.h"
#include "Common/BrAssert.h"
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <string>
#include <stack>



namespace BR {
namespace XML {

	////////////////////////////////////////////////////////////////
	//
	//	XML basic element
	//

	class DOMElement
	{
	public:
		// Children list
		typedef std::unordered_map<std::string,DOMElement*> ChildrenList;

	public:
		// Constructor/Destructors
		DOMElement();
		DOMElement( const char* strTypeName );
		virtual ~DOMElement();

		// name
		inline const std::string& GetName();
		virtual void SetName( const char* Name );

		// Type name
		inline const std::string& GetTypeName();

		// Child management
		virtual void AddChild( DOMElement *pChild );
		virtual void RemoveChild( DOMElement *pChild );
		virtual void RemoveAllChildren();

		// Get child element by name
		virtual DOMElement* GetChildElement( const std::string& childName );

		// for parsing
		virtual bool SetAttributeValue( const std::string& name, const std::string& value );

		// Called when the load done
		virtual bool OnLoadDone() { return true; }

	private:
		// Name of DOM
		std::string		m_Name;

		// Type of DOM
		std::string		m_TypeName;

		// chieldren
		ChildrenList	m_Children;
	};



	////////////////////////////////////////////////////////////////////
	//
	// XML Parser
	//

	class XMLParserHandler
	{
	public:

		// Element creator map
		typedef std::unordered_map<std::string,std::function<DOMElement*()>>	CreatorMap;

	private:
		// SAX handler for libxml,  See http://xmlsoft.org/html/libxml-tree.html#xmlSAXHandler
		xmlSAXHandler				m_saxHandler;

		// Creator Map
		CreatorMap					m_CreatorMap;

		// DOM processing stack
		std::stack<DOMElement*>		m_DOMStack;

	public:

		XMLParserHandler()
		{
			memset( &m_saxHandler, 0, sizeof(m_saxHandler) );

			m_saxHandler.initialized = XML_SAX2_MAGIC;  // so we do this to force parsing as SAX2.
			m_saxHandler.startElementNs = &XMLParserHandler::StartElementNs;
			m_saxHandler.endElementNs = &XMLParserHandler::EndElementNs;
			m_saxHandler.warning = &XMLParserHandler::Warning;
			m_saxHandler.error = &XMLParserHandler::Error;
		}

		operator xmlSAXHandler*() { return &m_saxHandler; }

		// Push Dom element
		inline const std::string& GetCurDOMName();

		// Push Dom element
		inline DOMElement* GetCurDOM();

		// Register element creator
		Result RegisterElementCreator( const std::string& elementName, std::function<DOMElement*()> funcCreator );

	protected:

		// Register Creator

		// Push Dom element
		inline size_t PushDOMElement( DOMElement* DOMObject );

		// Pop Dom element
		inline DOMElement* PopDOMElement();


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
		virtual void StartElementNs( 
									const xmlChar * localname, 
									const xmlChar * prefix, 
									const xmlChar * URI, 
									int nb_namespaces, 
									const xmlChar ** namespaces, 
									int nb_attributes, 
									int nb_defaulted, 
									const xmlChar ** attributes );


		/** SAX2 callback when an element end has been detected by the parser. It provides the namespace informations for the element.
			ctx:  the user data (XML parser context)
			localname:  the local name of the element
			prefix:  the element namespace prefix if available
			URI:  the element namespace name if available
			*/
		virtual void EndElementNs( 
									const xmlChar * localname, 
									const xmlChar * prefix, 
									const xmlChar * URI );


		/** Display and format an error messages, callback.
			ctx:  an XML parser context
			msg:  the message to display/transmit
			...:  extra parameters for the message display
			*/
		virtual void Error( 
							const char * msg, 
							... );


		/** Display and format a warning messages, callback.
			ctx:  an XML parser context
			msg:  the message to display/transmit
			...:  extra parameters for the message display
			*/
		virtual void Warning( 
							const char * msg, 
							... );






		/////////////////////////////////////////////////////////////////////////////////////
		//
		//	Callbacks
		//

		static void StartElementNs( void * ctx, 
									const xmlChar * localname, 
									const xmlChar * prefix, 
									const xmlChar * URI, 
									int nb_namespaces, 
									const xmlChar ** namespaces, 
									int nb_attributes, 
									int nb_defaulted, 
									const xmlChar ** attributes )
		{
			XMLParserHandler &parser = *( static_cast<XMLParserHandler *>( ctx ) );
			parser.StartElementNs( localname, prefix, URI, nb_namespaces, namespaces, nb_attributes, nb_defaulted, attributes );
		}


		static void EndElementNs( void * ctx, 
									const xmlChar * localname, 
									const xmlChar * prefix, 
									const xmlChar * URI )
		{
			XMLParserHandler &parser = *( static_cast<XMLParserHandler *>( ctx ) );
			parser.EndElementNs( localname, prefix, URI );
		}


		static void Error( void * ctx, 
							const char * msg, 
							... )
		{
			XMLParserHandler &parser = *( static_cast<XMLParserHandler *>( ctx ) );
			parser.Error( msg );
		}


		static void Warning( void * ctx, 
							const char * msg, 
							... )
		{
			XMLParserHandler &parser = *( static_cast<XMLParserHandler *>( ctx ) );
			parser.Warning( msg );
		}
	};


#include "BrXML.inl"

} // namespace XML 
}  // namespace BR



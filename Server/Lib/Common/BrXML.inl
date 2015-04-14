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




// name
const std::string& DOMElement::GetName()
{
	return m_Name;
}

void DOMElement::SetName( const char* Name )
{
	m_Name = Name;
}

// Type name
const std::string& DOMElement::GetTypeName()
{
	return m_TypeName;
}



// Push Dom element
size_t XMLParserHandler::PushDOMElement( DOMElement* DOMObject )
{
	Assert(DOMObject != nullptr);
	if( m_DOMStack.size() > 0 )
	{
		m_DOMStack.top()->AddChild( DOMObject );
	}
	m_DOMStack.push( DOMObject );

	return m_DOMStack.size();
}

// Pop Dom element
DOMElement* XMLParserHandler::PopDOMElement()
{
	if( m_DOMStack.size() == 0 )
	{
		return nullptr;
	}

	DOMElement* DOMObject = m_DOMStack.top();
	m_DOMStack.pop();

	return DOMObject;
}

		
// Push Dom element
const std::string& XMLParserHandler::GetCurDOMName()
{
	static std::string NoElement = "";

	if( m_DOMStack.size() == 0 )
	{
		return NoElement;
	}

	return m_DOMStack.top()->GetName();
}

// Push Dom element
DOMElement* XMLParserHandler::GetCurDOM()
{
	if( m_DOMStack.size() == 0 )
	{
		return nullptr;
	}

	return m_DOMStack.top();
}



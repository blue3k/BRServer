////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : KyungKun Ko
//
// Description : Argument
//	
//
////////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
#include "Common/Typedefs.h"
#include "Common/Argument.h"
#include "Common/ToString.h"


namespace BR
{
	
	////////////////////////////////////////////////////////////////////////////////
	//
	//	Argument wrapper base
	//

	int ArgBase::MyArgument = 0;


	Argument::Argument() 
		: m_pArgBase(nullptr)
	{
	}

	Argument::Argument(const ArgBase& src)
	{
		m_pArgBase = src.Clone(sizeof(m_DataBuffer), m_DataBuffer);
	}

	void Argument::MakeString(char*& pBuff, INT& iBuffLen) const
	{
		if (m_pArgBase)
		{
			m_pArgBase->ArgumentUsed();
			m_pArgBase->MakeString(pBuff, iBuffLen);
		}

	}

	std::string Argument::MakeString() const
	{
		char strBuff[1024] = "";
		INT Len = 1024;
		char *pBuff = strBuff;
		MakeString(pBuff, Len);
		return strBuff;
	}



/*
	template< >
	class Arg<double> : public ArgBase
	{
	private:
		double m_Data;
		unsigned m_digitAfterDecimalPoint;

	public:
		Arg(double src, unsigned digitAfterDecimalPoint = 5)
			:m_Data(src),
			m_digitAfterDecimalPoint(digitAfterDecimalPoint)
		{
		}

		Arg(const Arg& src)
			: m_Data(src.m_Data)
			, m_digitAfterDecimalPoint(src.m_digitAfterDecimalPoint)
		{
		}

		ArgBase* Clone(INT iBuffLen, BYTE* pBuff) const override
		{
			AssertRel(iBuffLen >= sizeof(Arg<double>));
			return new(pBuff)Arg<double>(*this);
		}

		virtual void MakeString(char*& pBuff, INT& iBuffLen) const
		{
			ToString<double>(pBuff, iBuffLen, m_Data, (int)(m_digitAfterDecimalPoint << 16));
		}
	};
*/





	template class Arg < INT8>;
	template class Arg < UINT8>;
	template class Arg < INT16>;
	template class Arg < UINT16>;
	template class Arg < INT32>;
	template class Arg < UINT32>;
	template class Arg < INT64>;
	template class Arg < UINT64>;
	template class Arg < LONG>;
	template class Arg < ULONG>;
	template class Arg < char>;
	template class Arg < LPCSTR>;
	template class Arg < LPSTR>;
	template class Arg < wchar_t>;
	template class Arg < LPCWSTR>;
	template class Arg < LPWSTR>;
	template class Arg < std::string>;
	template class Arg < std::wstring>;


}; // namespace StrUtil




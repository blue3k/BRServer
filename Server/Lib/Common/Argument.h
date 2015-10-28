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


#pragma once

#include "Common/Typedefs.h"
#include "Common/BrAssert.h"
#include "Common/ToStringBase.h"


namespace BR {

	////////////////////////////////////////////////////////////////////////////////
	//
	//	Argument wrapper base
	//

	class ArgBase
	{
		// Code compile branch variable
		static int MyArgument;

	public:

		// dummy function to escape optimizer
		void ArgumentUsed()
		{
			MyArgument++;
		}

		virtual ArgBase* Clone(INT iBuffLen, BYTE* pBuff) const = 0;

		virtual void MakeString(char*& pBuff, INT& iBuffLen, char option, float digits) const = 0;
	};

	// Argument wrapper
	class Argument
	{
	public:

		enum {
			BUFFER_SIZE = 24
		};

	private:
		BYTE m_DataBuffer[BUFFER_SIZE];

		ArgBase* m_pArgBase;

	public:

		Argument();
		Argument(const ArgBase& src);

		std::string MakeString() const;

		// print to input buff and return 
		void MakeString(char*& pBuff, INT& iBuffLen, char option, float digits) const;
	};



	///////////////////////////////////////////////////////////////////////////////////
	//
	// Default type argument implementations
	//

	// Base template bridge
	// Type must implemente char* ToString( char*& pBuff, int& iBuffLen, Type Data )
	template< class Type >
	class Arg : public ArgBase
	{
	private:
		const Type& m_Data;

	public:
		Arg( const Type& src )
			:m_Data(src)
		{
		}

		Arg(const Arg& src)
			:m_Data(src.m_Data)
		{
		}

		ArgBase* Clone(INT iBuffLen, BYTE* pBuff) const override
		{
			AssertRel(iBuffLen >= sizeof(Arg<Type>));
			return new(pBuff) Arg<Type>(*this);
		}

		virtual void MakeString( char*& pBuff, INT& iBuffLen, char option, float digits) const
		{
			ToString(pBuff, iBuffLen, m_Data, 0);
		}
	};

	// UINT32 custom argument with option
	template< >
	class Arg<UINT32> : public ArgBase
	{
	private:
		UINT32 m_Data;

	public:
		Arg( UINT32 src, int MaxDigit = -1, int Radix = 10 )
			: m_Data(src)
		{
		}

		Arg(const Arg& src)
			: m_Data(src.m_Data)
		{
		}

		ArgBase* Clone(INT iBuffLen, BYTE* pBuff) const override
		{
			AssertRel(iBuffLen >= sizeof(decltype(this)));
			return new(pBuff) Arg<UINT32>(*this);
		}

		virtual void MakeString( char*& pBuff, INT& iBuffLen, char option, float digits) const
		{
			int radix = 10;
			if (option == 'x' || option == 'X') radix = 16;
			ToStringHex( pBuff, iBuffLen, m_Data, digits, radix);
		}
	};
 
	// UINT32 custom argument with option
	template< >
	class Arg<UINT64> : public ArgBase
	{
	private:
		UINT64 m_Data;

	public:
		Arg( UINT64 src, int MaxDigit = -1, int Radix = 10 )
			:m_Data(src)
		{
		}

		Arg(const Arg& src)
			: m_Data(src.m_Data)
		{
		}

		ArgBase* Clone(INT iBuffLen, BYTE* pBuff) const override
		{
			AssertRel(iBuffLen >= sizeof(Arg<UINT64>));
			return new(pBuff) Arg<UINT64>(*this);
		}

		virtual void MakeString( char*& pBuff, INT& iBuffLen, char option, float digits) const
		{
			int radix = 10;
			if (option == 'x' || option == 'X') radix = 16;
			ToStringHex(pBuff, iBuffLen, m_Data, digits, radix);
		}
	};
 
	
	// Float custom argument with option
	template< >
	class Arg<float> : public ArgBase
	{
	private:
		float m_Data;
		unsigned m_digitAfterDecimalPoint;

	public:
		Arg( float src, unsigned digitAfterDecimalPoint = 5 )
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
			AssertRel(iBuffLen >= sizeof(Arg<float>));
			return new(pBuff) Arg<float>(*this);
		}

		virtual void MakeString( char*& pBuff, INT& iBuffLen, char option, float digits) const
		{
			option;
			ToString( pBuff, iBuffLen, (float)m_Data, (int)digits );
		}
	};
 
	// Double custom argument with option
	template< >
	class Arg<double> : public ArgBase
	{
	private:
		double m_Data;

	public:
		Arg( double src )
			:m_Data(src)
		{
		}

		Arg(const Arg& src)
			: m_Data(src.m_Data)
		{
		}

		ArgBase* Clone(INT iBuffLen, BYTE* pBuff) const override
		{
			AssertRel(iBuffLen >= sizeof(Arg<double>));
			return new(pBuff) Arg<double>(*this);
		}

		virtual void MakeString( char*& pBuff, INT& iBuffLen, char option, float digits) const
		{
			option; digits;
			ToString<double>(pBuff, iBuffLen, m_Data, 0);
		}
	};
 
	
	// Double custom argument with option
	template< >
	class Arg<void*> : public ArgBase
	{
	private:
		void* m_Data;

	public:
		Arg( void* src )
			:m_Data(src)
		{
		}

		Arg(const Arg& src)
			: m_Data(src.m_Data)
		{
		}

		ArgBase* Clone(INT iBuffLen, BYTE* pBuff) const override
		{
			AssertRel(iBuffLen >= sizeof(Arg<void*>));
			return new(pBuff) Arg<void*>(*this);
		}

		virtual void MakeString( char*& pBuff, INT& iBuffLen, char option, float digits) const
		{
			option; digits;
			ToString(pBuff, iBuffLen, (void*)m_Data, 0);
		}
	};
 

	// Double custom argument with option
	template< class Type >
	class ArgArray : public ArgBase
	{
	private:
		const Array<Type>& m_Array;

	public:
		ArgArray(const Array<Type>& arrayData)
			: m_Array(arrayData)
		{
		}

		ArgArray(const ArgArray<Type>& src)
			: m_Array(src.m_Array)
		{
		}

		ArgBase* Clone(INT iBuffLen, BYTE* pBuff) const override
		{
			AssertRel(iBuffLen >= sizeof(ArgArray<Type>));
			return new(pBuff) ArgArray<Type>(*this);
		}

		virtual void MakeString( char*& pBuff, INT& iBuffLen, char option, float digits) const
		{
			option; digits;
			ToStringArray(pBuff, iBuffLen, m_Array, 0);
		}
	};



	//////////////////////////////////////////////////////////////////////////////////////////////////
	//
	//	Argument format specification
	//


	extern template class Arg < INT8>;
	extern template class Arg < UINT8>;
	extern template class Arg < INT16>;
	extern template class Arg < UINT16>;
	extern template class Arg < INT32>;
	extern template class Arg < UINT32>;
	extern template class Arg < INT64>;
	extern template class Arg < UINT64>;
	extern template class Arg < LONG>;
	extern template class Arg < ULONG>;
	extern template class Arg < char>;
	extern template class Arg < LPCSTR>;
	extern template class Arg < LPSTR>;
	extern template class Arg < wchar_t>;
	extern template class Arg < LPCWSTR>;
	extern template class Arg < LPWSTR>;
	extern template class Arg < std::string>;
	extern template class Arg < std::wstring>;



}; // namespace StrUtil




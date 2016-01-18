////////////////////////////////////////////////////////////////////////////////
// 
// CopyRight (c) 2013 The Braves
// 
// Author : MadK
//
// Description : Spin buffer for single in single out
//
////////////////////////////////////////////////////////////////////////////////



#define __BUFFERMODE__ SpinBuffer<T>::BUFFER


template <typename T>
SpinBuffer<T>::SpinBuffer(void)
{
	m_nWritePos = m_nReadPos = 0;
}

template <typename T>
SpinBuffer<T>::~SpinBuffer(void)
{

}

template <typename T>
HRESULT SpinBuffer<T>::Push(const T& item)
{
	// check a condtion 
	if((this->m_SpinBuffer[m_nWritePos].eBufferMode != __BUFFERMODE__::BUFFERSTATE_FREE))
	{
		return E_SYSTEM_FAIL;
	}

	// Change a mode : free->write (writing now...)
	m_SpinBuffer[m_nWritePos].eBufferMode = __BUFFERMODE__::BUFFERSTATE_WRITE;	// W-1

	// Write to the SpinBuffer
	m_SpinBuffer[m_nWritePos].item = item;									// W-2

	int nBeforePos = m_nWritePos;

	m_nWritePos = ( m_nWritePos + 1 )%BUFFER_LENGTH;
	//if(BUFFER_LENGTH == ++m_nWritePos)  m_nWritePos = 0;

	// change a mode : write->Read (writing complete...)
	m_SpinBuffer[nBeforePos].eBufferMode = __BUFFERMODE__::BUFFERSTATE_READ;	// W-3

	// Writing success!
	return S_OK; 
}

// Comsumer(reader) read from the Readbuffer
template <typename T>
HRESULT SpinBuffer<T>::Pop(T& item)
{
	// check a condtion 
	if((this->m_SpinBuffer[m_nReadPos].eBufferMode != __BUFFERMODE__::BUFFERSTATE_READ))
	{
		return E_SYSTEM_FAIL;
	}

	item = m_SpinBuffer[m_nReadPos].item;									// R-1

	// initializing
	//memset(&m_SpinBuffer[m_nReadPos].item, -1, sizeof(T));
	m_SpinBuffer[m_nReadPos].item = T();

	int nBeforePos = m_nReadPos;

	m_nReadPos = ( m_nReadPos + 1 )%BUFFER_LENGTH;
	//if(BUFFER_LENGTH == ++m_nReadPos)  m_nReadPos = 0;

	// change a mode
	m_SpinBuffer[nBeforePos].eBufferMode = __BUFFERMODE__::BUFFERSTATE_FREE;	// R-2
	
	// Reading success!
	return S_OK; 
}


#undef __BUFFERMODE_

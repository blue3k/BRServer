


tag_MsgNetCtrlBuffer& tag_MsgNetCtrlBuffer::operator = ( const tag_MsgNetCtrlBuffer& src )
{
	memcpy( this, &src, sizeof(src) );
	return *this;
}

tag_MsgNetCtrlBuffer& tag_MsgNetCtrlBuffer::operator = (void* src)
{
	assert(src == nullptr);
	memcpy(this, 0, sizeof(tag_MsgNetCtrlBuffer));
	return *this;
}

bool tag_MsgNetCtrlBuffer::operator == ( const tag_MsgNetCtrlBuffer& src ) const
{
	return msgID.ID == src.msgID.ID;
}

bool tag_MsgNetCtrlBuffer::operator != ( const tag_MsgNetCtrlBuffer& src ) const
{
	return msgID.ID != src.msgID.ID;
}


bool tag_MsgNetCtrlBuffer::operator == (void* src) const
{
	assert(src == nullptr);
	return msgID.ID == 0;
}

bool tag_MsgNetCtrlBuffer::operator != (void* src) const
{
	assert(src == nullptr);
	return msgID.ID != 0;
}







tag_MsgNetCtrlBuffer& tag_MsgNetCtrlBuffer::operator = ( const tag_MsgNetCtrlBuffer& src )
{
	memcpy( this, &src, sizeof(src) );
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





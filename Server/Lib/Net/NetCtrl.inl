


inline tag_MsgNetCtrlBuffer::tag_MsgNetCtrlBuffer(const tag_MsgNetCtrlBuffer& src)
{
	memcpy(this, &src, sizeof(src));
}

inline tag_MsgNetCtrlBuffer& tag_MsgNetCtrlBuffer::operator = ( const tag_MsgNetCtrlBuffer& src )
{
	memcpy( this, &src, sizeof(src) );
	return *this;
}

inline bool tag_MsgNetCtrlBuffer::operator == ( const tag_MsgNetCtrlBuffer& src ) const
{
	return msgID.ID == src.msgID.ID;
}

inline bool tag_MsgNetCtrlBuffer::operator != ( const tag_MsgNetCtrlBuffer& src ) const
{
	return msgID.ID != src.msgID.ID;
}





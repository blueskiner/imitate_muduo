#ifndef __NET_CALL_BACKS_H__
#define __NET_CALL_BACKS_H__

// ͨ���ص� ���ڿ���ʹ��boost::bind
class IChannelCallBack
{
public:
	virtual void onRecvEvent() {}
};

#endif	// __NET_CALL_BACKS_H__

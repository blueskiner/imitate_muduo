#ifndef __NET_CALL_BACKS_H__
#define __NET_CALL_BACKS_H__

// 通道回调 后期考虑使用boost::bind
class IChannelCallBack
{
public:
	virtual void onRecvEvent() {}
};

#endif	// __NET_CALL_BACKS_H__

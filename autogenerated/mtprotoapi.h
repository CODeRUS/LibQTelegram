#ifndef MTPROTOAPI_H
#define MTPROTOAPI_H

// Generated by APIGenerator 1.0
// DO NOT EDIT!!!

#include "../mtproto/dc/dc.h"
#include "../mtproto/mtprotorequest.h"
#include <QObject>
#include "../types/basic.h"
#include "../autogenerated/types/types.h"
#include "../autogenerated/mtproto/mtproto.h"

class MTProtoAPI: public QObject
{
	Q_OBJECT

	public:
		explicit MTProtoAPI(QObject* parent = 0);
		static void reqPq(DC* dc, TLInt128 nonce);
		static void reqDHParams(DC* dc, TLInt128 nonce, TLInt128 server_nonce, TLBytes p, TLBytes q, TLLong public_key_fingerprint, TLBytes encrypted_data);
		static void setClientDHParams(DC* dc, TLInt128 nonce, TLInt128 server_nonce, TLBytes encrypted_data);
		static void rpcDropAnswer(DC* dc, TLLong req_msg_id);
		static void getFutureSalts(DC* dc, TLInt num);
		static void ping(DC* dc, TLLong ping_id);
		static void pingDelayDisconnect(DC* dc, TLLong ping_id, TLInt disconnect_delay);
		static void destroySession(DC* dc, TLLong session_id);
		static void contestSaveDeveloperInfo(DC* dc, TLInt vk_id, TLString name, TLString phone_number, TLInt age, TLString city);

};

#endif // MTPROTOAPI_H

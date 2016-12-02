#ifndef RESPQ_H
#define RESPQ_H

// Generated by APIGenerator 1.0
// DO NOT EDIT!!!

#include "../../types/basic.h"
#include "../../types/telegramobject.h"

class ResPQ: public TelegramObject
{
	Q_OBJECT

	Q_PROPERTY(TLInt128 nonce READ nonce WRITE setNonce NOTIFY nonceChanged)
	Q_PROPERTY(TLInt128 serverNonce READ serverNonce WRITE setServerNonce NOTIFY serverNonceChanged)
	Q_PROPERTY(TLBytes pq READ pq WRITE setPq NOTIFY pqChanged)
	Q_PROPERTY(TLVector<TLLong> serverPublicKeyFingerprints READ serverPublicKeyFingerprints WRITE setServerPublicKeyFingerprints NOTIFY serverPublicKeyFingerprintsChanged)

	Q_ENUMS(Constructors)

	public:
		enum Constructors {
			CtorResPQ = 0x05162463,
		};

	public:
		explicit ResPQ(QObject* parent = 0);
		virtual void read(MTProtoStream* mtstream);
		virtual void write(MTProtoStream* mtstream);
	protected:
		virtual void compileFlags();
	public:
		TLInt128 nonce() const;
		void setNonce(TLInt128 nonce);
		TLInt128 serverNonce() const;
		void setServerNonce(TLInt128 server_nonce);
		TLBytes pq() const;
		void setPq(TLBytes pq);
		const TLVector<TLLong>& serverPublicKeyFingerprints() const;
		void setServerPublicKeyFingerprints(const TLVector<TLLong>& server_public_key_fingerprints);

	signals:
		void nonceChanged();
		void serverNonceChanged();
		void pqChanged();
		void serverPublicKeyFingerprintsChanged();

	private:
		TLInt128 _nonce;
		TLInt128 _server_nonce;
		TLBytes _pq;
		TLVector<TLLong> _server_public_key_fingerprints;

};

#endif // RESPQ_H

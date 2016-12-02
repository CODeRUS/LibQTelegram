// Generated by APIGenerator 1.0
// DO NOT EDIT!!!

#include "respq.h"


ResPQ::ResPQ(QObject* parent) : TelegramObject(parent)
{
	this->_constructorid = ResPQ::CtorResPQ;
}

void ResPQ::read(MTProtoStream* mtstream) 
{
	this->_constructorid = mtstream->readTLConstructor();
	
	Q_ASSERT((this->_constructorid == ResPQ::CtorResPQ));
	
	if(this->_constructorid == ResPQ::CtorResPQ)
	{
		this->_nonce = mtstream->readTLInt128();
		this->_server_nonce = mtstream->readTLInt128();
		this->_pq = mtstream->readTLBytes();
		mtstream->readTLVector<TLLong>(this->_server_public_key_fingerprints, false);
	}
}

void ResPQ::write(MTProtoStream* mtstream) 
{
	Q_ASSERT((this->_constructorid == ResPQ::CtorResPQ));
	
	this->compileFlags();
	mtstream->writeTLConstructor(this->_constructorid);
	
	if(this->_constructorid == ResPQ::CtorResPQ)
	{
		mtstream->writeTLInt128(this->_nonce);
		mtstream->writeTLInt128(this->_server_nonce);
		mtstream->writeTLBytes(this->_pq);
		mtstream->writeTLVector(this->_server_public_key_fingerprints, false);
	}
}

void ResPQ::compileFlags() 
{
	
}

TLInt128 ResPQ::nonce() const
{
	return this->_nonce;
}

void ResPQ::setNonce(TLInt128 nonce) 
{
	if(this->_nonce == nonce)
		return;

	this->_nonce = nonce;
	emit nonceChanged();
}

TLInt128 ResPQ::serverNonce() const
{
	return this->_server_nonce;
}

void ResPQ::setServerNonce(TLInt128 server_nonce) 
{
	if(this->_server_nonce == server_nonce)
		return;

	this->_server_nonce = server_nonce;
	emit serverNonceChanged();
}

TLBytes ResPQ::pq() const
{
	return this->_pq;
}

void ResPQ::setPq(TLBytes pq) 
{
	if(this->_pq == pq)
		return;

	this->_pq = pq;
	emit pqChanged();
}

const TLVector<TLLong>& ResPQ::serverPublicKeyFingerprints() const
{
	return this->_server_public_key_fingerprints;
}

void ResPQ::setServerPublicKeyFingerprints(const TLVector<TLLong>& server_public_key_fingerprints) 
{
	if(this->_server_public_key_fingerprints == server_public_key_fingerprints)
		return;

	this->_server_public_key_fingerprints = server_public_key_fingerprints;
	emit serverPublicKeyFingerprintsChanged();
}


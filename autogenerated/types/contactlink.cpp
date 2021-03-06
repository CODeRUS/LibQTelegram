// Generated by APIGenerator 1.0
// DO NOT EDIT!!!

#include "contactlink.h"

ContactLink::ContactLink(QObject* parent) : TelegramObject(parent)
{
	
}

void ContactLink::read(MTProtoStream* mtstream) 
{
	this->_constructorid = mtstream->readTLConstructor();
	
	Q_ASSERT((this->_constructorid == ContactLink::CtorContactLinkUnknown) ||
		 (this->_constructorid == ContactLink::CtorContactLinkNone) ||
		 (this->_constructorid == ContactLink::CtorContactLinkHasPhone) ||
		 (this->_constructorid == ContactLink::CtorContactLinkContact));
}

void ContactLink::write(MTProtoStream* mtstream) 
{
	Q_ASSERT((this->_constructorid == ContactLink::CtorContactLinkUnknown) ||
		 (this->_constructorid == ContactLink::CtorContactLinkNone) ||
		 (this->_constructorid == ContactLink::CtorContactLinkHasPhone) ||
		 (this->_constructorid == ContactLink::CtorContactLinkContact));
	
	this->compileFlags();
	mtstream->writeTLConstructor(this->_constructorid);
}

void ContactLink::compileFlags() 
{
	
}


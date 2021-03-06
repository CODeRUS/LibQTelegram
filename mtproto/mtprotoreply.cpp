#include "mtprotoreply.h"
#include "../crypto/aes.h"
#include "../types/byteconverter.h"

MTProtoReply::MTProtoReply(const QByteArray &data, DCConfig *dcconfig, QObject *parent) : MTProtoStream(data, parent), _dcconfig(dcconfig)
{
    this->_requestid = 0;
    this->_bodystart = 0;
    this->_sessionid = 0;

    if(this->length() == 4)
    {
        this->_errorcode = this->readTLInt();
        return;
    }
    else
        this->_errorcode = 0;

    this->_authorizationkeyid = this->readTLLong();

    if(this->_authorizationkeyid != 0)
        this->readEncryptedMessage();
    else
        this->readPlainMessage();
}

MTProtoReply::MTProtoReply(MTProtoMessage *mtmessage, DCConfig *dcconfig, QObject *parent): MTProtoStream(mtmessage->body(), parent), _dcconfig(dcconfig)
{
    this->_requestid = 0;
    this->_bodystart = 0;
    this->_messageid = mtmessage->msgId();
    this->_constructorid = mtmessage->constructorId();
}

MTProtoReply::MTProtoReply(const QByteArray &body, TLLong messageid, DCConfig *dcconfig, QObject *parent): MTProtoStream(body, parent), _dcconfig(dcconfig)
{
    this->_requestid = 0;
    this->_bodystart = 0;
    this->_messageid = messageid;
    this->_constructorid = this->peekTLConstructor();
}

MTProtoReply::~MTProtoReply()
{
}

bool MTProtoReply::isError() const
{
    return this->length() == 4;
}

DCConfig *MTProtoReply::config() const
{
    return this->_dcconfig;
}

TLInt MTProtoReply::errorCode() const
{
    return this->_errorcode;
}

TLLong MTProtoReply::requestId() const
{
    return this->_requestid;
}

TLLong MTProtoReply::sessionId() const
{
    return this->_sessionid;
}

TLLong MTProtoReply::messageId() const
{
    return this->_messageid;
}

TLConstructor MTProtoReply::constructorId() const
{
    return this->_constructorid;
}

QByteArray MTProtoReply::cbody() const
{
    return this->_data.mid(this->_bodystart);
}

QByteArray MTProtoReply::body()
{
    return this->_buffer.readAll();
}

void MTProtoReply::seekToBody()
{
    this->seek(this->_bodystart);
}

void MTProtoReply::setRequestId(TLLong requestid)
{
    this->_requestid = requestid;
}

void MTProtoReply::readPlainMessage()
{
    this->_messageid = this->readTLLong();

    Q_ASSERT((qAbs((this->_messageid % 4)) == 1) || (qAbs((this->_messageid % 4)) == 3));

    this->_messagelength = this->readTLInt();
    this->_bodystart = this->_buffer.pos();
    this->_constructorid = this->peekTLConstructor();
}

void MTProtoReply::readEncryptedMessage()
{
    Q_ASSERT(this->_dcconfig && (this->_dcconfig->dcid() > 0));

    TLInt128 msgkey = this->readTLInt128();
    TLBytes aeskey, aesiv, msgkeybytes = ByteConverter::serialize(msgkey);
    Aes::calculateAesKeys(this->_dcconfig->authorizationKey(), msgkeybytes, true, aeskey, aesiv);

    // Replace encoded data with the decoded one
    this->_data.replace(this->_buffer.pos(), this->_buffer.bytesAvailable(), Aes::decrypt(this->_data.mid(this->_buffer.pos()), aeskey, aesiv));

    this->readTLLong(); // server_salt
    this->_sessionid = this->readTLLong();
    this->_messageid = this->readTLLong();

    Q_ASSERT((qAbs((this->_messageid % 4)) == 1) || (qAbs((this->_messageid % 4)) == 3));

    this->readTLInt(); // seq_no
    this->_messagelength = this->readTLInt();
    this->_bodystart = this->_buffer.pos();
    this->_constructorid = this->peekTLConstructor();
}

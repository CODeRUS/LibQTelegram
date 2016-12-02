#include "mtprotostream.h"
#include "../autogenerated/tltypes.h"
#include "../crypto/math.h"
#include <QFile>

MTProtoStream::MTProtoStream(QObject *parent): QObject(parent)
{
    this->initBuffer();
}

MTProtoStream::MTProtoStream(const QByteArray &data, QObject *parent): QObject(parent), _data(data)
{
    this->initBuffer();
}

MTProtoStream::~MTProtoStream()
{
    if(this->_buffer.isOpen())
        this->_buffer.close();
}

void MTProtoStream::reset()
{
    this->_buffer.seek(0);
}

void MTProtoStream::seek(qint64 off)
{
    this->_buffer.seek(off);
}

void MTProtoStream::randPad(int by)
{
    int padlen = Math::padRequired(this->_data.length(), by);

    if(padlen <= 0)
        return;

    QByteArray randpad;
    randpad.fill(0, padlen);

    Math::randomize(randpad);
    this->_buffer.write(randpad);
}

const QByteArray &MTProtoStream::data() const
{
    return this->_data;
}

TLInt MTProtoStream::length() const
{
    return this->_data.length();
}

bool MTProtoStream::atEnd() const
{
    return this->_buffer.atEnd();
}

TLInt MTProtoStream::bytesAvailable() const
{
    return this->_buffer.bytesAvailable();
}

void MTProtoStream::save(const QString &filepath) const
{
    QFile f(filepath);

    if(!f.open(QFile::WriteOnly))
    {
        qWarning() << "MTProtoStream: cannot write" << filepath;
        return;
    }

    f.write(this->_data);
    f.close();
}

void MTProtoStream::load(const QString &filepath)
{
    QFile f(filepath);

    if(!f.open(QFile::ReadOnly))
    {
        qWarning() << "MTProtoStream: cannot read" << filepath;
        return;
    }

    this->_buffer.write(f.readAll());
    this->_buffer.seek(0);
    f.close();
}

TLConstructor MTProtoStream::peekTLConstructor()
{
    TLConstructor constructorid = 0;

    this->_buffer.peek(reinterpret_cast<char*>(&constructorid), sizeof(TLConstructor));
    return constructorid;
}

TLConstructor MTProtoStream::readTLConstructor()
{
    return this->readTLInt();
}

int MTProtoStream::peekTLVector()
{
    TLInt length = -1;
    qint64 oldpos = this->_buffer.pos();
    TLConstructor constructorid = this->readTLConstructor();

    if(constructorid == TLTypes::Vector)
        length = this->readTLInt();

    this->_buffer.seek(oldpos);
    return length;
}

TLBool MTProtoStream::readTLBool()
{
    TLConstructor val = 0;
    this->expect<TLConstructor>(this->_buffer.read(reinterpret_cast<char*>(&val), sizeof(TLConstructor)));

    if(val == TLTypes::BoolTrue)
        return true;
    else if(val == TLTypes::BoolFalse)
        return false;

    qFatal("Invalid boolean");
    return false;
}

TLInt MTProtoStream::readTLInt()
{
    TLInt val = 0;
    this->expect<TLInt>(this->_buffer.read(reinterpret_cast<char*>(&val), sizeof(TLInt)));
    return val;
}

TLDouble MTProtoStream::readTLDouble()
{
    TLDouble val = 0;
    this->expect<TLDouble>(this->_buffer.read(reinterpret_cast<char*>(&val), sizeof(TLDouble)));
    return val;
}

TLInt128 MTProtoStream::readTLInt128()
{
    TLInt128 val;

    val.lo = this->readTLLong();
    val.hi = this->readTLLong();
    return val;
}

TLInt256 MTProtoStream::readTLInt256()
{
    TLInt256 val;

    val.lo = this->readTLInt128();
    val.hi = this->readTLInt128();

    return val;
}

TLLong MTProtoStream::readTLLong()
{
    TLLong val = 0;
    this->expect<TLLong>(this->_buffer.read(reinterpret_cast<char*>(&val), sizeof(TLLong)));
    return val;
}

TLBytes MTProtoStream::readTLBytes()
{
    TLBytes val;
    TLInt len = 0, extrabytes = 0;

    this->_buffer.read(reinterpret_cast<char*>(&len), sizeof(qint8));

    if(len <= 253)
    {
        extrabytes = 1;

        val.fill(0, len);
        this->_buffer.read(val.data(), len);
    }
    else //if(len >= 254)
    {
        extrabytes = 4;

        this->_buffer.read(reinterpret_cast<char*>(&len), 3);
        len &= 0x00FFFFFF;

        val.fill(0, len);
        this->_buffer.read(val.data(), len);
    }

    int padlen = this->padLength(val.length() + extrabytes, 4, 3);

    if(padlen == -1)
    {
        qDebug() << Q_FUNC_INFO << "Invalid padding";
        return QByteArray();
    }

    if(padlen) // Skip padding (if any)
        this->_buffer.seek(this->_buffer.pos() + padlen);

    return val;
}

TLString MTProtoStream::readTLString()
{
    return this->readTLBytes();
}

TLBytes MTProtoStream::readRaw(int len)
{
    return this->_buffer.read(len);
}

TLBytes MTProtoStream::readAll()
{
    return this->_buffer.readAll();
}

void MTProtoStream::writeTLConstructor(TLConstructor val)
{
    this->writeTLInt(val);
}

void MTProtoStream::writeTLBool(TLBool val)
{
    this->writeTLInt(val ? TLTypes::BoolTrue : TLTypes::BoolFalse);
}

void MTProtoStream::writeTLInt(TLInt val)
{
    this->_buffer.write(reinterpret_cast<const char*>(&val), sizeof(TLInt));
}

void MTProtoStream::writeTLDouble(TLDouble val)
{
    this->_buffer.write(reinterpret_cast<const char*>(&val), sizeof(TLDouble));
}

void MTProtoStream::writeTLInt128(const TLInt128 &val)
{
    this->writeTLLong(val.lo);
    this->writeTLLong(val.hi);
}

void MTProtoStream::writeTLInt256(const TLInt256 &val)
{
    this->writeTLInt128(val.lo);
    this->writeTLInt128(val.hi);
}

void MTProtoStream::writeTLLong(TLLong val)
{
    this->_buffer.write(reinterpret_cast<const char*>(&val), sizeof(TLLong));
}

void MTProtoStream::writeTLBytes(const TLBytes &val)
{
    TLBytes data;
    TLInt len = val.length();

    if(len <= 253)
    {
        data.append(static_cast<uint8_t>(val.length()));
        data.append(val);
    }
    else //if(len >= 254)
    {
        data.append(static_cast<uint8_t>(254));
        data.append(reinterpret_cast<const char*>(&len), 3);
        data.append(val);
    }

    if(!this->align(data, 4, 3))
    {
        qDebug() << Q_FUNC_INFO << "Alignment failed";
        return;
    }

    this->_buffer.write(data);
}

void MTProtoStream::writeTLString(const TLString &val)
{
    this->writeTLBytes(val.toUtf8());
}

void MTProtoStream::writeRaw(const QByteArray &data, bool prependlength)
{
    if(prependlength)
        this->writeTLInt(data.length());

    this->_buffer.write(data);
}

void MTProtoStream::writeRaw(const MTProtoStream *mtprotostream, bool prependlength)
{
    this->writeRaw(mtprotostream->_data, prependlength);
}

void MTProtoStream::initBuffer()
{
    this->_buffer.setBuffer(&this->_data);
    this->_buffer.open(QIODevice::ReadWrite);
}

int MTProtoStream::padLength(int len, int by, int maxpad)
{
    for(int i = 0; i <= maxpad; i++)
    {
        if(!((len + i) % by))
            return i;
    }

    return -1;
}

bool MTProtoStream::align(TLBytes &data, int by, int maxpad)
{
    for(int i = 0; i <= maxpad; i++)
    {
        if(!(data.length() % by))
            return true;

        data.append(static_cast<char>(0x00));
    }

    return false;
}

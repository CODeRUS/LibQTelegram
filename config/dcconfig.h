#ifndef DCCONFIG_H
#define DCCONFIG_H

#include <QObject>
#include "../types/basic.h"
#include "../autogenerated/types/types.h"

/*
 *         HI 32-Bit               LO 32-Bit
 * [IsIPv6Bit | IsMediaOnly] | [     DC ID     ]
 */

#define DCConfigIsIPv6Bit      63
#define DCConfigIsMediaOnlyBit 62

#define DCConfigIsIPv6BitMask      0x7FFFFFFFFFFFFFFF
#define DCConfigIsMediaOnlyBitMask 0xBFFFFFFFFFFFFFFF
#define DCConfigDcIdMask           0x00000000FFFFFFFF

#define MakeDCConfigId(ipv6, media, dcid) ((static_cast<DCConfig::Id>(ipv6) << DCConfigIsIPv6Bit) | (static_cast<DCConfig::Id>(media) << DCConfigIsMediaOnlyBit) | dcid)
#define DCConfig_DcId(dcconfig) (configid & DCConfigDcIdMask)

class DCConfig: public QObject
{
    Q_OBJECT

    public:
        enum DcAuthorization {
            NotAuthorized = 0,
            PQReceived,
            ServerDHParamsFailReceived,
            ServerDHParamsOkReceived,
            Authorized,
            Signed,
        };

    public:
        typedef quint64 Id;

    public:
        DCConfig(QObject* parent = 0);
        DCConfig(const QString& host, TLInt port, TLInt dcid, bool isipv6, QObject* parent = 0);
        DCConfig(DcOption* dcoption, QObject* parent = 0);
        Id id() const;
        DcOption* option() const;
        TLInt dcid() const;
        QString host() const;
        qint16 port() const;
        int authorization() const;
        bool isMain() const;
        QJsonObject toJson() const;
        void fromJson(const QJsonObject &jsonobj);
        void reset();

    public:
        static Id configId(const QJsonObject& jsonobj);

    public:
        void setDcOption(DcOption* dcoption);
        void setAuthorization(int authorization);
        void setAuthorizationKey(const QByteArray& authorizationkey);
        void setServerSalt(TLLong serversalt);
        void setDeltaTime(TLInt deltatime);
        void setIsMain(bool b);

    public: // Server data
        const QByteArray& authorizationKey() const;
        const QByteArray& authorizationKeyAuxHash();
        TLLong authorizationKeyId();
        TLLong serverSalt() const;
        TLLong deltaTime() const;

    private:
        DcOption* _dcoption;
        Id _id;

    private:
        QByteArray _authorizationkeyauxhash;
        QByteArray _authorizationkey;
        TLLong _authorizationkeyid;
        TLLong _serversalt;
        TLInt _deltatime;
        int _authorization;
        bool _ismain;
};

#endif // DCCONFIG_H

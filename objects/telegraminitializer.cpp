#include "telegraminitializer.h"
#include "mtproto/mtprotoupdatehandler.h"
#include "config/cache/cacheinitializer.h"
#include "config/cache/telegramcache.h"
#include <QDebug>

TelegramInitializer::TelegramInitializer(QObject *parent) : QObject(parent), _apiid(0), _port(0), _debugmode(false)
{
}

TelegramInitializer::~TelegramInitializer()
{
    if(!DCConfig_isLoggedIn)
        return;

    TelegramConfig_save;
    TelegramCache_save;
}

const QString &TelegramInitializer::publicKey() const
{
    return this->_publickey;
}

const QString &TelegramInitializer::host() const
{
    return this->_host;
}

qint32 TelegramInitializer::apiId() const
{
    return this->_apiid;
}

const QString &TelegramInitializer::apiHash() const
{
    return this->_apihash;
}

const QString &TelegramInitializer::phoneNumber() const
{
    return this->_phonenumber;
}

qint32 TelegramInitializer::port() const
{
    return this->_port;
}

bool TelegramInitializer::debugMode() const
{
    return this->_debugmode;
}

void TelegramInitializer::setPublicKey(const QString &publickey)
{
    if(this->_publickey == publickey)
        return;

    this->_publickey = publickey;

    this->tryConnect();
    emit publicKeyChanged();
}

void TelegramInitializer::setHost(const QString &host)
{
    if(this->_host == host)
        return;

    this->_host = host;

    this->tryConnect();
    emit hostChanged();
}

void TelegramInitializer::setApiHash(const QString &apphash)
{
    if(this->_apihash == apphash)
        return;

    this->_apihash = apphash;
    this->tryConnect();
    emit apiHashChanged();
}

void TelegramInitializer::setPhoneNumber(const QString &phonenumber)
{
    if(this->_phonenumber == phonenumber)
        return;

    this->_phonenumber = phonenumber;
    this->tryConnect();
    emit phoneNumberChanged();
}

void TelegramInitializer::setApiId(qint32 appid)
{
    if(this->_apiid == appid)
        return;

    this->_apiid = appid;
    this->tryConnect();
    emit apiIdChanged();
}

void TelegramInitializer::setPort(qint32 port)
{
    if(this->_port == port)
        return;

    this->_port = port;

    this->tryConnect();
    emit portChanged();
}

void TelegramInitializer::setDebugMode(bool dbgmode)
{
    if(this->_debugmode == dbgmode)
        return;

    this->_debugmode = dbgmode;
    emit debugModeChanged();
}

void TelegramInitializer::signIn(const QString &phonecode)
{
    MTProtoRequest* req = TelegramAPI::authSignIn(DC_MainSession, this->_phonenumber, this->_phonecodehash, phonecode);
    connect(req, &MTProtoRequest::replied, this, &TelegramInitializer::onLoginCompleted);

    this->_phonecodehash.clear();
}

void TelegramInitializer::signUp(const QString &firstname, const QString &lastname, const QString &phonecode)
{
    MTProtoRequest* req = TelegramAPI::authSignUp(DC_MainSession, this->_phonenumber, this->_phonecodehash, phonecode, firstname, lastname);
    connect(req, &MTProtoRequest::replied, this, &TelegramInitializer::onLoginCompleted);

    this->_phonecodehash.clear();
}

void TelegramInitializer::tryConnect()
{
    if(this->_publickey.isEmpty() || this->_host.isEmpty() || this->_phonenumber.isEmpty() || this->_apihash.isEmpty())
        return;

    if(!this->_apiid || !this->_port)
        return;

    TelegramConfig::init(TELEGRAM_API_LAYER, this->_apiid, this->_apihash, this->_publickey, this->_phonenumber);
    TelegramConfig::config()->setDebugMode(true);

    if(DCConfig_isLoggedIn)
    {
        DCConfig& dcconfig = DCConfig_fromDcId(DCConfig_mainDcId);
        DCSessionManager::instance()->createMainSession(dcconfig);

        TelegramCache_load;
        UpdateHandler_sync;
        emit loginCompleted();
        return;
    }

    DCSessionManager::instance()->createMainSession(this->_host, this->_port, 0); // The entry point doesn't have a Dc Id, set it to 0
    MTProtoRequest* req = TelegramAPI::authSendCode(DC_MainSession, this->_phonenumber, false, this->_apiid, this->_apihash);
    connect(req, &MTProtoRequest::replied, this, &TelegramInitializer::onAuthCheckPhoneReplied);
}

void TelegramInitializer::onAuthCheckPhoneReplied(MTProtoReply *mtreply)
{
    AuthSentCode sentcode;
    sentcode.read(mtreply);

    this->_phonecodehash = sentcode.phoneCodeHash();

    if(sentcode.isPhoneRegistered())
    {
        emit signInRequested();
        return;
    }

    emit signUpRequested();
}

void TelegramInitializer::onLoginCompleted(MTProtoReply *mtreply)
{
    DCConfig& dcconfig = DCConfig_fromDcId(mtreply->dcid());

    AuthAuthorization authorization;
    authorization.read(mtreply);

    dcconfig.setAuthorization(DCConfig::Signed);

    TelegramCache_store(authorization.user()); // Cache "me"
    TelegramConfig_instance->setMe(authorization.user());
    TelegramConfig_save;

    CacheInitializer* cacheinitializer = new CacheInitializer(this);

    connect(cacheinitializer, &CacheInitializer::initialized, [this, cacheinitializer] {
        emit this->loginCompleted();
        cacheinitializer->deleteLater();
    });

    cacheinitializer->initialize();
}

#include "telegram.h"
#include "mtproto/mtprotoupdatehandler.h"
#include "cache/telegramcache.h"
#include "cache/file/filecache.h"
#include "types/telegramhelper.h"

#define StatusUpdateDelay 5000 // 5 seconds

Telegram::Telegram(QObject *parent) : QObject(parent), _initializer(NULL), _timstatus(0), _autodownload(false), _loggedin(false), _online(false)
{
    connect(DCSessionManager_instance, &DCSessionManager::mainSessionConnectedChanged, this, &Telegram::connectedChanged);
    connect(DCSessionManager_instance, &DCSessionManager::mainSessionTimeout, this, &Telegram::connectionTimeout);
    connect(UpdateHandler_instance, &MTProtoUpdateHandler::syncingChanged, this, &Telegram::syncingChanged);

    connect(this, &Telegram::loginCompleted, this, &Telegram::loggedInChanged);
}

Telegram::~Telegram()
{
    if(!this->connected())
        return;

    DC_CloseSession(DC_MainSession);
    DCSessionManager_instance->deleteLater();
    TelegramCache_instance->deleteLater();
}

TelegramInitializer *Telegram::initializer() const
{
    return this->_initializer;
}

User *Telegram::me() const
{
    return TelegramConfig_me;
}

int Telegram::apiLayer() const
{
    return TELEGRAM_API_LAYER;
}

bool Telegram::loggedIn() const
{
    return this->_loggedin;
}

bool Telegram::connected() const
{
    if(!DC_MainSession)
        return false;

    DC* dc = SessionToDC(DC_MainSession);
    return dc->state() == DC::ConnectedState;
}

bool Telegram::syncing() const
{
    return UpdateHandler_syncing;
}

bool Telegram::autoDownload() const
{
    return this->_autodownload;
}

bool Telegram::online() const
{
    return this->_online;
}

int Telegram::unreadCount() const
{
    if(!this->_loggedin)
        return 0;

    return TelegramCache_unreadCount;
}

void Telegram::setInitializer(TelegramInitializer *initializer)
{
    if(this->_initializer == initializer)
        return;

    if(this->_initializer)
    {
        disconnect(this->_initializer, &TelegramInitializer::floodLock, this, 0);
        disconnect(this->_initializer, &TelegramInitializer::phoneCodeError, this, 0);
        disconnect(this->_initializer, &TelegramInitializer::signUpRequested, this, 0);
        disconnect(this->_initializer, &TelegramInitializer::signInRequested, this, 0);
        disconnect(this->_initializer, &TelegramInitializer::invalidPassword, this, 0);
        disconnect(this->_initializer, &TelegramInitializer::sessionPasswordNeeded, this, 0);
        disconnect(this->_initializer, &TelegramInitializer::loginCompleted, this, 0);
    }

    this->_initializer = initializer;

    connect(this->_initializer, &TelegramInitializer::floodLock, this, &Telegram::floodLock);
    connect(this->_initializer, &TelegramInitializer::phoneCodeError, this, &Telegram::phoneCodeError);
    connect(this->_initializer, &TelegramInitializer::signUpRequested, this, &Telegram::signUpRequested);
    connect(this->_initializer, &TelegramInitializer::signInRequested, this, &Telegram::signInRequested);
    connect(this->_initializer, &TelegramInitializer::invalidPassword, this, &Telegram::invalidPassword);
    connect(this->_initializer, &TelegramInitializer::sessionPasswordNeeded, this, &Telegram::sessionPasswordNeeded);
    connect(this->_initializer, &TelegramInitializer::loginCompleted, this, &Telegram::onLoginCompleted);

    emit initializerChanged();
}

void Telegram::setAutoDownload(bool b)
{
    if(this->_autodownload == b)
        return;

    this->_autodownload = b;

    if(TelegramConfig_instance)
        TelegramConfig_setAutoDownload(b);

    emit autoDownloadChanged();
}

void Telegram::setOnline(bool b)
{
    if(!this->connected() || !this->loggedIn() || (this->_online == b))
        return;

    this->_online = b;

    if(!this->_timstatus)
        this->_timstatus = this->startTimer(StatusUpdateDelay);

    emit onlineChanged();
}

void Telegram::sortDialogs(QList<Dialog *> &dialogs) const
{
    std::sort(dialogs.begin(), dialogs.end(), [](Dialog* dlg1, Dialog* dlg2) {
        if(!dlg1->topMessage())
            return false;

        if(!dlg2->topMessage())
            return true;

        Message* msg1 = TelegramCache_message(dlg1->topMessage(), dlg1);
        Message* msg2 = TelegramCache_message(dlg2->topMessage(), dlg2);

        if(!msg1)
            return false;

        if(!msg2)
            return true;

        return msg1->date() > msg2->date();
    });
}

bool Telegram::muteDialog(Dialog *dialog, bool mute)
{
    PeerNotifySettings* notifysettings = dialog->notifySettings();

    if(TelegramHelper::isMuted(dialog) == mute)
        return false;

    notifysettings->setIsSilent(mute);
    notifysettings->setMuteUntil(mute ? Future10Years : 0);
    TelegramCache_store(dialog);

    InputNotifyPeer* inputnotifypeer = TelegramHelper::inputNotifyPeer(dialog, TelegramCache_accessHash(dialog));
    InputPeerNotifySettings* inputpeernotifysettings = TelegramHelper::inputPeerNotifySettings(notifysettings);
    TelegramAPI::accountUpdateNotifySettings(DC_MainSession, inputnotifypeer, inputpeernotifysettings);

    inputpeernotifysettings->deleteLater();
    inputnotifypeer->deleteLater();
    return true;
}

bool Telegram::dialogIsWritable(Dialog *dialog) const
{
    TLInt dialogid = TelegramHelper::identifier(dialog);

    if(TelegramHelper::isChat(dialog) || TelegramHelper::isChannel(dialog))
    {
        Chat* chat = TelegramCache_chat(dialogid);

        if(!chat)
            return false;

        if(chat->isBroadcast())
            return dialogid == TelegramConfig_me->id();

        return (chat->constructorId() == TLTypes::Chat) || (chat->constructorId() == TLTypes::Channel);
    }

    return true;
}

QString Telegram::messageMediaText(MessageMedia *messagemedia) const
{
    TLConstructor ctorid = messagemedia->constructorId();

    if(ctorid == TLTypes::MessageMediaWebPage)
        return messagemedia->caption();

    if(ctorid == TLTypes::MessageMediaDocument)
    {
        DocumentAttribute* attribute = NULL;
        Document* document = messagemedia->document();

        if(TelegramHelper::documentHas(document, TLTypes::DocumentAttributeAnimated))
            return tr("GIF");

        if((attribute = TelegramHelper::documentHas(document, TLTypes::DocumentAttributeSticker)))
            return tr("%1 Sticker").arg(attribute->alt());

        if(TelegramHelper::documentHas(document, TLTypes::DocumentAttributeVideo))
            return tr("Video file");

        if(TelegramHelper::documentHas(document, TLTypes::DocumentAttributeAudio))
            return tr("Audio recording");

        if(TelegramHelper::documentHas(document, TLTypes::DocumentAttributeHasStickers))
            return tr("Sticker Set");

        if((attribute = TelegramHelper::documentHas(document, TLTypes::DocumentAttributeFilename)))
            return attribute->fileName();

        return tr("Document");
    }

    QString result;

    if(ctorid == TLTypes::MessageMediaContact)
        result = tr("Contact");
    else if(ctorid == TLTypes::MessageMediaGame)
        result = tr("Game");
    else if((ctorid == TLTypes::MessageMediaGeo)  || (ctorid == TLTypes::MessageMediaVenue))
        result = tr("Location");
    else if(ctorid == TLTypes::MessageMediaPhoto)
        result = tr("Photo");

    if(!messagemedia->caption().isEmpty())
        result += ", " + messagemedia->caption();

    return result;
}

QString Telegram::messageActionText(Message* message) const
{
    User *fromuser = NULL, *inviteruser = NULL, *user = NULL;
    QString fromfullname, fullname, inviterfullname;
    MessageAction* messageaction = message->action();
    TLConstructor ctorid = messageaction->constructorId();

    if(message->fromId())
    {
        fromuser = TelegramCache_user(message->fromId());

        if(fromuser)
            fromfullname = TelegramHelper::fullName(fromuser);
    }

    if(messageaction->userId())
    {
        user = TelegramCache_user(messageaction->userId());

        if(user)
            fullname = TelegramHelper::fullName(user);
    }

    if(messageaction->inviterId())
    {
        inviteruser = TelegramCache_user(messageaction->inviterId());

        if(inviteruser)
            inviterfullname = TelegramHelper::fullName(inviteruser);
    }

    if(ctorid == TLTypes::MessageActionChatCreate)
    {
        Chat* chat = TelegramCache_chat(TelegramHelper::identifier(message->toId()));

        if(messageaction->channelId())
            return tr("%1 created channel «%2»").arg(fromfullname, (chat ? chat->title() : QString()));

        return tr("%1 created group «%2»").arg(fromfullname, (chat ? chat->title() : QString()));
    }

    if(ctorid == TLTypes::MessageActionChatEditTitle)
    {
        if(message->isPost())
            return tr("Channel name changed to «%1»").arg(messageaction->title());

        return tr("«%1» changed group name to «%2»").arg(fromfullname, messageaction->title());
    }

    if(ctorid == TLTypes::MessageActionChatEditPhoto)
    {
        if(message->isPost())
            return tr("Channel photo updated");

        return tr("«%1» updated group photo").arg(fromfullname);
    }

    if(ctorid == TLTypes::MessageActionChatDeletePhoto)
    {
        if(message->isPost())
            return tr("Channel photo deleted");

        return tr("«%1» deleted group photo").arg(fromfullname);
    }

    if(ctorid == TLTypes::MessageActionChatAddUser)
    {
        if(messageaction->users().length() > 0)
            return tr("«%1» added «%2» ").arg(fromfullname, this->userList(messageaction->users()));

        return tr("«%1» has joined the group").arg(fullname);
    }

    if(ctorid == TLTypes::MessageActionChatDeleteUser)
    {
        if(messageaction->userId() && (message->fromId() != messageaction->userId()))
            return tr("«%1» removed «%2»").arg(fromfullname, fullname);

        return tr("«%1» has left the group").arg(fullname);
    }

    if(ctorid == TLTypes::MessageActionChatJoinedByLink)
        return tr("«%1» has joined the group via invite link").arg(fromfullname);

    if(ctorid == TLTypes::MessageActionChannelCreate)
        return tr("Channel «%1» created").arg(messageaction->title());

    if(ctorid == TLTypes::MessageActionChatMigrateTo)
    {
        if(messageaction->channelId())
            return tr("This group was upgraded to a supergroup");

        return tr("Unhandled chat migration type");
    }

    if(ctorid == TLTypes::MessageActionGameScore) // NOTE: MessageActionGameScore, unhandled and untested
        return tr("Unhandled MessageActionGameScore");

    return QString("Unhandled action: %1").arg(ctorid, 0, 16);
}

QString Telegram::dialogTitle(Dialog *dialog) const
{
    if(!dialog)
        return QString();

    TLInt peerid = TelegramHelper::identifier(dialog->peer());

    if(TelegramHelper::isChat(dialog) || TelegramHelper::isChannel(dialog))
    {
        Chat* chat = TelegramCache_chat(peerid);
        return chat->title();
    }

    User* user = TelegramCache_user(peerid);
    return TelegramHelper::fullName(user);
}

QString Telegram::dialogStatusText(Dialog *dialog) const
{
    if(!dialog)
        return QString();

    TLInt dialogid = TelegramHelper::identifier(dialog);

    if(TelegramHelper::isChannel(dialog))
    {
        ChatFull* chatfull = TelegramCache_chatFull(dialogid);
        return tr("%1 members").arg(chatfull ? chatfull->participantsCount() : 0);
    }

    if(TelegramHelper::isChat(dialog))
    {
        Chat* chat = TelegramCache_chat(dialogid);
        return tr("%1 members").arg(chat ? chat->participantsCount() : 0);
    }

    User* user = TelegramCache_user(dialogid);

    if(user)
        return TelegramHelper::statusText(user);

    return QString();
}

TelegramObject *Telegram::messageFrom(Message *message) const
{
    if(!message)
        return NULL;

    if(message->isPost()) // Post = messages from channels
    {
        Chat* chat = TelegramCache_chat(TelegramHelper::identifier(message->toId()));

        if(chat)
            return chat;
    }
    else
    {
        User* user = TelegramCache_user(message->fromId());

        if(user)
            return user;
    }

    return NULL;
}

QString Telegram::messageText(Message *message) const
{
    if(!message)
        return QString();

    if(message->action() && (message->action()->constructorId() != TLTypes::MessageActionEmpty))
        return this->messageActionText(message);

    return message->message();
}

QString Telegram::messagePreview(Message *message) const
{
    if(!message)
        return QString();

    if(message->action())
        return this->messageActionText(message);

    if(message->media())
    {
        QString mediatext = this->messageMediaText(message->media());

        if(!mediatext.isEmpty())
            return mediatext;
    }

    return message->message();
}

void Telegram::signIn(const QString &phonecode) const
{
    this->_initializer->signIn(phonecode);
}

void Telegram::signUp(const QString &firstname, const QString &lastname, const QString &phonecode) const
{
    this->_initializer->signUp(firstname, lastname, phonecode);
}

void Telegram::sendPassword(const QString &password) const
{
    this->_initializer->sendPassword(password);
}

void Telegram::reconnect() const
{
    DCSessionManager_instance->restoreSessions();
}

void Telegram::resendCode() const
{
    this->_initializer->resendCode();
}

void Telegram::onLoginCompleted()
{
    this->_loggedin = true;

    TelegramConfig_setAutoDownload(this->_autodownload);
    connect(TelegramCache_instance, &TelegramCache::unreadCountChanged, this, &Telegram::unreadCountChanged);

    this->setOnline(true);

    emit loginCompleted();
    emit unreadCountChanged();
}

void Telegram::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == this->_timstatus)
    {
        TelegramAPI::accountUpdateStatus(DC_MainSession, !this->_online);
        this->killTimer(event->timerId());
        this->_timstatus = 0;
    }
}

QString Telegram::userList(const TLVector<TLInt> users) const
{
    QString s;

    foreach(TLInt userid, users)
    {
        User* user = TelegramCache_user(userid);

        if(!user)
            continue;

        if(!s.isEmpty())
            s += ", ";

        s += TelegramHelper::fullName(user);
    }

    return s;
}

#include "telegramhelper.h"
#include <QDateTime>

TelegramHelper::TelegramHelper()
{

}

Message *TelegramHelper::createMessage(Updates *updates, User* me)
{
    Q_ASSERT((updates->constructorId() == TLTypes::UpdateShortMessage) ||
             (updates->constructorId() == TLTypes::UpdateShortChatMessage));

    Message* message = new Message();
    message->setConstructorId(Message::CtorMessage);
    message->setFlags(updates->flags());
    message->setIsOut(updates->isOut());
    message->setIsMentioned(updates->isMentioned());
    message->setIsMediaUnread(updates->isMediaUnread());
    message->setIsSilent(updates->isSilent());
    message->setId(updates->id());
    message->setMessage(updates->message());
    message->setDate(updates->date());
    message->setFwdFrom(updates->fwdFrom());
    message->setViaBotId(updates->viaBotId());
    message->setReplyToMsgId(updates->replyToMsgId());
    message->setEntities(updates->entities());

    if(updates->constructorId() == TLTypes::UpdateShortMessage)
    {
        Peer* topeer = new Peer();
        topeer->setConstructorId(Peer::CtorPeerUser);
        topeer->setUserId(updates->isOut() ? updates->userId() : me->id());

        message->setFromId(updates->isOut() ? me->id() : updates->userId());
        message->setToId(topeer);
    }
    else if(updates->constructorId() == TLTypes::UpdateShortChatMessage)
    {
        Peer* topeer = new Peer();
        topeer->setConstructorId(Peer::CtorPeerChat);
        topeer->setChatId(updates->chatId());

        message->setFromId(updates->fromId());
        message->setToId(topeer);
    }

    return message;
}

QString TelegramHelper::dateString(TLInt timestamp)
{
    QDateTime datetime = QDateTime::fromTime_t(timestamp);
    const QDateTime& currentdatetime = QDateTime::currentDateTime();
    qint64 seconds = datetime.secsTo(currentdatetime);
    int days = datetime.daysTo(currentdatetime);

    if(seconds < 24 * 60 * 60)
        return QObject::tr("Yesterday %1").arg(days ? datetime.toString("HH:mm") : datetime.toString("HH:mm"));

    return datetime.toString("MMM dd, HH:mm");
}

QString TelegramHelper::fullName(User *user)
{
    if(!user)
        return QString();

    if(user->lastName().isEmpty())
        return user->firstName();

    return user->firstName() + " " + user->lastName();
}

QString TelegramHelper::statusText(User *user)
{
    if(user->status())
    {
        if(user->status()->constructorId() == TLTypes::UserStatusLastMonth)
            return QObject::tr("Last month");

        if(user->status()->constructorId() == TLTypes::UserStatusLastWeek)
            return QObject::tr("Last week");

        if(user->status()->constructorId() == TLTypes::UserStatusOffline)
            return QObject::tr("Last seen %1").arg(TelegramHelper::dateString(user->status()->wasOnline()));

        if(user->status()->constructorId() == TLTypes::UserStatusOnline)
            return QObject::tr("Online");

        if(user->status()->constructorId() == TLTypes::UserStatusRecently)
            return QObject::tr("Recently");
    }

    return QObject::tr("Long time ago");
}

QString TelegramHelper::messageText(Message *message)
{
    if(message->media())
    {
        MessageMedia* messagemedia = message->media();
        TLConstructor ctorid = messagemedia->constructorId();

        if(ctorid == TLTypes::MessageMediaWebPage)
        {
            if(!messagemedia->caption().isEmpty())
                return  messagemedia->caption();

            return messagemedia->webpage()->url();
        }

        if(ctorid == TLTypes::MessageMediaDocument)
        {
            Document* document = messagemedia->document();

            foreach(DocumentAttribute* attribute, document->attributes())
            {
                if(attribute->constructorId() == TLTypes::DocumentAttributeImageSize)
                    return QObject::tr("Photo");

                if(attribute->constructorId() == TLTypes::DocumentAttributeAnimated)
                    return QObject::tr("GIF");

                if(attribute->constructorId() == TLTypes::DocumentAttributeSticker)
                    return QObject::tr("Sticker");

                if(attribute->constructorId() == TLTypes::DocumentAttributeVideo)
                    return QObject::tr("Video");

                if(attribute->constructorId() == TLTypes::DocumentAttributeAudio)
                    return QObject::tr("Audio recording");

                if(attribute->constructorId() == TLTypes::DocumentAttributeHasStickers)
                    return QObject::tr("Sticker set");
            }

            return QObject::tr("Document");
        }

        QString result;

        if(ctorid == TLTypes::MessageMediaContact)
            result = QObject::tr("Contact");
        else if(ctorid == TLTypes::MessageMediaGame)
            result = QObject::tr("Game");
        else if((ctorid == TLTypes::MessageMediaGeo)  || (ctorid == TLTypes::MessageMediaVenue))
            result = QObject::tr("Location");
        else if(ctorid == TLTypes::MessageMediaPhoto)
            result = QObject::tr("Photo");

        if(!messagemedia->caption().isEmpty())
            result += ", " + message->media()->caption();

        return result;
    }

    return message->message();
}

bool TelegramHelper::isChat(Dialog *dialog)
{
    return dialog->peer()->constructorId() == TLTypes::PeerChat;
}

bool TelegramHelper::isChannel(Dialog *dialog)
{
    return dialog->peer()->constructorId() == TLTypes::PeerChannel;
}

TLInt TelegramHelper::identifier(User *user)
{
    return user->id();
}

TLInt TelegramHelper::identifier(Chat *chat)
{
    return chat->id();
}

TLInt TelegramHelper::identifier(Peer *peer)
{
    if(peer->constructorId() == TLTypes::PeerChannel)
        return peer->channelId();

    if(peer->constructorId() == TLTypes::PeerChat)
        return peer->chatId();

    if(peer->constructorId() == TLTypes::PeerUser)
        return peer->userId();

    Q_ASSERT(false);
    return -1;
}

TLInt TelegramHelper::identifier(Dialog *dialog)
{
    return TelegramHelper::identifier(dialog->peer());
}

TLInt TelegramHelper::identifier(Message *message)
{
    return message->id();
}

TLInt TelegramHelper::dialogIdentifier(Message *message)
{
    if(message->isOut())
        return TelegramHelper::identifier(message->toId());

    return message->fromId();
}

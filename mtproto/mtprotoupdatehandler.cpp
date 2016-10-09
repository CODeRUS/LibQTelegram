#include "mtprotoupdatehandler.h"
#include "../autogenerated/mtproto/decompiler/decompilertable.h"
#include "../autogenerated/telegramapi.h"
#include "../autogenerated/clientsyncmanager.h"
#include "../config/telegramconfig.h"
#include "../types/telegramhelper.h"
#include "dc/dcsessionmanager.h"

#define Nop

MTProtoUpdateHandler* MTProtoUpdateHandler::_instance = NULL;

MTProtoUpdateHandler::MTProtoUpdateHandler(QObject *parent) : QObject(parent)
{
}

MTProtoUpdateHandler *MTProtoUpdateHandler::instance()
{
    if(!MTProtoUpdateHandler::_instance)
        MTProtoUpdateHandler::_instance = new MTProtoUpdateHandler();

    return MTProtoUpdateHandler::_instance;
}

bool MTProtoUpdateHandler::handle(MTProtoReply *mtreply)
{
    switch(mtreply->constructorId())
    {
        case TLTypes::Updates:
        case TLTypes::UpdateShort:
        case TLTypes::UpdateShortMessage:
        case TLTypes::UpdateShortChatMessage:
            this->handleUpdates(mtreply);
            return true;

        case TLTypes::UpdatesDifference:
        case TLTypes::UpdatesDifferenceSlice:
            this->handleUpdatesDifference(mtreply);
            return true;

        case TLTypes::UpdatesState:
            this->handleUpdatesState(mtreply);

        default:
            break;
    }

    return false;
}

void MTProtoUpdateHandler::sync()
{
    TelegramAPI::updatesGetState(DC_MAIN_SESSION);
}

void MTProtoUpdateHandler::handleUpdates(MTProtoReply *mtreply)
{
    Updates updates;
    updates.read(mtreply);

    UpdatesState* clientstate = TelegramConfig_clientState;
    ClientSyncManager::syncUpdates(&updates, clientstate);

    if((updates.constructorId() == TLTypes::UpdateShortMessage) || (updates.constructorId() == TLTypes::UpdateShortChatMessage))
    {
        Message* message = TelegramHelper::createMessage(&updates, TelegramConfig_instance->me());
        emit newMessage(message);
    }
    else if(updates.constructorId() == TLTypes::UpdateShort)
        this->handleUpdate(updates.update());
    else
    {
        const TLVector<Update*>& updatelist = updates.updates();
        this->handleUpdates(updatelist);
    }
}

void MTProtoUpdateHandler::handleUpdatesState(MTProtoReply *mtreply)
{
    UpdatesState serverstate;
    serverstate.read(mtreply);

    UpdatesState* clientstate = TelegramConfig_clientState;

    if(((serverstate.seq() - clientstate->seq()) > 0) && (serverstate.seq() != (clientstate->seq() + 1)))
        this->getDifferences();
}

void MTProtoUpdateHandler::handleUpdatesDifference(MTProtoReply *mtreply)
{
    UpdatesDifference updatedifference;
    updatedifference.read(mtreply);

    foreach(User* user, updatedifference.users())
        emit newUser(user);

    foreach(Chat* chat, updatedifference.chats())
        emit newChat(chat);

    foreach(Message* message, updatedifference.newMessages())
        emit newMessage(message);

    this->handleUpdates(updatedifference.otherUpdates());

    if(updatedifference.constructorId() == TLTypes::UpdatesDifferenceSlice)
    {
        this->syncState(updatedifference.intermediateState());
        this->getDifferences();
    }
}

void MTProtoUpdateHandler::syncState(UpdatesState *serverstate)
{
    UpdatesState* clientstate = TelegramConfig_clientState;

    clientstate->setPts(serverstate->pts());
    clientstate->setQts(serverstate->qts());
    clientstate->setDate(serverstate->date());
    clientstate->setSeq(serverstate->seq());
    clientstate->setUnreadCount(serverstate->unreadCount());
}

void MTProtoUpdateHandler::getDifferences()
{
    UpdatesState* clientstate = TelegramConfig_clientState;
    TelegramAPI::updatesGetDifference(DC_MAIN_SESSION, clientstate->pts(), clientstate->date(), clientstate->qts());
}

void MTProtoUpdateHandler::handleUpdates(TLVector<Update *> updatelist)
{
    UpdatesState* clientstate = TelegramConfig_clientState;

    for(int i = 0; i < updatelist.length(); i++)
    {
        Update* update = updatelist[i];
        ClientSyncManager::syncUpdate(update, clientstate);
        this->handleUpdate(update);
    }
}

void MTProtoUpdateHandler::handleUpdate(Update *update)
{
    switch(update->constructorId())
    {
        case TLTypes::UpdateNewMessage:
            emit newMessage(update->messageUpdatenewmessage());

        case TLTypes::UpdateNewChannelMessage:
            emit newMessage(update->messageUpdatenewchannelmessage());
            break;

        case TLTypes::UpdateEditMessage:
        case TLTypes::UpdateEditChannelMessage:
            emit editMessage(update->message());
            break;

        case TLTypes::UpdateUserStatus:
            emit newUserStatus(update);
            break;

        case TLTypes::UpdateDraftMessage:
            emit newDraftMessage(update);
            break;

        case TLTypes::UpdateMessageID:
            Nop;
            break;

        default:
            qWarning("Unhandled update: %s#%08x", DecompilerTable::constructorName(update->constructorId()).toUtf8().constData(), update->constructorId());
            break;
    }
}

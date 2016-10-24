#ifndef CACHEINITIALIZER_H
#define CACHEINITIALIZER_H

#include <QObject>
#include "../../mtproto/mtprotoreply.h"
#include "../../autogenerated/types/types.h"

class CacheInitializer : public QObject
{
    Q_OBJECT

    private:
        enum State {
            First = 0,
            RequestState = First,
            RequestContacts,
            RequestDialogs,
            Last = RequestDialogs,
        };

    public:
        explicit CacheInitializer(QObject *parent = 0);
        void initialize();

    private:
        void requestState();
        void requestContacts();
        void requestDialogs();
        bool seekDialogs(MessagesDialogs *messagesdialogs);
        Message* findMessage(MessagesDialogs* messagesdialogs, TLInt messageid) const;
        Chat* findChat(MessagesDialogs* messagesdialogs, TLInt chatid) const;
        User* findUser(MessagesDialogs* messagesdialogs, TLInt userid) const;

    private:
        void onRequestStateReplied(MTProtoReply* mtreply);
        void onRequestContactsReplied(MTProtoReply* mtreply);
        void onRequestDialogsReplied(MTProtoReply* mtreply);

    signals:
        void initialized();

    private:
        int _state;
        int _totaldialogs;
        int _loadeddialogs;
        TLInt _dialogsoffsetdate;
        TLInt _dialogsoffsetid;
        InputPeer _dialogoffsetpeer;
};

#endif // CACHEINITIALIZER_H

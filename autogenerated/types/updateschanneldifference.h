#ifndef UPDATESCHANNELDIFFERENCE_H
#define UPDATESCHANNELDIFFERENCE_H

// Generated by APIGenerator 1.0
// DO NOT EDIT!!!

#include "../../types/basic.h"
#include "../../types/telegramobject.h"
#include "message.h"
#include "update.h"
#include "chat.h"
#include "user.h"

class UpdatesChannelDifference: public TelegramObject
{
	Q_OBJECT

	Q_PROPERTY(TLInt flags READ flags WRITE setFlags NOTIFY flagsChanged)
	Q_PROPERTY(TLTrue isFinal READ isFinal WRITE setIsFinal NOTIFY isFinalChanged)
	Q_PROPERTY(TLInt pts READ pts WRITE setPts NOTIFY ptsChanged)
	Q_PROPERTY(TLInt timeout READ timeout WRITE setTimeout NOTIFY timeoutChanged)
	Q_PROPERTY(TLInt topMessage READ topMessage WRITE setTopMessage NOTIFY topMessageChanged)
	Q_PROPERTY(TLInt readInboxMaxId READ readInboxMaxId WRITE setReadInboxMaxId NOTIFY readInboxMaxIdChanged)
	Q_PROPERTY(TLInt readOutboxMaxId READ readOutboxMaxId WRITE setReadOutboxMaxId NOTIFY readOutboxMaxIdChanged)
	Q_PROPERTY(TLInt unreadCount READ unreadCount WRITE setUnreadCount NOTIFY unreadCountChanged)
	Q_PROPERTY(TLVector<Message*> messages READ messages WRITE setMessages NOTIFY messagesChanged)
	Q_PROPERTY(TLVector<Chat*> chats READ chats WRITE setChats NOTIFY chatsChanged)
	Q_PROPERTY(TLVector<User*> users READ users WRITE setUsers NOTIFY usersChanged)
	Q_PROPERTY(TLVector<Message*> newMessages READ newMessages WRITE setNewMessages NOTIFY newMessagesChanged)
	Q_PROPERTY(TLVector<Update*> otherUpdates READ otherUpdates WRITE setOtherUpdates NOTIFY otherUpdatesChanged)

	Q_ENUMS(Constructors)

	public:
		enum Constructors {
			CtorUpdatesChannelDifferenceEmpty = 0x3e11affb,
			CtorUpdatesChannelDifferenceTooLong = 0x410dee07,
			CtorUpdatesChannelDifference = 0x2064674e,
		};

	public:
		explicit UpdatesChannelDifference(QObject* parent = 0);
		virtual void read(MTProtoStream* mtstream);
		virtual void write(MTProtoStream* mtstream);
	protected:
		virtual void compileFlags();
	public:
		TLInt flags() const;
		void setFlags(TLInt flags);
		TLTrue isFinal() const;
		void setIsFinal(TLTrue is_final);
		TLInt pts() const;
		void setPts(TLInt pts);
		TLInt timeout() const;
		void setTimeout(TLInt timeout);
		TLInt topMessage() const;
		void setTopMessage(TLInt top_message);
		TLInt readInboxMaxId() const;
		void setReadInboxMaxId(TLInt read_inbox_max_id);
		TLInt readOutboxMaxId() const;
		void setReadOutboxMaxId(TLInt read_outbox_max_id);
		TLInt unreadCount() const;
		void setUnreadCount(TLInt unread_count);
		const TLVector<Message*>& messages() const;
		void setMessages(const TLVector<Message*>& messages);
		const TLVector<Chat*>& chats() const;
		void setChats(const TLVector<Chat*>& chats);
		const TLVector<User*>& users() const;
		void setUsers(const TLVector<User*>& users);
		const TLVector<Message*>& newMessages() const;
		void setNewMessages(const TLVector<Message*>& new_messages);
		const TLVector<Update*>& otherUpdates() const;
		void setOtherUpdates(const TLVector<Update*>& other_updates);

	signals:
		void flagsChanged();
		void isFinalChanged();
		void ptsChanged();
		void timeoutChanged();
		void topMessageChanged();
		void readInboxMaxIdChanged();
		void readOutboxMaxIdChanged();
		void unreadCountChanged();
		void messagesChanged();
		void chatsChanged();
		void usersChanged();
		void newMessagesChanged();
		void otherUpdatesChanged();

	private:
		TLInt _flags;
		TLTrue _is_final;
		TLInt _pts;
		TLInt _timeout;
		TLInt _top_message;
		TLInt _read_inbox_max_id;
		TLInt _read_outbox_max_id;
		TLInt _unread_count;
		TLVector<Message*> _messages;
		TLVector<Chat*> _chats;
		TLVector<User*> _users;
		TLVector<Message*> _new_messages;
		TLVector<Update*> _other_updates;

};

#endif // UPDATESCHANNELDIFFERENCE_H

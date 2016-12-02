#ifndef CONTACTSCONTACTS_H
#define CONTACTSCONTACTS_H

// Generated by APIGenerator 1.0
// DO NOT EDIT!!!

#include "../../types/basic.h"
#include "../../types/telegramobject.h"
#include "contact.h"
#include "user.h"

class ContactsContacts: public TelegramObject
{
	Q_OBJECT

	Q_PROPERTY(TLVector<Contact*> contacts READ contacts WRITE setContacts NOTIFY contactsChanged)
	Q_PROPERTY(TLVector<User*> users READ users WRITE setUsers NOTIFY usersChanged)

	Q_ENUMS(Constructors)

	public:
		enum Constructors {
			CtorContactsContactsNotModified = 0xb74ba9d2,
			CtorContactsContacts = 0x6f8b8cb2,
		};

	public:
		explicit ContactsContacts(QObject* parent = 0);
		virtual void read(MTProtoStream* mtstream);
		virtual void write(MTProtoStream* mtstream);
	protected:
		virtual void compileFlags();
	public:
		const TLVector<Contact*>& contacts() const;
		void setContacts(const TLVector<Contact*>& contacts);
		const TLVector<User*>& users() const;
		void setUsers(const TLVector<User*>& users);

	signals:
		void contactsChanged();
		void usersChanged();

	private:
		TLVector<Contact*> _contacts;
		TLVector<User*> _users;

};

#endif // CONTACTSCONTACTS_H

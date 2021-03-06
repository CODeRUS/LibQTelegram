#ifndef AUTHAUTHORIZATION_H
#define AUTHAUTHORIZATION_H

// Generated by APIGenerator 1.0
// DO NOT EDIT!!!

#include "../../types/basic.h"
#include "../../types/telegramobject.h"
#include "user.h"

class AuthAuthorization: public TelegramObject
{
	Q_OBJECT

	Q_PROPERTY(TLInt flags READ flags WRITE setFlags NOTIFY flagsChanged)
	Q_PROPERTY(TLInt tmpSessions READ tmpSessions WRITE setTmpSessions NOTIFY tmpSessionsChanged)
	Q_PROPERTY(User* user READ user WRITE setUser NOTIFY userChanged)

	Q_ENUMS(Constructors)

	public:
		enum Constructors {
			CtorAuthAuthorization = 0xcd050916,
		};

	public:
		explicit AuthAuthorization(QObject* parent = 0);
		virtual void read(MTProtoStream* mtstream);
		virtual void write(MTProtoStream* mtstream);
	protected:
		virtual void compileFlags();
	public:
		TLInt flags() const;
		void setFlags(TLInt flags);
		TLInt tmpSessions() const;
		void setTmpSessions(TLInt tmp_sessions);
		User* user() const;
		void setUser(User* user);

	signals:
		void flagsChanged();
		void tmpSessionsChanged();
		void userChanged();

	private:
		TLInt _flags;
		TLInt _tmp_sessions;
		User* _user;

};

#endif // AUTHAUTHORIZATION_H

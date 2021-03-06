#ifndef DESTROYSESSIONRES_H
#define DESTROYSESSIONRES_H

// Generated by APIGenerator 1.0
// DO NOT EDIT!!!

#include "../../types/basic.h"
#include "../../types/telegramobject.h"

class DestroySessionRes: public TelegramObject
{
	Q_OBJECT

	Q_PROPERTY(TLLong sessionId READ sessionId WRITE setSessionId NOTIFY sessionIdChanged)

	Q_ENUMS(Constructors)

	public:
		enum Constructors {
			CtorDestroySessionOk = 0xe22045fc,
			CtorDestroySessionNone = 0x62d350c9,
		};

	public:
		explicit DestroySessionRes(QObject* parent = 0);
		virtual void read(MTProtoStream* mtstream);
		virtual void write(MTProtoStream* mtstream);
	protected:
		virtual void compileFlags();
	public:
		TLLong sessionId() const;
		void setSessionId(TLLong session_id);

	signals:
		void sessionIdChanged();

	private:
		TLLong _session_id;

};

#endif // DESTROYSESSIONRES_H

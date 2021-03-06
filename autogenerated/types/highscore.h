#ifndef HIGHSCORE_H
#define HIGHSCORE_H

// Generated by APIGenerator 1.0
// DO NOT EDIT!!!

#include "../../types/basic.h"
#include "../../types/telegramobject.h"

class HighScore: public TelegramObject
{
	Q_OBJECT

	Q_PROPERTY(TLInt pos READ pos WRITE setPos NOTIFY posChanged)
	Q_PROPERTY(TLInt userId READ userId WRITE setUserId NOTIFY userIdChanged)
	Q_PROPERTY(TLInt score READ score WRITE setScore NOTIFY scoreChanged)

	Q_ENUMS(Constructors)

	public:
		enum Constructors {
			CtorHighScore = 0x58fffcd0,
		};

	public:
		explicit HighScore(QObject* parent = 0);
		virtual void read(MTProtoStream* mtstream);
		virtual void write(MTProtoStream* mtstream);
	protected:
		virtual void compileFlags();
	public:
		TLInt pos() const;
		void setPos(TLInt pos);
		TLInt userId() const;
		void setUserId(TLInt user_id);
		TLInt score() const;
		void setScore(TLInt score);

	signals:
		void posChanged();
		void userIdChanged();
		void scoreChanged();

	private:
		TLInt _pos;
		TLInt _user_id;
		TLInt _score;

};

#endif // HIGHSCORE_H

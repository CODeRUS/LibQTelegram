#ifndef STICKERSETCOVERED_H
#define STICKERSETCOVERED_H

// Generated by APIGenerator 1.0
// DO NOT EDIT!!!

#include "../../types/basic.h"
#include "document.h"
#include "../../types/telegramobject.h"
#include "stickerset.h"

class StickerSetCovered: public TelegramObject
{
	Q_OBJECT

	Q_PROPERTY(StickerSet* set READ set WRITE setSet NOTIFY setChanged)
	Q_PROPERTY(Document* cover READ cover WRITE setCover NOTIFY coverChanged)
	Q_PROPERTY(TLVector<Document*> covers READ covers WRITE setCovers NOTIFY coversChanged)

	Q_ENUMS(Constructors)

	public:
		enum Constructors {
			CtorStickerSetCovered = 0x6410a5d2,
			CtorStickerSetMultiCovered = 0x3407e51b,
		};

	public:
		explicit StickerSetCovered(QObject* parent = 0);
		virtual void read(MTProtoStream* mtstream);
		virtual void write(MTProtoStream* mtstream);
	protected:
		virtual void compileFlags();
	public:
		StickerSet* set() const;
		void setSet(StickerSet* set);
		Document* cover() const;
		void setCover(Document* cover);
		const TLVector<Document*>& covers() const;
		void setCovers(const TLVector<Document*>& covers);

	signals:
		void setChanged();
		void coverChanged();
		void coversChanged();

	private:
		StickerSet* _set;
		Document* _cover;
		TLVector<Document*> _covers;

};

#endif // STICKERSETCOVERED_H

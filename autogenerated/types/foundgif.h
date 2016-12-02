#ifndef FOUNDGIF_H
#define FOUNDGIF_H

// Generated by APIGenerator 1.0
// DO NOT EDIT!!!

#include "../../types/basic.h"
#include "document.h"
#include "../../types/telegramobject.h"
#include "photo.h"

class FoundGif: public TelegramObject
{
	Q_OBJECT

	Q_PROPERTY(TLString url READ url WRITE setUrl NOTIFY urlChanged)
	Q_PROPERTY(TLString thumbUrl READ thumbUrl WRITE setThumbUrl NOTIFY thumbUrlChanged)
	Q_PROPERTY(TLString contentUrl READ contentUrl WRITE setContentUrl NOTIFY contentUrlChanged)
	Q_PROPERTY(TLString contentType READ contentType WRITE setContentType NOTIFY contentTypeChanged)
	Q_PROPERTY(TLInt w READ w WRITE setW NOTIFY wChanged)
	Q_PROPERTY(TLInt h READ h WRITE setH NOTIFY hChanged)
	Q_PROPERTY(Photo* photo READ photo WRITE setPhoto NOTIFY photoChanged)
	Q_PROPERTY(Document* document READ document WRITE setDocument NOTIFY documentChanged)

	Q_ENUMS(Constructors)

	public:
		enum Constructors {
			CtorFoundGif = 0x162ecc1f,
			CtorFoundGifCached = 0x9c750409,
		};

	public:
		explicit FoundGif(QObject* parent = 0);
		virtual void read(MTProtoStream* mtstream);
		virtual void write(MTProtoStream* mtstream);
	protected:
		virtual void compileFlags();
	public:
		TLString url() const;
		void setUrl(TLString url);
		TLString thumbUrl() const;
		void setThumbUrl(TLString thumb_url);
		TLString contentUrl() const;
		void setContentUrl(TLString content_url);
		TLString contentType() const;
		void setContentType(TLString content_type);
		TLInt w() const;
		void setW(TLInt w);
		TLInt h() const;
		void setH(TLInt h);
		Photo* photo() const;
		void setPhoto(Photo* photo);
		Document* document() const;
		void setDocument(Document* document);

	signals:
		void urlChanged();
		void thumbUrlChanged();
		void contentUrlChanged();
		void contentTypeChanged();
		void wChanged();
		void hChanged();
		void photoChanged();
		void documentChanged();

	private:
		TLString _url;
		TLString _thumb_url;
		TLString _content_url;
		TLString _content_type;
		TLInt _w;
		TLInt _h;
		Photo* _photo;
		Document* _document;

};

#endif // FOUNDGIF_H

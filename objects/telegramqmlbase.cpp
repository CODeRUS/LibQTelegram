#include "telegramqmlbase.h"
#include "telegram.h"
#include "telegraminitializer.h"
#include "peerprofile.h"
#include "models/dialogsmodel.h"
#include "models/messagesmodel.h"
#include "models/contactsmodel.h"
#include "models/forwarddialogsmodel.h"
#include "models/stickerpackmodel.h"
#include "models/stickersetsmodel.h"
#include "notifications/telegramnotifications.h"
#include "notifications/notificationobject.h"
#include "quick/qquickpeerview.h"
#include "quick/qquickmediamessageitem.h"
#include "quick/qquickstickerview.h"
#include "quick/qquickwaveform.h"

TelegramQmlBase::TelegramQmlBase()
{

}

void TelegramQmlBase::initialize(const QString &uri)
{
    // Basic types
    REGISTER_TYPE(TLTrue);
    REGISTER_TYPE(TLBool);
    REGISTER_TYPE(TLInt);
    REGISTER_TYPE(TLDouble);
    REGISTER_TYPE(TLBytes);
    REGISTER_TYPE(TLString);
    REGISTER_TYPE(TLConstructor);

    // Uncreatable objects
    REGISTER_UNCREATABLE_OBJECT(TelegramObject);

    // Main objects
    REGISTER_OBJECT(TelegramInitializer);
    REGISTER_OBJECT(Telegram);
    REGISTER_OBJECT(TelegramNotifications);
    REGISTER_OBJECT(NotificationObject);
    REGISTER_OBJECT(PeerProfile);

    // Models
    REGISTER_OBJECT(ContactsModel);
    REGISTER_OBJECT(DialogsModel);
    REGISTER_OBJECT(MessagesModel);
    REGISTER_OBJECT(ForwardDialogsModel);
    REGISTER_OBJECT(StickerPackModel);
    REGISTER_OBJECT(StickerSetsModel);

    // Components
    REGISTER_OBJECT_AS(QQuickPeerView, "PeerView");
    REGISTER_OBJECT_AS(QQuickMediaMessageItem, "MediaMessageItem");
    REGISTER_OBJECT_AS(QQuickStickerView, "StickerView");
    REGISTER_OBJECT_AS(QQuickWaveform, "Waveform");
}

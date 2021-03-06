#ifndef TELEGRAM_H
#define TELEGRAM_H

#include <QObject>
#include "objects/telegraminitializer.h"

class Telegram : public QObject
{
    Q_OBJECT

    Q_PROPERTY(TelegramInitializer* initializer READ initializer WRITE setInitializer NOTIFY initializerChanged)
    Q_PROPERTY(User* me READ me CONSTANT FINAL)
    Q_PROPERTY(int apiLayer READ apiLayer CONSTANT FINAL)
    Q_PROPERTY(bool loggedIn READ loggedIn NOTIFY loggedInChanged)
    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)
    Q_PROPERTY(bool syncing READ syncing NOTIFY syncingChanged)
    Q_PROPERTY(bool online READ online WRITE setOnline NOTIFY onlineChanged)
    Q_PROPERTY(bool autoDownload READ autoDownload WRITE setAutoDownload NOTIFY autoDownloadChanged)
    Q_PROPERTY(int unreadCount READ unreadCount NOTIFY unreadCountChanged)

    public:
        explicit Telegram(QObject *parent = 0);
        ~Telegram();
        TelegramInitializer* initializer() const;
        User* me() const;
        int apiLayer() const;
        bool loggedIn() const;
        bool connected() const;
        bool syncing() const;
        bool autoDownload() const;
        bool online() const;
        int unreadCount() const;
        void setInitializer(TelegramInitializer* initializer);
        void setAutoDownload(bool b);
        void setOnline(bool b);

    public: // C++ side API
        void sortDialogs(QList<Dialog*>& dialogs) const;
        bool muteDialog(Dialog* dialog, bool mute);
        bool dialogIsWritable(Dialog* dialog) const;
        QString dialogTitle(Dialog* dialog) const;
        QString dialogStatusText(Dialog* dialog) const;
        QString messagePreview(Message* message) const;
        QString messageText(Message* message) const;
        TelegramObject* messageFrom(Message *message) const;

    public slots: // Login
        void signIn(const QString& phonecode) const;
        void signUp(const QString& firstname, const QString& lastname, const QString& phonecode) const;
        void sendPassword(const QString& password) const;
        void reconnect() const;
        void resendCode() const;

    private slots:
        void onLoginCompleted();

    protected:
        virtual void timerEvent(QTimerEvent *event);

    private:
        QString userList(const TLVector<TLInt> users) const;
        QString messageMediaText(MessageMedia* messagemedia) const;
        QString messageActionText(Message *message) const;

    signals:
        void initializerChanged();
        void syncingChanged();
        void autoDownloadChanged();
        void onlineChanged();
        void loggedInChanged();
        void connectedChanged();
        void signUpRequested();
        void signInRequested();
        void loginCompleted();
        void invalidPassword();
        void unreadCountChanged();
        void floodLock(int seconds);
        void connectionTimeout(int seconds);
        void sessionPasswordNeeded(QString hint);
        void phoneCodeError(QString errormessage);

    private:
        TelegramInitializer* _initializer;
        int _timstatus;
        bool _autodownload;
        bool _loggedin;
        bool _online;
};

#endif // TELEGRAM_H

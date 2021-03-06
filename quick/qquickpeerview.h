#ifndef QQUICKPEERVIEW_H
#define QQUICKPEERVIEW_H

#include "qquickbaseitem.h"

class QQuickPeerView : public QQuickBaseItem
{
    Q_OBJECT

    Q_PROPERTY(TelegramObject* peer READ peer WRITE setPeer NOTIFY peerChanged)
    Q_PROPERTY(QQmlComponent* delegate READ delegate WRITE setDelegate NOTIFY delegateChanged)
    Q_PROPERTY(QString fallbackText READ fallbackText NOTIFY fallbackTextChanged)
    Q_PROPERTY(int size READ size WRITE setSize NOTIFY sizeChanged)

    public:
        QQuickPeerView(QQuickItem* parent = 0);
        TelegramObject* peer() const;
        QQmlComponent* delegate() const;
        QString fallbackText() const;
        int size() const;
        void setPeer(TelegramObject* peer);
        void setDelegate(QQmlComponent* component);
        void setSize(int size);

    private:
        TelegramObject* findPeer(TelegramObject* peer);
        QString fallbackText(TelegramObject* telegramobject) const;
        QString fallbackTitle(const QString& title) const;
        void updateMetrics();
        void initialize();

    private slots:
        void updateView();

    protected:
        virtual void componentComplete();

    signals:
        void peerChanged();
        void delegateChanged();
        void fallbackTextChanged();
        void sizeChanged();

    private:
        QQmlComponent* _delegate;
        TelegramObject* _peer;
        int _size;
};

#endif // QQUICKPEERVIEW_H

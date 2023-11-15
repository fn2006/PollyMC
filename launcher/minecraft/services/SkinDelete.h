#pragma once

#include <minecraft/auth/MinecraftAccount.h>
#include <QFile>
#include <QtNetwork/QtNetwork>
#include "tasks/Task.h"

using SkinDeletePtr = shared_qobject_ptr<class SkinDelete>;

class SkinDelete : public Task {
    Q_OBJECT
   public:
    SkinDelete(QObject* parent, MinecraftAccountPtr acct);
    virtual ~SkinDelete() = default;

   private:
    MinecraftAccountPtr m_account;
    shared_qobject_ptr<QNetworkReply> m_reply;

   protected:
    virtual void executeTask();

   public slots:
    void downloadError(QNetworkReply::NetworkError);
    void sslErrors(const QList<QSslError>& errors);
    void downloadFinished();
};

#pragma once

#include <minecraft/auth/MinecraftAccount.h>
#include <QFile>
#include <QtNetwork/QtNetwork>
#include <memory>
#include "tasks/Task.h"

using SkinUploadPtr = shared_qobject_ptr<class SkinUpload>;

class SkinUpload : public Task {
    Q_OBJECT
   public:
    enum Model { STEVE, ALEX };

    // Note this class takes ownership of the file.
    SkinUpload(QObject* parent, MinecraftAccountPtr account, QByteArray skin, Model model = STEVE);
    virtual ~SkinUpload() {}

   private:
    Model m_model;
    QByteArray m_skin;
    MinecraftAccountPtr m_account;
    shared_qobject_ptr<QNetworkReply> m_reply;

   protected:
    virtual void executeTask();

   public slots:

    void downloadError(QNetworkReply::NetworkError);
    void sslErrors(const QList<QSslError>& errors);

    void downloadFinished();
};

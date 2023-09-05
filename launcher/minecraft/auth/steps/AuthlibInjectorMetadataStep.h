#pragma once
#include <QObject>

#include "QObjectPtr.h"
#include "minecraft/auth/AuthStep.h"

class AuthlibInjectorMetadataStep : public AuthStep {
    Q_OBJECT

   public:
    explicit AuthlibInjectorMetadataStep(AccountData* data);
    virtual ~AuthlibInjectorMetadataStep() noexcept;

    void perform() override;
    void rehydrate() override;

    QString describe() override;

   private slots:
    void onRequestDone(QNetworkReply::NetworkError, QByteArray, QList<QNetworkReply::RawHeaderPair>);
};

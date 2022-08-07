#pragma once
#include <QObject>

#include "QObjectPtr.h"
#include "minecraft/auth/AuthStep.h"


class ElybyProfileStep : public AuthStep {
    Q_OBJECT

public:
    explicit ElybyProfileStep(AccountData *data);
    virtual ~ElybyProfileStep() noexcept;

    void perform() override;
    void rehydrate() override;

    QString describe() override;

private slots:
    void onRequestDone(QNetworkReply::NetworkError, QByteArray, QList<QNetworkReply::RawHeaderPair>);
};

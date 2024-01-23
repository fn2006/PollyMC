#include "AuthlibInjectorMetadataStep.h"

#include <QJsonDocument>
#include <QNetworkRequest>

#include "minecraft/auth/AuthRequest.h"
#include "minecraft/auth/Parsers.h"

AuthlibInjectorMetadataStep::AuthlibInjectorMetadataStep(AccountData* data) : AuthStep(data) {}

AuthlibInjectorMetadataStep::~AuthlibInjectorMetadataStep() noexcept = default;

QString AuthlibInjectorMetadataStep::describe()
{
    return tr("Prefetching authlib-injector metadata.");
}

void AuthlibInjectorMetadataStep::perform()
{
    if (m_data->customAuthlibInjectorUrl == "") {
        emit finished(AccountTaskState::STATE_WORKING, tr("Account has no authlib-injector URL."));
        return;
    }
    QNetworkRequest request = QNetworkRequest(m_data->customAuthlibInjectorUrl);
    AuthRequest* requestor = new AuthRequest(this);
    connect(requestor, &AuthRequest::finished, this, &AuthlibInjectorMetadataStep::onRequestDone);
    requestor->get(request);
}

void AuthlibInjectorMetadataStep::rehydrate() {}

void AuthlibInjectorMetadataStep::onRequestDone(QNetworkReply::NetworkError error,
                                                QByteArray data,
                                                QList<QNetworkReply::RawHeaderPair> headers)
{
    auto requestor = qobject_cast<AuthRequest*>(QObject::sender());
    requestor->deleteLater();

    if (error == QNetworkReply::NoError && data.size() > 0) {
        QJsonParseError jsonError;
        QJsonDocument doc = QJsonDocument::fromJson(data, &jsonError);
        if (jsonError.error == QJsonParseError::NoError) {
            m_data->authlibInjectorMetadata = data.toBase64();
            emit finished(AccountTaskState::STATE_WORKING, tr("Got authlib-injector metadata."));
            return;
        }
    }
    emit finished(AccountTaskState::STATE_WORKING, tr("Didn't get authlib-injector metadata, continuing anyway."));
}

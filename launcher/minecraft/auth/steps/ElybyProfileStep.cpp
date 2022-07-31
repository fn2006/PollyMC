#include "ElybyProfileStep.h"

#include <QNetworkRequest>

#include "minecraft/auth/AuthRequest.h"
#include "minecraft/auth/Parsers.h"
#include "net/NetUtils.h"

ElybyProfileStep::ElybyProfileStep(AccountData* data) : AuthStep(data) {

}

ElybyProfileStep::~ElybyProfileStep() noexcept = default;

QString ElybyProfileStep::describe() {
    return tr("Fetching the Minecraft profile.");
}


void ElybyProfileStep::perform() {
    if (m_data->minecraftProfile.id.isEmpty()) {
        emit finished(AccountTaskState::STATE_FAILED_HARD, tr("A UUID is required to get the profile."));
        return;
    }

    QUrl url = QUrl("https://authserver.ely.by/session/profile/" + m_data->minecraftProfile.id);
    QNetworkRequest req = QNetworkRequest(url);
    AuthRequest *request = new AuthRequest(this);
    connect(request, &AuthRequest::finished, this, &ElybyProfileStep::onRequestDone);
    request->get(req);
}

void ElybyProfileStep::rehydrate() {
    // NOOP, for now. We only save bools and there's nothing to check.
}

void ElybyProfileStep::onRequestDone(
    QNetworkReply::NetworkError error,
    QByteArray data,
    QList<QNetworkReply::RawHeaderPair> headers
) {
    auto requestor = qobject_cast<AuthRequest *>(QObject::sender());
    requestor->deleteLater();

#ifndef NDEBUG
    qDebug() << data;
#endif
    if (error == QNetworkReply::ContentNotFoundError) {
        // NOTE: Succeed even if we do not have a profile. This is a valid account state.
        m_data->minecraftProfile = MinecraftProfile();
        emit finished(
            AccountTaskState::STATE_SUCCEEDED,
            tr("Account has no Minecraft profile.")
        );
        return;
    }
    if (error != QNetworkReply::NoError) {
        qWarning() << "Error getting profile:";
        qWarning() << " HTTP Status:        " << requestor->httpStatus_;
        qWarning() << " Internal error no.: " << error;
        qWarning() << " Error string:       " << requestor->errorString_;

        qWarning() << " Response:";
        qWarning() << QString::fromUtf8(data);

        if (Net::isApplicationError(error)) {
            emit finished(
                AccountTaskState::STATE_FAILED_SOFT,
                tr("Minecraft Java profile acquisition failed: %1").arg(requestor->errorString_)
            );
        }
        else {
            emit finished(
                AccountTaskState::STATE_OFFLINE,
                tr("Minecraft Java profile acquisition failed: %1").arg(requestor->errorString_)
            );
        }
        return;
    }
    if(!Parsers::parseMinecraftProfileMojang(data, m_data->minecraftProfile)) {
        m_data->minecraftProfile = MinecraftProfile();
        emit finished(
            AccountTaskState::STATE_FAILED_SOFT,
            tr("Minecraft Java profile response could not be parsed")
        );
        return;
    }

    emit finished(
        AccountTaskState::STATE_WORKING,
        tr("Minecraft Java profile acquisition succeeded.")
    );
}

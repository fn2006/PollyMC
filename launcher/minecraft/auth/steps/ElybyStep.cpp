#include "ElybyStep.h"

#include "minecraft/auth/AuthRequest.h"
#include "minecraft/auth/Parsers.h"
#include "minecraft/auth/Yggdrasil.h"

ElybyStep::ElybyStep(AccountData* data, QString password) : AuthStep(data), m_password(password) {
    m_yggdrasil = new Yggdrasil(m_data, this);

    connect(m_yggdrasil, &Task::failed, this, &ElybyStep::onAuthFailed);
    connect(m_yggdrasil, &Task::succeeded, this, &ElybyStep::onAuthSucceeded);
    connect(m_yggdrasil, &Task::aborted, this, &ElybyStep::onAuthFailed);
}

ElybyStep::~ElybyStep() noexcept = default;

QString ElybyStep::describe() {
    return tr("Logging in with Ely.by account.");
}

void ElybyStep::rehydrate() {
    // NOOP, for now.
}

void ElybyStep::perform() {
    if(m_password.size()) {
        m_yggdrasil->login(m_password, "https://authserver.ely.by/auth/");
    }
    else {
        m_yggdrasil->refresh("https://authserver.ely.by/auth/");
    }
}

void ElybyStep::onAuthSucceeded() {
    emit finished(AccountTaskState::STATE_WORKING, tr("Logged in with Ely.by"));
}

void ElybyStep::onAuthFailed() {
    // TODO: hook these in again, expand to MSA
    // m_error = m_yggdrasil->m_error;
    // m_aborted = m_yggdrasil->m_aborted;

    auto state = m_yggdrasil->taskState();
    QString errorMessage = tr("Ely.by user authentication failed.");

    // NOTE: soft error in the first step means 'offline'
    if(state == AccountTaskState::STATE_FAILED_SOFT) {
        state = AccountTaskState::STATE_OFFLINE;
        errorMessage = tr("Ely.by user authentication ended with a network error.");
    }
    emit finished(state, errorMessage);
}

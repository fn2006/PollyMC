#pragma once
#include "AuthFlow.h"

class ElybyRefresh : public AuthFlow
{
    Q_OBJECT
public:
    explicit ElybyRefresh(
        AccountData *data,
        QObject *parent = 0
    );
};

class ElybyLogin : public AuthFlow
{
    Q_OBJECT
public:
    explicit ElybyLogin(
        AccountData *data,
        QString password,
        QObject *parent = 0
    );

private:
    QString m_password;
};

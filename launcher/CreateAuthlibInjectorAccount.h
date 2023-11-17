// SPDX-License-Identifier: GPL-3.0-only
/*
 *  Prism Launcher - Minecraft Launcher
 *  Copyright (C) 2023 Evan Goode <mail@evangoo.de>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 3.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include "minecraft/auth/MinecraftAccount.h"
#include "net/NetAction.h"

typedef shared_qobject_ptr<class CreateAuthlibInjectorAccount> CreateAuthlibInjectorAccountPtr;
class CreateAuthlibInjectorAccount : public NetAction {
   public:
    explicit CreateAuthlibInjectorAccount(QUrl url, MinecraftAccountPtr account, QString username);
    static CreateAuthlibInjectorAccountPtr make(QUrl url, MinecraftAccountPtr account, QString username)
    {
        return CreateAuthlibInjectorAccountPtr(new CreateAuthlibInjectorAccount(url, account, username));
    }
    MinecraftAccountPtr getAccount();

    void init() override{};

   protected slots:
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal) override {}
    void downloadError(QNetworkReply::NetworkError error) override;
    void downloadFinished() override;
    void downloadReadyRead() override {}

   public slots:
    void executeTask() override;

   private:
    QUrl m_url;
    MinecraftAccountPtr m_account;
    QString m_username;
};

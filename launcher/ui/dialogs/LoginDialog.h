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

#include <QtCore/QEventLoop>
#include <QtWidgets/QDialog>

#include "minecraft/auth/MinecraftAccount.h"
#include "tasks/Task.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog {
    Q_OBJECT

   public:
    ~LoginDialog();

    static MinecraftAccountPtr newAccount(QWidget* parent, QString message);

   private:
    explicit LoginDialog(QWidget* parent = 0);

    void setUserInputsEnabled(bool enable);

   protected slots:
    void accept();

    void onTaskFailed(const QString& reason);
    void onTaskSucceeded();
    void onTaskStatus(const QString& status);
    void onTaskProgress(qint64 current, qint64 total);

    void on_userTextBox_textEdited(const QString& newText);
    void on_passTextBox_textEdited(const QString& newText);

   private:
    Ui::LoginDialog* ui;
    MinecraftAccountPtr m_account;
    Task::Ptr m_loginTask;
};

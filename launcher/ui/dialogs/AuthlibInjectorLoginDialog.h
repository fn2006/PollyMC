/* Copyright 2013-2021 MultiMC Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QtCore/QEventLoop>
#include <QtNetwork>
#include <QtWidgets/QDialog>

#include <net/NetJob.h>
#include "Application.h"
#include "CreateAuthlibInjectorAccount.h"
#include "minecraft/auth/MinecraftAccount.h"
#include "tasks/Task.h"

namespace Ui {
class AuthlibInjectorLoginDialog;
}

class AuthlibInjectorLoginDialog : public QDialog {
    Q_OBJECT

   public:
    ~AuthlibInjectorLoginDialog();

    static MinecraftAccountPtr newAccount(QWidget* parent, QString message);

   private:
    explicit AuthlibInjectorLoginDialog(QWidget* parent = 0);

    void setUserInputsEnabled(bool enable);

   protected slots:
    void accept();

    void onTaskFailed(const QString& reason);
    void onUrlTaskSucceeded();
    void onTaskSucceeded();
    void onTaskStatus(const QString& status);
    void onTaskProgress(qint64 current, qint64 total);

    void on_userTextBox_textEdited(const QString& newText);
    void on_passTextBox_textEdited(const QString& newText);
    void on_authlibInjectorTextBox_textEdited(const QString& newText);

    void dragEnterEvent(QDragEnterEvent* event);
    void dropEvent(QDropEvent* event);

   private:
    QString fixUrl(QString url);
    QUrl extractAuthlibInjectorUrl(const QMimeData* mimeData);
    Ui::AuthlibInjectorLoginDialog* ui;
    MinecraftAccountPtr m_account;
    Task::Ptr m_loginTask;
    Task::Ptr m_createAuthlibInjectorAccountNetJob;
    CreateAuthlibInjectorAccountPtr m_createAuthlibInjectorAccountTask;
};

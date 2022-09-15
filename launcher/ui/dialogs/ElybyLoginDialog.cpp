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

#include "ElybyLoginDialog.h"
#include "ui_ElybyLoginDialog.h"

#include "minecraft/auth/AccountTask.h"

#include <QtWidgets/QPushButton>

ElybyLoginDialog::ElybyLoginDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ElybyLoginDialog)
{
    ui->setupUi(this);
    ui->progressBar->setVisible(false);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

ElybyLoginDialog::~ElybyLoginDialog()
{
    delete ui;
}

// Stage 1: User interaction
void ElybyLoginDialog::accept()
{
    setUserInputsEnabled(false);
    ui->progressBar->setVisible(true);

    // Setup the login task and start it
    m_account = MinecraftAccount::createElyby(ui->userTextBox->text());
    m_loginTask = m_account->loginElyby(ui->passTextBox->text());
    connect(m_loginTask.get(), &Task::failed, this, &ElybyLoginDialog::onTaskFailed);
    connect(m_loginTask.get(), &Task::succeeded, this, &ElybyLoginDialog::onTaskSucceeded);
    connect(m_loginTask.get(), &Task::status, this, &ElybyLoginDialog::onTaskStatus);
    connect(m_loginTask.get(), &Task::progress, this, &ElybyLoginDialog::onTaskProgress);
    m_loginTask->start();
}

void ElybyLoginDialog::setUserInputsEnabled(bool enable)
{
    ui->userTextBox->setEnabled(enable);
    ui->passTextBox->setEnabled(enable);
    ui->buttonBox->setEnabled(enable);
}

// Enable the OK button only when both textboxes contain something.
void ElybyLoginDialog::on_userTextBox_textEdited(const QString &newText)
{
    ui->buttonBox->button(QDialogButtonBox::Ok)
        ->setEnabled(!newText.isEmpty() && !ui->passTextBox->text().isEmpty());
}
void ElybyLoginDialog::on_passTextBox_textEdited(const QString &newText)
{
    ui->buttonBox->button(QDialogButtonBox::Ok)
        ->setEnabled(!newText.isEmpty() && !ui->userTextBox->text().isEmpty());
}

void ElybyLoginDialog::onTaskFailed(const QString &reason)
{
    // Set message
    auto lines = reason.split('\n');
    QString processed;
    for(auto line: lines) {
        if(line.size()) {
            processed += "<font color='red'>" + line + "</font><br />";
        }
        else {
            processed += "<br />";
        }
    }
    ui->label->setText(processed);

    // Re-enable user-interaction
    setUserInputsEnabled(true);
    ui->progressBar->setVisible(false);
}

void ElybyLoginDialog::onTaskSucceeded()
{
    QDialog::accept();
}

void ElybyLoginDialog::onTaskStatus(const QString &status)
{
    ui->label->setText(status);
}

void ElybyLoginDialog::onTaskProgress(qint64 current, qint64 total)
{
    ui->progressBar->setMaximum(total);
    ui->progressBar->setValue(current);
}

// Public interface
MinecraftAccountPtr ElybyLoginDialog::newAccount(QWidget *parent, QString msg)
{
    ElybyLoginDialog dlg(parent);
    dlg.ui->label->setText(msg);
    if (dlg.exec() == QDialog::Accepted)
    {
        return dlg.m_account;
    }
    return nullptr;
}

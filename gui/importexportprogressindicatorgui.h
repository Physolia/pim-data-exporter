/*
   Copyright (C) 2015-2019 Montel Laurent <montel@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef IMPORTEXPORTPROGRESSINDICATORGUI_H
#define IMPORTEXPORTPROGRESSINDICATORGUI_H

#include "importexportprogressindicatorbase.h"
class QProgressDialog;
class ImportExportProgressIndicatorGui : public ImportExportProgressIndicatorBase
{
    Q_OBJECT
public:
    explicit ImportExportProgressIndicatorGui(QWidget *parentWidget, QObject *parent = nullptr);
    ~ImportExportProgressIndicatorGui() override;

    void increaseProgressDialog() override;
    void createProgressDialog(const QString &title) override;

    void setProgressDialogLabel(const QString &text) override;
    Q_REQUIRED_RESULT bool wasCanceled() const override;
    Q_REQUIRED_RESULT int mergeConfigMessageBox(const QString &configName) const override;
    Q_REQUIRED_RESULT bool overwriteConfigMessageBox(const QString &configName) const override;
    Q_REQUIRED_RESULT bool overwriteDirectoryMessageBox(const QString &directory) const override;
    void showErrorMessage(const QString &message, const QString &title) override;

private:
    QProgressDialog *mProgressDialog = nullptr;
    QWidget *mParentWidget = nullptr;
};

#endif // IMPORTEXPORTPROGRESSINDICATORGUI_H

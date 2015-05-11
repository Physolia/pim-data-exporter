/*
  Copyright (c) 2015 Montel Laurent <montel@kde.org>

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License, version 2, as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef IMPORTEXPORTPROGRESSINDICATORGUI_H
#define IMPORTEXPORTPROGRESSINDICATORGUI_H

#include "importexportprogressindicatorbase.h"
class QProgressDialog;
class ImportExportProgressIndicatorGui : public ImportExportProgressIndicatorBase
{
    Q_OBJECT
public:
    explicit ImportExportProgressIndicatorGui(QWidget *parentWidget, QObject *parent = Q_NULLPTR);
    ~ImportExportProgressIndicatorGui();

    void increaseProgressDialog() Q_DECL_OVERRIDE;
    void createProgressDialog() Q_DECL_OVERRIDE;

    void showInfo(const QString &text) Q_DECL_OVERRIDE;
    bool wasCanceled() const Q_DECL_OVERRIDE;
    int mergeConfigMessageBox(const QString &configName) const Q_DECL_OVERRIDE;
    bool overwriteConfigMessageBox(const QString &configName) const Q_DECL_OVERRIDE;
    bool overwriteDirectoryMessageBox(const QString &directory) const Q_DECL_OVERRIDE;
    void showErrorMessage(const QString &message, const QString &title) Q_DECL_OVERRIDE;
private:
    QProgressDialog *mProgressDialog;
    QWidget *mParentWidget;
};

#endif // IMPORTEXPORTPROGRESSINDICATORGUI_H
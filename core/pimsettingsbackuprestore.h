/*
   Copyright (C) 2014-2017 Montel Laurent <montel@kde.org>

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

#ifndef PIMSETTINGSBACKUPRESTORE_H
#define PIMSETTINGSBACKUPRESTORE_H

#include <QObject>
#include <QHash>
#include "utils.h"
class AbstractImportExportJob;
class ArchiveStorage;

class PIMSETTINGEXPORTER_EXPORT PimSettingsBackupRestore : public QObject
{
    Q_OBJECT
public:
    explicit PimSettingsBackupRestore(QObject *parent = nullptr);
    ~PimSettingsBackupRestore();

    bool backupStart(const QString &filename);
    bool restoreStart(const QString &filename);

    void setStoredParameters(const QHash<Utils::AppsType, Utils::importExportParameters> &stored);

    void nextStep();
    void closeArchive();
    void setExportedInfoFileName(const QString &filename);
private Q_SLOTS:
    void slotJobFinished();

Q_SIGNALS:
    void addInfo(const QString &);
    void addEndLine();
    void updateActions(bool state);
    void addError(const QString &);
    void addTitle(const QString &);
    void jobFinished();
    void backupDone();
    void restoreDone();
    void jobFailed();
    void needSyncResource(const QString &name, const QString &identifier);

protected:
    virtual bool continueToRestore();

    virtual void addExportProgressIndicator();
    AbstractImportExportJob *mImportExportData;
private:
    enum Action {
        Backup,
        Restore
    };
    bool openArchive(const QString &filename, bool readWrite);
    void backupNextStep();
    void restoreNextStep();
    void backupFinished();
    void restoreFinished();
    void executeJob();
    void addDate();

    QString mExportedInfoFileName;
    QHash<Utils::AppsType, Utils::importExportParameters> mStored;
    QHash<Utils::AppsType, Utils::importExportParameters>::const_iterator mStoreIterator;
    Action mAction;
    ArchiveStorage *mArchiveStorage;
};

#endif // PIMSETTINGSBACKUPRESTORE_H

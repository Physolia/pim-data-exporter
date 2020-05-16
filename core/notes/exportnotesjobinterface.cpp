﻿/*
   Copyright (C) 2013-2020 Laurent Montel <montel@kde.org>

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

#include "exportnotesjobinterface.h"

#include <KLocalizedString>
#include <KZip>
#include <QTemporaryFile>
#include <KConfigGroup>

#include <QDir>
#include <QTimer>
#include <QStandardPaths>
#include <resourceconverterimpl.h>

ExportNotesJobInterface::ExportNotesJobInterface(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : AbstractImportExportJob(parent, archiveStorage, typeSelected, numberOfStep)
{
}

ExportNotesJobInterface::~ExportNotesJobInterface()
{
}

void ExportNotesJobInterface::start()
{
    Q_EMIT title(i18n("Start export KNotes settings..."));
    createProgressDialog(i18n("Export KNotes settings"));
    if (mTypeSelected & Utils::Resources) {
        QTimer::singleShot(0, this, &ExportNotesJobInterface::slotCheckBackupResource);
    } else if (mTypeSelected & Utils::Config) {
        QTimer::singleShot(0, this, &ExportNotesJobInterface::slotCheckBackupConfig);
    } else {
        Q_EMIT jobFinished();
    }
}

void ExportNotesJobInterface::backupTheme()
{
    const QString notesThemeDir = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QStringLiteral("/knotes/print/");
    QDir notesThemeDirectory(notesThemeDir);
    if (notesThemeDirectory.exists()) {
        const bool notesDirAdded = archive()->addLocalDirectory(notesThemeDir, Utils::dataPath() +  QStringLiteral("knotes/print"));
        if (!notesDirAdded) {
            Q_EMIT error(i18n("\"%1\" directory cannot be added to backup file.", notesThemeDir));
        }
    }
}

void ExportNotesJobInterface::slotCheckBackupResource()
{
    setProgressDialogLabel(i18n("Backing up resources..."));
    increaseProgressDialog();
    backupTheme();
    mAkonadiInstanceInfo = listOfResource();
    QTimer::singleShot(0, this, &ExportNotesJobInterface::slotWriteNextArchiveResource);
}

void ExportNotesJobInterface::slotCheckBackupConfig()
{
    if (mTypeSelected & Utils::Config) {
        backupConfig();
        increaseProgressDialog();
        if (wasCanceled()) {
            Q_EMIT jobFinished();
            return;
        }
    }
    Q_EMIT jobFinished();
}

void ExportNotesJobInterface::backupConfig()
{
    setProgressDialogLabel(i18n("Backing up config..."));

    const QString knotesStr(QStringLiteral("knotesrc"));
    const QString knotesrc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QLatin1Char('/') + knotesStr;
    if (QFileInfo::exists(knotesrc)) {
        backupFile(knotesrc, Utils::configsPath(), knotesStr);
    }
    const QString globalNoteSettingsStr(QStringLiteral("globalnotesettings"));
    const QString globalNoteSettingsrc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QLatin1Char('/') + globalNoteSettingsStr;

    if (QFileInfo::exists(globalNoteSettingsrc)) {
        KSharedConfigPtr globalnotesettingsrc = KSharedConfig::openConfig(globalNoteSettingsrc);

        QTemporaryFile tmp;
        tmp.open();

        KConfig *knoteConfig = globalnotesettingsrc->copyTo(tmp.fileName());
        const QString selectFolderNoteStr(QStringLiteral("SelectNoteFolder"));
        if (knoteConfig->hasGroup(selectFolderNoteStr)) {
            KConfigGroup selectFolderNoteGroup = knoteConfig->group(selectFolderNoteStr);

            const QString selectFolderNoteGroupStr(QStringLiteral("DefaultFolder"));
            convertCollectionIdsToRealPath(selectFolderNoteGroup, selectFolderNoteGroupStr);
        }
        knoteConfig->sync();
        backupFile(tmp.fileName(), Utils::configsPath(), globalNoteSettingsStr);
        delete knoteConfig;
    }
    backupUiRcFile(QStringLiteral("knotesappui.rc"), QStringLiteral("knotes"));
    backupUiRcFile(QStringLiteral("knotesui.rc"), QStringLiteral("knotes"));
    backupUiRcFile(QStringLiteral("knotes_part.rc"), QStringLiteral("knotes"));
    backupConfigFile(QStringLiteral("akonadi_notes_agent.notifyrc"));
    storeDirectory(QStringLiteral("/knotes/print/theme/"));
    Q_EMIT info(i18n("Config backup done."));
}

void ExportNotesJobInterface::slotNoteJobTerminated()
{
    if (wasCanceled()) {
        Q_EMIT jobFinished();
        return;
    }
    mIndexIdentifier++;
    QTimer::singleShot(0, this, &ExportNotesJobInterface::slotWriteNextArchiveResource);
}

void ExportNotesJobInterface::slotWriteNextArchiveResource()
{
    if (mIndexIdentifier < mAkonadiInstanceInfo.count()) {
        const Utils::AkonadiInstanceInfo agent = mAkonadiInstanceInfo.at(mIndexIdentifier);
        const QString identifier = agent.identifier;
        if (identifier.contains(QLatin1String("akonadi_akonotes_resource_"))) {
            const QString archivePath = Utils::notePath() + identifier + QLatin1Char('/');

            const QString url = resourcePath(identifier);
            if (!mAgentPaths.contains(url) && QDir(url).exists()) {
                if (!url.isEmpty()) {
                    mAgentPaths << url;
                    exportResourceToArchive(archivePath, url, identifier);
                } else {
                    qCDebug(PIMDATAEXPORTERCORE_LOG) << "Url is empty for " << identifier;
                    QTimer::singleShot(0, this, &ExportNotesJobInterface::slotNoteJobTerminated);
                }
            } else {
                QTimer::singleShot(0, this, &ExportNotesJobInterface::slotNoteJobTerminated);
            }
        } else {
            QTimer::singleShot(0, this, &ExportNotesJobInterface::slotNoteJobTerminated);
        }
    } else {
        Q_EMIT info(i18n("Resources backup done."));
        QTimer::singleShot(0, this, &ExportNotesJobInterface::slotCheckBackupConfig);
    }
}
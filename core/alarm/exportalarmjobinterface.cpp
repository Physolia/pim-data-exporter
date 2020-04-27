/*
   Copyright (C) 2012-2020 Laurent Montel <montel@kde.org>

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

#include "exportalarmjobinterface.h"
#include "exportresourcearchivejob.h"

#include <KLocalizedString>

#include <QTemporaryFile>
#include <QTimer>
#include <KConfigGroup>

#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include "resourceconverterimpl.h"

ExportAlarmJobInterface::ExportAlarmJobInterface(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : AbstractImportExportJob(parent, archiveStorage, typeSelected, numberOfStep)
{
}

ExportAlarmJobInterface::~ExportAlarmJobInterface()
{
}

void ExportAlarmJobInterface::start()
{
    Q_EMIT title(i18n("Start export KAlarm settings..."));
    createProgressDialog(i18n("Export KAlarm settings"));
    if (mTypeSelected & Utils::Resources) {
        QTimer::singleShot(0, this, &ExportAlarmJobInterface::slotCheckBackupResource);
    } else if (mTypeSelected & Utils::Config) {
        QTimer::singleShot(0, this, &ExportAlarmJobInterface::slotCheckBackupConfig);
    } else {
        Q_EMIT jobFinished();
    }
}

void ExportAlarmJobInterface::slotCheckBackupResource()
{
    setProgressDialogLabel(i18n("Backing up resources..."));
    increaseProgressDialog();
    exportArchiveResource();
}

void ExportAlarmJobInterface::slotCheckBackupConfig()
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


void ExportAlarmJobInterface::backupConfig()
{
    setProgressDialogLabel(i18n("Backing up config..."));

    const QString kalarmStr(QStringLiteral("kalarmrc"));
    const QString kalarmrc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QLatin1Char('/') + kalarmStr;
    if (QFileInfo::exists(kalarmrc)) {
        KSharedConfigPtr kalarm = KSharedConfig::openConfig(kalarmrc);

        QTemporaryFile tmp;
        tmp.open();

        KConfig *kalarmConfig = kalarm->copyTo(tmp.fileName());

        const QString collectionsStr(QStringLiteral("Collections"));
        if (kalarmConfig->hasGroup(collectionsStr)) {
            KConfigGroup group = kalarmConfig->group(collectionsStr);
            const QString selectionKey(QStringLiteral("FavoriteCollectionIds"));
            ResourceConverterImpl converter;
            converter.convertCollectionIdsToRealPath(group, selectionKey);
        }

        kalarmConfig->sync();
        backupFile(tmp.fileName(), Utils::configsPath(), kalarmStr);
        delete kalarmConfig;
    }

    backupUiRcFile(QStringLiteral("kalarmui.rc"), QStringLiteral("kalarm"));

    Q_EMIT info(i18n("Config backup done."));
}
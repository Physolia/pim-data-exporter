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

#include "exportalarmjobinterfaceimpl.h"

#include <KLocalizedString>
#include <MailCommon/MailUtil>

#include <QTemporaryFile>
#include <QTimer>
#include <KConfigGroup>

#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include "exportresourcearchivejob.h"
#include "resourceconverterimpl.h"

ExportAlarmJobInterfaceImpl::ExportAlarmJobInterfaceImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : ExportAlarmJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{
}

ExportAlarmJobInterfaceImpl::~ExportAlarmJobInterfaceImpl()
{
}

QVector<Utils::AkonadiInstanceInfo> ExportAlarmJobInterfaceImpl::listOfResource()
{
    return Utils::listOfResource();
}

void ExportAlarmJobInterfaceImpl::exportArchiveResource()
{
    mAkonadiInstanceInfo = listOfResource();
    QTimer::singleShot(0, this, &ExportAlarmJobInterfaceImpl::slotWriteNextArchiveResource);
}

Akonadi::Collection::Id ExportAlarmJobInterfaceImpl::convertFolderPathToCollectionId(const QString &path)
{
    ResourceConverterImpl converter;
    return converter.convertFolderPathToCollectionId(path);
}

void ExportAlarmJobInterfaceImpl::exportResourceToArchive(const QString &archivePath, const QString &url, const QString &identifier)
{
    ExportResourceArchiveJob *resourceJob = new ExportResourceArchiveJob(this);
    resourceJob->setArchivePath(archivePath);
    resourceJob->setUrl(url);
    resourceJob->setIdentifier(identifier);
    resourceJob->setArchive(archive());
    resourceJob->setArchiveName(QStringLiteral("alarm.zip"));
    connect(resourceJob, &ExportResourceArchiveJob::error, this, &ExportAlarmJobInterfaceImpl::error);
    connect(resourceJob, &ExportResourceArchiveJob::info, this, &ExportAlarmJobInterfaceImpl::info);
    connect(resourceJob, &ExportResourceArchiveJob::terminated, this, &ExportAlarmJobInterfaceImpl::slotAlarmJobTerminated);
    resourceJob->start();
}

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

#include "exportcalendarjobinterfaceimpl.h"
#include <MailCommon/MailUtil>

#include <KLocalizedString>

#include <QTemporaryFile>
#include <KConfigGroup>

#include <QTimer>
#include <QFile>
#include <QDir>
#include <QColor>
#include "resourceconverterimpl.h"

#include <QStandardPaths>
#include "exportresourcearchivejob.h"

ExportCalendarJobInterfaceImpl::ExportCalendarJobInterfaceImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : ExportCalendarJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{
}

ExportCalendarJobInterfaceImpl::~ExportCalendarJobInterfaceImpl()
{
}

QVector<Utils::AkonadiInstanceInfo> ExportCalendarJobInterfaceImpl::listOfResource()
{
    return Utils::listOfResource();
}

Akonadi::Collection::Id ExportCalendarJobInterfaceImpl::convertFolderPathToCollectionId(const QString &path)
{
    ResourceConverterImpl converter;
    return converter.convertFolderPathToCollectionId(path);
}

void ExportCalendarJobInterfaceImpl::convertCollectionListToRealPath(KConfigGroup &group, const QString &currentKey)
{
    ResourceConverterImpl converter;
    converter.convertCollectionListToRealPath(group, currentKey);
}

QString ExportCalendarJobInterfaceImpl::convertToFullCollectionPath(const qlonglong collectionValue)
{
    ResourceConverterImpl converter;
    return converter.convertToFullCollectionPath(collectionValue);
}

void ExportCalendarJobInterfaceImpl::exportResourceToArchive(const QString &archivePath, const QString &url, const QString &identifier)
{
    ExportResourceArchiveJob *resourceJob = new ExportResourceArchiveJob(this);
    resourceJob->setArchivePath(archivePath);
    resourceJob->setUrl(url);
    resourceJob->setIdentifier(identifier);
    resourceJob->setArchive(archive());
    resourceJob->setArchiveName(Utils::resourceCalendarArchiveName());
    connect(resourceJob, &ExportResourceArchiveJob::error, this, &ExportCalendarJobInterfaceImpl::error);
    connect(resourceJob, &ExportResourceArchiveJob::info, this, &ExportCalendarJobInterfaceImpl::info);
    connect(resourceJob, &ExportResourceArchiveJob::terminated, this, &ExportCalendarJobInterfaceImpl::slotCalendarJobTerminated);
    resourceJob->start();
}

QString ExportCalendarJobInterfaceImpl::resourcePath(const QString &identifier) const
{
    ResourceConverterImpl converter;
    const QString url = converter.resourcePath(identifier);
    return url;
}

void ExportCalendarJobInterfaceImpl::backupCalendarResourceFile(const QString &agentIdentifier, const QString &defaultPath)
{
    backupResourceFile(agentIdentifier, defaultPath);
}

QString ExportCalendarJobInterfaceImpl::adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath)
{
    ResourceConverterImpl converter;
    return converter.adaptNewResourceUrl(overwriteResources, resourceConfig, storePath);
}

QString ExportCalendarJobInterfaceImpl::createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree)
{
    Q_UNREACHABLE();
    return {};
}
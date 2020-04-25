/*
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

#include "exportaddressbookjob.h"
#include "exportresourcearchivejob.h"

#include <AkonadiCore/AgentManager>

#include <KLocalizedString>

#include <QTemporaryFile>
#include <KConfigGroup>

#include <QDir>
#include <QStandardPaths>
#include <QTimer>
#include <resourceconverterimpl.h>

ExportAddressbookJob::ExportAddressbookJob(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : AbstractImportExportJob(parent, archiveStorage, typeSelected, numberOfStep)
{
}

ExportAddressbookJob::~ExportAddressbookJob()
{
}

void ExportAddressbookJob::start()
{
    Q_EMIT title(i18n("Start export KAddressBook settings..."));
    createProgressDialog(i18n("Export KAddressBook settings"));
    if (mTypeSelected & Utils::Resources) {
        QTimer::singleShot(0, this, &ExportAddressbookJob::slotCheckBackupResource);
    } else if (mTypeSelected & Utils::Config) {
        QTimer::singleShot(0, this, &ExportAddressbookJob::slotCheckBackupConfig);
    } else {
        Q_EMIT jobFinished();
    }
}

void ExportAddressbookJob::slotCheckBackupResource()
{
    setProgressDialogLabel(i18n("Backing up resources..."));
    increaseProgressDialog();
    QTimer::singleShot(0, this, &ExportAddressbookJob::slotWriteNextArchiveResource);
}

void ExportAddressbookJob::slotCheckBackupConfig()
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

void ExportAddressbookJob::slotAddressbookJobTerminated()
{
    if (wasCanceled()) {
        Q_EMIT jobFinished();
        return;
    }
    mIndexIdentifier++;
    QTimer::singleShot(0, this, &ExportAddressbookJob::slotWriteNextArchiveResource);
}

void ExportAddressbookJob::slotWriteNextArchiveResource()
{
    Akonadi::AgentManager *manager = Akonadi::AgentManager::self();
    const Akonadi::AgentInstance::List list = manager->instances();
    if (mIndexIdentifier < list.count()) {
        const Akonadi::AgentInstance agent = list.at(mIndexIdentifier);
        const QString identifier = agent.identifier();
        if (identifier.contains(QLatin1String("akonadi_vcarddir_resource_")) || identifier.contains(QLatin1String("akonadi_contacts_resource_"))) {
            const QString archivePath = Utils::addressbookPath() + identifier + QLatin1Char('/');

            ResourceConverterImpl converter;
            const QString url = converter.resourcePath(identifier, QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QStringLiteral("/contacts/"));
            if (!mAgentPaths.contains(url)) {
                mAgentPaths << url;
                if (!url.isEmpty()) {
                    ExportResourceArchiveJob *resourceJob = new ExportResourceArchiveJob(this);
                    resourceJob->setArchivePath(archivePath);
                    resourceJob->setUrl(url);
                    resourceJob->setIdentifier(identifier);
                    resourceJob->setArchive(archive());
                    resourceJob->setArchiveName(QStringLiteral("addressbook.zip"));
                    connect(resourceJob, &ExportResourceArchiveJob::error, this, &ExportAddressbookJob::error);
                    connect(resourceJob, &ExportResourceArchiveJob::info, this, &ExportAddressbookJob::info);
                    connect(resourceJob, &ExportResourceArchiveJob::terminated, this, &ExportAddressbookJob::slotAddressbookJobTerminated);
                    resourceJob->start();
                } else {
                    qCDebug(PIMDATAEXPORTERCORE_LOG) << "Url is empty for " << identifier;
                    QTimer::singleShot(0, this, &ExportAddressbookJob::slotAddressbookJobTerminated);
                }
            } else {
                QTimer::singleShot(0, this, &ExportAddressbookJob::slotAddressbookJobTerminated);
            }
        } else if (identifier.contains(QLatin1String("akonadi_vcard_resource_"))) {
            backupResourceFile(agent, Utils::addressbookPath());
            QTimer::singleShot(0, this, &ExportAddressbookJob::slotAddressbookJobTerminated);
        } else {
            QTimer::singleShot(0, this, &ExportAddressbookJob::slotAddressbookJobTerminated);
        }
    } else {
        Q_EMIT info(i18n("Resources backup done."));
        QTimer::singleShot(0, this, &ExportAddressbookJob::slotCheckBackupConfig);
    }
}

void ExportAddressbookJob::backupConfig()
{
    setProgressDialogLabel(i18n("Backing up config..."));

    const QString kaddressbookStr(QStringLiteral("kaddressbookrc"));
    const QString kaddressbookrc = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + QLatin1Char('/') + kaddressbookStr;
    if (QFileInfo::exists(kaddressbookrc)) {
        KSharedConfigPtr kaddressbook = KSharedConfig::openConfig(kaddressbookrc);

        QTemporaryFile tmp;
        tmp.open();

        KConfig *kaddressBookConfig = kaddressbook->copyTo(tmp.fileName());

        const QString collectionViewCheckStateStr(QStringLiteral("CollectionViewCheckState"));
        if (kaddressBookConfig->hasGroup(collectionViewCheckStateStr)) {
            KConfigGroup group = kaddressBookConfig->group(collectionViewCheckStateStr);
            const QString selectionKey(QStringLiteral("Selection"));
            ResourceConverterImpl converter;
            converter.convertCollectionListToRealPath(group, selectionKey);
        }

        const QString collectionViewStateStr(QStringLiteral("CollectionViewState"));
        if (kaddressBookConfig->hasGroup(collectionViewStateStr)) {
            KConfigGroup group = kaddressBookConfig->group(collectionViewStateStr);
            QString currentKey(QStringLiteral("Current"));
            ResourceConverterImpl converter;
            converter.convertCollectionToRealPath(group, currentKey);

            currentKey = QStringLiteral("Expansion");
            converter.convertCollectionToRealPath(group, currentKey);

            currentKey = QStringLiteral("Selection");
            converter.convertCollectionToRealPath(group, currentKey);
        }
        kaddressBookConfig->sync();
        backupFile(tmp.fileName(), Utils::configsPath(), kaddressbookStr);
        delete kaddressBookConfig;
    }
    backupUiRcFile(QStringLiteral("kaddressbookui.rc"), QStringLiteral("kaddressbook"));

    storeDirectory(QStringLiteral("/kaddressbook/csv-templates/"));
    storeDirectory(QStringLiteral("/kaddressbook/viewertemplates/"));
    storeDirectory(QStringLiteral("/kaddressbook/printing/"));

    Q_EMIT info(i18n("Config backup done."));
}

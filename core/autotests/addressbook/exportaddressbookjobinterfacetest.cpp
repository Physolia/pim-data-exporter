/*
   Copyright (C) 2020 Laurent Montel <montel@kde.org>

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

#include "exportaddressbookjobinterfacetest.h"
#include "archivestorage.h"
#include "testexportfile.h"
#include "resourceconvertertest.h"
#include <QDebug>
#include <QTest>
#include <saveresourceconfigtest.h>
#include <testbackupresourcefilejob.h>

QTEST_MAIN(ExportAddressbookJobInterfaceTest)

ExportAddressbookJobInterfaceTestImpl::ExportAddressbookJobInterfaceTestImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : ExportAddressbookJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{
}

ExportAddressbookJobInterfaceTestImpl::~ExportAddressbookJobInterfaceTestImpl()
{
}

Akonadi::Collection::Id ExportAddressbookJobInterfaceTestImpl::convertFolderPathToCollectionId(const QString &path)
{
    ResourceConverterTest resourceConverterTest;
    resourceConverterTest.setTestPath(mPathConfig);
    return resourceConverterTest.convertFolderPathToCollectionId(path);
}

void ExportAddressbookJobInterfaceTestImpl::exportResourceToArchive(const QString &archivePath, const QString &url, const QString &identifier)
{
    SaveResoureConfigTest saveResourceConfig;
    saveResourceConfig.setArchive(mArchiveStorage->archive());
    saveResourceConfig.exportResourceToArchive(archivePath, url, identifier, Utils::resourceAddressbookArchiveName(),
                                               { QLatin1String("akonadi_vcarddir_resource_"), QLatin1String("akonadi_contacts_resource_") });
    slotAddressbookJobTerminated();
}

QVector<Utils::AkonadiInstanceInfo> ExportAddressbookJobInterfaceTestImpl::listOfResource()
{
    return mListAkonadiInstanceInfo;
}

void ExportAddressbookJobInterfaceTestImpl::convertCollectionToRealPath(KConfigGroup &group, const QString &currentKey)
{
    ResourceConverterTest resourceConverter;
    resourceConverter.setTestPath(mPathConfig);
    resourceConverter.convertCollectionToRealPath(group, currentKey);
}

void ExportAddressbookJobInterfaceTestImpl::convertCollectionListToRealPath(KConfigGroup &group, const QString &currentKey)
{
    ResourceConverterTest resourceConverter;
    resourceConverter.setTestPath(mPathConfig);
    resourceConverter.convertCollectionListToRealPath(group, currentKey);
}

QString ExportAddressbookJobInterfaceTestImpl::adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath)
{
    ResourceConverterTest resourceConverterTest;
    resourceConverterTest.setTestPath(mPathConfig);
    return resourceConverterTest.adaptNewResourceUrl(overwriteResources, resourceConfig, storePath);
}

QString ExportAddressbookJobInterfaceTestImpl::createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree)
{
    Q_UNREACHABLE();
    return {};
}

QString ExportAddressbookJobInterfaceTestImpl::resourcePath(const QString &agentIdentifier, const QString &defaultPath) const
{
    ResourceConverterTest converter;
    converter.setTestPath(mPathConfig);
    const QString url = converter.resourcePath(agentIdentifier, defaultPath);
    return url;
}

void ExportAddressbookJobInterfaceTestImpl::backupAddressBookResourceFile(const QString &agentIdentifier, const QString &defaultPath)
{
    TestBackupResourceFileJob *job = new TestBackupResourceFileJob(this);
    job->setDefaultPath(defaultPath);
    job->setIdentifier(agentIdentifier);
    job->setTestPath(mPathConfig);
    job->setZip(archive());
    connect(job, &TestBackupResourceFileJob::error, this, &ExportAddressbookJobInterfaceTestImpl::error);
    connect(job, &TestBackupResourceFileJob::info, this, &ExportAddressbookJobInterfaceTestImpl::info);
    job->start();
}

ExportAddressbookJobInterfaceTest::ExportAddressbookJobInterfaceTest(QObject *parent)
    : QObject(parent)
{
}

void ExportAddressbookJobInterfaceTest::exportAddressBookConfig_data()
{
    QTest::addColumn<QByteArray>("configpath");
    const QByteArray pathConfig(QByteArray(PIMDATAEXPORTER_DIR) + "/export/");
    QTest::newRow("addressbookonlyconfig") << pathConfig + QByteArray("addressbookonlyconfig/");
}

void ExportAddressbookJobInterfaceTest::exportAddressBookConfig()
{
    QFETCH(QByteArray, configpath);
    TestExportFile *file = new TestExportFile(this);
    file->setPathConfig(configpath);
    ExportAddressbookJobInterfaceTestImpl *exportNote = new ExportAddressbookJobInterfaceTestImpl(this, {Utils::StoredType::Config}, file->archiveStorage(), 1);
    exportNote->setPathConfig(QLatin1String(configpath));
    file->setAbstractImportExportJob(exportNote);
    file->start();
    delete exportNote;
}

void ExportAddressbookJobInterfaceTest::exportAddressBookConfigAndResource_data()
{
    QTest::addColumn<QByteArray>("configpath");
    const QByteArray pathConfig(QByteArray(PIMDATAEXPORTER_DIR) + "/export/");
    QTest::newRow("addressbookconfigandresources") << pathConfig + QByteArray("addressbookconfigandresources/");
}

void ExportAddressbookJobInterfaceTest::exportAddressBookConfigAndResource()
{
    QFETCH(QByteArray, configpath);
    TestExportFile *file = new TestExportFile(this);
    file->setPathConfig(configpath);
    QVector<Utils::AkonadiInstanceInfo> lstInfo;
    Utils::AkonadiInstanceInfo info;

    info.identifier = QStringLiteral("akonadi_vcarddir_resource_1");
    lstInfo << info;
    info.identifier = QStringLiteral("akonadi_contacts_resource_1");
    lstInfo << info;
    info.identifier = QStringLiteral("akonadi_vcard_resource_1");
    lstInfo << info;
    //Add extra resource.
    info.identifier = QStringLiteral("akonadi_kolab_resource_2");
    lstInfo << info;

    ExportAddressbookJobInterfaceTestImpl *exportNote = new ExportAddressbookJobInterfaceTestImpl(this, {Utils::StoredType::Config|Utils::StoredType::Resources}, file->archiveStorage(), 1);
    exportNote->setListOfResource(lstInfo);
    exportNote->setPathConfig(QLatin1String(configpath));
    file->setAbstractImportExportJob(exportNote);
    file->start();
    delete exportNote;
}
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

#include "importaddressbookjobinterfacetest.h"
#include "archivestorage.h"
#include "resourceconvertertest.h"
#include "testimportfile.h"
#include <QSignalSpy>
#include <QTest>
QTEST_MAIN(ImportAddressbookJobInterfaceTest)

ImportAddressbookJobInterfaceTestImpl::ImportAddressbookJobInterfaceTestImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : ImportAddressbookJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{
}

ImportAddressbookJobInterfaceTestImpl::~ImportAddressbookJobInterfaceTestImpl()
{
    //Clean up temp repo
    QVERIFY(QDir(QDir::tempPath() + QLatin1Char('/') + Utils::storeAddressbook()).removeRecursively());
}

Akonadi::Collection::Id ImportAddressbookJobInterfaceTestImpl::convertFolderPathToCollectionId(const QString &path)
{
    ResourceConverterTest resourceConverterTest;
    resourceConverterTest.setTestPath(mPathConfig);
    return resourceConverterTest.convertFolderPathToCollectionId(path);
}

QString ImportAddressbookJobInterfaceTestImpl::createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree)
{
    return mLogCreateResource->logCreateResource(resources, name, settings, synchronizeTree);
}

void ImportAddressbookJobInterfaceTestImpl::synchronizeResource(const QStringList &lst)
{
    mLogCreateResource->logSynchronizeResource(lst);
    slotNextStep();
}

QString ImportAddressbookJobInterfaceTestImpl::adaptResourcePath(const KSharedConfigPtr &resourceConfig, const QString &storedData)
{
    ResourceConverterTest resourceConverterTest;
    resourceConverterTest.setTestPath(mPathConfig);
    return resourceConverterTest.adaptResourcePath(resourceConfig, storedData);
}

QString ImportAddressbookJobInterfaceTestImpl::adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath)
{
    ResourceConverterTest resourceConverterTest;
    resourceConverterTest.setTestPath(mPathConfig);
    return resourceConverterTest.adaptNewResourceUrl(overwriteResources, resourceConfig, storePath);
}

ImportAddressbookJobInterfaceTest::ImportAddressbookJobInterfaceTest(QObject *parent)
    : QObject(parent)
{
}

void ImportAddressbookJobInterfaceTest::importAddressbook_data()
{
    QTest::addColumn<QString>("zipFilePath");
    QTest::addColumn<QString>("testPath");
    const QByteArray pathConfig(QByteArray(PIMDATAEXPORTER_DIR) + "/import/");
    QTest::newRow("addressbookonlyconfig") << QString::fromLatin1(pathConfig) << QStringLiteral("/addressbookonlyconfig/");
}

void ImportAddressbookJobInterfaceTest::importAddressbookConfig()
{
    QFETCH(QString, zipFilePath);
    QFETCH(QString, testPath);
    TestImportFile *file = new TestImportFile(zipFilePath + testPath, this);
    file->setPathConfig(zipFilePath + testPath);
    file->setExtractPath(QDir::tempPath() + testPath);
    file->setExcludePath(Utils::addressbookPath());
    ImportAddressbookJobInterfaceTestImpl *impl = new ImportAddressbookJobInterfaceTestImpl(this, {Utils::StoredType::Config}, file->archiveStorage(), 1);
    file->setAbstractImportExportJob(impl);
    file->setLoggingFilePath(impl->loggingFilePath());
    file->start();
    delete impl;
    delete file;
}

void ImportAddressbookJobInterfaceTest::importAddressbookConfigAndResources_data()
{
    QTest::addColumn<QString>("zipFilePath");
    QTest::addColumn<QString>("testPath");
    const QByteArray pathConfig(QByteArray(PIMDATAEXPORTER_DIR) + "/import/");
    QTest::newRow("addressbookconfigandresources") << QString::fromLatin1(pathConfig) << QStringLiteral("/addressbookconfigandresources/");
}

void ImportAddressbookJobInterfaceTest::importAddressbookConfigAndResources()
{
    QFETCH(QString, zipFilePath);
    QFETCH(QString, testPath);
    TestImportFile *file = new TestImportFile(zipFilePath + testPath, this);
    file->setPathConfig(zipFilePath + testPath);
    file->setExtractPath(QDir::tempPath() + testPath);
    file->setExcludePath(Utils::addressbookPath());
    ImportAddressbookJobInterfaceTestImpl *impl = new ImportAddressbookJobInterfaceTestImpl(this, {Utils::StoredType::Config | Utils::StoredType::Resources}, file->archiveStorage(), 1);
    file->setAbstractImportExportJob(impl);
    file->setLoggingFilePath(impl->loggingFilePath());
    file->start();
    delete impl;
    delete file;
}
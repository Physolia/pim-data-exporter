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

#include "importcalendarjobinterfacetest.h"
#include "archivestorage.h"
#include "resourceconvertertest.h"
#include "testimportfile.h"
#include <QSignalSpy>
#include <QTest>
QTEST_MAIN(ImportCalendarJobInterfaceTest)

ImportCalendarJobInterfaceTestImpl::ImportCalendarJobInterfaceTestImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : ImportCalendarJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{
}

ImportCalendarJobInterfaceTestImpl::~ImportCalendarJobInterfaceTestImpl()
{
    //Clean up temp repo
    QVERIFY(QDir(QDir::tempPath() + QLatin1Char('/') + Utils::storeCalendar()).removeRecursively());
}

Akonadi::Collection::Id ImportCalendarJobInterfaceTestImpl::convertFolderPathToCollectionId(const QString &path)
{
    ResourceConverterTest resourceConverterTest;
    resourceConverterTest.setTestPath(mPathConfig);
    return resourceConverterTest.convertFolderPathToCollectionId(path);
}

QString ImportCalendarJobInterfaceTestImpl::createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree)
{
    return mLogCreateResource->logCreateResource(resources, name, settings, synchronizeTree);
}

void ImportCalendarJobInterfaceTestImpl::synchronizeResource(const QStringList &lst)
{
    mLogCreateResource->logSynchronizeResource(lst);
    slotNextStep();
}

QString ImportCalendarJobInterfaceTestImpl::adaptResourcePath(const KSharedConfigPtr &resourceConfig, const QString &storedData)
{
    ResourceConverterTest resourceConverterTest;
    resourceConverterTest.setTestPath(mPathConfig);
    return resourceConverterTest.adaptResourcePath(resourceConfig, storedData);
}

QString ImportCalendarJobInterfaceTestImpl::adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath)
{
    ResourceConverterTest resourceConverterTest;
    resourceConverterTest.setTestPath(mPathConfig);
    return resourceConverterTest.adaptNewResourceUrl(overwriteResources, resourceConfig, storePath);
}

ImportCalendarJobInterfaceTest::ImportCalendarJobInterfaceTest(QObject *parent)
    : QObject(parent)
{
}

void ImportCalendarJobInterfaceTest::importCalendar_data()
{
    QTest::addColumn<QString>("zipFilePath");
    QTest::addColumn<QString>("testPath");
    QTest::addColumn<Utils::StoredTypes>("options");
    Utils::StoredTypes options = {Utils::StoredType::Config};

    const QByteArray pathConfig(QByteArray(PIMDATAEXPORTER_DIR) + "/import/");
    QTest::newRow("calendaronlyconfig") << QString::fromLatin1(pathConfig) << QStringLiteral("/calendaronlyconfig/") << options;
}

void ImportCalendarJobInterfaceTest::importCalendar()
{
    QFETCH(QString, zipFilePath);
    QFETCH(QString, testPath);
    QFETCH(Utils::StoredTypes, options);
    TestImportFile *file = new TestImportFile(zipFilePath + testPath, this);
    file->setPathConfig(zipFilePath + testPath);
    file->setExtractPath(QDir::tempPath() + testPath);
    file->setExcludePath(Utils::calendarPath());
    ImportCalendarJobInterfaceTestImpl *impl = new ImportCalendarJobInterfaceTestImpl(this, options, file->archiveStorage(), 1);
    file->setAbstractImportExportJob(impl);
    file->setLoggingFilePath(impl->loggingFilePath());
    file->start();
    delete impl;
    delete file;
}
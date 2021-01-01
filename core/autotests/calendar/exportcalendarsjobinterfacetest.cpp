/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "exportcalendarsjobinterfacetest.h"
#include "exportcalendarsjobinterfacetestimpl.h"
#include "archivestorage.h"
#include "testexportfile.h"
#include <QTest>

QTEST_MAIN(ExportCalendarsJobInterfaceTest)

ExportCalendarsJobInterfaceTest::ExportCalendarsJobInterfaceTest(QObject *parent)
    : QObject(parent)
{
}

void ExportCalendarsJobInterfaceTest::exportCalendar_data()
{
    QTest::addColumn<QByteArray>("configpath");
    QTest::addColumn<Utils::StoredTypes>("options");
    Utils::StoredTypes options = {Utils::StoredType::Config};
    const QByteArray pathConfig(QByteArray(PIMDATAEXPORTER_DIR) + "/export/");
    QTest::newRow("calendaronlyconfig") << pathConfig + QByteArray("calendaronlyconfig/") << options;
    options = {Utils::StoredType::Config|Utils::StoredType::Resources};
    QTest::newRow("calendarconfigandresources") << pathConfig + QByteArray("calendarconfigandresources/") << options;
}

void ExportCalendarsJobInterfaceTest::exportCalendar()
{
    QFETCH(QByteArray, configpath);
    QFETCH(Utils::StoredTypes, options);
    auto *file = new TestExportFile(this);
    file->setPathConfig(configpath);
    QVector<Utils::AkonadiInstanceInfo> lstInfo;
    Utils::AkonadiInstanceInfo info;
    info.identifier = QLatin1String("akonadi_icaldir_resource_1");
    lstInfo << info;
    info.identifier = QLatin1String("akonadi_ical_resource_2");
    lstInfo << info;
    //Add extra resource.
    info.identifier = QLatin1String("akonadi_contacts_resource_1");
    lstInfo << info;
    info.identifier = QStringLiteral("akonadi_kontact_resource_2");
    lstInfo << info;

    auto *exportNote = new ExportCalendarsJobInterfaceTestImpl(this, options, file->archiveStorage(), 1);
    exportNote->setListOfResource(lstInfo);
    exportNote->setPathConfig(QLatin1String(configpath));
    file->setAbstractImportExportJob(exportNote);
    file->start();
    delete exportNote;
}

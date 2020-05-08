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

#ifndef EXPORTCALENDARSJOBINTERFACETEST_H
#define EXPORTCALENDARSJOBINTERFACETEST_H

#include <QObject>

#include "calendar/exportcalendarjobinterface.h"

class ExportCalendarsJobInterfaceTestImpl : public ExportCalendarJobInterface
{
    Q_OBJECT
public:
    explicit ExportCalendarsJobInterfaceTestImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep);
    ~ExportCalendarsJobInterfaceTestImpl();

    void setListOfResource(const QVector<Utils::AkonadiInstanceInfo> &instanceInfoList);
    void setPathConfig(const QString &pathConfig);
protected:
    void exportArchiveResource() override;
    Q_REQUIRED_RESULT Akonadi::Collection::Id convertFolderPathToCollectionId(const QString &path) override;
    Q_REQUIRED_RESULT QString convertToFullCollectionPath(const qlonglong collectionValue) override;
    void convertCollectionListToRealPath(KConfigGroup &group, const QString &currentKey) override;
    void exportResourceToArchive(const QString &archivePath, const QString &url, const QString &identifier) override;
    Q_REQUIRED_RESULT QVector<Utils::AkonadiInstanceInfo> listOfResource() override;
    Q_REQUIRED_RESULT QString resourcePath(const QString &identifier) const override;
private:
    QVector<Utils::AkonadiInstanceInfo> mListAkonadiInstanceInfo;
    QString mPathConfig;
};

class ExportCalendarsJobInterfaceTest : public QObject
{
    Q_OBJECT
public:
    explicit ExportCalendarsJobInterfaceTest(QObject *parent = nullptr);
    ~ExportCalendarsJobInterfaceTest() = default;
private Q_SLOTS:
    void exportCalendarConfig_data();
    void exportCalendarConfig();
};

#endif // EXPORTCALENDARSJOBINTERFACETEST_H

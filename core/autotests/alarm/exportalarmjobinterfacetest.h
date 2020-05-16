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

#ifndef ExportAlarmJobInterfaceTest_H
#define ExportAlarmJobInterfaceTest_H

#include <QObject>

#include "alarm/exportalarmjobinterface.h"
#include "exportimportutil.h"

class ExportAlarmJobInterfaceTestImpl : public ExportAlarmJobInterface, public ExportImportUtil
{
    Q_OBJECT
public:
    explicit ExportAlarmJobInterfaceTestImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep);
    ~ExportAlarmJobInterfaceTestImpl();
protected:
    Q_REQUIRED_RESULT Akonadi::Collection::Id convertFolderPathToCollectionId(const QString &path) override;
    void exportResourceToArchive(const QString &archivePath, const QString &url, const QString &identifier) override;
    Q_REQUIRED_RESULT QVector<Utils::AkonadiInstanceInfo> listOfResource() override;
    Q_REQUIRED_RESULT QString resourcePath(const QString &identifier) const override;
    void backupAlarmResourceFile(const QString &agentIdentifier, const QString &defaultPath) override;
    Q_REQUIRED_RESULT QString adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath) override;
    Q_REQUIRED_RESULT QString createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree) override;
};

class ExportAlarmJobInterfaceTest : public QObject
{
    Q_OBJECT
public:
    explicit ExportAlarmJobInterfaceTest(QObject *parent = nullptr);
    ~ExportAlarmJobInterfaceTest() = default;
private Q_SLOTS:
    void exportAlarmConfig();
    void exportAlarmConfig_data();

    void exportAlarmConfigAndResource_data();
    void exportAlarmConfigAndResource();
};

#endif // ExportAlarmJobInterfaceTest_H
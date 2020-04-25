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

#include "exportnotesjobinterfacetest.h"
#include "notes/exportnotesjobinterface.h"
#include "archivestorage.h"
#include <QStandardPaths>
#include <QTest>
#include <QSignalSpy>

QTEST_MAIN(ExportNotesJobInterfaceTest)

ExportNotesJobInterfaceTestImpl::ExportNotesJobInterfaceTestImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : ExportNotesJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{

}

ExportNotesJobInterfaceTestImpl::~ExportNotesJobInterfaceTestImpl()
{
}

void ExportNotesJobInterfaceTestImpl::convertCollectionIdsToRealPath(KConfigGroup &selectFolderNoteGroup, const QString &selectFolderNoteGroupStr)
{
    //TODO
}

void ExportNotesJobInterfaceTestImpl::exportArchiveResource()
{
    //TODO
}

ExportNotesJobInterfaceTest::ExportNotesJobInterfaceTest(QObject *parent)
    : QObject(parent)
{
}

void ExportNotesJobInterfaceTest::importNoteConfigTest1()
{
    //Don't use setTestModeEnabled otherwise we can set env
    //QStandardPaths::setTestModeEnabled(true);
    qputenv("XDG_DATA_HOME", PIMDATAEXPORTER_DIR "/test1/share");
    qputenv("XDG_CONFIG_HOME", PIMDATAEXPORTER_DIR "/test1/config");

    //TODO fix file name.
    const QString storeArchivePath(QStringLiteral("/tmp/foo.zip"));
    ArchiveStorage *archiveStorage = new ArchiveStorage(storeArchivePath, this);
    Q_ASSERT(archiveStorage->openArchive(true));
    Utils::addVersion(archiveStorage->archive());
    Utils::storeDataExportInfo(storeArchivePath, archiveStorage->archive());

    ExportNotesJobInterfaceTestImpl *exportNote = new ExportNotesJobInterfaceTestImpl(this, {Utils::StoredType::Config}, archiveStorage, 1);
    QSignalSpy finish(exportNote, &ExportNotesJobInterfaceTestImpl::jobFinished);
    QSignalSpy error(exportNote, &ExportNotesJobInterfaceTestImpl::error);
    exportNote->start();
    QVERIFY(finish.wait());
    QCOMPARE(error.count(), 0);
    delete exportNote;
    delete archiveStorage;
}


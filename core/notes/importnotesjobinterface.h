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

#ifndef IMPORTNOTESJOB_H
#define IMPORTNOTESJOB_H

#include "abstractimportexportjob.h"
#include "pimdataexportercore_private_export.h"
class ArchiveStorage;

class PIMDATAEXPORTER_TESTS_EXPORT ImportNotesJobInterface : public AbstractImportExportJob
{
    Q_OBJECT
public:
    explicit ImportNotesJobInterface(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep);
    ~ImportNotesJobInterface() override;

    void start() override;
protected:
    virtual Q_REQUIRED_RESULT QString createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree = false) = 0;
    virtual void synchronizeResource(const QStringList &lst) = 0;
    void slotNextStep() override;
private:
    void importKNoteGlobalSettings(const KArchiveFile *kmailsnippet, const QString &kmail2rc, const QString &filename, const QString &prefix);
    void restoreConfig();
    void restoreData();
    void restoreResources();
};

#endif // IMPORTNOTESJOB_H

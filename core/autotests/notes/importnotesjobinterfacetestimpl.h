/*
   SPDX-FileCopyrightText: 2020-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "core/utils.h"
#include "exportimportutil.h"
#include "notes/importnotesjobinterface.h"

class ImportNotesJobInterfaceTestImpl : public ImportNotesJobInterface, public ExportImportUtil
{
    Q_OBJECT
public:
    explicit ImportNotesJobInterfaceTestImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep);
    ~ImportNotesJobInterfaceTestImpl() override;
    void setExistingPathConfig(const QString &path);

protected:
    [[nodiscard]] Akonadi::Collection::Id convertFolderPathToCollectionId(const QString &path) override;
    [[nodiscard]] QString
    createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree = false) override;
    void synchronizeResource(const QStringList &lst) override;
    [[nodiscard]] QString adaptResourcePath(const KSharedConfigPtr &resourceConfig, const QString &storedData) override;
    [[nodiscard]] QString adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath) override;
    [[nodiscard]] QString configLocation() const override;

private:
    QString mExistingPathConfig;
};

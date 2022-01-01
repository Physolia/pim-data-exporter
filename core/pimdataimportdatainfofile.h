/*
   SPDX-FileCopyrightText: 2016-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "pimdataexporter_export.h"
#include <QObject>
#include <QString>
class QTemporaryDir;
class PIMDATAEXPORTER_EXPORT PimDataImportDataInfoFile : public QObject
{
    Q_OBJECT
public:
    explicit PimDataImportDataInfoFile(QObject *parent = nullptr);
    ~PimDataImportDataInfoFile() override;

    void setCurrentFileName(const QString &filename);

    Q_REQUIRED_RESULT QString importDataInfoPath();

private:
    QTemporaryDir *mTempDir = nullptr;
    QString mFilename;
};

/*
   SPDX-FileCopyrightText: 2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef COMPAREEXPORTFILE_H
#define COMPAREEXPORTFILE_H

#include <QString>
class KZip;
class QTemporaryDir;
class CompareExportFile
{
public:
    CompareExportFile();
    ~CompareExportFile();
    void compareFiles();

    Q_REQUIRED_RESULT QString tempFilePath() const;
    void setTempFilePath(const QString &tempFilePath);

    Q_REQUIRED_RESULT QString listFilePath() const;
    void setListFilePath(const QString &listFilePath);

private:
    QString mTempFilePath;
    QString mListFilePath;
    KZip *mZip = nullptr;
    QTemporaryDir *mTempDir = nullptr;
};

#endif // COMPAREEXPORTFILE_H

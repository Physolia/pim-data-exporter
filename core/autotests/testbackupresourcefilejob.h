/*
   SPDX-FileCopyrightText: 2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef TESTBACKUPRESOURCEFILEJOB_H
#define TESTBACKUPRESOURCEFILEJOB_H

#include <backupresourcefilejobbase.h>

class TestBackupResourceFileJob : public BackupResourceFileJobBase
{
    Q_OBJECT
public:
    explicit TestBackupResourceFileJob(QObject *parent = nullptr);
    ~TestBackupResourceFileJob();

    Q_REQUIRED_RESULT QString resourcePath(const QString &identifier) const override;
    void setTestPath(const QString &str);
private:
    QString mTestPath;
};

#endif // TESTBACKUPRESOURCEFILEJOB_H

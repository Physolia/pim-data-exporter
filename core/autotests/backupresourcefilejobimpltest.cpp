/*
   SPDX-FileCopyrightText: 2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "backupresourcefilejobimpltest.h"
#include "backupresourcefilejobimpl.h"
#include <QTest>
QTEST_MAIN(BackupResourceFileJobImplTest)
BackupResourceFileJobImplTest::BackupResourceFileJobImplTest(QObject *parent)
    : QObject(parent)
{
}

void BackupResourceFileJobImplTest::shouldHaveDefaultValue()
{
    BackupResourceFileJobImpl *job = new BackupResourceFileJobImpl;
    QVERIFY(!job->canStart());
    QVERIFY(job->identifier().isEmpty());
    QVERIFY(!job->zip());
    QVERIFY(job->defaultPath().isEmpty());
    delete job;
}

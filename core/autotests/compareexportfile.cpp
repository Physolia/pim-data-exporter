/*
   SPDX-FileCopyrightText: 2020-2021 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "compareexportfile.h"
#include "generatelistfilefromarchive.h"
#include "loadlistfromfile.h"
#include "comparefilehelper.h"
#include <QTest>
#include <QDebug>
#include <KZip>

#define REMOVE_TEMPORARY_FILES 1

CompareExportFile::CompareExportFile()
{
}

CompareExportFile::~CompareExportFile()
{
    delete mZip;
    mZip = nullptr;
    delete mTempDir;
    mTempDir = nullptr;
}

void CompareExportFile::compareFiles()
{
    GenerateListFileFromArchive archive(mTempFilePath);
    //qDebug() << " archive " << archive.listFile();

    LoadListFromFile f(mListFilePath + QStringLiteral("/list.txt"));
    const QStringList archiveList = archive.listFile();
    const bool equal = (f.fileList() == archiveList);
    if (!equal) {
        qDebug() << "Requested : " << f.fileList();
        qDebug() << "List File : " << archiveList;
    }
    QVERIFY(equal);
    mZip = new KZip(mTempFilePath);
    //qDebug() << " mFileName" << mFileName;
    const bool result = mZip->open(QIODevice::ReadOnly);
    QVERIFY(result);
    const KArchiveDirectory *topDirectory = mZip->directory();
    for (const QString &file : archiveList) {
        if (file == QLatin1String("information/VERSION_2") || file == QLatin1String("information/exportdatatype.xml")) {
            continue;
        }
        const KArchiveEntry *currentEntry = topDirectory->entry(file);
        if (currentEntry && currentEntry->isFile()) {
            if (!mTempDir) {
                mTempDir = new QTemporaryDir;
#ifdef REMOVE_TEMPORARY_FILES
                mTempDir->setAutoRemove(true);
#else
                mTempDir->setAutoRemove(false);
#endif
            }
            const auto *currentFile = static_cast<const KArchiveFile *>(currentEntry);

            QString adaptFile = file;
            //We store in zip as configs, but we extract in config/
            adaptFile.replace(QStringLiteral("configs/"), QStringLiteral("config/"));
            const QString fileName = mTempDir->path() + QLatin1Char('/') + adaptFile;
            //create path
            const QFileInfo fileInfo(fileName);
            QDir().mkpath(fileInfo.dir().path());

            QFile f(fileName);
            //qDebug() << " fileName" << fileName;
            QVERIFY(f.open(QIODevice::WriteOnly));

            //Store data.
            const QByteArray data = currentFile->data();
            QCOMPARE(f.write(data), data.length());
            f.close();

            CompareFileHelper::compareFile(mListFilePath + QStringLiteral("/references/") + adaptFile, fileName);
        }
    }
}

QString CompareExportFile::tempFilePath() const
{
    return mTempFilePath;
}

void CompareExportFile::setTempFilePath(const QString &tempFilePath)
{
    mTempFilePath = tempFilePath;
}

QString CompareExportFile::listFilePath() const
{
    return mListFilePath;
}

void CompareExportFile::setListFilePath(const QString &listFilePath)
{
    mListFilePath = listFilePath;
}

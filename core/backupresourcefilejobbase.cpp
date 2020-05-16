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

#include "backupresourcefilejobbase.h"
#include "resourceconverterimpl.h"
#include "storeresourcejob.h"
#include "pimdataexportcore_debug.h"

#include <KLocalizedString>

#include <QFileInfo>

BackupResourceFileJobBase::BackupResourceFileJobBase(QObject *parent)
    : QObject(parent)
{
}

BackupResourceFileJobBase::~BackupResourceFileJobBase()
{
}

void BackupResourceFileJobBase::start()
{
    if (!canStart()) {
        qCWarning(PIMDATAEXPORTERCORE_LOG) << "Impossible to start backupResourceFileJob";
        deleteLater();
        return;
    }

    const QString archivePath = mDefaultPath + mIdentifier + QLatin1Char('/');

    const QString url = resourcePath(mIdentifier);
    if (!url.isEmpty()) {
        QFileInfo fi(url);
        if (!fi.isFile()) {
            Q_EMIT info(i18n("\"%1\" is not a file.", url));
        } else {
            const QString filename = fi.fileName();
            const bool fileAdded = mZip->addLocalFile(url, archivePath + filename);
            if (fileAdded) {
                Q_EMIT info(i18n("\"%1\" was backed up.", filename));

                StoreResourceJob *job = new StoreResourceJob(this);
                connect(job, &StoreResourceJob::error, this, &BackupResourceFileJobBase::error);
                connect(job, &StoreResourceJob::info, this, &BackupResourceFileJobBase::info);
                job->setArchivePath(archivePath);
                job->setZip(mZip);
                job->setIdentifier(mIdentifier);
                job->start();
            } else {
                Q_EMIT error(i18n("\"%1\" file cannot be added to backup file.", filename));
            }
        }
    }
    deleteLater();
}

bool BackupResourceFileJobBase::canStart() const
{
    return mZip && !mDefaultPath.isEmpty() && !mIdentifier.isEmpty();
}

QString BackupResourceFileJobBase::identifier() const
{
    return mIdentifier;
}

void BackupResourceFileJobBase::setIdentifier(const QString &identifier)
{
    mIdentifier = identifier;
}

KZip *BackupResourceFileJobBase::zip() const
{
    return mZip;
}

void BackupResourceFileJobBase::setZip(KZip *zip)
{
    mZip = zip;
}

QString BackupResourceFileJobBase::defaultPath() const
{
    return mDefaultPath;
}

void BackupResourceFileJobBase::setDefaultPath(const QString &defaultPath)
{
    mDefaultPath = defaultPath;
}
/*
   Copyright (C) 2015-2017 Montel Laurent <montel@kde.org>

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

#ifndef EXPORTRESOURCEARCHIVEJOB_H
#define EXPORTRESOURCEARCHIVEJOB_H

#include <QObject>
class KZip;
class PimSettingBackupThread;
class ExportResourceArchiveJob : public QObject
{
    Q_OBJECT
public:
    explicit ExportResourceArchiveJob(QObject *parent = nullptr);
    ~ExportResourceArchiveJob();

    void setArchivePath(const QString &archivePath);

    void setUrl(const QString &url);

    void setIdentifier(const QString &identifier);

    void setArchive(KZip *zip);
    void start();
    void setArchiveName(const QString &archiveName);

Q_SIGNALS:
    void error(const QString &str);
    void info(const QString &str);
    void terminated();


public Q_SLOTS:
    void slotTaskCanceled();

private:
    void slotTerminated(bool success);
    void finished();
    QString mArchiveName;
    QString mUrl;
    QString mArchivePath;
    QString mIdentifier;
    KZip *mZip = nullptr;
    PimSettingBackupThread *mThread = nullptr;
};

#endif // EXPORTRESOURCEARCHIVEJOB_H

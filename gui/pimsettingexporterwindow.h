/*
   Copyright (C) 2012-2017 Montel Laurent <montel@kde.org>

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

#ifndef PIMSETTINGEXPORTERWINDOW_H
#define PIMSETTINGEXPORTERWINDOW_H
#include <kxmlguiwindow.h>
#include "utils.h"
class LogWidget;
class KRecentFilesAction;
class QAction;
class QCommandLineParser;
class PimSettingsBackupRestoreUI;
class PimSettingsTrayIcon;
class PimSettingExporterWindow : public KXmlGuiWindow
{
    Q_OBJECT
public:
    explicit PimSettingExporterWindow(QWidget *parent = nullptr);
    ~PimSettingExporterWindow();
    void handleCommandLine(const QCommandLineParser &parser);

private Q_SLOTS:
    void slotBackupData();
    void slotRestoreData();
    void slotAddInfo(const QString &info);
    void slotAddError(const QString &info);
    void slotAddTitle(const QString &info);
    void slotAddEndLine();
    void slotSaveLog();
    void slotShowStructureInfos();
    void slotRestoreFile(const QUrl &url);
    void slotShowArchiveInformations();
    void slotUpdateActions(bool inAction);
    void slotShowBackupFinishDialogInformation();
    void slotJobFailed();
    void slotJobFinished();
    void slotShowCurrentArchiveInformations();
    void slotAddResourceToSync(const QString &name, const QString &identifier);
    void slotFullSyncFinished();
    void slotFullSyncInstanceDone(const QString &identifier);
    void slotFullSyncInstanceFailed(const QString &identifier);
    void slotRestoreDone();
    void slotConfigure();
private:
    void initializeBackupRestoreUi();
    void backupData(const QString &filename = QString(), const QString &templateFile = QString());
    void loadData(const QString &filename = QString(), const QString &templateFile = QString());
    void setupActions(bool canZipFile);
    void showFinishInformation();
    QString mLastArchiveFileName;
    // Name, identifier
    QHash<QString, QString> mNeedToSyncResources;
    LogWidget *mLogWidget= nullptr;
    KRecentFilesAction *mRecentFilesAction= nullptr;
    QAction *mBackupAction= nullptr;
    QAction *mRestoreAction= nullptr;
    QAction *mSaveLogAction= nullptr;
    QAction *mArchiveStructureInfo= nullptr;
    QAction *mShowArchiveInformationsAction= nullptr;
    QAction *mShowArchiveInformationsAboutCurrentArchiveAction= nullptr;
    PimSettingsBackupRestoreUI *mPimSettingsBackupRestoreUI= nullptr;
    PimSettingsTrayIcon *mTrayIcon = nullptr;
};

#endif /* PIMSETTINGEXPORTERWINDOW_H */

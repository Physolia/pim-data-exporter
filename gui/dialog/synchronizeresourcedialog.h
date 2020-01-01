/*
   Copyright (C) 2015-2020 Laurent Montel <montel@kde.org>

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

#ifndef SYNCHRONIZERESOURCEDIALOG_H
#define SYNCHRONIZERESOURCEDIALOG_H

#include <QDialog>
class QListWidget;
class SynchronizeResourceDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SynchronizeResourceDialog(QWidget *parent = nullptr);
    ~SynchronizeResourceDialog();

    void setResources(const QHash<QString, QString> &resources);
    Q_REQUIRED_RESULT QStringList resources() const;

private:
    void slotAccepted();
    void slotSelectAll();
    void slotUnselectAll();

    enum DataType {
        ResourceIdentifier = Qt::UserRole + 1
    };

    void readConfig();
    void writeConfig();
    void selectItem(bool state);
    QListWidget *mListResourceWidget = nullptr;
};

#endif // SYNCHRONIZERESOURCEDIALOG_H

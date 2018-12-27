/*
   Copyright (C) 2013-2019 Montel Laurent <montel@kde.org>

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

#ifndef SELECTIONTYPETREEWIDGET_H
#define SELECTIONTYPETREEWIDGET_H

#include <QTreeWidget>
#include <QHash>
#include "utils.h"

class QTreeWidgetItem;

class SelectionTypeTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit SelectionTypeTreeWidget(QWidget *parent = nullptr);
    ~SelectionTypeTreeWidget();

    QHash<Utils::AppsType, Utils::importExportParameters> storedType() const;

    void selectAllItems();
    void unSelectAllItems();

    void saveAsTemplate();

    void loadTemplate(const QString &fileName = QString());
    void saveAsDefaultTemplate();
    void loadDefaultTemplate();
    QString exportedFileInfo();
    void removeNotSelectedItems();


private:
    void slotItemChanged(QTreeWidgetItem *, int);

    enum ActionType {
        action = Qt::UserRole + 1
    };

    void initialize();
    void setSelectItems(bool b);
    void changeState(QTreeWidgetItem *item, bool b);
    void createSubItem(QTreeWidgetItem *parent, Utils::StoredType type);
    void setParameters(const QHash<Utils::AppsType, Utils::importExportParameters> &params);

    Utils::importExportParameters typeChecked(QTreeWidgetItem *parent) const;
    void initializeSubItem(QTreeWidgetItem *item, Utils::StoredTypes types);
    void loadFileName(const QString &fileName);
    QString templateSelectionToString();
    bool removeNotSelectedItem(QTreeWidgetItem *parent);

    QTreeWidgetItem *mKmailItem = nullptr;
    QTreeWidgetItem *mKalarmItem = nullptr;
    QTreeWidgetItem *mKaddressbookItem = nullptr;
    QTreeWidgetItem *mKorganizerItem = nullptr;
    QTreeWidgetItem *mKNotesItem = nullptr;
    QTreeWidgetItem *mAkregatorItem = nullptr;
};

#endif // SELECTIONTYPETREEWIDGET_H

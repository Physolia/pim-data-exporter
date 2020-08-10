/*
   SPDX-FileCopyrightText: 2012-2020 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef SELECTIONTYPEDIALOG_H
#define SELECTIONTYPEDIALOG_H

#include <QDialog>
#include "utils.h"
class SelectionTypeTreeWidget;
class QCheckBox;
class SelectionTypeDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SelectionTypeDialog(bool backupData, QWidget *parent = nullptr);
    ~SelectionTypeDialog();

    Q_REQUIRED_RESULT QHash<Utils::AppsType, Utils::importExportParameters> storedType() const;

    void loadTemplate(const QString &fileName);

    Q_REQUIRED_RESULT QString exportedFileInfo() const;
    void removeNotSelectedItems();
private:
    void slotSelectAll();
    void slotUnselectAll();

    void slotSaveAsTemplate();
    void slotLoadTemplate();
    void readConfig();
    void writeConfig();
    void loadDefaultTemplate();
    void saveDefaultTemplate();
    SelectionTypeTreeWidget *mSelectionTreeWidget = nullptr;
    QCheckBox *mUseTemplateByDefault = nullptr;
    QPushButton *mSaveTemplate = nullptr;
    QPushButton *mLoadTemplate = nullptr;
};

#endif // SELECTIONTYPEDIALOG_H

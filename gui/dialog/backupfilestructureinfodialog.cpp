/*
   SPDX-FileCopyrightText: 2013-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "backupfilestructureinfodialog.h"

#include <KPIMTextEdit/PlainTextEditor>
#include <KPIMTextEdit/PlainTextEditorWidget>

#include <KLocalizedString>
#include <KSharedConfig>

#include <KMessageBox>

#include <KConfigGroup>
#include <QDialogButtonBox>
#include <QFile>
#include <QLabel>
#include <QPushButton>
#include <QStandardPaths>
#include <QVBoxLayout>

BackupFileStructureInfoDialog::BackupFileStructureInfoDialog(QWidget *parent)
    : QDialog(parent)
    , mEditor(new KPIMTextEdit::PlainTextEditorWidget(this))
{
    setWindowTitle(i18nc("@title:window", "Archive File Structure"));
    auto mainLayout = new QVBoxLayout(this);
    auto lab = new QLabel(i18n("Backup Archive Structure:"));
    mainLayout->addWidget(lab);
    mEditor->editor()->setWebShortcutSupport(false);
    mEditor->setReadOnly(true);
    mainLayout->addWidget(mEditor);

    auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &BackupFileStructureInfoDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &BackupFileStructureInfoDialog::reject);
    buttonBox->button(QDialogButtonBox::Close)->setDefault(true);
    setModal(true);
    mainLayout->addWidget(buttonBox);

    loadStructure();
    readConfig();
}

BackupFileStructureInfoDialog::~BackupFileStructureInfoDialog()
{
    writeConfig();
}

void BackupFileStructureInfoDialog::loadStructure()
{
    QFile f(QStringLiteral(":/structure/backup-structure.txt"));
    if (!f.open(QIODevice::ReadOnly)) {
        KMessageBox::error(this, i18n("backup-structure.txt file was not found."));
        return;
    }
    mEditor->setPlainText(QString::fromLatin1(f.readAll()));
}

void BackupFileStructureInfoDialog::writeConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), "BackupFileStructureInfoDialog");
    group.writeEntry("Size", size());
}

void BackupFileStructureInfoDialog::readConfig()
{
    KConfigGroup group(KSharedConfig::openStateConfig(), "BackupFileStructureInfoDialog");
    const QSize sizeDialog = group.readEntry("Size", QSize(600, 400));
    if (sizeDialog.isValid()) {
        resize(sizeDialog);
    }
}

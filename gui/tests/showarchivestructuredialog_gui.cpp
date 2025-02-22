/*
   SPDX-FileCopyrightText: 2015-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "../dialog/showarchivestructuredialog.h"

#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QFileDialog>
#include <QStandardPaths>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QStandardPaths::setTestModeEnabled(true);
    QCommandLineParser parser;
    parser.addVersionOption();
    parser.addHelpOption();
    parser.addOption(QCommandLineOption(QStringList() << QStringLiteral("+[url]"), QStringLiteral("URL of a archive to open")));
    parser.process(app);

    QString fileName;
    if (parser.positionalArguments().isEmpty()) {
        fileName = QFileDialog::getOpenFileName(nullptr, QString(), QString(), QStringLiteral("Zip file (*.zip)"));
    } else {
        fileName = parser.positionalArguments().at(0);
    }
    if (fileName.isEmpty()) {
        return 0;
    }
    auto dialog = new ShowArchiveStructureDialog(fileName);
    dialog->resize(800, 600);
    dialog->show();
    app.exec();
    delete dialog;
    return 0;
}

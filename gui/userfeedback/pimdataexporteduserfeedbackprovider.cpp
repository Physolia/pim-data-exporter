/*
   SPDX-FileCopyrightText: 2020-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "pimdataexporteduserfeedbackprovider.h"
#include <KUserFeedbackQt6/ApplicationVersionSource>
#include <KUserFeedbackQt6/LocaleInfoSource>
#include <KUserFeedbackQt6/PlatformInfoSource>
#include <KUserFeedbackQt6/QtVersionSource>
#include <KUserFeedbackQt6/ScreenInfoSource>
#include <KUserFeedbackQt6/StartCountSource>
#include <KUserFeedbackQt6/UsageTimeSource>

PimDataExportedUserFeedbackProvider::PimDataExportedUserFeedbackProvider(QObject *parent)
    : KUserFeedback::Provider(parent)
{
    setProductIdentifier(QStringLiteral("org.kde.pim-data-exporter"));
    setFeedbackServer(QUrl(QStringLiteral("https://telemetry.kde.org/")));
    setSubmissionInterval(7);
    setApplicationStartsUntilEncouragement(5);
    setEncouragementDelay(30);

    addDataSource(new KUserFeedback::ApplicationVersionSource);
    addDataSource(new KUserFeedback::PlatformInfoSource);
    addDataSource(new KUserFeedback::ScreenInfoSource);
    addDataSource(new KUserFeedback::QtVersionSource);

    addDataSource(new KUserFeedback::StartCountSource);
    addDataSource(new KUserFeedback::UsageTimeSource);

    addDataSource(new KUserFeedback::LocaleInfoSource);
}

PimDataExportedUserFeedbackProvider::~PimDataExportedUserFeedbackProvider() = default;

#include "moc_pimdataexporteduserfeedbackprovider.cpp"

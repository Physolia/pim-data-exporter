/*
   SPDX-FileCopyrightText: 2012-2023 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "pimdataexporterkernel.h"

#include <KContacts/Addressee>
#include <KContacts/ContactGroup>

#include <Akonadi/ChangeRecorder>
#include <Akonadi/EntityMimeTypeFilterModel>
#include <Akonadi/EntityTreeModel>
#include <Akonadi/NoteUtils>
#include <Akonadi/Session>
#include <KCalendarCore/Event>
#include <KCalendarCore/Todo>
#include <KIdentityManagementCore/IdentityManager>
#include <KSharedConfig>
#include <MailCommon/FolderCollectionMonitor>
#include <MessageComposer/AkonadiSender>

// Same element as kalarmcal
const QLatin1String MIME_ACTIVE("application/x-vnd.kde.alarm.active");
const QLatin1String MIME_ARCHIVED("application/x-vnd.kde.alarm.archived");
const QLatin1String MIME_TEMPLATE("application/x-vnd.kde.alarm.template");

PimDataExporterKernel::PimDataExporterKernel(QObject *parent)
    : QObject(parent)
{
    mIdentityManager = KIdentityManagementCore::IdentityManager::self();
    auto session = new Akonadi::Session("Backup Mail Kernel ETM", this);

    mFolderCollectionMonitor = new MailCommon::FolderCollectionMonitor(session, this);
    mFolderCollectionMonitor->monitor()->setMimeTypeMonitored(KContacts::Addressee::mimeType(), true);
    mFolderCollectionMonitor->monitor()->setMimeTypeMonitored(KContacts::ContactGroup::mimeType(), true);
    mFolderCollectionMonitor->monitor()->setMimeTypeMonitored(KCalendarCore::Todo::todoMimeType(), true);
    mFolderCollectionMonitor->monitor()->setMimeTypeMonitored(KCalendarCore::Event::eventMimeType(), true);
    mFolderCollectionMonitor->monitor()->setMimeTypeMonitored(Akonadi::NoteUtils::noteMimeType(), true);
    mFolderCollectionMonitor->monitor()->setMimeTypeMonitored(MIME_ACTIVE);
    mFolderCollectionMonitor->monitor()->setMimeTypeMonitored(MIME_ARCHIVED);
    mFolderCollectionMonitor->monitor()->setMimeTypeMonitored(MIME_TEMPLATE);

    mEntityTreeModel = new Akonadi::EntityTreeModel(folderCollectionMonitor(), this);
    mEntityTreeModel->setListFilter(Akonadi::CollectionFetchScope::Enabled);
    mEntityTreeModel->setItemPopulationStrategy(Akonadi::EntityTreeModel::LazyPopulation);

    mCollectionModel = new Akonadi::EntityMimeTypeFilterModel(this);
    mCollectionModel->setSourceModel(mEntityTreeModel);
    mCollectionModel->addMimeTypeInclusionFilter(Akonadi::Collection::mimeType());
    mCollectionModel->setHeaderGroup(Akonadi::EntityTreeModel::CollectionTreeHeaders);
    mCollectionModel->setDynamicSortFilter(true);
    mCollectionModel->setSortCaseSensitivity(Qt::CaseInsensitive);
}

KIdentityManagementCore::IdentityManager *PimDataExporterKernel::identityManager()
{
    return mIdentityManager;
}

MessageComposer::MessageSender *PimDataExporterKernel::msgSender()
{
    Q_ASSERT(false);
    return nullptr;
}

Akonadi::EntityMimeTypeFilterModel *PimDataExporterKernel::collectionModel() const
{
    return mCollectionModel;
}

KSharedConfig::Ptr PimDataExporterKernel::config()
{
    return KSharedConfig::openConfig();
}

void PimDataExporterKernel::syncConfig()
{
    Q_ASSERT(false);
}

MailCommon::JobScheduler *PimDataExporterKernel::jobScheduler() const
{
    Q_ASSERT(false);
    return nullptr;
}

Akonadi::ChangeRecorder *PimDataExporterKernel::folderCollectionMonitor() const
{
    return mFolderCollectionMonitor->monitor();
}

void PimDataExporterKernel::updateSystemTray()
{
    Q_ASSERT(false);
}

bool PimDataExporterKernel::showPopupAfterDnD()
{
    return false;
}

qreal PimDataExporterKernel::closeToQuotaThreshold()
{
    return 80;
}

QStringList PimDataExporterKernel::customTemplates()
{
    Q_ASSERT(false);
    return {};
}

bool PimDataExporterKernel::excludeImportantMailFromExpiry()
{
    Q_ASSERT(false);
    return true;
}

Akonadi::Collection::Id PimDataExporterKernel::lastSelectedFolder()
{
    Q_ASSERT(false);
    return Akonadi::Collection::Id();
}

void PimDataExporterKernel::setLastSelectedFolder(Akonadi::Collection::Id col)
{
    Q_UNUSED(col)
}

void PimDataExporterKernel::expunge(Akonadi::Collection::Id col, bool sync)
{
    Q_UNUSED(col)
    Q_UNUSED(sync)
}

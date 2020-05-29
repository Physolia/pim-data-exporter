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

#include "importmailjobinterfacetestimpl.h"
#include "archivestorage.h"
#include "resourceconvertertest.h"
#include "testimportfile.h"
#include "utils.h"
#include "mail/smtpmailtransport.h"
#include <KConfigGroup>
#include <KIdentityManagement/Identity>
#include <QTest>

ImportMailJobInterfaceTestImpl::ImportMailJobInterfaceTestImpl(QObject *parent, Utils::StoredTypes typeSelected, ArchiveStorage *archiveStorage, int numberOfStep)
    : ImportMailJobInterface(parent, typeSelected, archiveStorage, numberOfStep)
{
}

ImportMailJobInterfaceTestImpl::~ImportMailJobInterfaceTestImpl()
{
    //QVERIFY(QDir(extractPath()).removeRecursively());
}

void ImportMailJobInterfaceTestImpl::start()
{
    QDir().mkpath(extractPath());
    ImportMailJobInterface::start();
}

Akonadi::Collection::Id ImportMailJobInterfaceTestImpl::convertFolderPathToCollectionId(const QString &path)
{
    ResourceConverterTest resourceConverterTest;
    resourceConverterTest.setTestPath(mPathConfig);
    return resourceConverterTest.convertFolderPathToCollectionId(path);
}

void ImportMailJobInterfaceTestImpl::synchronizeResource(const QStringList &lst)
{
    mLogCreateResource->logSynchronizeResource(lst);
    slotNextStep();
}

QString ImportMailJobInterfaceTestImpl::createResource(const QString &resources, const QString &name, const QMap<QString, QVariant> &settings, bool synchronizeTree)
{
    return mLogCreateResource->logCreateResource(resources, name, settings, synchronizeTree);
}

void ImportMailJobInterfaceTestImpl::registerSpecialCollection(Akonadi::SpecialMailCollections::Type type, qint64 colId)
{
    mLogCreateResource->logRegisterSpecialCollection(type, colId);
}

void ImportMailJobInterfaceTestImpl::importFilters(const QVector<MailCommon::MailFilter *> &filters)
{
    qDebug() << " void ImportMailJobInterfaceTestImpl::importFilters(const QVector<MailCommon::MailFilter *> &filters)";
}

QString ImportMailJobInterfaceTestImpl::adaptResourcePath(const KSharedConfigPtr &resourceConfig, const QString &storedData)
{
    ResourceConverterTest resourceConverterTest;
    resourceConverterTest.setTestPath(mPathConfig);
    return resourceConverterTest.adaptResourcePath(resourceConfig, storedData);
}

QString ImportMailJobInterfaceTestImpl::adaptNewResourceUrl(bool overwriteResources, const KSharedConfig::Ptr &resourceConfig, const QString &storePath)
{
    ResourceConverterTest resourceConverterTest;
    resourceConverterTest.setTestPath(mPathConfig);
    return resourceConverterTest.adaptNewResourceUrl(overwriteResources, resourceConfig, storePath);
}

void ImportMailJobInterfaceTestImpl::addNewIdentity(const QString &name, KConfigGroup &group, int defaultIdentities, int oldUid)
{
    //TODO order ????
    //qDebug() << " void ImportMailJobInterfaceTestImpl::addNewIdentity(const QString &name, KConfigGroup &group, int defaultIdentities, int oldUid) not implemented yet" << oldUid << " name " << name;
    const QString uniqueName = uniqueIdentityName(name);
    KIdentityManagement::Identity identity;
    uint value = mIdentityUoid;
    mIdentityUoid++;
    identity.setUoid(value);
    group.writeEntry(QStringLiteral("Name"), uniqueName);
    group.sync();

    //TODO ????
    KConfig config(QStringLiteral("/tmp/identities//identities/emailidentities"));

    identity.readConfig(group);

    if (oldUid != -1) {
        mHashIdentity.insert(oldUid, identity.uoid());
        if (oldUid == defaultIdentities) {
            KConfigGroup grpGeneral = config.group(QStringLiteral("General"));
            grpGeneral.writeEntry("Default Identity", identity.uoid());
        }
    }
    KConfigGroup grp = config.group(QStringLiteral("Identity #%1").arg(value-1));
    identity.writeConfig(grp);
}

QString ImportMailJobInterfaceTestImpl::uniqueIdentityName(const QString &name)
{
    QString newName(name);
    int i = 0;

    const QStringList existingIdentityNames{QStringLiteral("identity1"), QStringLiteral("identity2")};

    while (existingIdentityNames.contains(newName)) {
        newName = QStringLiteral("%1_%2").arg(name).arg(i);
        ++i;
    }
    return newName;
}

void ImportMailJobInterfaceTestImpl::importCustomMailTransport(const QString &identifierValue, const KConfigGroup &group, int defaultTransport, int transportId)
{
    qDebug() << "importCustomMailTransport(const QString &identifierValue, const KConfigGroup &group, int defaultTransport, int transportId) not implemented yet";
}

void ImportMailJobInterfaceTestImpl::importSmtpMailTransport(const SmtpMailTransport &smtpMailTransport, int defaultTransport, int transportId)
{
    qDebug() << "ImportMailJobInterfaceTestImpl::importSmtpMailTransport(const KConfigGroup &group, int defaultTransport, int transportId) not implement yet";
    qDebug() << " defaultTransport " << defaultTransport << " transportId " << transportId;
    KConfig config(QStringLiteral("/tmp/mailtransport/config/mailtransports"));
    //TODO use transportId ?
    const int transportValue = mMailTransportId;
    mMailTransportId++;
    KConfigGroup grp = config.group(QStringLiteral("Transport %1").arg(transportValue));

    const auto name = smtpMailTransport.name();
    grp.writeEntry(QStringLiteral("name"), name);
    const auto host = smtpMailTransport.host();
    grp.writeEntry(QStringLiteral("host"), host);
    const auto port = smtpMailTransport.port();
    grp.writeEntry(QStringLiteral("port"), port);
    const auto userName = smtpMailTransport.userName();
    grp.writeEntry(QStringLiteral("userName"), userName);
    const auto precommand = smtpMailTransport.precommand();
    grp.writeEntry(QStringLiteral("precommand"), precommand);
    const auto requiresAuthentication = smtpMailTransport.requiresAuthentication();
    grp.writeEntry(QStringLiteral("requiresAuthentication"), requiresAuthentication);
    const auto specifyHostname = smtpMailTransport.specifyHostname();
    grp.writeEntry(QStringLiteral("specifyHostname"), specifyHostname);
    const auto localHostname = smtpMailTransport.localHostname();
    grp.writeEntry(QStringLiteral("localHostname"), localHostname);
    const auto specifySenderOverwriteAddress = smtpMailTransport.specifySenderOverwriteAddress();
    grp.writeEntry(QStringLiteral("specifySenderOverwriteAddress"), specifySenderOverwriteAddress);
    const auto storePassword = smtpMailTransport.storePassword();
    grp.writeEntry(QStringLiteral("storePassword"), storePassword);
    const auto senderOverwriteAddress = smtpMailTransport.senderOverwriteAddress();
    grp.writeEntry(QStringLiteral("senderOverwriteAddress"), senderOverwriteAddress);
    const auto encryption = smtpMailTransport.encryption();
    grp.writeEntry(QStringLiteral("encryption"), encryption);
    const auto authenticationType = smtpMailTransport.authenticationType();
    grp.writeEntry(QStringLiteral("authenticationType"), authenticationType);

    if (transportId == defaultTransport) {
        KConfigGroup generalGrp = config.group(QStringLiteral("General"));
        generalGrp.writeEntry(QStringLiteral("default-transport"), transportValue);
    }
}
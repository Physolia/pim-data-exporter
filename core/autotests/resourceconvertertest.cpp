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

#include "resourceconvertertest.h"
#include <QDebug>
ResourceConverterTest::ResourceConverterTest()
{
}

ResourceConverterTest::~ResourceConverterTest()
{
}

QString ResourceConverterTest::convertToFullCollectionPath(const qlonglong collectionValue)
{
    return QStringLiteral("foo/bla bla/bli%1").arg(collectionValue);
}

Akonadi::Collection::Id ResourceConverterTest::convertFolderPathToCollectionId(const QString &path)
{
    QString str = path;
    str.remove(QStringLiteral("foo/bla bla/bli"));
    auto result = str.toLongLong();
    //Make sure that we use another number.
    if (result != -1) {
        result += 10;
    }
    return result;
}

QString ResourceConverterTest::changeResourcePath(QString url) const
{
    qDebug() << " need to fix " << url;
    if (!url.isEmpty()) {
        url.replace(QLatin1String("[TEST_PATH]"), mTestPath);
    }
    qDebug() <<" FIXED " << url;
    //TODO FIXME
    return url;
}

void ResourceConverterTest::setTestPath(const QString &testPath)
{
    mTestPath = testPath;
}

// SPDX-License-Identifier: GPL-3.0-only
/*
 *  PolyMC - Minecraft Launcher
 *  Copyright (c) 2022 flowln <flowlnlnln@gmail.com>
 *  Copyright (C) 2022 Sefa Eyeoglu <contact@scrumplex.net>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 3.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * This file incorporates work covered by the following copyright and
 * permission notice:
 *
 *      Copyright 2013-2021 MultiMC Contributors
 *
 *      Licensed under the Apache License, Version 2.0 (the "License");
 *      you may not use this file except in compliance with the License.
 *      You may obtain a copy of the License at
 *
 *          http://www.apache.org/licenses/LICENSE-2.0
 *
 *      Unless required by applicable law or agreed to in writing, software
 *      distributed under the License is distributed on an "AS IS" BASIS,
 *      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *      See the License for the specific language governing permissions and
 *      limitations under the License.
 */

#include "ResourcePackFolderModel.h"

#include "Version.h"

#include "minecraft/mod/tasks/BasicFolderLoadTask.h"
#include "minecraft/mod/tasks/LocalResourcePackParseTask.h"

ResourcePackFolderModel::ResourcePackFolderModel(const QString& dir) : ResourceFolderModel(QDir(dir))
{
    m_column_sort_keys = { SortType::ENABLED, SortType::NAME, SortType::PACK_FORMAT, SortType::DATE };
}

QVariant ResourcePackFolderModel::data(const QModelIndex& index, int role) const
{
    if (!validateIndex(index))
        return {};

    int row = index.row();
    int column = index.column();

    switch (role) {
        case Qt::DisplayRole:
            switch (column) {
                case NameColumn:
                    return m_resources[row]->name();
                case PackFormatColumn: {
                    auto resource = at(row);
                    auto pack_format = resource->packFormat();
                    if (pack_format == 0)
                        return tr("Unrecognized");

                    auto version_bounds = resource->compatibleVersions();
                    if (version_bounds.first.toString().isEmpty())
                        return QString::number(pack_format);

                    return QString("%1 (%2 - %3)")
                        .arg(QString::number(pack_format), version_bounds.first.toString(), version_bounds.second.toString());
                }
                case DateColumn:
                    return m_resources[row]->dateTimeChanged();

                default:
                    return {};
            }

        case Qt::ToolTipRole: {
            if (column == PackFormatColumn) {
                //: The string being explained by this is in the format: ID (Lower version - Upper version)
                return tr("The resource pack format ID, as well as the Minecraft versions it was designed for.");
            }
            return m_resources[row]->internal_id();
        }
        case Qt::CheckStateRole:
            switch (column) {
                case ActiveColumn:
                    return at(row)->enabled() ? Qt::Checked : Qt::Unchecked;
                default:
                    return {};
            }
        default:
            return {};
    }
}

QVariant ResourcePackFolderModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch (role) {
        case Qt::DisplayRole:
            switch (section) {
                case ActiveColumn:
                    return QString();
                case NameColumn:
                    return tr("Name");
                case PackFormatColumn:
                    return tr("Pack Format");
                case DateColumn:
                    return tr("Last changed");
                default:
                    return {};
            }

        case Qt::ToolTipRole:
            switch (section) {
                case ActiveColumn:
                    return tr("Is the resource pack enabled? (Only valid for ZIPs)");
                case NameColumn:
                    return tr("The name of the resource pack.");
                case PackFormatColumn:
                    //: The string being explained by this is in the format: ID (Lower version - Upper version)
                    return tr("The resource pack format ID, as well as the Minecraft versions it was designed for.");
                case DateColumn:
                    return tr("The date and time this resource pack was last changed (or added).");
                default:
                    return {};
            }
        default:
            return {};
    }
    return {};
}

int ResourcePackFolderModel::columnCount(const QModelIndex& parent) const
{
    return NUM_COLUMNS;
}

Task* ResourcePackFolderModel::createUpdateTask()
{
    return new BasicFolderLoadTask(m_dir, [](QFileInfo const& entry) { return new ResourcePack(entry); });
}

Task* ResourcePackFolderModel::createParseTask(Resource& resource)
{
    return new LocalResourcePackParseTask(m_next_resolution_ticket, static_cast<ResourcePack&>(resource));
}

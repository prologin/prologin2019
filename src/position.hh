// SPDX-License-Identifier: GPL-2.0-or-later
/*
** This file is part of Prologin2019, a rules library for stechec2.
**
** Copyright (c) 2019 Association Prologin <info@prologin.org>
** Copyright (c) 2019 Paul Guénézan
**
** Prologin2019 is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with Prologin2019.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef POSITION_HH
#define POSITION_HH

#include "constant.hh"

inline bool operator==(const position& a, const position& b)
{
    return a.ligne == b.ligne && a.colonne == b.colonne;
}

inline bool inside_map(const position& pos)
{
    return pos.ligne >= 0 && pos.ligne < TAILLE_MINE 
        && pos.colonne >= 0 && pos.colonne < TAILLE_MINE;
}

#endif

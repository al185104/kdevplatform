/*
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "ipatchsource.h"
#include <qicon.h>

using namespace KDevelop;


QIcon IPatchSource::icon() const
{
    return QIcon();
}

IPatchReview::~IPatchReview()
{

}

void IPatchSource::cancelReview()
{

}

void IPatchSource::finishReview()
{

}

bool IPatchSource::canCancel() const
{
    return false;
}

#include "ipatchsource.moc"
/* This file is part of the KDE libraries
   Copyright (C) 2000 Ronny Standtke <Ronny.Standtke@gmx.de>

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


#ifndef CPPDEVTK_GUI_K_SQUEEZED_TEXT_LABEL_P_H_INCLUDED_
#define CPPDEVTK_GUI_K_SQUEEZED_TEXT_LABEL_P_H_INCLUDED_


#include <cppdevtk/gui/config.hpp>

#include <QtCore/QtGlobal>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/Qt>
#include <QtGui/QClipboard>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/QApplication>
#else
#include <QtGui/QApplication>
#endif


namespace cppdevtk {
namespace gui {


class KSqueezedTextLabelPrivate: public QObject
{
	Q_OBJECT
public Q_SLOTS:
    void _k_copyFullText() {
        QApplication::clipboard()->setText(fullText);
    }

public:
    QString fullText;
    Qt::TextElideMode elideMode;
};


}	// namespace gui
}	// namespace cppdevtk


#endif	// CPPDEVTK_GUI_K_SQUEEZED_TEXT_LABEL_P_H_INCLUDED_

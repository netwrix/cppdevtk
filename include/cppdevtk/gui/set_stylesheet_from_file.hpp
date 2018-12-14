/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
///
/// \copyright Copyright (C) 2015 - 2019 CoSoSys Ltd <info@cososys.com>\n
/// Licensed under the Apache License, Version 2.0 (the "License");\n
/// you may not use this file except in compliance with the License.\n
/// You may obtain a copy of the License at\n
/// http://www.apache.org/licenses/LICENSE-2.0\n
/// Unless required by applicable law or agreed to in writing, software\n
/// distributed under the License is distributed on an "AS IS" BASIS,\n
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n
/// See the License for the specific language governing permissions and\n
/// limitations under the License.\n
/// Please see the file COPYING.
///
/// \authors Cristian ANITA <cristian.anita@cososys.com>, <cristian_anita@yahoo.com>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef CPPDEVTK_GUI_SET_STYLESHEET_FROM_FILE_HPP_INCLUDED_
#define CPPDEVTK_GUI_SET_STYLESHEET_FROM_FILE_HPP_INCLUDED_


#include "config.hpp"
#include <cppdevtk/util/filesystem_utils.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/static_assert.hpp>
#include <cppdevtk/base/dbc.hpp>

#include <QtCore/QtGlobal>
#include <QtCore/QString>
#include <QtCore/QFileInfo>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#else
#include <QtGui/QApplication>
#include <QtGui/QWidget>
#endif

#include CPPDEVTK_TR1_HEADER(type_traits)


namespace cppdevtk {
namespace gui {


/// Set stylesheet loaded from given file.
/// \tparam TTarget must be (derived from) QApplication or QWidget
template <class TTarget>
bool SetStyleSheetFromFile(TTarget& target, const QString& path, const QString& completeBaseName,
		bool append = true, const QString& suffix = "qss");

/// Set neutral and platform specific stylesheets from files.
/// \tparam TTarget must be (derived from) QApplication or QWidget
/// \return \c true if at least one stylesheet file is found and readed.
template <class TTarget>
bool SetStyleSheetFromFileCross(TTarget& target, const QString& path, const QString& completeBaseName,
		bool append = true, const QString& suffix = "qss");




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Template definitions.

template <class TTarget>
bool SetStyleSheetFromFile(TTarget& target, const QString& path, const QString& completeBaseName,
		bool append, const QString& suffix) {
	CPPDEVTK_STATIC_ASSERT_W_MSG((CPPDEVTK_TR1_NS::is_base_of<QApplication, TTarget>::value ||
			CPPDEVTK_TR1_NS::is_base_of<QWidget, TTarget>::value), "invalid type");
	CPPDEVTK_DBC_CHECK_ARGUMENT(!path.contains('\\'), "path contains non-Qt separator");
	
	const QString kOldStyleSheet = target.styleSheet();
	CPPDEVTK_LOG_TRACE(QString("%1 oldStyleSheet:\n").arg(target.objectName()) << kOldStyleSheet);
	if (!append) {
		target.setStyleSheet("");
	}
	
	QString filePath = path;
	if (!filePath.endsWith('/')) {
		filePath += '/';
	}
	filePath += completeBaseName + '.' + suffix;
	
	QFileInfo fileInfo(filePath);
	if (!fileInfo.exists()) {
		CPPDEVTK_LOG_TRACE("Style sheet file not found: " << filePath);
		return false;
	}
	if (!fileInfo.isFile()) {
		CPPDEVTK_LOG_ERROR("Style sheet file is not a file: " << filePath);
		return false;
	}
	
	QString readedStyleSheet;
	if (!cppdevtk::util::ReadTextFile(filePath, readedStyleSheet)) {
		CPPDEVTK_LOG_ERROR("Failed to load style sheet file: " << filePath);
		return false;
	}
	CPPDEVTK_LOG_TRACE(QString("%1 readedStyleSheet:\n").arg(target.objectName()) << readedStyleSheet);
	
	QString newStyleSheet;
	if (append) {
		newStyleSheet = kOldStyleSheet + '\n' + readedStyleSheet;
	}
	else {
		newStyleSheet = readedStyleSheet;
	}
	newStyleSheet += '\n';
	CPPDEVTK_LOG_TRACE(QString("%1 newStyleSheet:\n").arg(target.objectName()) << newStyleSheet);
	
	if (newStyleSheet.isEmpty()) {
		return true;
	}
	
	target.setStyleSheet(newStyleSheet);
	
	return true;
}

template <class TTarget>
bool SetStyleSheetFromFileCross(TTarget& target, const QString& path, const QString& completeBaseName,
		bool append, const QString& suffix) {
	CPPDEVTK_STATIC_ASSERT_W_MSG((CPPDEVTK_TR1_NS::is_base_of<QApplication, TTarget>::value ||
			CPPDEVTK_TR1_NS::is_base_of<QWidget, TTarget>::value), "invalid type");
	CPPDEVTK_DBC_CHECK_ARGUMENT(!path.contains('\\'), "path contains non-Qt separator");
	
	bool retValue = SetStyleSheetFromFile(target, path, completeBaseName, append, suffix);
	
	QString platformCompleteBaseName = completeBaseName +
#			if (CPPDEVTK_PLATFORM_UNIX)
			"_unx"
#			elif (CPPDEVTK_PLATFORM_WINDOWS)
			"_win"
#			else
#			error "Unsupported platform!!!"
#			endif
	;
	retValue = SetStyleSheetFromFile(target, path, platformCompleteBaseName, true, suffix) || retValue;
	
#	if (CPPDEVTK_PLATFORM_UNIX)
	platformCompleteBaseName = completeBaseName +
#			if (CPPDEVTK_PLATFORM_LINUX)
			"_lnx"
#			elif (CPPDEVTK_PLATFORM_MACOSX)
			"_mac"
#			else
#			error "Unsupported Unix platform!!!"
#			endif
	;
	retValue = SetStyleSheetFromFile(target, path, platformCompleteBaseName, true, suffix) || retValue;
#	endif	// (CPPDEVTK_PLATFORM_UNIX)
	
	return retValue;
}


}	// namespace gui
}	// namespace cppdevtk


#endif	// CPPDEVTK_GUI_SET_STYLESHEET_FROM_FILE_HPP_INCLUDED_

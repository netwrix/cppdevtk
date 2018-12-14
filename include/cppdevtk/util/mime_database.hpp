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


#ifndef CPPDEVTK_UTIL_MIME_DATABASE_HPP_INCLUDED_
#define CPPDEVTK_UTIL_MIME_DATABASE_HPP_INCLUDED_


#include "config.hpp"

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#	error "This file requires Qt 5"
#endif

#include <QtCore/QMimeDatabase>


namespace cppdevtk {
namespace util {


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Improved mime database.
/// \attention QMimeDatabase dtor is not virtual...
class CPPDEVTK_UTIL_API MimeDatabase: public QMimeDatabase {
public:
	enum MatchOrder {
		moExtensionOverContent = MatchDefault,	// extension has priority over content
		moExtension = MatchExtension,
		moContent = MatchContent,
		moContentOverExtension,	// content has priority over extension
		moContentIfInheritsExtension,	// content is preferred if inherits extension
		moExtensionIfInheritsContent,	// extension is preferred if inherits content
		moDefault = moExtensionIfInheritsContent
	};
    
    
	QMimeType MimeTypeForFile(const QString& fileName, MatchOrder matchOrder = moDefault) const;
	QMimeType MimeTypeForFile(const QFileInfo& fileInfo, MatchOrder matchOrder = moDefault) const;
	
    QMimeType MimeTypeForFileNameAndData(const QString& fileName, QIODevice* device,
			MatchOrder matchOrder = moDefault) const;
    QMimeType MimeTypeForFileNameAndData(const QString& fileName, const QByteArray& data,
			MatchOrder matchOrder = moDefault) const;
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions


}	// namespace util
}	// namespace cppdevtk


#endif	// CPPDEVTK_UTIL_MIME_DATABASE_HPP_INCLUDED_

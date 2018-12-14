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


#include <cppdevtk/util/mime_database.hpp>
#include <cppdevtk/util/filesystem_utils.hpp>
#include <cppdevtk/base/stdexcept.hpp>
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/cassert.hpp>

#include <QtCore/QFileInfo>
#include <QtCore/QString>
#include <QtCore/QMimeType>


namespace cppdevtk {
namespace util {


QMimeType MimeDatabase::MimeTypeForFile(const QString& fileName, MatchOrder matchOrder) const {
	CPPDEVTK_DBC_CHECK_ARGUMENT(IsValidPath(fileName), "fileName must be valid");
	
	QMimeType mimeType = mimeTypeForName("application/octet-stream");
	
	switch (matchOrder) {
		case moExtensionOverContent:
		case moExtension:
		case moContent:
			mimeType = mimeTypeForFile(fileName, static_cast<MatchMode>(matchOrder));
			break;
		case moContentOverExtension:
			mimeType = mimeTypeForFile(fileName, MatchContent);
			if (mimeType.name().compare("application/octet-stream", Qt::CaseInsensitive) == 0) {
				mimeType = mimeTypeForFile(fileName, MatchExtension);
			}
			break;
		case moContentIfInheritsExtension:
			mimeType = mimeTypeForFile(fileName, MatchExtension);
			if (mimeType.name().compare("application/octet-stream", Qt::CaseInsensitive) != 0) {
				const QMimeType kContentMimeType = mimeTypeForFile(fileName, MatchContent);
				if (kContentMimeType.name().compare("application/octet-stream", Qt::CaseInsensitive) != 0) {
					if (kContentMimeType.inherits(mimeType.name())) {
						mimeType = kContentMimeType;
					}
				}
			}
			break;
		case moExtensionIfInheritsContent:
			mimeType = mimeTypeForFile(fileName, MatchContent);
			if (mimeType.name().compare("application/octet-stream", Qt::CaseInsensitive) != 0) {
				const QMimeType kExtMimeType = mimeTypeForFile(fileName, MatchExtension);
				if (kExtMimeType.name().compare("application/octet-stream", Qt::CaseInsensitive) != 0) {
					if (kExtMimeType.inherits(mimeType.name())) {
						mimeType = kExtMimeType;
					}
				}
			}
			break;
		default:
			CPPDEVTK_ASSERT(0 && "UB: matchOrder is not a valid enum value");
			throw CPPDEVTK_INVALID_ARGUMENT_EXCEPTION("UB: matchOrder is not a valid enum value");
	}
	
	return mimeType;
}

QMimeType MimeDatabase::MimeTypeForFile(const QFileInfo& fileInfo, MatchOrder matchOrder) const {
	CPPDEVTK_DBC_CHECK_ARGUMENT(IsValidPath(fileInfo.filePath()), "fileInfo.filePath() must be valid");
	
	QMimeType mimeType = mimeTypeForName("application/octet-stream");
	
	switch (matchOrder) {
		case moExtensionOverContent:
		case moExtension:
		case moContent:
			mimeType = mimeTypeForFile(fileInfo, static_cast<MatchMode>(matchOrder));
			break;
		case moContentOverExtension:
			mimeType = mimeTypeForFile(fileInfo, MatchContent);
			if (mimeType.name().compare("application/octet-stream", Qt::CaseInsensitive) == 0) {
				mimeType = mimeTypeForFile(fileInfo, MatchExtension);
			}
			break;
		case moContentIfInheritsExtension:
			mimeType = mimeTypeForFile(fileInfo, MatchExtension);
			if (mimeType.name().compare("application/octet-stream", Qt::CaseInsensitive) != 0) {
				const QMimeType kContentMimeType = mimeTypeForFile(fileInfo, MatchContent);
				if (kContentMimeType.name().compare("application/octet-stream", Qt::CaseInsensitive) != 0) {
					if (kContentMimeType.inherits(mimeType.name())) {
						mimeType = kContentMimeType;
					}
				}
			}
			break;
		case moExtensionIfInheritsContent:
			mimeType = mimeTypeForFile(fileInfo, MatchContent);
			if (mimeType.name().compare("application/octet-stream", Qt::CaseInsensitive) != 0) {
				const QMimeType kExtMimeType = mimeTypeForFile(fileInfo, MatchExtension);
				if (kExtMimeType.name().compare("application/octet-stream", Qt::CaseInsensitive) != 0) {
					if (kExtMimeType.inherits(mimeType.name())) {
						mimeType = kExtMimeType;
					}
				}
			}
			break;
		default:
			CPPDEVTK_ASSERT(0 && "UB: matchOrder is not a valid enum value");
			throw CPPDEVTK_INVALID_ARGUMENT_EXCEPTION("UB: matchOrder is not a valid enum value");
	}
	
	return mimeType;
}

QMimeType MimeDatabase::MimeTypeForFileNameAndData(const QString& fileName, QIODevice* device,
		MatchOrder matchOrder) const {
	CPPDEVTK_DBC_CHECK_ARGUMENT((fileName.isEmpty() || IsValidPath(fileName)), "fileName must be empty or valid");
	CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(device);
	
	QMimeType mimeType = mimeTypeForName("application/octet-stream");
	
	switch (matchOrder) {
		case moExtensionOverContent:
			mimeType = mimeTypeForFileNameAndData(fileName, device);
			break;
		case moExtension:
			mimeType = mimeTypeForFile(fileName, MatchExtension);
			//throw CPPDEVTK_INVALID_ARGUMENT_EXCEPTION("matchOrder can not be moExtension");
			break;
		case moContent:
			mimeType = mimeTypeForData(device);
			//throw CPPDEVTK_INVALID_ARGUMENT_EXCEPTION("matchOrder can not be moContent");
			break;
		case moContentOverExtension:
			mimeType = mimeTypeForData(device);
			if (mimeType.name().compare("application/octet-stream", Qt::CaseInsensitive) == 0) {
				mimeType = mimeTypeForFile(fileName, MatchExtension);
			}
			break;
		case moContentIfInheritsExtension:
			mimeType = mimeTypeForFile(fileName, MatchExtension);
			if (mimeType.name().compare("application/octet-stream", Qt::CaseInsensitive) != 0) {
				const QMimeType kDataMimeType = mimeTypeForData(device);
				if (kDataMimeType.name().compare("application/octet-stream", Qt::CaseInsensitive) != 0) {
					if (kDataMimeType.inherits(mimeType.name())) {
						mimeType = kDataMimeType;
					}
				}
			}
			break;
		case moExtensionIfInheritsContent:
			mimeType = mimeTypeForData(device);
			if (mimeType.name().compare("application/octet-stream", Qt::CaseInsensitive) != 0) {
				const QMimeType kExtMimeType = mimeTypeForFile(fileName, MatchExtension);
				if (kExtMimeType.name().compare("application/octet-stream", Qt::CaseInsensitive) != 0) {
					if (kExtMimeType.inherits(mimeType.name())) {
						mimeType = kExtMimeType;
					}
				}
			}
			break;
		default:
			CPPDEVTK_ASSERT(0 && "UB: matchOrder is not a valid enum value");
			throw CPPDEVTK_INVALID_ARGUMENT_EXCEPTION("UB: matchOrder is not a valid enum value");
	}
	
	return mimeType;
}

QMimeType MimeDatabase::MimeTypeForFileNameAndData(const QString& fileName, const QByteArray& data,
			MatchOrder matchOrder) const {
	CPPDEVTK_DBC_CHECK_ARGUMENT((fileName.isEmpty() || IsValidPath(fileName)), "fileName must be empty or valid");
	
	QMimeType mimeType = mimeTypeForName("application/octet-stream");
	
	switch (matchOrder) {
		case moExtensionOverContent:
			mimeType = mimeTypeForFileNameAndData(fileName, data);
			break;
		case moExtension:
			mimeType = mimeTypeForFile(fileName, MatchExtension);
			//throw CPPDEVTK_INVALID_ARGUMENT_EXCEPTION("matchOrder can not be moExtension");
			break;
		case moContent:
			mimeType = mimeTypeForData(data);
			//throw CPPDEVTK_INVALID_ARGUMENT_EXCEPTION("matchOrder can not be moContent");
			break;
		case moContentOverExtension:
			mimeType = mimeTypeForData(data);
			if (mimeType.name().compare("application/octet-stream", Qt::CaseInsensitive) == 0) {
				mimeType = mimeTypeForFile(fileName, MatchExtension);
			}
			break;
		case moContentIfInheritsExtension:
			mimeType = mimeTypeForFile(fileName, MatchExtension);
			if (mimeType.name().compare("application/octet-stream", Qt::CaseInsensitive) != 0) {
				const QMimeType kDataMimeType = mimeTypeForData(data);
				if (kDataMimeType.name().compare("application/octet-stream", Qt::CaseInsensitive) != 0) {
					if (kDataMimeType.inherits(mimeType.name())) {
						mimeType = kDataMimeType;
					}
				}
			}
			break;
		case moExtensionIfInheritsContent:
			mimeType = mimeTypeForData(data);
			if (mimeType.name().compare("application/octet-stream", Qt::CaseInsensitive) != 0) {
				const QMimeType kExtMimeType = mimeTypeForFile(fileName, MatchExtension);
				if (kExtMimeType.name().compare("application/octet-stream", Qt::CaseInsensitive) != 0) {
					if (kExtMimeType.inherits(mimeType.name())) {
						mimeType = kExtMimeType;
					}
				}
			}
			break;
		default:
			CPPDEVTK_ASSERT(0 && "UB: matchOrder is not a valid enum value");
			throw CPPDEVTK_INVALID_ARGUMENT_EXCEPTION("UB: matchOrder is not a valid enum value");
	}
	
	return mimeType;
}


}	// namespace util
}	// namespace cppdevtk

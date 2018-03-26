#****************************************************************************************************************************
# Copyright (C) 2015 - 2018 CoSoSys Ltd <info@cososys.com>
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# http://www.apache.org/licenses/LICENSE-2.0
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# Please see the file COPYING.
#
# Author(s): Cristian ANITA <cristian.anita@cososys.com>, <cristian_anita@yahoo.com>
#****************************************************************************************************************************


TEMPLATE = subdirs


include(./../../../common.pri)


SUBDIRS += json


HEADERS += \
	config.hpp \
	core_application.hpp \
	core_application_base.hpp \
	damerau_levenshtein_distance.hpp \
	dynamic_library.hpp \
	dynamic_loader.hpp \
	dynamic_loader_exception.hpp \
	exception_to_errno.hpp	\
	filesystem_exception.hpp \
	filesystem_utils.hpp \
	get_user_name.hpp \
	info.hpp \
	info_customization.hpp	\
	init_resources.hpp \
	language_info.hpp \
	no_such_file_or_directory_exception.hpp	\
	password_requirements.hpp \
	q_zip_reader.hpp \
	q_zip_writer.hpp \
	qt_local_peer.hpp \
	qt_locked_file.hpp \
	qt_service.hpp \
	qt_single_core_application.hpp \
	single_core_application.hpp \
	socket_pair.hpp \
	tinyxml2.h

unix {
	HEADERS += filesystem_utils_unx.hpp	\
		posix_signals_watcher_unx.hpp
	
	linux* {
		HEADERS += filesystem_utils_lnx.hpp
		!android {
			HEADERS += libudev_lnx.hpp
		}
	}
	else {
		macx|ios {
			HEADERS += filesystem_utils_mac.hpp
		}
		else {
			error("Unsupported Unix platform!!!")
		}
	}
}
else {
	win32 {
		HEADERS += filesystem_utils_win.hpp
	}
	else {
		error("Unsupported platform!!!")
	}
}


headers.files = $${HEADERS}
headers.path = $${CPPDEVTK_INCLUDE_DIR}/util
INSTALLS += headers

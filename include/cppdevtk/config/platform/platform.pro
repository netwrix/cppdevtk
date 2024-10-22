#****************************************************************************************************************************
# Copyright (C) 2015 - 2020 CoSoSys Ltd <info@cososys.com>
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


include(./../../../../common.pri)


SUBDIRS +=


unix {
	HEADERS += unix.hpp
	
	linux* {
		HEADERS += linux.hpp
	}
	else {
		macx|ios {
			HEADERS += macosx.hpp
		}
		else {
			error("Unsupported Unix platform!!!")
		}
	}
}
else {
	win32 {
		HEADERS += windows.hpp
	}
	else {
		error("Unsupported platform!!!")
	}
}


headers.files = $${HEADERS}
headers.path = $${CPPDEVTK_INCLUDE_DIR}/config/platform
INSTALLS += headers

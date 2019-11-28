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


!isEqual(CPPDEVTK_ENABLE_QTSOLUTIONS, "true") {
    error("This subproject require QtSolutions enabled!!!")
}


HEADERS +=	\
	config.hpp	\
	init_resources.hpp	\
	QtCopyDialog	\
	qtcopydialog.h	\
	QtFileCopier	\
	qtfilecopier.h	\



headers.files = $${HEADERS}
headers.path = $${CPPDEVTK_INCLUDE_DIR}/QtSolutions/QtCopyDialog
INSTALLS += headers

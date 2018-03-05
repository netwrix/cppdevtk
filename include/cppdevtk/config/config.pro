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


SUBDIRS +=	\
	compiler	\
	platform	\
	platform_compiler


HEADERS +=	\
	architecture.hpp	\
	company_info.hpp	\
	company_info_customization.hpp	\
	compiler.hpp	\
	config.hpp	\
	features.hpp	\
	info.hpp	\
	platform.hpp	\
	platform_compiler.hpp	\
	product_info.hpp	\
	product_info_customization.hpp	\
	third_party.hpp


headers.files = $${HEADERS}
headers.path = $${CPPDEVTK_INCLUDE_DIR}/config
INSTALLS += headers

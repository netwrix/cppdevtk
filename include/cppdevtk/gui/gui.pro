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


SUBDIRS +=


HEADERS += \
	application.hpp \
	application_base.hpp \
	caps_lock_widget.hpp \
	config.hpp \
	custom_wizard.hpp \
	disk_space_widget.hpp \
	eula_widget.hpp \
	info.hpp \
	info_customization.hpp	\
	info_tr.hpp	\
	info_tr_customization.hpp	\
	init_resources.hpp \
	invisible_widget.hpp \
	is_caps_lock_on.hpp \
	k_squeezed_text_label.hpp \
	language_menu.hpp	\
	language_widget.hpp \
	login_widget.hpp \
	message_box.hpp \
	password_widget.hpp \
	progress_dialog.hpp \
	qt_copy_dialog.hpp \
	qt_file_copier.hpp \
	qt_single_application.hpp \
	set_stylesheet_from_file.hpp \
	single_application.hpp \
	timed_computer_management_widget.hpp \
	widget_base.hpp

!android:!ios {
	HEADERS += \
		screensaver.hpp	\
		storage_device_notifier.hpp	\
		power_notifier.hpp	\
		session_manager.hpp	\
		session.hpp
}


headers.files = $${HEADERS}
headers.path = $${CPPDEVTK_INCLUDE_DIR}/gui
INSTALLS += headers

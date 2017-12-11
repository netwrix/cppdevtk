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


QT *= gui core
greaterThan(QT_MAJOR_VERSION, 4) {
	QT *= widgets
	linux*:!android {
		QT *= x11extras
	}
}
linux*:!android {
	QT *= dbus
}
android|ios {
	QT *= network
}


TEMPLATE = lib


include(./gui_customization.pri)
include(./../../common.pri)


# Build marker.
DEFINES *= CPPDEVTK_DETAIL_GUI_BUILD


# CONFIG
!static_and_shared|build_pass {
	CONFIG(static, static|shared) {
		CONFIG *= staticlib
	}
	else {
		CONFIG *= dll dylib
	}
}
CONFIG *= create_prl


# Version.
unix {
	CONFIG(shared, static|shared) {
		VER_MAJ = $${CPPDEVTK_VERSION_MAJOR}
		VER_MIN = $${CPPDEVTK_VERSION_MINOR}
		VER_PAT = $${CPPDEVTK_VERSION_PATCH}
	}
}


# TARGET + INSTALLS

cppdevtk_enable_target_suffix_qt_major_version {
	CPPDEVTK_UTIL_TARGET = $${qtLibraryTarget($${CPPDEVTK_UTIL_TARGET}_qt$${QT_MAJOR_VERSION})}
	CPPDEVTK_BASE_TARGET = $${qtLibraryTarget($${CPPDEVTK_BASE_TARGET}_qt$${QT_MAJOR_VERSION})}
}
else {
	CPPDEVTK_UTIL_TARGET = $${qtLibraryTarget($${CPPDEVTK_UTIL_TARGET})}
	CPPDEVTK_BASE_TARGET = $${qtLibraryTarget($${CPPDEVTK_BASE_TARGET})}
}

target.path = $${CPPDEVTK_LIB_DIR}
INSTALLS += target


# LIBS + PRE_TARGETDEPS
!debug_and_release|build_pass {
	LIBS += -l$${CPPDEVTK_UTIL_TARGET} -l$${CPPDEVTK_BASE_TARGET}
	!static_and_shared|build_pass {
		unix {
			cppdevtk_enable_android_destdir_workaround {
				PRE_TARGETDEPS += $${OUT_PWD}/../util/lib$${CPPDEVTK_UTIL_TARGET}.$${CPPDEVTK_LIB_EXT}
				PRE_TARGETDEPS += $${OUT_PWD}/../base/lib$${CPPDEVTK_BASE_TARGET}.$${CPPDEVTK_LIB_EXT}
			}
			else {
				PRE_TARGETDEPS += $${DESTDIR}/lib$${CPPDEVTK_UTIL_TARGET}.$${CPPDEVTK_LIB_EXT}
				PRE_TARGETDEPS += $${DESTDIR}/lib$${CPPDEVTK_BASE_TARGET}.$${CPPDEVTK_LIB_EXT}
			}
		}
		else {
			win32 {
				PRE_TARGETDEPS += $${DESTDIR}/$${CPPDEVTK_UTIL_TARGET}.$${CPPDEVTK_LIB_EXT}
				PRE_TARGETDEPS += $${DESTDIR}/$${CPPDEVTK_BASE_TARGET}.$${CPPDEVTK_LIB_EXT}
			}
			else {
				error("Unsupported platform!!!")
			}
		}
	}
	
	unix {
		linux* {
			!android {
				LIBS *= -lXext -lXss
			}
		}
		else {
			macx {
				LIBS += -framework ApplicationServices -framework Carbon -framework IOKit -framework CoreFoundation
			}
			else {
				ios {
					LIBS += -framework CoreFoundation
				}
				else {
					error("Unsupported Unix platform!!!")
				}
			}
		}
	}
	else {
		win32 {
			LIBS *= -ladvapi32 -luser32
		}
		else {
			error("Unsupported platform!!!")
		}
	}
}


# Strip
!debug_and_release|build_pass {
	CONFIG(release, debug|release) {
		unix {
			!static_and_shared|build_pass {
				CONFIG(static, static|shared) {
					isEqual(CPPDEVTK_ENABLE_STRIP_STATIC_LIB, "true") {
						QMAKE_POST_LINK += $${CPPDEVTK_STRIP_STATIC_LIB_CMD} $${DESTDIR}/lib$${TARGET}.$${CPPDEVTK_LIB_EXT}
					}
				}
				else {
					isEqual(CPPDEVTK_ENABLE_STRIP_SHARED_LIB, "true") {
						QMAKE_POST_LINK += $${CPPDEVTK_STRIP_SHARED_LIB_CMD} $${DESTDIR}/lib$${TARGET}.$${CPPDEVTK_LIB_EXT}
					}
				}
			}
		}
	}
}




# Files


RESOURCES += cppdevtk_gui.qrc	\
    cppdevtk_gui_customization.qrc

win32 {
	RC_FILE = cppdevtk_gui.rc
	HEADERS += cppdevtk_gui_res_win.hpp
}


FORMS += \
	caps_lock_widget.ui \
	custom_wizard.ui	\
	disk_space_widget.ui \
	eula_widget.ui \
	language_widget.ui \
	login_widget.ui	\
	password_widget.ui	\
	qt_copy_dialog.ui \
	qt_other_dialog.ui \
	qt_overwrite_dialog.ui \
	timed_computer_management_widget.ui


SOURCES += \
	application.cpp \
	application_base.cpp \
	caps_lock_widget.cpp \
	custom_wizard.cpp \
	disk_space_widget.cpp \
	eula_widget.cpp \
	init_resources.cpp	\
	invisible_widget.cpp \
	k_squeezed_text_label.cpp \
	language_widget.cpp \
	login_widget.cpp \
	message_box.cpp \
	password_widget.cpp \
	progress_dialog.cpp \
	qt_copy_dialog.cpp \
	qt_file_copier.cpp \
	qt_single_application.cpp \
	single_application.cpp \
	timed_computer_management_widget.cpp \
	widget_base.cpp

unix {
	linux* {
		SOURCES += is_caps_lock_on_lnx.cpp	\
			user_idle_time_lnx.cpp	\
			computer_manager_lnx.cpp
	}
	else {
		macx|ios {
			SOURCES += is_caps_lock_on_mac.cpp	\
				user_idle_time_mac.cpp	\
				computer_manager_mac.cpp
		}
		else {
			error("Unsupported Unix platform!!!")
		}
	}
}
else {
	win32 {
		SOURCES += is_caps_lock_on_win.cpp	\
			user_idle_time_win.cpp	\
			computer_manager_win.cpp
	}
	else {
		error("Unsupported platform!!!")
	}
}

HEADERS += \
	../../include/cppdevtk/gui/application.hpp \
	../../include/cppdevtk/gui/application_base.hpp \
	../../include/cppdevtk/gui/caps_lock_widget.hpp \
	../../include/cppdevtk/gui/computer_manager.hpp \
	../../include/cppdevtk/gui/config.hpp \
	../../include/cppdevtk/gui/custom_wizard.hpp \
	../../include/cppdevtk/gui/disk_space_widget.hpp \
	../../include/cppdevtk/gui/eula_widget.hpp \
	../../include/cppdevtk/gui/info.hpp \
	../../include/cppdevtk/gui/init_resources.hpp \
	../../include/cppdevtk/gui/invisible_widget.hpp \
	../../include/cppdevtk/gui/is_caps_lock_on.hpp \
	../../include/cppdevtk/gui/k_squeezed_text_label.hpp \
	../../include/cppdevtk/gui/language_widget.hpp \
	../../include/cppdevtk/gui/login_widget.hpp \
	../../include/cppdevtk/gui/message_box.hpp \
	../../include/cppdevtk/gui/password_widget.hpp \
	../../include/cppdevtk/gui/progress_dialog.hpp \
	../../include/cppdevtk/gui/qt_copy_dialog.hpp \
	../../include/cppdevtk/gui/qt_file_copier.hpp \
	../../include/cppdevtk/gui/qt_single_application.hpp \
	../../include/cppdevtk/gui/set_stylesheet_from_file.hpp \
	../../include/cppdevtk/gui/single_application.hpp \
	../../include/cppdevtk/gui/timed_computer_management_widget.hpp \
	../../include/cppdevtk/gui/user_idle_time.hpp \
	../../include/cppdevtk/gui/widget_base.hpp

#****************************************************************************************************************************
# Copyright (C) 2015 - 2019 CoSoSys Ltd <info@cososys.com>
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


greaterThan(QT_MAJOR_VERSION, 4) {
	QT *= widgets
	linux*:!android {
		QT *= x11extras
	}
	win32 {
		QT *= gui-private
	}
}
QT *= gui
contains(QT_CONFIG, dbus) {
	QT *= dbus
}
QT *= core


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
	CPPDEVTK_QTSOL_QTCOPYDIALOG_TARGET = $${CPPDEVTK_QTSOL_QTCOPYDIALOG_TARGET}_qt$${QT_MAJOR_VERSION}
	CPPDEVTK_QTSOL_QTSINGLEAPPLICATION_TARGET = $${CPPDEVTK_QTSOL_QTSINGLEAPPLICATION_TARGET}_qt$${QT_MAJOR_VERSION}
	CPPDEVTK_UTIL_TARGET = $${CPPDEVTK_UTIL_TARGET}_qt$${QT_MAJOR_VERSION}
	CPPDEVTK_QTSOL_QTSINGLECOREAPPLICATION_TARGET = $${CPPDEVTK_QTSOL_QTSINGLECOREAPPLICATION_TARGET}_qt$${QT_MAJOR_VERSION}
	CPPDEVTK_QTSOL_QTLOCKEDFILE_TARGET = $${CPPDEVTK_QTSOL_QTLOCKEDFILE_TARGET}_qt$${QT_MAJOR_VERSION}
	CPPDEVTK_QTSOL_QTSERVICE_TARGET = $${CPPDEVTK_QTSOL_QTSERVICE_TARGET}_qt$${QT_MAJOR_VERSION}
	CPPDEVTK_BASE_TARGET = $${CPPDEVTK_BASE_TARGET}_qt$${QT_MAJOR_VERSION}
}
CPPDEVTK_QTSOL_QTCOPYDIALOG_TARGET = $${qtLibraryTarget($${CPPDEVTK_QTSOL_QTCOPYDIALOG_TARGET})}
CPPDEVTK_QTSOL_QTSINGLEAPPLICATION_TARGET = $${qtLibraryTarget($${CPPDEVTK_QTSOL_QTSINGLEAPPLICATION_TARGET})}
CPPDEVTK_UTIL_TARGET = $${qtLibraryTarget($${CPPDEVTK_UTIL_TARGET})}
CPPDEVTK_QTSOL_QTSINGLECOREAPPLICATION_TARGET = $${qtLibraryTarget($${CPPDEVTK_QTSOL_QTSINGLECOREAPPLICATION_TARGET})}
CPPDEVTK_QTSOL_QTLOCKEDFILE_TARGET = $${qtLibraryTarget($${CPPDEVTK_QTSOL_QTLOCKEDFILE_TARGET})}
CPPDEVTK_QTSOL_QTSERVICE_TARGET = $${qtLibraryTarget($${CPPDEVTK_QTSOL_QTSERVICE_TARGET})}
CPPDEVTK_BASE_TARGET = $${qtLibraryTarget($${CPPDEVTK_BASE_TARGET})}

target.path = $${CPPDEVTK_LIB_DIR}
INSTALLS += target


# LIBS + PRE_TARGETDEPS
!debug_and_release|build_pass {
	isEqual(CPPDEVTK_ENABLE_QTSOLUTIONS, "true") {
		LIBS += -l$${CPPDEVTK_QTSOL_QTCOPYDIALOG_TARGET} -l$${CPPDEVTK_QTSOL_QTSINGLEAPPLICATION_TARGET}
	}
	LIBS += -l$${CPPDEVTK_UTIL_TARGET}
	isEqual(CPPDEVTK_ENABLE_QTSOLUTIONS, "true") {
		LIBS += -l$${CPPDEVTK_QTSOL_QTSINGLECOREAPPLICATION_TARGET} -l$${CPPDEVTK_QTSOL_QTLOCKEDFILE_TARGET} -l$${CPPDEVTK_QTSOL_QTSERVICE_TARGET}
	}
	LIBS += -l$${CPPDEVTK_BASE_TARGET}
	
	unix {
		linux* {
			!android {
				LIBS *= -lXss -lX11
			}
		}
		else {
			macx {
				LIBS += -framework Security -framework ApplicationServices -framework AppKit -framework IOKit
				LIBS += -framework Carbon -framework Foundation -framework CoreFoundation
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
			LIBS *= -lWtsapi32 -ladvapi32 -luser32
		}
		else {
			error("Unsupported platform!!!")
		}
	}
	
	
	!static_and_shared|build_pass {
		unix {
			cppdevtk_enable_android_destdir_workaround {
				isEqual(CPPDEVTK_ENABLE_QTSOLUTIONS, "true") {
					PRE_TARGETDEPS += $${OUT_PWD}/../QtCopyDialog/lib$${CPPDEVTK_QTSOL_QTCOPYDIALOG_TARGET}.$${CPPDEVTK_LIB_EXT}
					PRE_TARGETDEPS += $${OUT_PWD}/../QtSingleApplication/lib$${CPPDEVTK_QTSOL_QTSINGLEAPPLICATION_TARGET}.$${CPPDEVTK_LIB_EXT}
				}
				PRE_TARGETDEPS += $${OUT_PWD}/../util/lib$${CPPDEVTK_UTIL_TARGET}.$${CPPDEVTK_LIB_EXT}
				isEqual(CPPDEVTK_ENABLE_QTSOLUTIONS, "true") {
					PRE_TARGETDEPS += $${OUT_PWD}/../QtSingleCoreApplication/lib$${CPPDEVTK_QTSOL_QTSINGLECOREAPPLICATION_TARGET}.$${CPPDEVTK_LIB_EXT}
					PRE_TARGETDEPS += $${OUT_PWD}/../QtLockedFile/lib$${CPPDEVTK_QTSOL_QTLOCKEDFILE_TARGET}.$${CPPDEVTK_LIB_EXT}
					PRE_TARGETDEPS += $${OUT_PWD}/../QtService/lib$${CPPDEVTK_QTSOL_QTSERVICE_TARGET}.$${CPPDEVTK_LIB_EXT}
				}
				PRE_TARGETDEPS += $${OUT_PWD}/../base/lib$${CPPDEVTK_BASE_TARGET}.$${CPPDEVTK_LIB_EXT}
			}
			else {
				isEqual(CPPDEVTK_ENABLE_QTSOLUTIONS, "true") {
					PRE_TARGETDEPS += $${DESTDIR}/lib$${CPPDEVTK_QTSOL_QTCOPYDIALOG_TARGET}.$${CPPDEVTK_LIB_EXT}
					PRE_TARGETDEPS += $${DESTDIR}/lib$${CPPDEVTK_QTSOL_QTSINGLEAPPLICATION_TARGET}.$${CPPDEVTK_LIB_EXT}
				}
				PRE_TARGETDEPS += $${DESTDIR}/lib$${CPPDEVTK_UTIL_TARGET}.$${CPPDEVTK_LIB_EXT}
				isEqual(CPPDEVTK_ENABLE_QTSOLUTIONS, "true") {
					PRE_TARGETDEPS += $${DESTDIR}/lib$${CPPDEVTK_QTSOL_QTSINGLECOREAPPLICATION_TARGET}.$${CPPDEVTK_LIB_EXT}
					PRE_TARGETDEPS += $${DESTDIR}/lib$${CPPDEVTK_QTSOL_QTLOCKEDFILE_TARGET}.$${CPPDEVTK_LIB_EXT}
					PRE_TARGETDEPS += $${DESTDIR}/lib$${CPPDEVTK_QTSOL_QTSERVICE_TARGET}.$${CPPDEVTK_LIB_EXT}
				}
				PRE_TARGETDEPS += $${DESTDIR}/lib$${CPPDEVTK_BASE_TARGET}.$${CPPDEVTK_LIB_EXT}
			}
		}
		else {
			win32 {
				isEqual(CPPDEVTK_ENABLE_QTSOLUTIONS, "true") {
					PRE_TARGETDEPS += $${DESTDIR}/$${CPPDEVTK_QTSOL_QTCOPYDIALOG_TARGET}.$${CPPDEVTK_LIB_EXT}
					PRE_TARGETDEPS += $${DESTDIR}/$${CPPDEVTK_QTSOL_QTSINGLEAPPLICATION_TARGET}.$${CPPDEVTK_LIB_EXT}
				}
				PRE_TARGETDEPS += $${DESTDIR}/$${CPPDEVTK_UTIL_TARGET}.$${CPPDEVTK_LIB_EXT}
				isEqual(CPPDEVTK_ENABLE_QTSOLUTIONS, "true") {
					PRE_TARGETDEPS += $${DESTDIR}/$${CPPDEVTK_QTSOL_QTSINGLECOREAPPLICATION_TARGET}.$${CPPDEVTK_LIB_EXT}
					PRE_TARGETDEPS += $${DESTDIR}/$${CPPDEVTK_QTSOL_QTLOCKEDFILE_TARGET}.$${CPPDEVTK_LIB_EXT}
					PRE_TARGETDEPS += $${DESTDIR}/$${CPPDEVTK_QTSOL_QTSERVICE_TARGET}.$${CPPDEVTK_LIB_EXT}
				}
				PRE_TARGETDEPS += $${DESTDIR}/$${CPPDEVTK_BASE_TARGET}.$${CPPDEVTK_LIB_EXT}
			}
			else {
				error("Unsupported platform!!!")
			}
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
	language_menu.cpp	\
	language_widget.cpp \
	login_widget.cpp \
	message_box.cpp \
	password_widget.cpp \
	progress_dialog.cpp \
	timed_computer_management_widget.cpp \
	widget_base.cpp

!android:!ios {
	SOURCES += \
		power_notifier.cpp	\
		screensaver.cpp	\
		session_manager.cpp	\
		storage_device_notifier.cpp
}

unix {
	linux* {
		SOURCES += is_caps_lock_on_lnx.cpp
		
		!android {
			SOURCES +=	\
				screensaver_lnx.cpp	\
				storage_device_notifier_lnx.cpp	\
				storage_device_notifier_impl_lnx.cpp	\
				udisks1_device_notifier_lnx.cpp	\
				udisks2_device_notifier_lnx.cpp	\
				udisks1_filesystem_block_device_lnx.cpp	\
				udisks2_filesystem_block_device_lnx.cpp	\
				power_notifier_lnx.cpp	\
				power_notifier_impl_lnx.cpp	\
				upower_notifier_lnx.cpp	\
				logind_power_notifier_lnx.cpp	\
				session_manager_lnx.cpp	\
				session_lnx.cpp	\
				console_kit_manager_lnx.cpp	\
				console_kit_session_lnx.cpp	\
				logind_manager_lnx.cpp	\
				logind_session_lnx.cpp	\
				session_impl_lnx.cpp	\
				session_manager_impl_lnx.cpp
		}
	}
	else {
		macx|ios {
			SOURCES += is_caps_lock_on_mac.cpp
			
			macx {
				SOURCES += screensaver_mac.cpp	\
					screensaver_mac_carbon.cpp	\
					storage_device_notifier_mac.cpp	\
					power_notifier_mac.cpp	\
					session_manager_mac.cpp	\
					session_mac.cpp	\
					session_mac_carbon.cpp
				
				OBJECTIVE_SOURCES += screensaver_mac_cocoa.mm	\
					session_mac_cocoa.mm
			}
		}
		else {
			error("Unsupported Unix platform!!!")
		}
	}
}
else {
	win32 {
		SOURCES += is_caps_lock_on_win.cpp	\
			screensaver_win.cpp	\
			storage_device_notifier_win.cpp	\
			power_notifier_win.cpp	\
			session_manager_win.cpp	\
			session_win.cpp
	}
	else {
		error("Unsupported platform!!!")
	}
}

isEqual(CPPDEVTK_ENABLE_QTSOLUTIONS, "true") {
	SOURCES +=	\
		single_application.cpp	\
		copy_dialog.cpp
}


HEADERS += \
	../../include/cppdevtk/gui/application.hpp \
	../../include/cppdevtk/gui/application_base.hpp \
	../../include/cppdevtk/gui/caps_lock_widget.hpp \
	../../include/cppdevtk/gui/config.hpp \
	../../include/cppdevtk/gui/custom_wizard.hpp \
	../../include/cppdevtk/gui/disk_space_widget.hpp \
	../../include/cppdevtk/gui/eula_widget.hpp \
	../../include/cppdevtk/gui/info.hpp \
	../../include/cppdevtk/gui/info_customization.hpp	\
	../../include/cppdevtk/gui/info_tr.hpp	\
	../../include/cppdevtk/gui/info_tr_customization.hpp	\
	../../include/cppdevtk/gui/init_resources.hpp \
	../../include/cppdevtk/gui/invisible_widget.hpp \
	../../include/cppdevtk/gui/is_caps_lock_on.hpp \
	../../include/cppdevtk/gui/k_squeezed_text_label.hpp \
	../../include/cppdevtk/gui/language_menu.hpp	\
	../../include/cppdevtk/gui/language_widget.hpp \
	../../include/cppdevtk/gui/login_widget.hpp \
	../../include/cppdevtk/gui/message_box.hpp \
	../../include/cppdevtk/gui/password_widget.hpp \
	../../include/cppdevtk/gui/progress_dialog.hpp \
	../../include/cppdevtk/gui/set_stylesheet_from_file.hpp \
	../../include/cppdevtk/gui/timed_computer_management_widget.hpp \
	../../include/cppdevtk/gui/widget_base.hpp

!android:!ios {
	HEADERS += \
		../../include/cppdevtk/gui/screensaver.hpp	\
		../../include/cppdevtk/gui/storage_device_notifier.hpp	\
		../../include/cppdevtk/gui/power_notifier.hpp	\
		../../include/cppdevtk/gui/session_manager.hpp	\
		../../include/cppdevtk/gui/session.hpp
}

linux*:!android {
	HEADERS += \
		storage_device_notifier_impl_lnx.hpp	\
		udisks1_device_notifier_lnx.hpp	\
		udisks2_device_notifier_lnx.hpp	\
		udisks1_filesystem_block_device_lnx.hpp	\
		udisks2_filesystem_block_device_lnx.hpp	\
		power_notifier_impl_lnx.hpp	\
		upower_notifier_lnx.hpp	\
		logind_power_notifier_lnx.hpp	\
		console_kit_manager_lnx.hpp	\
		console_kit_session_lnx.hpp	\
		logind_manager_lnx.hpp	\
		logind_session_lnx.hpp	\
		session_impl_lnx.hpp	\
		session_manager_impl_lnx.hpp
}

isEqual(CPPDEVTK_ENABLE_QTSOLUTIONS, "true") {
	HEADERS +=	\
		../../include/cppdevtk/gui/single_application.hpp	\
		../../include/cppdevtk/gui/copy_dialog.hpp
}

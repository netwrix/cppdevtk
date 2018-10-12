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


greaterThan(QT_MAJOR_VERSION, 4): QT *= widgets
QT *= gui
unix {
	QT *= network
}
QT *= core


TEMPLATE = app


include(./test_caps_lock_widget_customization.pri)
SAVED_TARGET = $${TARGET}
include(./../../common.pri)


# CONFIG
CONFIG *= windows app_bundle
CONFIG *= link_prl


# TARGET + INSTALLS

TEMPLATE += cppdevtkphonylib
cppdevtk_enable_target_suffix_qt_major_version {
	CPPDEVTK_GUI_TARGET = $${qtLibraryTarget($${CPPDEVTK_GUI_TARGET}_qt$${QT_MAJOR_VERSION})}
	CPPDEVTK_QTSOL_QTCOPYDIALOG_TARGET = $${qtLibraryTarget($${CPPDEVTK_QTSOL_QTCOPYDIALOG_TARGET}_qt$${QT_MAJOR_VERSION})}
	CPPDEVTK_QTSOL_QTSINGLEAPPLICATION_TARGET = $${qtLibraryTarget($${CPPDEVTK_QTSOL_QTSINGLEAPPLICATION_TARGET}_qt$${QT_MAJOR_VERSION})}
	CPPDEVTK_UTIL_TARGET = $${qtLibraryTarget($${CPPDEVTK_UTIL_TARGET}_qt$${QT_MAJOR_VERSION})}
	CPPDEVTK_QTSOL_QTSINGLECOREAPPLICATION_TARGET = $${qtLibraryTarget($${CPPDEVTK_QTSOL_QTSINGLECOREAPPLICATION_TARGET}_qt$${QT_MAJOR_VERSION})}
	CPPDEVTK_QTSOL_QTLOCKEDFILE_TARGET = $${qtLibraryTarget($${CPPDEVTK_QTSOL_QTLOCKEDFILE_TARGET}_qt$${QT_MAJOR_VERSION})}
	CPPDEVTK_QTSOL_QTSERVICE_TARGET = $${qtLibraryTarget($${CPPDEVTK_QTSOL_QTSERVICE_TARGET}_qt$${QT_MAJOR_VERSION})}
	CPPDEVTK_BASE_TARGET = $${qtLibraryTarget($${CPPDEVTK_BASE_TARGET}_qt$${QT_MAJOR_VERSION})}
}
else {
	CPPDEVTK_GUI_TARGET = $${qtLibraryTarget($${CPPDEVTK_GUI_TARGET})}
	CPPDEVTK_QTSOL_QTCOPYDIALOG_TARGET = $${qtLibraryTarget($${CPPDEVTK_QTSOL_QTCOPYDIALOG_TARGET})}
	CPPDEVTK_QTSOL_QTSINGLEAPPLICATION_TARGET = $${qtLibraryTarget($${CPPDEVTK_QTSOL_QTSINGLEAPPLICATION_TARGET})}
	CPPDEVTK_UTIL_TARGET = $${qtLibraryTarget($${CPPDEVTK_UTIL_TARGET})}
	CPPDEVTK_QTSOL_QTSINGLECOREAPPLICATION_TARGET = $${qtLibraryTarget($${CPPDEVTK_QTSOL_QTSINGLECOREAPPLICATION_TARGET})}
	CPPDEVTK_QTSOL_QTLOCKEDFILE_TARGET = $${qtLibraryTarget($${CPPDEVTK_QTSOL_QTLOCKEDFILE_TARGET})}
	CPPDEVTK_QTSOL_QTSERVICE_TARGET = $${qtLibraryTarget($${CPPDEVTK_QTSOL_QTSERVICE_TARGET})}
	CPPDEVTK_BASE_TARGET = $${qtLibraryTarget($${CPPDEVTK_BASE_TARGET})}
}
TEMPLATE -= cppdevtkphonylib

target.path = $${CPPDEVTK_BIN_DIR}
INSTALLS += target

linux*:!android {
	desktop_file.files = $${SAVED_TARGET}.desktop
	desktop_file.path = $${CPPDEVTK_DATA_DIR}/applications
	INSTALLS += desktop_file
}


# LIBS + PRE_TARGETDEPS
!debug_and_release|build_pass {
	LIBS += -l$${CPPDEVTK_GUI_TARGET}
	isEqual(CPPDEVTK_ENABLE_QTSOLUTIONS, "true") {
		LIBS += -l$${CPPDEVTK_QTSOL_QTCOPYDIALOG_TARGET} -l$${CPPDEVTK_QTSOL_QTSINGLEAPPLICATION_TARGET}
	}
	LIBS += -l$${CPPDEVTK_UTIL_TARGET}
	isEqual(CPPDEVTK_ENABLE_QTSOLUTIONS, "true") {
		LIBS += -l$${CPPDEVTK_QTSOL_QTSINGLECOREAPPLICATION_TARGET} -l$${CPPDEVTK_QTSOL_QTLOCKEDFILE_TARGET} -l$${CPPDEVTK_QTSOL_QTSERVICE_TARGET}
	}
	LIBS += -l$${CPPDEVTK_BASE_TARGET}
	
	!static_and_shared|build_pass {
		android {
			cppdevtk_enable_android_destdir_workaround {
				ANDROID_EXTRA_LIBS += $${OUT_PWD}/../../src/base/lib$${CPPDEVTK_BASE_TARGET}.$${CPPDEVTK_LIB_EXT}
				isEqual(CPPDEVTK_ENABLE_QTSOLUTIONS, "true") {
					ANDROID_EXTRA_LIBS += $${OUT_PWD}/../../src/QtService/lib$${CPPDEVTK_QTSOL_QTSERVICE_TARGET}.$${CPPDEVTK_LIB_EXT}
					ANDROID_EXTRA_LIBS += $${OUT_PWD}/../../src/QtLockedFile/lib$${CPPDEVTK_QTSOL_QTLOCKEDFILE_TARGET}.$${CPPDEVTK_LIB_EXT}
					ANDROID_EXTRA_LIBS += $${OUT_PWD}/../../src/QtSingleCoreApplication/lib$${CPPDEVTK_QTSOL_QTSINGLECOREAPPLICATION_TARGET}.$${CPPDEVTK_LIB_EXT}
				}
				ANDROID_EXTRA_LIBS += $${OUT_PWD}/../../src/util/lib$${CPPDEVTK_UTIL_TARGET}.$${CPPDEVTK_LIB_EXT}
				isEqual(CPPDEVTK_ENABLE_QTSOLUTIONS, "true") {
					ANDROID_EXTRA_LIBS += $${OUT_PWD}/../../src/QtSingleApplication/lib$${CPPDEVTK_QTSOL_QTSINGLEAPPLICATION_TARGET}.$${CPPDEVTK_LIB_EXT}
					ANDROID_EXTRA_LIBS += $${OUT_PWD}/../../src/QtCopyDialog/lib$${CPPDEVTK_QTSOL_QTCOPYDIALOG_TARGET}.$${CPPDEVTK_LIB_EXT}
				}
				ANDROID_EXTRA_LIBS += $${OUT_PWD}/../../src/gui/lib$${CPPDEVTK_GUI_TARGET}.$${CPPDEVTK_LIB_EXT}
			}
			else {
				ANDROID_EXTRA_LIBS += $${DESTDIR}/lib$${CPPDEVTK_BASE_TARGET}.$${CPPDEVTK_LIB_EXT}
				isEqual(CPPDEVTK_ENABLE_QTSOLUTIONS, "true") {
					ANDROID_EXTRA_LIBS += $${DESTDIR}/lib$${CPPDEVTK_QTSOL_QTSERVICE_TARGET}.$${CPPDEVTK_LIB_EXT}
					ANDROID_EXTRA_LIBS += $${DESTDIR}/lib$${CPPDEVTK_QTSOL_QTLOCKEDFILE_TARGET}.$${CPPDEVTK_LIB_EXT}
					ANDROID_EXTRA_LIBS += $${DESTDIR}/lib$${CPPDEVTK_QTSOL_QTSINGLECOREAPPLICATION_TARGET}.$${CPPDEVTK_LIB_EXT}
				}
				ANDROID_EXTRA_LIBS += $${DESTDIR}/lib$${CPPDEVTK_UTIL_TARGET}.$${CPPDEVTK_LIB_EXT}
				isEqual(CPPDEVTK_ENABLE_QTSOLUTIONS, "true") {
					ANDROID_EXTRA_LIBS += $${DESTDIR}/lib$${CPPDEVTK_QTSOL_QTSINGLEAPPLICATION_TARGET}.$${CPPDEVTK_LIB_EXT}
					ANDROID_EXTRA_LIBS += $${DESTDIR}/lib$${CPPDEVTK_QTSOL_QTCOPYDIALOG_TARGET}.$${CPPDEVTK_LIB_EXT}
				}
				ANDROID_EXTRA_LIBS += $${DESTDIR}/lib$${CPPDEVTK_GUI_TARGET}.$${CPPDEVTK_LIB_EXT}
			}
		}
		
		unix {
			cppdevtk_enable_android_destdir_workaround {
				PRE_TARGETDEPS += $${OUT_PWD}/../../src/gui/lib$${CPPDEVTK_GUI_TARGET}.$${CPPDEVTK_LIB_EXT}
				isEqual(CPPDEVTK_ENABLE_QTSOLUTIONS, "true") {
					PRE_TARGETDEPS += $${OUT_PWD}/../../src/QtCopyDialog/lib$${CPPDEVTK_QTSOL_QTCOPYDIALOG_TARGET}.$${CPPDEVTK_LIB_EXT}
					PRE_TARGETDEPS += $${OUT_PWD}/../../src/QtSingleApplication/lib$${CPPDEVTK_QTSOL_QTSINGLEAPPLICATION_TARGET}.$${CPPDEVTK_LIB_EXT}
				}
				PRE_TARGETDEPS += $${OUT_PWD}/../../src/util/lib$${CPPDEVTK_UTIL_TARGET}.$${CPPDEVTK_LIB_EXT}
				isEqual(CPPDEVTK_ENABLE_QTSOLUTIONS, "true") {
					PRE_TARGETDEPS += $${OUT_PWD}/../../src/QtSingleCoreApplication/lib$${CPPDEVTK_QTSOL_QTSINGLECOREAPPLICATION_TARGET}.$${CPPDEVTK_LIB_EXT}
					PRE_TARGETDEPS += $${OUT_PWD}/../../src/QtLockedFile/lib$${CPPDEVTK_QTSOL_QTLOCKEDFILE_TARGET}.$${CPPDEVTK_LIB_EXT}
					PRE_TARGETDEPS += $${OUT_PWD}/../../src/QtService/lib$${CPPDEVTK_QTSOL_QTSERVICE_TARGET}.$${CPPDEVTK_LIB_EXT}
				}
				PRE_TARGETDEPS += $${OUT_PWD}/../../src/base/lib$${CPPDEVTK_BASE_TARGET}.$${CPPDEVTK_LIB_EXT}
			}
			else {
				PRE_TARGETDEPS += $${DESTDIR}/lib$${CPPDEVTK_GUI_TARGET}.$${CPPDEVTK_LIB_EXT}
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
				PRE_TARGETDEPS += $${DESTDIR}/$${CPPDEVTK_GUI_TARGET}.$${CPPDEVTK_LIB_EXT}
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
			isEqual(CPPDEVTK_ENABLE_STRIP_APP, "true") {
				!static_and_shared|build_pass {
					linux* {
						QMAKE_POST_LINK += $${CPPDEVTK_STRIP_APP_CMD} $${DESTDIR}/$${TARGET}
					}
					else {
						macx|ios {
							QMAKE_POST_LINK += $${CPPDEVTK_STRIP_APP_CMD} $${DESTDIR}/$${TARGET}.app/Contents/MacOS/$${TARGET}
						}
						else {
							error("Unsupported Unix platform!!!")
						}
					}
				}
			}
		}
	}
}


#qico plugin
!static_and_shared|build_pass {
	CONFIG(static, static|shared) {
		!debug_and_release|build_pass {
			TEMPLATE += cppdevtkphonylib
			unix {
				!exists($$[QT_INSTALL_PLUGINS]/imageformats/$${qtLibraryTarget(libqico)}.$${CPPDEVTK_LIB_EXT}) {
					error("qico plugin is missing!!!")
				}
			}
			else {
				win32 {
					!exists($$[QT_INSTALL_PLUGINS]/imageformats/$${qtLibraryTarget(qico)}.$${CPPDEVTK_LIB_EXT}) {
						error("qico plugin is missing!!!")
					}
				}
				else {
					error("Unsupported platform!!!")
				}
			}
			QTPLUGIN *= qico
			
			LIBS *= -L$$[QT_INSTALL_PLUGINS]/imageformats
			
			TEMPLATE -= cppdevtkphonylib
		}
	}
}




# Files


RESOURCES += cppdevtk_test_caps_lock_widget.qrc	\
	cppdevtk_test_caps_lock_widget_customization.qrc

win32 {
	RC_FILE = cppdevtk_test_caps_lock_widget.rc
	HEADERS += cppdevtk_test_caps_lock_widget_res_win.hpp
}

macx|ios {
	ICON = ./res/ico/application.icns
	
	ios {
		ios_icon.files = $$files($$PWD/ios/ico/app_icon_*.png)
		QMAKE_BUNDLE_DATA += ios_icon
	}
	
	!debug_and_release|build_pass {
		CONFIG(debug, debug|release) {
			macx {
				cppdevtk_enable_app_target_debug_suffix {
					QMAKE_INFO_PLIST = ./cppdevtk_test_caps_lock_widget_mac_debug.plist
				}
				else {
					QMAKE_INFO_PLIST = ./cppdevtk_test_caps_lock_widget_mac.plist
				}
			}
			else {
				cppdevtk_enable_app_target_debug_suffix {
					QMAKE_INFO_PLIST = ./cppdevtk_test_caps_lock_widget_ios_debug.plist
				}
				else {
					QMAKE_INFO_PLIST = ./cppdevtk_test_caps_lock_widget_ios.plist
				}
			}
		}
		else {
			macx {
				QMAKE_INFO_PLIST = ./cppdevtk_test_caps_lock_widget_mac.plist
			}
			else {
				QMAKE_INFO_PLIST = ./cppdevtk_test_caps_lock_widget_ios.plist
			}
		}
	}
	
	#!isEmpty(QMAKE_INFO_PLIST) {
		#!isEmpty(QMAKE_POST_LINK) {
		#	QMAKE_POST_LINK += &&
		#}
	#}
	#QMAKE_POST_LINK += rm -Rf $${DESTDIR}/$${TARGET}.app/Contents/Resources/qt_menu.nib
	#QMAKE_POST_LINK += && cp -R $$[QT_INSTALL_PREFIX]/src/gui/mac/qt_menu.nib $${DESTDIR}/$${TARGET}.app/Contents/Resources
	macx {
		CONFIG(shared, static|shared) {
			#!isEmpty(QMAKE_POST_LINK) {
			#	QMAKE_POST_LINK += &&
			#}
			#QMAKE_POST_LINK += cp -f $${_PRO_FILE_PWD_}/res/qt.conf $${DESTDIR}/$${TARGET}.app/Contents/Resources
		}
	}
}


SOURCES += main.cpp	\
    widget.cpp


HEADERS += \
	info.hpp \
	info_customization.hpp	\
    config.hpp \
    init_resources.hpp \
    widget.hpp

FORMS += \
    widget.ui

android {
	cppdevtk_enable_android_manifest {
		ANDROID_PACKAGE_SOURCE_DIR = $${_PRO_FILE_PWD_}/android
	
		DISTFILES += android/AndroidManifest.xml
	}
}

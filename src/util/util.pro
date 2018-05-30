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


greaterThan(QT_MAJOR_VERSION, 4): QT -= widgets
QT -= gui
unix {
	QT *= network
}
contains(QT_CONFIG, dbus) {
	QT *= dbus
}
QT *= core


TEMPLATE = lib


include(./util_customization.pri)
include(./../../common.pri)


# Build marker.
DEFINES *= CPPDEVTK_DETAIL_UTIL_BUILD


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
	CPPDEVTK_QTSOL_QTSINGLECOREAPPLICATION_TARGET = $${qtLibraryTarget($${CPPDEVTK_QTSOL_QTSINGLECOREAPPLICATION_TARGET}_qt$${QT_MAJOR_VERSION})}
	CPPDEVTK_QTSOL_QTLOCKEDFILE_TARGET = $${qtLibraryTarget($${CPPDEVTK_QTSOL_QTLOCKEDFILE_TARGET}_qt$${QT_MAJOR_VERSION})}
	CPPDEVTK_QTSOL_QTSERVICE_TARGET = $${qtLibraryTarget($${CPPDEVTK_QTSOL_QTSERVICE_TARGET}_qt$${QT_MAJOR_VERSION})}
	CPPDEVTK_BASE_TARGET = $${qtLibraryTarget($${CPPDEVTK_BASE_TARGET}_qt$${QT_MAJOR_VERSION})}
}
else {
	CPPDEVTK_QTSOL_QTSINGLECOREAPPLICATION_TARGET = $${qtLibraryTarget($${CPPDEVTK_QTSOL_QTSINGLECOREAPPLICATION_TARGET})}
	CPPDEVTK_QTSOL_QTLOCKEDFILE_TARGET = $${qtLibraryTarget($${CPPDEVTK_QTSOL_QTLOCKEDFILE_TARGET})}
	CPPDEVTK_QTSOL_QTSERVICE_TARGET = $${qtLibraryTarget($${CPPDEVTK_QTSOL_QTSERVICE_TARGET})}
	CPPDEVTK_BASE_TARGET = $${qtLibraryTarget($${CPPDEVTK_BASE_TARGET})}
}

target.path = $${CPPDEVTK_LIB_DIR}
INSTALLS += target


# LIBS + PRE_TARGETDEPS
!debug_and_release|build_pass {
	isEqual(CPPDEVTK_ENABLE_QTSOLUTIONS, "true") {
		LIBS += -l$${CPPDEVTK_QTSOL_QTSINGLECOREAPPLICATION_TARGET} -l$${CPPDEVTK_QTSOL_QTLOCKEDFILE_TARGET} -l$${CPPDEVTK_QTSOL_QTSERVICE_TARGET}
	}
	LIBS += -l$${CPPDEVTK_BASE_TARGET}
	
	unix {
		linux* {
			#LIBS *= -ludev
			LIBS *= -ldl
		}
		else {
			macx {
				LIBS += -framework Security -framework DiskArbitration -framework IOKit -framework CoreFoundation
				#LIBS *= -ldyld
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
		
		cppdevtk_with_zlib {
			# -lz also on Mac OS X debug (not z_debug)
			#LIBS *= -l$${qtLibraryTarget(z)}
			LIBS *= -lz
		}
	}
	else {
		win32 {
			cppdevtk_with_zlib {
				!static_and_shared|build_pass {
					CONFIG(static, static|shared) {
						LIBS *= -l$${qtLibraryTarget(zlibstat)}
					}
					else {
						LIBS *= -l$${qtLibraryTarget(zlibwapi)}
					}
				}
			}
			
			LIBS *= -ladvapi32 -luser32
		}
		else {
			error("Unsupported platform!!!")
		}
	}
	
	
	!static_and_shared|build_pass {
		unix {
			cppdevtk_enable_android_destdir_workaround {
				isEqual(CPPDEVTK_ENABLE_QTSOLUTIONS, "true") {
					PRE_TARGETDEPS += $${OUT_PWD}/../QtSingleCoreApplication/lib$${CPPDEVTK_QTSOL_QTSINGLECOREAPPLICATION_TARGET}.$${CPPDEVTK_LIB_EXT}
					PRE_TARGETDEPS += $${OUT_PWD}/../QtLockedFile/lib$${CPPDEVTK_QTSOL_QTLOCKEDFILE_TARGET}.$${CPPDEVTK_LIB_EXT}
					PRE_TARGETDEPS += $${OUT_PWD}/../QtService/lib$${CPPDEVTK_QTSOL_QTSERVICE_TARGET}.$${CPPDEVTK_LIB_EXT}
				}
				PRE_TARGETDEPS += $${OUT_PWD}/../base/lib$${CPPDEVTK_BASE_TARGET}.$${CPPDEVTK_LIB_EXT}
			}
			else {
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


RESOURCES += cppdevtk_util.qrc	\
    cppdevtk_util_customization.qrc

isEqual(QT_MAJOR_VERSION, "4") {
	RESOURCES += cppdevtk_util_qt4qm.qrc
}
else {
	isEqual(QT_MAJOR_VERSION, "5") {
		RESOURCES += cppdevtk_util_qt5qm.qrc
	}
	else {
		error("Qt 4 and Qt 5 are supported!!!")
	}
}

win32 {
	RC_FILE = cppdevtk_util.rc
	HEADERS += cppdevtk_util_res_win.hpp
}


SOURCES += \
	core_application.cpp \
	core_application_base.cpp \
	damerau_levenshtein_distance.cpp \
	dynamic_library.cpp \
	exception_to_errno.cpp	\
	filesystem_exception.cpp \
	filesystem_utils.cpp \
	init_resources.cpp	\
	jsoncpp.cpp	\
	language_info.cpp \
	no_such_file_or_directory_exception.cpp	\
	password_requirements.cpp \
	q_zip.cpp \
	tinyxml2.cpp

unix {
	SOURCES +=	\
		dynamic_library_unx.cpp \
		dynamic_loader_exception_unx.cpp \
		dynamic_loader_unx.cpp	\
		filesystem_utils_unx.cpp	\
		get_user_name_unx.cpp	\
		core_application_base_unx.cpp
	
	linux* {
		SOURCES += filesystem_utils_lnx.cpp
		!android {
			SOURCES += libudev_lnx.cpp
		}
	}
	else {
		macx|ios {
			SOURCES += filesystem_utils_mac.cpp
		}
		else {
			error("Unsupported Unix platform!!!")
		}
	}
}
else {
	win32 {
		SOURCES +=	\
			dynamic_library_win.cpp \
			dynamic_loader_exception_win.cpp \
			dynamic_loader_win.cpp	\
			filesystem_utils_win.cpp	\
			get_user_name_win.cpp	\
			core_application_base_win.cpp
	}
	else {
		error("Unsupported platform!!!")
	}
}

contains(QT_CONFIG, dbus) {
	SOURCES += \
		dbus_utils.cpp	\
		dbus_exception.cpp
}

isEqual(CPPDEVTK_ENABLE_QTSOLUTIONS, "true") {
	SOURCES += single_core_application.cpp
}


HEADERS += \
	../../include/cppdevtk/util/config.hpp \
	../../include/cppdevtk/util/core_application.hpp \
	../../include/cppdevtk/util/core_application_base.hpp \
	../../include/cppdevtk/util/damerau_levenshtein_distance.hpp \
	../../include/cppdevtk/util/dynamic_library.hpp \
	../../include/cppdevtk/util/dynamic_loader.hpp \
	../../include/cppdevtk/util/dynamic_loader_exception.hpp \
	../../include/cppdevtk/util/exception_to_errno.hpp	\
	../../include/cppdevtk/util/filesystem_exception.hpp \
	../../include/cppdevtk/util/filesystem_utils.hpp \
	../../include/cppdevtk/util/get_user_name.hpp \
	../../include/cppdevtk/util/info.hpp \
	../../include/cppdevtk/util/info_customization.hpp	\
	../../include/cppdevtk/util/info_tr.hpp	\
	../../include/cppdevtk/util/info_tr_customization.hpp	\
	../../include/cppdevtk/util/init_resources.hpp \
	../../include/cppdevtk/util/json/json-forwards.h	\
	../../include/cppdevtk/util/json/json.h	\
	../../include/cppdevtk/util/language_info.hpp \
	../../include/cppdevtk/util/no_such_file_or_directory_exception.hpp	\
	../../include/cppdevtk/util/password_requirements.hpp \
	../../include/cppdevtk/util/q_zip_reader.hpp \
	../../include/cppdevtk/util/q_zip_writer.hpp \
	../../include/cppdevtk/util/tinyxml2.h

unix {
	HEADERS += ../../include/cppdevtk/util/filesystem_utils_unx.hpp
	
	linux* {
		HEADERS += ../../include/cppdevtk/util/filesystem_utils_lnx.hpp
		!android {
			HEADERS += ../../include/cppdevtk/util/libudev_lnx.hpp
		}
	}
	else {
		macx|ios {
			HEADERS += ../../include/cppdevtk/util/filesystem_utils_mac.hpp
		}
		else {
			error("Unsupported Unix platform!!!")
		}
	}
}
else {
	win32 {
		HEADERS += ../../include/cppdevtk/util/filesystem_utils_win.hpp
	}
	else {
		error("Unsupported platform!!!")
	}
}

contains(QT_CONFIG, dbus) {
	HEADERS += \
		../../include/cppdevtk/util/dbus_utils.hpp	\
		../../include/cppdevtk/util/dbus_exception.hpp
}

isEqual(CPPDEVTK_ENABLE_QTSOLUTIONS, "true") {
	HEADERS += ../../include/cppdevtk/util/single_core_application.hpp
}

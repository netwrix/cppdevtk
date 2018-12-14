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


greaterThan(QT_MAJOR_VERSION, 4): QT -= widgets
QT -= gui
QT *= network
QT *= core


TEMPLATE = lib


include(./QtSingleCoreApplicationCustomization.pri)
include(./../../common.pri)


!isEqual(CPPDEVTK_ENABLE_QTSOLUTIONS, "true") {
    error("This subproject require QtSolutions enabled!!!")
}


# Build marker.
DEFINES *= CPPDEVTK_DETAIL_QTSINGLECOREAPP_BUILD


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
	CPPDEVTK_QTSOL_QTLOCKEDFILE_TARGET = $${CPPDEVTK_QTSOL_QTLOCKEDFILE_TARGET}_qt$${QT_MAJOR_VERSION}
	CPPDEVTK_BASE_TARGET = $${CPPDEVTK_BASE_TARGET}_qt$${QT_MAJOR_VERSION}
}
CPPDEVTK_QTSOL_QTLOCKEDFILE_TARGET = $${qtLibraryTarget($${CPPDEVTK_QTSOL_QTLOCKEDFILE_TARGET})}
CPPDEVTK_BASE_TARGET = $${qtLibraryTarget($${CPPDEVTK_BASE_TARGET})}

target.path = $${CPPDEVTK_LIB_DIR}
INSTALLS += target


# LIBS + PRE_TARGETDEPS
!debug_and_release|build_pass {
	LIBS += -l$${CPPDEVTK_QTSOL_QTLOCKEDFILE_TARGET} -l$${CPPDEVTK_BASE_TARGET}
	
	
	!static_and_shared|build_pass {
		unix {
			cppdevtk_enable_android_destdir_workaround {
				PRE_TARGETDEPS += $${OUT_PWD}/../QtLockedFile/lib$${CPPDEVTK_QTSOL_QTLOCKEDFILE_TARGET}.$${CPPDEVTK_LIB_EXT}
				PRE_TARGETDEPS += $${OUT_PWD}/../base/lib$${CPPDEVTK_BASE_TARGET}.$${CPPDEVTK_LIB_EXT}
			}
			else {
				PRE_TARGETDEPS += $${DESTDIR}/lib$${CPPDEVTK_QTSOL_QTLOCKEDFILE_TARGET}.$${CPPDEVTK_LIB_EXT}
				PRE_TARGETDEPS += $${DESTDIR}/lib$${CPPDEVTK_BASE_TARGET}.$${CPPDEVTK_LIB_EXT}
			}
		}
		else {
			win32 {
				PRE_TARGETDEPS += $${DESTDIR}/$${CPPDEVTK_QTSOL_QTLOCKEDFILE_TARGET}.$${CPPDEVTK_LIB_EXT}
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


win32 {
	RC_FILE = cppdevtk_qtsinglecoreapplication.rc
	HEADERS += cppdevtk_qtsinglecoreapplication_res_win.hpp
}


SOURCES +=	\
	qtlocalpeer.cpp	\
	qtsinglecoreapplication.cpp


HEADERS +=	\
	../../include/cppdevtk/QtSolutions/QtSingleCoreApplication/config.hpp	\
	../../include/cppdevtk/QtSolutions/QtSingleCoreApplication/qtlocalpeer.h	\
	../../include/cppdevtk/QtSolutions/QtSingleCoreApplication/QtSingleCoreApplication	\
	../../include/cppdevtk/QtSolutions/QtSingleCoreApplication/qtsinglecoreapplication.h

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


CPPDEVTK_PACKAGE_NAME = cppdevtk


# CppDevTk specific CONFIG options
CONFIG += cppdevtk_verbose
#CONFIG += cppdevtk_enable_debuginfo_in_release
#CONFIG += cppdevtk_enable_target_suffix_qt_major_version
CONFIG += cppdevtk_enable_app_target_debug_suffix
CONFIG += cppdevtk_disable_warnings

# TODO: keep in sync with CPPDEVTK_WITH_ZLIB in config/features.hpp
CONFIG += cppdevtk_with_zlib

win32 {
	#CONFIG += cppdevtk_target_xp
}
# TODO: keep in sync with CPPDEVTK_DISABLE_OLD_OS in config/features.hpp
CONFIG += cppdevtk_disable_old_os

# TODO: keep in sync with CPPDEVTK_ENABLE_QT_SOLUTIONS in config/features.hpp
CPPDEVTK_ENABLE_QTSOLUTIONS = true


# target OS version
# TODO: keep in sync with config/features.hpp
unix {
	linux* {
		android {
			# ignored by Qt Creator and qmake; must be set as environment variable
			cppdevtk_disable_old_os {
				#ANDROID_NDK_PLATFORM = android-19
			}
			else {
				##ANDROID_NDK_PLATFORM = android-15
				##ANDROID_NDK_PLATFORM = android-17
				#ANDROID_NDK_PLATFORM = android-19
			}
		}
	}
	else {
		macx {
			*g++* {
				cppdevtk_disable_old_os {
					CPPDEVTK_MAC_OS_X_VERSION_MIN_REQUIRED = 1050
					CPPDEVTK_MACOSX_DEPLOYMENT_TARGET = 10.5
				}
				else {
					CPPDEVTK_MAC_OS_X_VERSION_MIN_REQUIRED = 1040
					CPPDEVTK_MACOSX_DEPLOYMENT_TARGET = 10.4
				}
			}
			else {
				*clang* {
					cppdevtk_disable_old_os {
						CPPDEVTK_MAC_OS_X_VERSION_MIN_REQUIRED = 101000
						CPPDEVTK_MACOSX_DEPLOYMENT_TARGET = 10.10
					}
					else {
						CPPDEVTK_MAC_OS_X_VERSION_MIN_REQUIRED = 1070
						CPPDEVTK_MACOSX_DEPLOYMENT_TARGET = 10.7
					}
				}
				else {
					error("Unsupported compiler for Mac OS X platform!!!")
				}
			}
		}
		else {
			ios {
				cppdevtk_disable_old_os {
					CPPDEVTK_IOS_DEPLOYMENT_TARGET = 10.0
				}
				else {
					CPPDEVTK_IOS_DEPLOYMENT_TARGET = 10.0
				}
			}
			else {
				error("Unsupported Unix platform!!!")
			}
		}
	}
}
else {
	win32 {
		cppdevtk_disable_old_os {
			cppdevtk_target_xp {
				# Win Vista SP2
				CPPDEVTK_WIN32_WINNT = 0x0600
				CPPDEVTK_NTDDI_VERSION = 0x06000200
			}
			else {
				# Win 7
				CPPDEVTK_WIN32_WINNT = 0x0601
				CPPDEVTK_NTDDI_VERSION = 0x06010000
			}
		}
		else {
			cppdevtk_target_xp {
				# Win XP SP3
				CPPDEVTK_WIN32_WINNT = 0x0501
				CPPDEVTK_NTDDI_VERSION = 0x05010300
			}
			else {
				# Win Vista SP2
				CPPDEVTK_WIN32_WINNT = 0x0600
				CPPDEVTK_NTDDI_VERSION = 0x06000200
			}
		}
	}
	else {
		error("Unsupported platform!!!")
	}
}

cppdevtk_enable_debuginfo_in_release {
	*clang* {
		QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO *= -O2 -g1
		QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO *= -O2 -g1
		QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO *= -g1
	}
	else {
		*g++* {
			QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO *= -O2 -g1
			QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO *= -O2 -g1
			QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO *= -g1
		}
		else {
			*msvc* {
				QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO *= -O2 -Z7
				QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO -= -Zi
				!static_and_shared|build_pass {
					CONFIG(shared, static|shared) {
						QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO *= -MD
					}
					else {
						QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO *= -MT
					}
				}
				QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO *= $${QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO}
				QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO *= /INCREMENTAL:NO /OPT:REF /OPT:ICF
				QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO -= /DEBUG
			}
			else {
				error("Unsupported compiler!!!")
			}
		}
	}
	
	CONFIG *= force_debug_info
}
else {
	*clang* {
		QMAKE_CFLAGS_RELEASE *= -O2 -g0
		QMAKE_CXXFLAGS_RELEASE *= -O2 -g0
		QMAKE_LFLAGS_RELEASE *= -g0
		#QMAKE_LFLAGS_RELEASE *= -s
	}
	else {
		*g++* {
			QMAKE_CFLAGS_RELEASE *= -O2 -g0
			QMAKE_CXXFLAGS_RELEASE *= -O2 -g0
			QMAKE_LFLAGS_RELEASE *= -g0
			QMAKE_LFLAGS_RELEASE *= -s
		}
		else {
			*msvc* {
				QMAKE_CFLAGS_RELEASE *= -O2
				QMAKE_CFLAGS_RELEASE -= -Zi
				CONFIG(shared, static|shared) {
					QMAKE_CFLAGS_RELEASE *= -MD
				}
				else {
					QMAKE_CFLAGS_RELEASE *= -MT
				}
				QMAKE_CXXFLAGS_RELEASE *= $${QMAKE_CFLAGS_RELEASE}
				QMAKE_LFLAGS_RELEASE *= /INCREMENTAL:NO /OPT:REF /OPT:ICF
				QMAKE_LFLAGS_RELEASE -= /DEBUG
			}
			else {
				error("Unsupported compiler!!!")
			}
		}
	}
	
	CONFIG -= force_debug_info
}


# CPPDEVTK_PREFIX
# Ex:
# linux: /opt/cososys/local/(static)
# android: C:/cososys-android/local/arch (/opt/cososys-android/local/arch)
# mac: /opt/cososys/local/mac1010/arch/(static)
# iphonesimulator: /opt/cososys-ios/iphonesimulator/local
# iphoneos: /opt/cososys-ios/iphoneos/local
# windows: C:/cososys/local/win7/qtver/compiler/arch/(static)

isEmpty(CPPDEVTK_PREFIX) {
	unix {
		linux* {
			android {
				isEqual(QMAKE_HOST.os, "Windows") {
					CPPDEVTK_PREFIX = /opt/cososys-android/local
				}
				else {
					CPPDEVTK_PREFIX = /opt/cososys-android/local
				}
			}
			else {
				CPPDEVTK_PREFIX = /opt/cososys/local
			}
		}
		else {
			macx {
				*g++* {
					cppdevtk_disable_old_os {
						CPPDEVTK_PREFIX = /opt/cososys/local/mac105
					}
					else {
						CPPDEVTK_PREFIX = /opt/cososys/local/mac104
					}
				}
				else {
					*clang* {
						cppdevtk_disable_old_os {
							CPPDEVTK_PREFIX = /opt/cososys/local/mac1010
						}
						else {
							CPPDEVTK_PREFIX = /opt/cososys/local/mac107
						}
					}
					else {
						error("Unsupported compiler for Mac OS X platform!!!")
					}
				}
			}
			else {
				ios {
					CPPDEVTK_PREFIX = /opt/cososys-ios
					CONFIG(iphonesimulator, iphonesimulator|iphoneos) {
						CPPDEVTK_PREFIX = $${CPPDEVTK_PREFIX}/iphonesimulator
					}
					else {
						CPPDEVTK_PREFIX = $${CPPDEVTK_PREFIX}/iphoneos
					}
					CPPDEVTK_PREFIX = $${CPPDEVTK_PREFIX}/local
				}
				else {
					error("Unsupported Unix platform!!!")
				}
			}
		}
	}
	else {
		win32 {
			cppdevtk_disable_old_os {
				CPPDEVTK_PREFIX = C:/cososys/local/win7
			}
			else {
				CPPDEVTK_PREFIX = C:/cososys/local/winxpsp3
			}
		}
		else {
			error("Unsupported platform!!!")
		}
	}
	
	# qtver
	win32 {
		CPPDEVTK_PREFIX = $${CPPDEVTK_PREFIX}/qt$${QT_MAJOR_VERSION}
	}
	
	# compiler
	win32 {
		greaterThan(QT_MAJOR_VERSION, 4) {	# QMAKESPEC does not work on Qt 4
			isEmpty(QMAKESPEC) {
				error("QMAKESPEC is empty")
			}
			CPPDEVTK_QMAKESPEC_BASENAME = $$basename(QMAKESPEC)
			CPPDEVTK_QMAKESPEC_BASENAME_SPLIT = $$split(CPPDEVTK_QMAKESPEC_BASENAME, "-")
			CPPDEVTK_COMPILER = $$member(CPPDEVTK_QMAKESPEC_BASENAME_SPLIT, 1)
			isEmpty(CPPDEVTK_COMPILER) {
				error("CPPDEVTK_COMPILER is empty")
			}
			isEqual(CPPDEVTK_COMPILER, "msvc") {
				CPPDEVTK_COMPILER = msvc2015
			}
			CPPDEVTK_PREFIX = $${CPPDEVTK_PREFIX}/$${CPPDEVTK_COMPILER}
		}
	}
	
	# arch
	unix {
		linux* {
			android {
				isEmpty(QT_ARCH) {
					error("QT_ARCH is empty!!!")
				}
				CPPDEVTK_PREFIX = $${CPPDEVTK_PREFIX}/$${QT_ARCH}
			}
			else {
				# we do not use arch on linux because we do not cross-compile (i586 on x86_64)
			}
		}
		else {
			macx {
				# QT_ARCH does not work in Qt 4 on mac.
				greaterThan(QT_MAJOR_VERSION, 4) {
					isEmpty(QT_ARCH) {
						error("QT_ARCH is empty!!!")
					}
					CPPDEVTK_PREFIX = $${CPPDEVTK_PREFIX}/$${QT_ARCH}
				}
				else {
					x86 {
						CPPDEVTK_PREFIX = $${CPPDEVTK_PREFIX}/x86
					}
					else {
						x86_64 {
							CPPDEVTK_PREFIX = $${CPPDEVTK_PREFIX}/x86_64
						}
						else {
							error("arch missing from CONFIG!!!")
						}
					}
				}
			}
			else {
				ios {
					# arch not used
				}
				else {
					error("Unsupported Unix platform!!!")
				}
			}
		}
	}
	else {
		win32 {
			# QT_ARCH does not work in Qt 4 on Windows; use QMAKE_TARGET.arch
			isEmpty(QMAKE_TARGET.arch) {
				error("QMAKE_TARGET.arch is empty!!!")
			}
			CPPDEVTK_PREFIX = $${CPPDEVTK_PREFIX}/$${QMAKE_TARGET.arch}
		}
		else {
			error("Unsupported platform!!!")
		}
	}

	# link
	!android:!ios {
		!static_and_shared|build_pass {
			CONFIG(static, static|shared) {
				CPPDEVTK_PREFIX = $${CPPDEVTK_PREFIX}/static
			}
		}
	}
}

# third-party
unix {
	linux* {
		android {
			CONFIG += cppdevtk_have_third_party
		}
		else {
			!static_and_shared|build_pass {
				CONFIG(static, static|shared) {
					#CONFIG += cppdevtk_have_third_party
				}
				else {
					#CONFIG += cppdevtk_have_third_party
				}
			}
		}
	}
	else {
		macx {
			!static_and_shared|build_pass {
				CONFIG(static, static|shared) {
					CONFIG += cppdevtk_have_third_party
				}
				else {
					CONFIG += cppdevtk_have_third_party
				}
			}
		}
		else {
			ios {
				CONFIG += cppdevtk_have_third_party
			}
			else {
				error("Unsupported Unix platform!!!")
			}
		}
	}
	
	cppdevtk_have_third_party {
		isEmpty(CPPDEVTK_THIRD_PARTY_PREFIX) {
			android:isEqual(QMAKE_HOST.os, "Windows") {
					CPPDEVTK_THIRD_PARTY_PREFIX = $$replace(CPPDEVTK_PREFIX, "/opt/", "C:/")
			}
			else {
				CPPDEVTK_THIRD_PARTY_PREFIX = $${CPPDEVTK_PREFIX}
			}
		}
	}
}
else {
	CONFIG += cppdevtk_have_third_party
	
	win32 {
		cppdevtk_have_third_party {
			isEmpty(CPPDEVTK_THIRD_PARTY_PREFIX) {
				CPPDEVTK_THIRD_PARTY_PREFIX = $${CPPDEVTK_PREFIX}
			}
		}
	}
	else {
		error("Unsupported platform!!!")
	}
}

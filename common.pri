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


include(./common_customization.pri)


#****************************************************************************************************************************
# qmake bugs/workarounds
# TODO: modify when fixed
android {
	# Qt does not handle versionName and versionCode in AndroidManifest.xml
	# This option enable usage of cppdevtk internal AndroidManifest.xml with proper version fields
	# The disadvantage is that permissions are no longer automatically updated,
	# so pay attention when adding new Qt modules or libraries.
	CONFIG += cppdevtk_enable_android_manifest
	
	# Build fails if DESTDIR is changed (Qt/qmake/Android SDK/NDK bug)
	CONFIG += cppdevtk_enable_android_destdir_workaround
}
ios {
	#CONFIG += cppdevtk_qmake_bundle_data_buggy
}


#****************************************************************************************************************************
# print header.
cppdevtk_verbose {
	message("********************************************************************************")
	message("* Entering $${_FILE_} included from $${_PRO_FILE_} ($${_PRO_FILE_PWD_})")
}


#****************************************************************************************************************************
# Check multiple inclusion.
isEqual(CPPDEVTK_COMMON_PRI_INCLUDED, "true") {
	error("$${_FILE_} already included!!!")
}
else {
	CPPDEVTK_COMMON_PRI_INCLUDED = true
}


#****************************************************************************************************************************
# version
CPPDEVTK_VERSION_MAJOR = 1
CPPDEVTK_VERSION_MINOR = 1
CPPDEVTK_VERSION_PATCH = 0
win32 {
	CPPDEVTK_VERSION_BUILD = 1
}


#****************************************************************************************************************************
# Build marker.
DEFINES += CPPDEVTK_DETAIL_BUILD


#****************************************************************************************************************************
# CONFIG
# Please see:
# CONFIG, Qt 4: http://doc.qt.io/qt-4.8/qmake-variable-reference.html#config
# CONFIG, Qt 5: http://doc.qt.io/qt-5/qmake-variable-reference.html#config
# Undocumented Qmake v1: http://paulf.free.fr/undocumented_qmake.html
# Undocumented Qmake v2: https://wiki.qt.io/Undocumented_QMake

CONFIG -= depend_includepath
CONFIG -= precompile_header
CONFIG -= warn_off
CONFIG *= warn_on
CONFIG *= thread
CONFIG *= resources
CONFIG *= exceptions
CONFIG *= rtti
CONFIG *= stl
CONFIG *= copy_dir_files
CONFIG *= no_empty_targets
CONFIG *= no_include_pwd
#CONFIG *= no_lflags_merge
#CONFIG *= no_smart_library_merge
CONFIG *= debug_and_release_target
CONFIG *= no_keywords

unix {
	# rpath_libdirs is related to QMAKE_LFLAGS_RPATH and we use QMAKE_RPATHDIR
	#CONFIG *= rpath_libdirs
	
	linux* {
		CONFIG *= largefile
		
		!android {
			!contains(QT_CONFIG, dbus) {
				error("qt dbus is required on Linux!!!")
			}
		}
	}
	else {
		macx {
			CONFIG -= lib_bundle
			CONFIG -= -arch ppc
			CONFIG -= -arch ppc64
		}
		else {
			ios {
				CONFIG -= lib_bundle
				CONFIG -= -arch ppc
				CONFIG -= -arch ppc64
			}
			else {
				error("Unsupported Unix platform!!!")
			}
		}
	}
}
else {
	win32 {
		CONFIG *= embed_manifest_exe
	}
	else {
		error("Unsupported platform!!!")
	}
}


#****************************************************************************************************************************
# debug/release

# safety checks
!debug:!release {
    error("debug/release option missing from CONFIG!!!")
}

!debug_and_release|build_pass {
	CONFIG(debug, debug|release) {
		DEFINES *= _DEBUG DEBUG=1 QT_DEBUG
	}
	else {
		DEFINES *= NDEBUG NO_DEBUG DEBUG=0 QT_NO_DEBUG
	}
}


#****************************************************************************************************************************
# static/shared

# safety checks
!static:!shared {
    error("static/shared option missing from CONFIG!!!")
}

!static_and_shared|build_pass {
	CONFIG(shared, static|shared) {
		win32 {
			CONFIG *= embed_manifest_dll
		}
		
		ios {
			error("Only static build is allowed on iOS!!!")
		}
		
		DEFINES *= CPPDEVTK_SHARED
	}
	else {
		android {
			error("Only shared build is allowed on Android!!!")
		}
	}
}


# safety checks

ios {
	!iphonesimulator:!iphoneos {
		error("both iphonesimulator and iphoneos missing!!!")
	}
}

build_pass {
	!debug_and_release:!static_and_shared:!iphonesimulator_and_iphoneos {
		error("build_pass and !debug_and_release:!static_and_shared:!iphonesimulator_and_iphoneos!!!")
	}
}


#****************************************************************************************************************************
# Compilers
# TODO: investigate how "QMAKE_LFLAGS_RELEASE *= -s" affect stack trace

*g++* {
	# Minimum requirement: GCC 4.0.1
	lessThan(QT_GCC_MAJOR_VERSION, 4) {
		error("GCC >= 4.0.1 required!!!")
	}
	isEqual(QT_GCC_MAJOR_VERSION, "4"):isEqual(QT_GCC_MINOR_VERSION, "0"):lessThan(QT_GCC_PATCH_VERSION, 1) {
		error("GCC >= 4.0.1 required!!!")
	}
	
	# enable C++11
	# C++11 support was introduced in 4.3.0 and >= 6.1.0 (first 6.x.x) use C++14 by default
	lessThan(QT_GCC_MAJOR_VERSION, 6) {
		greaterThan(QT_GCC_MAJOR_VERSION, 4)|greaterThan(QT_GCC_MINOR_VERSION, 2) {
			!c++11:!c++14:!c++1z {
				!contains(QMAKE_CXXFLAGS, -std=c++0x):!contains(QMAKE_CXXFLAGS, -std=gnu++0x):!contains(QMAKE_CXXFLAGS, -std=c++11):!contains(QMAKE_CXXFLAGS, -std=gnu++11) {
					!contains(QMAKE_CXXFLAGS, -std=c++1y):!contains(QMAKE_CXXFLAGS, -std=gnu++1y):!contains(QMAKE_CXXFLAGS, -std=c++14):!contains(QMAKE_CXXFLAGS, -std=gnu++14) {
						!contains(QMAKE_CXXFLAGS, -std=c++1z):!contains(QMAKE_CXXFLAGS, -std=gnu++1z):!contains(QMAKE_CXXFLAGS, -std=c++17):!contains(QMAKE_CXXFLAGS, -std=gnu++17) {
							# g++ 4.3 - 4.6 use -std=c++0x or -std=gnu++0x
							# g++ >= 4.7.0 use -std=c++11 or -std=gnu++11
							greaterThan(QT_GCC_MAJOR_VERSION, 4)|greaterThan(QT_GCC_MINOR_VERSION, 6) {
								QMAKE_CXXFLAGS += -std=gnu++11
							}
							else {
								QMAKE_CXXFLAGS += -std=gnu++0x
							}
						}
					}
				}
			}
		}
	}
	
	# enable extensions
	contains(QMAKE_CXXFLAGS, -std=c++98) {
		QMAKE_CXXFLAGS -= -std=c++98
		QMAKE_CXXFLAGS += -std=gnu++98
	}
	contains(QMAKE_CXXFLAGS, -std=c++03) {
		QMAKE_CXXFLAGS -= -std=c++03
		QMAKE_CXXFLAGS += -std=gnu++03
	}
	contains(QMAKE_CXXFLAGS, -std=c++0x) {
		QMAKE_CXXFLAGS -= -std=c++0x
		QMAKE_CXXFLAGS += -std=gnu++0x
	}
	contains(QMAKE_CXXFLAGS, -std=c++11) {
		QMAKE_CXXFLAGS -= -std=c++11
		QMAKE_CXXFLAGS += -std=gnu++11
	}
	contains(QMAKE_CXXFLAGS, -std=c++1y) {
		QMAKE_CXXFLAGS -= -std=c++1y
		QMAKE_CXXFLAGS += -std=gnu++1y
	}
	contains(QMAKE_CXXFLAGS, -std=c++14) {
		QMAKE_CXXFLAGS -= -std=c++14
		QMAKE_CXXFLAGS += -std=gnu++14
	}
	contains(QMAKE_CXXFLAGS, -std=c++1z) {
		QMAKE_CXXFLAGS -= -std=c++1z
		QMAKE_CXXFLAGS += -std=gnu++1z
	}
	contains(QMAKE_CXXFLAGS, -std=c++17) {
		QMAKE_CXXFLAGS -= -std=c++17
		QMAKE_CXXFLAGS += -std=gnu++17
	}
	contains(QMAKE_CXXFLAGS, -std=c++2a) {
		QMAKE_CXXFLAGS -= -std=c++2a
		QMAKE_CXXFLAGS += -std=gnu++2a
	}
	
	# safety checks
	contains(QMAKE_CXXFLAGS, -fno-rtti) {
		error("RTTI must be enabled")
	}
	contains(QMAKE_CXXFLAGS, -fno-exceptions) {
		error("exceptions must be enabled")
	}
	contains(QMAKE_CXXFLAGS, -fno-threadsafe-statics) {
		error("threadsafe-statics must be enabled")
	}
	
	QMAKE_CFLAGS_WARN_ON *= -Wall -Wextra -Wunused-parameter
	QMAKE_CXXFLAGS_WARN_ON *= -Wall -Wextra -Wunused-parameter -Woverloaded-virtual

	QMAKE_CFLAGS *= -fstack-check -fstack-protector-all
	QMAKE_CXXFLAGS *= -fstack-check -fstack-protector-all
	
	QMAKE_CFLAGS_DEBUG *= -O0 -g3
	QMAKE_CXXFLAGS_DEBUG *= -O0 -g3
	
	QMAKE_CFLAGS_RELEASE *= -fno-inline-functions
	QMAKE_CXXFLAGS_RELEASE *= -fno-inline-functions
	# fno-inline-small-functions: gcc 4.3.0
	greaterThan(QT_GCC_MAJOR_VERSION, 4)|greaterThan(QT_GCC_MINOR_VERSION, 2) {
		QMAKE_CFLAGS_RELEASE *= -fno-inline-small-functions
		QMAKE_CXXFLAGS_RELEASE *= -fno-inline-small-functions
	}
	# fno-inline-functions-called-once: gcc 4.0.2
	greaterThan(QT_GCC_MAJOR_VERSION, 4)|greaterThan(QT_GCC_MINOR_VERSION, 0)|greaterThan(QT_GCC_PATCH_VERSION, 1) {
		QMAKE_CFLAGS_RELEASE *= -fno-inline-functions-called-once
		QMAKE_CXXFLAGS_RELEASE *= -fno-inline-functions-called-once
	}
	
	# visibility hidden: gcc 4
	!static_and_shared|build_pass {
		CONFIG(shared, static|shared) {
			QMAKE_CFLAGS *= -fvisibility=hidden
			QMAKE_CXXFLAGS *= -fvisibility=hidden
			QMAKE_CXXFLAGS *= -fvisibility-inlines-hidden
		}
	}
	
	# treat as error
	# warning: control reaches end of non-void function [-Wreturn-type]
	greaterThan(QT_GCC_MAJOR_VERSION, 4)|greaterThan(QT_GCC_MINOR_VERSION, 2) {
		QMAKE_CFLAGS *= -Werror=return-type
		QMAKE_CXXFLAGS *= -Werror=return-type
	}
	
	QMAKE_LFLAGS *= -rdynamic
	
	!sensitivityio_enable_debuginfo_in_release {
		QMAKE_LFLAGS_RELEASE *= -s
	}
}
else {
	*clang* {
		# Minimum requirement: Clang 3.2
		# TODO: investigate why test does not work
		lessThan(QT_CLANG_MAJOR_VERSION, 3) {
			#error("Clang >= 3.2 required!!!")
		}
		isEqual(QT_CLANG_MAJOR_VERSION, "3"):lessThan(QT_CLANG_MINOR_VERSION, 2) {
			#error("Clang >= 3.2 required!!!")
		}
		
		# enable C++11
		# By default, Clang builds C++ code according to the C++98 standard, with many C++11 features accepted as extensions.
		# Default XCode generated project enable C++11.
		!c++11:!c++14:!c++1z {
			!contains(QMAKE_CXXFLAGS, -std=c++0x):!contains(QMAKE_CXXFLAGS, -std=gnu++0x):!contains(QMAKE_CXXFLAGS, -std=c++11):!contains(QMAKE_CXXFLAGS, -std=gnu++11) {
				!contains(QMAKE_CXXFLAGS, -std=c++1y):!contains(QMAKE_CXXFLAGS, -std=gnu++1y):!contains(QMAKE_CXXFLAGS, -std=c++14):!contains(QMAKE_CXXFLAGS, -std=gnu++14) {
					!contains(QMAKE_CXXFLAGS, -std=c++1z):!contains(QMAKE_CXXFLAGS, -std=gnu++1z):!contains(QMAKE_CXXFLAGS, -std=c++17):!contains(QMAKE_CXXFLAGS, -std=gnu++17) {
						QMAKE_CXXFLAGS += -std=gnu++11
					}
				}
			}
		}
		
		# enable extensions
		contains(QMAKE_CXXFLAGS, -std=c++98) {
			QMAKE_CXXFLAGS -= -std=c++98
			QMAKE_CXXFLAGS += -std=gnu++98
		}
		contains(QMAKE_CXXFLAGS, -std=c++03) {
			QMAKE_CXXFLAGS -= -std=c++03
			QMAKE_CXXFLAGS += -std=gnu++03
		}
		contains(QMAKE_CXXFLAGS, -std=c++0x) {
			QMAKE_CXXFLAGS -= -std=c++0x
			QMAKE_CXXFLAGS += -std=gnu++0x
		}
		contains(QMAKE_CXXFLAGS, -std=c++11) {
			QMAKE_CXXFLAGS -= -std=c++11
			QMAKE_CXXFLAGS += -std=gnu++11
		}
		contains(QMAKE_CXXFLAGS, -std=c++1y) {
			QMAKE_CXXFLAGS -= -std=c++1y
			QMAKE_CXXFLAGS += -std=gnu++1y
		}
		contains(QMAKE_CXXFLAGS, -std=c++14) {
			QMAKE_CXXFLAGS -= -std=c++14
			QMAKE_CXXFLAGS += -std=gnu++14
		}
		contains(QMAKE_CXXFLAGS, -std=c++1z) {
			QMAKE_CXXFLAGS -= -std=c++1z
			QMAKE_CXXFLAGS += -std=gnu++1z
		}
		contains(QMAKE_CXXFLAGS, -std=c++17) {
			QMAKE_CXXFLAGS -= -std=c++17
			QMAKE_CXXFLAGS += -std=gnu++17
		}
		contains(QMAKE_CXXFLAGS, -std=c++2a) {
			QMAKE_CXXFLAGS -= -std=c++2a
			QMAKE_CXXFLAGS += -std=gnu++2a
		}
		
		# safety checks
		contains(QMAKE_CXXFLAGS, -fno-rtti) {
			error("RTTI must be enabled")
		}
		contains(QMAKE_CXXFLAGS, -fno-exceptions) {
			error("exceptions must be enabled")
		}
		contains(QMAKE_CXXFLAGS, -fno-threadsafe-statics) {
			error("threadsafe-statics must be enabled")
		}
		
		QMAKE_CFLAGS_WARN_ON *= -Wall -Wextra -Wunused-parameter
		QMAKE_CXXFLAGS_WARN_ON *= -Wall -Wextra -Wunused-parameter -Woverloaded-virtual

		QMAKE_CFLAGS *= -fstack-check -fstack-protector-all
		QMAKE_CXXFLAGS *= -fstack-check -fstack-protector-all
		
		QMAKE_CXXFLAGS -= -stdlib=libstdc++
		QMAKE_CXXFLAGS *= -stdlib=libc++
		
		QMAKE_CFLAGS_DEBUG *= -O0 -g3
		QMAKE_CXXFLAGS_DEBUG *= -O0 -g3
		
		#QMAKE_CFLAGS_RELEASE *= -fno-inline-functions -fno-inline-small-functions -fno-inline-functions-called-once
		#QMAKE_CXXFLAGS_RELEASE *= -fno-inline-functions -fno-inline-small-functions -fno-inline-functions-called-once
		
		# visibility hidden
		!static_and_shared|build_pass {
			CONFIG(shared, static|shared) {
				QMAKE_CFLAGS *= -fvisibility=hidden
				QMAKE_CXXFLAGS *= -fvisibility=hidden
				QMAKE_CXXFLAGS *= -fvisibility-inlines-hidden
			}
		}
		
		# treat as error
		# warning: control reaches end of non-void function [-Wreturn-type]
		QMAKE_CFLAGS *= -Werror=return-type
		QMAKE_CXXFLAGS *= -Werror=return-type
		
		QMAKE_LFLAGS -= -stdlib=libstdc++
		QMAKE_LFLAGS *= -stdlib=libc++
		QMAKE_LFLAGS *= -rdynamic
		
		!sensitivityio_enable_debuginfo_in_release {
			#QMAKE_LFLAGS_RELEASE *= -s
		}
	}
	else {
		*msvc* {
			# Minimum requirement: MSVC 2008 (9.0) SP1.
			# TODO: use regexp in $$find
			CPPDEVTK_MSC_VER = $$find(QMAKE_COMPILER_DEFINES, _MSC_VER=.*)
			CPPDEVTK_MSC_VER = $$replace(CPPDEVTK_MSC_VER, "_MSC_VER=", "")
			isEmpty(CPPDEVTK_MSC_VER) {
				error("Qt must set _MSC_VER in QMAKE_COMPILER_DEFINES")
			}
			lessThan(CPPDEVTK_MSC_VER, 1500) {
				error("MSVC >= 2008 (9.0) SP1 required!!!")
			}
			
			contains(QMAKE_CFLAGS, -Zc:wchar_t-) {
				error("Please set treat wchar_t as a built-in type!!!")
			}
			contains(QMAKE_CXXFLAGS, -Zc:wchar_t-) {
				error("Please set treat wchar_t as a built-in type!!!")
			}
			
			QMAKE_CFLAGS_WARN_ON -= -W4 -W3 -W2 -W1 -W0
			QMAKE_CXXFLAGS_WARN_ON -= -W4 -W3 -W2 -W1 -W0
			
			QMAKE_CFLAGS_WARN_ON *= -Wall
			QMAKE_CXXFLAGS_WARN_ON *= -Wall
			
			cppdevtk_disable_warnings {
				# C4820: 'bytes' bytes padding added after construct 'member_name'
				# C4548: expression before comma has no effect; expected expression with side-effect
				# C4619: #pragma warning : there is no warning number 'number'
				# C4711: function 'function' selected for inline expansion
				# C4464: relative include path contains '..'
				QMAKE_CFLAGS_WARN_ON *= -wd4820 -wd4548 -wd4619 -wd4711 -wd4464
				QMAKE_CXXFLAGS_WARN_ON *= -wd4820 -wd4548 -wd4619 -wd4711 -wd4464
				
				!static_and_shared|build_pass {
					CONFIG(static, static|shared) {
						# C4365: action' : conversion from 'type_1' to 'type_2', signed/unsigned mismatch
						# C4571: Informational: catch(...) semantics changed since Visual C++ 7.1; structured exceptions (SEH) are no longer caught
						QMAKE_CFLAGS_WARN_ON *= -wd4365 -wd4571
						QMAKE_CXXFLAGS_WARN_ON *= -wd4365 -wd4571
					}
				}
			}
			
			# treat as error
			# C4715: function : not all control paths return a value
			QMAKE_CFLAGS *= -we4715
			QMAKE_CXXFLAGS *= -we4715
			
			
			#QMAKE_CXXFLAGS *= -analyze
			
			# warning: LNK4221: no public symbols found; archive member will be inaccessible
			QMAKE_LFLAGS *= /IGNORE:4221
			
			cppdevtk_enable_debuginfo_in_release {
				QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO *= -O2 -Ot
				QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO *= -O2 -Ot
				CONFIG(static, static|shared) {
					QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO *= -MT
					QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO *= -MT
				}
				else {
					QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO *= -MD
					QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO *= -MD
				}
				QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO *= /DEBUG /OPT:REF /OPT:ICF /INCREMENTAL:NO
				
				QMAKE_CFLAGS_RELEASE = $${QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO}
				QMAKE_CXXFLAGS_RELEASE = $${QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO}
				QMAKE_LFLAGS_RELEASE = $${QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO}
			}
			else {
				QMAKE_CFLAGS_RELEASE *= -O2 -Ot
				QMAKE_CXXFLAGS_RELEASE *= -O2 -Ot
				CONFIG(static, static|shared) {
					QMAKE_CFLAGS_RELEASE *= -MT
					QMAKE_CXXFLAGS_RELEASE *= -MT
				}
				else {
					QMAKE_CFLAGS_RELEASE *= -MD
					QMAKE_CXXFLAGS_RELEASE *= -MD
				}
			}
			
			QMAKE_CFLAGS_RELEASE *= -Ob1
			QMAKE_CXXFLAGS_RELEASE *= -Ob1
			
			QMAKE_CFLAGS_DEBUG *= -Zi
			QMAKE_CXXFLAGS_DEBUG *= -Zi
			CONFIG(static, static|shared) {
				QMAKE_CFLAGS_DEBUG *= -MTd
				QMAKE_CXXFLAGS_DEBUG *= -MTd
			}
			else {
				QMAKE_CFLAGS_DEBUG *= -MDd
				QMAKE_CXXFLAGS_DEBUG *= -MDd
			}
		}
		else {
			error("Unsupported compiler!!!")
		}
	}
}


#****************************************************************************************************************************
# Platforms
unix {
	linux* {
		android {
			# ignored by Qt Creator and qmake; must be set as environment variable
			#ANDROID_NDK_PLATFORM = android-17
		}
	}
	else {
		macx {
				DEFINES += MAC_OS_X_VERSION_MIN_REQUIRED=$${CPPDEVTK_MAC_OS_X_VERSION_MIN_REQUIRED}
				DEFINES += __MAC_OS_X_VERSION_MIN_REQUIRED=$${CPPDEVTK_MAC_OS_X_VERSION_MIN_REQUIRED}
				DEFINES += MAC_OS_X_VERSION_MAX_ALLOWED=$${CPPDEVTK_MAC_OS_X_VERSION_MAX_ALLOWED}
				DEFINES += __MAC_OS_X_VERSION_MAX_ALLOWED=$${CPPDEVTK_MAC_OS_X_VERSION_MAX_ALLOWED}
				QMAKE_MACOSX_DEPLOYMENT_TARGET = $${CPPDEVTK_MACOSX_DEPLOYMENT_TARGET}
				QMAKE_CFLAGS *= -mmacosx-version-min=$${CPPDEVTK_MACOSX_DEPLOYMENT_TARGET}
				QMAKE_CXXFLAGS *= -mmacosx-version-min=$${CPPDEVTK_MACOSX_DEPLOYMENT_TARGET}
				QMAKE_LFLAGS *= -mmacosx-version-min=$${CPPDEVTK_MACOSX_DEPLOYMENT_TARGET}
				#QMAKE_MAC_SDK=/Developer/SDKs/MacOSX$${CPPDEVTK_MACOSX_DEPLOYMENT_TARGET}u.sdk
				
				#greaterThan(CPPDEVTK_MAC_OS_X_VERSION_MIN_REQUIRED, 1040) {
				#	QMAKE_SONAME_PREFIX *= @rpath
				#}
		}
		else {
			ios {
				# safety check
				!iphonesimulator:!iphoneos {
					error("both iphonesimulator and iphoneos are missing from CONFIG")
				}
				
				DEFINES += __IPHONE_OS_VERSION_MIN_REQUIRED=$${CPPDEVTK_IPHONE_OS_VERSION_MIN_REQUIRED}
				DEFINES += __IPHONE_OS_VERSION_MAX_ALLOWED=$${CPPDEVTK_IPHONE_OS_VERSION_MAX_ALLOWED}
				QMAKE_IOS_DEPLOYMENT_TARGET = $${CPPDEVTK_IOS_DEPLOYMENT_TARGET}
				CONFIG(iphonesimulator, iphonesimulator|iphoneos) {
					QMAKE_CFLAGS *= -mios-simulator-version-min=$${CPPDEVTK_IOS_DEPLOYMENT_TARGET}
					QMAKE_CXXFLAGS *= -mios-simulator-version-min=$${CPPDEVTK_IOS_DEPLOYMENT_TARGET}
					QMAKE_LFLAGS *= -mios-simulator-version-min=$${CPPDEVTK_IOS_DEPLOYMENT_TARGET}
				}
				else {
					QMAKE_CFLAGS *= -miphoneos-version-min=$${CPPDEVTK_IOS_DEPLOYMENT_TARGET}
					QMAKE_CXXFLAGS *= -miphoneos-version-min=$${CPPDEVTK_IOS_DEPLOYMENT_TARGET}
					QMAKE_LFLAGS *= -miphoneos-version-min=$${CPPDEVTK_IOS_DEPLOYMENT_TARGET}
				}
				
				# does not work...
				#QMAKE_RANLIB *= -no_warning_for_no_symbol
			}
			else {
				error("Unsupported Unix platform!!!")
			}
		}
	}
}
else {
	win32 {
		DEFINES += _WIN32_WINNT=$${CPPDEVTK_WIN32_WINNT}
		DEFINES += NTDDI_VERSION=$${CPPDEVTK_NTDDI_VERSION}
		DEFINES += WINVER=$${CPPDEVTK_WINVER}
		DEFINES += _WIN32_IE=$${CPPDEVTK_WIN32_IE}
	}
	else {
		error("Unsupported platform!!!")
	}
}


#****************************************************************************************************************************
# Platforms Compilers

unix {
	*g++* {
		# nothing to do for now...
	}
	else {
		*clang* {
			# nothing to do for now...
		}
		else {
			error("Unsupported compiler for Unix platform!!!")
		}
	}
	
	linux* {
		*g++* {
			android {
				# TODO: investigate: Qt crashes on Android x86 (stack corruption detected)
				equals(ANDROID_TARGET_ARCH, x86) {
					QMAKE_CFLAGS -= -fstack-check -fstack-protector-all
					QMAKE_CXXFLAGS -= -fstack-check -fstack-protector-all
				}
			}
		}
		else {
			error("Unsupported compiler for Linux platform!!!")
		}
	}
	else {
		macx {
			*g++* {
				# -fstack-protector-all does not work on Mac OS X 10.4 gcc
				isEqual(CPPDEVTK_MAC_OS_X_VERSION_MIN_REQUIRED, "1040") {
					QMAKE_CFLAGS -= -fstack-protector-all
					QMAKE_CXXFLAGS -= -fstack-protector-all
				}
				
				# -s does not work on Mac OS X 10.4 gcc:
				#dyld: lazy symbol binding failed: lazy pointer not found
				#dyld: lazy pointer not found
				#Trace/BPT trap
				isEqual(CPPDEVTK_MAC_OS_X_VERSION_MIN_REQUIRED, "1040") {
					QMAKE_LFLAGS_RELEASE -= -s
				}
				
				# rdynamic does not work on Mac OS X gcc
				QMAKE_LFLAGS -= -rdynamic
			}
			else {
				*clang* {
					# Minimum requirement: highest Xcode on Mac OS X 10.7 is 4.6.3:
					# clang: Apple LLVM version 4.2 (clang-425.0.28) (based on LLVM 3.2svn)
					lessThan(QT_APPLE_CLANG_MAJOR_VERSION, 4) {
						error("Apple Clang >= 4.2 required!!!")
					}
					isEqual(QT_APPLE_CLANG_MAJOR_VERSION, "4"):lessThan(QT_APPLE_CLANG_MINOR_VERSION, 2) {
						error("Apple Clang >= 4.2 required!!!")
					}
				}
				else {
					error("Unsupported compiler for Mac OS X platform!!!")
				}
			}
		}
		else {
			ios {
				*clang* {
					# Minimum requirement: highest Xcode on Mac OS X 10.7 is 4.6.3:
					# clang: Apple LLVM version 4.2 (clang-425.0.28) (based on LLVM 3.2svn)
					lessThan(QT_APPLE_CLANG_MAJOR_VERSION, 4) {
						error("Apple Clang >= 4.2 required!!!")
					}
					isEqual(QT_APPLE_CLANG_MAJOR_VERSION, "4"):lessThan(QT_APPLE_CLANG_MINOR_VERSION, 2) {
						error("Apple Clang >= 4.2 required!!!")
					}
					
					CONFIG(iphonesimulator, iphonesimulator|iphoneos) {
						QMAKE_CFLAGS *= -fembed-bitcode-marker
						QMAKE_CXXFLAGS *= -fembed-bitcode-marker
					}
					else {
						QMAKE_CFLAGS *= -fembed-bitcode
						QMAKE_CXXFLAGS *= -fembed-bitcode
					}
				}
				else {
					error("Unsupported compiler for iOS platform!!!")
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
		*msvc* {
			CPPDEVTK_MSC_VER = $$find(QMAKE_COMPILER_DEFINES, _MSC_VER=.*)
			CPPDEVTK_MSC_VER = $$replace(CPPDEVTK_MSC_VER, "_MSC_VER=", "")
			isEmpty(CPPDEVTK_MSC_VER) {
				error("Qt must set _MSC_VER in QMAKE_COMPILER_DEFINES")
			}
			if(isEqual(CPPDEVTK_MSC_VER, "1700")|greaterThan(CPPDEVTK_MSC_VER, 1700)):contains(DEFINES, _USING_V110_SDK71_) {
				INCLUDEPATH = "C:/Program Files (x86)/Microsoft SDKs/Windows/v7.1A/Include" $${INCLUDEPATH}
				isEqual(QMAKE_TARGET.arch, "x86_64") {
					QMAKE_LFLAGS_WINDOWS *= /SUBSYSTEM:WINDOWS,5.02
					QMAKE_LFLAGS_CONSOLE *= /SUBSYSTEM:CONSOLE,5.02
					LIBS = -L"C:/Program Files (x86)/Microsoft SDKs/Windows/v7.1A/Lib/x64" $${LIBS}
				}
				else {
					QMAKE_LFLAGS_WINDOWS *= /SUBSYSTEM:WINDOWS,5.01
					QMAKE_LFLAGS_CONSOLE *= /SUBSYSTEM:CONSOLE,5.01
					LIBS = -L"C:/Program Files (x86)/Microsoft SDKs/Windows/v7.1A/Lib" $${LIBS}
				}
			}
		}
		else {
			error("Unsupported compiler for Windows platform!!!")
		}
	}
	else {
		error("Unsupported platform!!!")
	}
}


#****************************************************************************************************************************
# Qt.
# Notes:
# - Both Qt 4 and Qt 5 are supported.
# - The oldest Qt 4 supported is 4.6.2 (CentOS 6.0).
# - The oldest Qt 5 supported is 5.6.1 (CentOS 7.3, SLED12SP2); openSUSE 13.2 has 5.4.2 in updates and 5.7.1 in KDE Qt5

lessThan(QT_MAJOR_VERSION, 4)|greaterThan(QT_MAJOR_VERSION, 5) {
	error("Qt 4 and Qt 5 are supported!!!")
}
isEqual(QT_MAJOR_VERSION, "4"):lessThan(QT_VERSION, 0x040602) {
	error("The oldest Qt 4 supported is 4.6.2!!!")
}
isEqual(QT_MAJOR_VERSION, "5"):lessThan(QT_VERSION, 0x050601) {
	error("The oldest Qt 5 supported is 5.6.1!!!")
}

android|ios {
	lessThan(QT_MAJOR_VERSION, 5) {
		error("Android/iOS require Qt >= 5!!!")
	}
	
	#QMAKE_PROJECT_DEPTH = 0
}
QMAKE_PROJECT_DEPTH = 0

greaterThan(QT_MAJOR_VERSION, 4): DEFINES *= QT_DISABLE_DEPRECATED_BEFORE=0

# TARGET
!isEmpty(TEMPLATE) {
	isEqual(TEMPLATE, "app") {
		cppdevtk_enable_app_target_debug_suffix {
			TEMPLATE += cppdevtkphonylib
			cppdevtk_enable_target_suffix_qt_major_version {
				TARGET = $${qtLibraryTarget($${TARGET}_qt$${QT_MAJOR_VERSION})}
			}
			else {
				TARGET = $${qtLibraryTarget($${TARGET})}
			}
			TEMPLATE -= cppdevtkphonylib
		}
		else {
			cppdevtk_enable_target_suffix_qt_major_version {
				TARGET = $${TARGET}_qt$${QT_MAJOR_VERSION}
			}
		}
		message($${_PRO_FILE_}: TARGET: $${TARGET})
	}
	else {
		isEqual(TEMPLATE, "lib") {
			cppdevtk_enable_target_suffix_qt_major_version {
				TARGET = $${qtLibraryTarget($${TARGET}_qt$${QT_MAJOR_VERSION})}
			}
			else {
				TARGET = $${qtLibraryTarget($${TARGET})}
			}
			message($${_PRO_FILE_}: TARGET: $${TARGET})
		}
	}
}

unix {
	!android:!ios {
		!static_and_shared|build_pass {
			CONFIG(shared, static|shared) {
				!contains(QMAKE_RPATHDIR, $$[QT_INSTALL_LIBS]) {
					#QMAKE_RPATHDIR = $$[QT_INSTALL_LIBS]/:$${QMAKE_RPATHDIR}
				}
			}
		}
	}
	else {
		lessThan(QT_MAJOR_VERSION, 5) {
			error("Android and iOS require Qt >= 5")
		}
	}
}


#****************************************************************************************************************************
# Libraries extension

# Note: QMAKE_EXTENSION_SHLIB and TARGET_EXT are evaluated when running make, not qmake
CPPDEVTK_LIB_EXT = phonylibext
!static_and_shared|build_pass {
	CONFIG(static, static|shared) {
		unix {
			CPPDEVTK_LIB_EXT = a
		}
		else {
			win32 {
				CPPDEVTK_LIB_EXT = lib
			}
			else {
				error("Unsupported platform!!!")
			}
		}
	}
	else {
		unix {
			linux* {
				CPPDEVTK_LIB_EXT = so
			}
			else {
				macx|ios {
					CPPDEVTK_LIB_EXT = dylib
				}
				else {
					error("Unsupported Unix platform!!!")
				}
			}
		}
		else {
			win32 {
				CPPDEVTK_LIB_EXT = dll
			}
			else {
				error("Unsupported platform!!!")
			}
		}
	}
}


#****************************************************************************************************************************
# Third party

cppdevtk_have_third_party {
	isEmpty(CPPDEVTK_THIRD_PARTY_PREFIX) {
		error("CPPDEVTK_THIRD_PARTY_PREFIX is empty")
	}
	
	CPPDEVTK_THIRD_PARTY_INCLUDE_DIR = $${CPPDEVTK_THIRD_PARTY_PREFIX}/include
	if(linux*:!android)|macx {
		isEqual(QT_ARCH, "x86_64") {
			CPPDEVTK_THIRD_PARTY_LIB_DIR = $${CPPDEVTK_THIRD_PARTY_PREFIX}/lib64
		}
		else {
			CPPDEVTK_THIRD_PARTY_LIB_DIR = $${CPPDEVTK_THIRD_PARTY_PREFIX}/lib
		}
	}
	else {
		CPPDEVTK_THIRD_PARTY_LIB_DIR = $${CPPDEVTK_THIRD_PARTY_PREFIX}/lib
	}
	CPPDEVTK_THIRD_PARTY_BIN_DIR = $${CPPDEVTK_THIRD_PARTY_PREFIX}/bin
	
	!contains(INCLUDEPATH, $${CPPDEVTK_THIRD_PARTY_INCLUDE_DIR}) {
		INCLUDEPATH = $${CPPDEVTK_THIRD_PARTY_INCLUDE_DIR} $${INCLUDEPATH}
	}
	!contains(LIBS, "-L$${CPPDEVTK_THIRD_PARTY_LIB_DIR}") {
		LIBS = -L$${CPPDEVTK_THIRD_PARTY_LIB_DIR} $${LIBS}
	}
	unix {
		!android:!ios {
			!static_and_shared|build_pass {
				CONFIG(shared, static|shared) {
					!contains(QMAKE_RPATHDIR, $${CPPDEVTK_THIRD_PARTY_LIB_DIR}) {
						linux* {
							QMAKE_RPATHDIR = $${CPPDEVTK_THIRD_PARTY_LIB_DIR}/:$${QMAKE_RPATHDIR}
						}
					}
				}
			}
		}
	}
}


#****************************************************************************************************************************
# Build directories

# DESTDIR
cppdevtk_enable_android_destdir_workaround {
	!debug_and_release|build_pass {
		# TODO: keep updated
		LIBS *= -L$${OUT_PWD}/../../src/gui -L$${OUT_PWD}/../../src/jni -L$${OUT_PWD}/../../src/util -L$${OUT_PWD}/../../src/base
		LIBS *= -L$${OUT_PWD}/../gui -L$${OUT_PWD}/../jni -L$${OUT_PWD}/../util -L$${OUT_PWD}/../base
	}
}
else {
	# Ex:
	# linux: $${PWD}/build/linux/qtver/link/type
	# android: $${PWD}/build/android/arch/type
	# mac: $${PWD}/build/macosx/qtver/arch/link/type
	# iphonesimulator: $${PWD}/build/ios/iphonesimulator/type
	# iphoneos: $${PWD}/build/ios/iphoneos/type
	# windows: $${PWD}/build/windows/qtver/compiler/arch/link/type
	
	DESTDIR = $${PWD}/build

	# os
	unix {
		linux* {
			android {
				DESTDIR = $${DESTDIR}/android
			}
			else {
				DESTDIR = $${DESTDIR}/linux
			}
		}
		else {
			macx {
				DESTDIR = $${DESTDIR}/macosx
			}
			else {
				ios {
					DESTDIR = $${DESTDIR}/ios
					CONFIG(iphonesimulator, iphonesimulator|iphoneos) {
						DESTDIR = $${DESTDIR}/iphonesimulator
					}
					else {
						DESTDIR = $${DESTDIR}/iphoneos
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
			DESTDIR = $${DESTDIR}/windows
		}
		else {
			error("Unsupported platform!!!")
		}
	}
	
	# qtver
	!android:!ios {
		DESTDIR = $${DESTDIR}/qt$${QT_MAJOR_VERSION}
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
			DESTDIR = $${DESTDIR}/$${CPPDEVTK_COMPILER}
		}
	}
	
	# arch
	unix {
		linux* {
			android {
				isEmpty(QT_ARCH) {
					error("QT_ARCH is empty!!!")
				}
				DESTDIR = $${DESTDIR}/$${QT_ARCH}
			}
			else {
				# we do not use arch on linux because we do not cross-compile (i586 on x86_64)
			}
		}
		else {
			macx {
				# QT_ARCH does not work in Qt 4 on mac; fortunatelly we use Qt 4 only on 32 bits.
				greaterThan(QT_MAJOR_VERSION, 4) {
					isEmpty(QT_ARCH) {
						error("QT_ARCH is empty!!!")
					}
					DESTDIR = $${DESTDIR}/$${QT_ARCH}
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
			DESTDIR = $${DESTDIR}/$${QMAKE_TARGET.arch}
		}
		else {
			error("Unsupported platform!!!")
		}
	}
	
	# link
	!android:!ios {
		!static_and_shared|build_pass {
			CONFIG(static, static|shared) {
				DESTDIR = $${DESTDIR}/static
			}
			else {
				DESTDIR = $${DESTDIR}/shared
			}
		}
	}
	
	# type
	!debug_and_release|build_pass {
		CONFIG(debug, debug|release) {
			DESTDIR = $${DESTDIR}/debug
		}
		else {
			DESTDIR = $${DESTDIR}/release
		}
		
		!contains(LIBS, "-L$${DESTDIR}") {
			LIBS = -L$${DESTDIR} $${LIBS}
		}
	}
}

# intermediate dirs
if(!static_and_shared:!debug_and_release)|build_pass {
	CPPDEVTK_INTERMEDIATE_DIR = $${DESTDIR}/intermediate/$${TARGET}
	OBJECTS_DIR = $${CPPDEVTK_INTERMEDIATE_DIR}/obj
	RCC_DIR = $${CPPDEVTK_INTERMEDIATE_DIR}/rcc
	MOC_DIR = $${CPPDEVTK_INTERMEDIATE_DIR}/moc
	UI_DIR = $${CPPDEVTK_INTERMEDIATE_DIR}/ui
	# UI_HEADERS_DIR and UI_SOURCES_DIR were removed in Qt 5
	lessThan(QT_MAJOR_VERSION, 5) {
		UI_HEADERS_DIR = $${UI_DIR}/headers
		UI_SOURCES_DIR = $${UI_DIR}/sources
	}
}


#****************************************************************************************************************************
# Installation directories
# autotools:
#--bindir=DIR            user executables [EPREFIX/bin]
#--sbindir=DIR           system admin executables [EPREFIX/sbin]
#--libexecdir=DIR        program executables [EPREFIX/libexec]
#--sysconfdir=DIR        read-only single-machine data [PREFIX/etc]
#--sharedstatedir=DIR    modifiable architecture-independent data [PREFIX/com]
#--localstatedir=DIR     modifiable single-machine data [PREFIX/var]
#--libdir=DIR            object code libraries [EPREFIX/lib]
#--includedir=DIR        C header files [PREFIX/include]
#--oldincludedir=DIR     C header files for non-gcc [/usr/include]
#--datarootdir=DIR       read-only arch.-independent data root [PREFIX/share]
#--datadir=DIR           read-only architecture-independent data [DATAROOTDIR]
#--infodir=DIR           info documentation [DATAROOTDIR/info]
#--localedir=DIR         locale-dependent data [DATAROOTDIR/locale]
#--mandir=DIR            man documentation [DATAROOTDIR/man]
#--docdir=DIR            documentation root [DATAROOTDIR/doc/packagename]
#--htmldir=DIR           html documentation [DOCDIR]
#--dvidir=DIR            dvi documentation [DOCDIR]
#--pdfdir=DIR            pdf documentation [DOCDIR]
#--psdir=DIR             ps documentation [DOCDIR]

isEmpty(CPPDEVTK_PREFIX) {
	error("CPPDEVTK_PREFIX is empty")
}

CPPDEVTK_INCLUDE_DIR = $${CPPDEVTK_PREFIX}/include/cppdevtk
if(linux*:!android)|macx {
	isEqual(QT_ARCH, "x86_64") {
		CPPDEVTK_LIB_DIR = $${CPPDEVTK_PREFIX}/lib64
	}
	else {
		CPPDEVTK_LIB_DIR = $${CPPDEVTK_PREFIX}/lib
	}
}
else {
	CPPDEVTK_LIB_DIR = $${CPPDEVTK_PREFIX}/lib
}
CPPDEVTK_BIN_DIR = $${CPPDEVTK_PREFIX}/bin
CPPDEVTK_SBIN_DIR = $${CPPDEVTK_PREFIX}/sbin

isEmpty(CPPDEVTK_DATA_ROOT_DIR) {
	CPPDEVTK_DATA_ROOT_DIR = $${CPPDEVTK_PREFIX}/share
}
CPPDEVTK_DATA_DIR = $${CPPDEVTK_DATA_ROOT_DIR}
CPPDEVTK_DOC_DIR = $${CPPDEVTK_DATA_ROOT_DIR}/doc/$${CPPDEVTK_PACKAGE_NAME}-devel

isEmpty(CPPDEVTK_SYS_CONF_DIR) {
	CPPDEVTK_SYS_CONF_DIR = $${CPPDEVTK_PREFIX}/etc
}

isEmpty(CPPDEVTK_LOCAL_STATE_DIR) {
	CPPDEVTK_LOCAL_STATE_DIR = $${CPPDEVTK_PREFIX}/var
}

!static_and_shared|build_pass {
	CONFIG(shared, static|shared) {
		!isEmpty(CPPDEVTK_RPATHDIR) {
			!contains(QMAKE_RPATHDIR, $${CPPDEVTK_RPATHDIR}) {
				linux* {
					QMAKE_RPATHDIR = $${CPPDEVTK_RPATHDIR}/:$${QMAKE_RPATHDIR}
				}
				macx {
					QMAKE_RPATHDIR = $${CPPDEVTK_RPATHDIR}/
				}
			}
			macx {
				!contains(QMAKE_RPATHDIR, $${CPPDEVTK_RPATHDIR}/plugins) {
					#QMAKE_RPATHDIR = $${CPPDEVTK_RPATHDIR}/plugins/:$${QMAKE_RPATHDIR}
				}
				QMAKE_SONAME_PREFIX = $${CPPDEVTK_RPATHDIR}
			}
		}
		else {
			!contains(QMAKE_RPATHDIR, $${CPPDEVTK_LIB_DIR}) {
				linux {
					QMAKE_RPATHDIR = $${CPPDEVTK_LIB_DIR}/:$${QMAKE_RPATHDIR}
				}
				macx {
					QMAKE_RPATHDIR = $${CPPDEVTK_LIB_DIR}/
				}
			}
			macx {
				!contains(QMAKE_RPATHDIR, $${CPPDEVTK_LIB_DIR}/plugins) {
					#QMAKE_RPATHDIR = $${CPPDEVTK_LIB_DIR}/plugins/:$${QMAKE_RPATHDIR}
				}
				QMAKE_SONAME_PREFIX = $${CPPDEVTK_LIB_DIR}
			}
		}
	}
}


#****************************************************************************************************************************
# INCLUDEPATH, DEPENDPATH

!contains(INCLUDEPATH, $${PWD}/include) {
	INCLUDEPATH = $${PWD}/include $${INCLUDEPATH}
}
DEPENDPATH *= $${PWD}/include

lessThan(QT_MAJOR_VERSION, 5) {
	#INCLUDEPATH = $${UI_HEADERS_DIR} $${INCLUDEPATH}
	#DEPENDPATH *= $${UI_HEADERS_DIR}
}
else {
	#INCLUDEPATH = $${UI_DIR} $${INCLUDEPATH}
	#DEPENDPATH *= $${UI_DIR}
}

#INCLUDEPATH = $${_PRO_FILE_PWD_} $${INCLUDEPATH}
DEPENDPATH *= $${_PRO_FILE_PWD_}

#INCLUDEPATH = . $${INCLUDEPATH}
#DEPENDPATH *= .

DEPENDPATH *= $${PWD}/include/cppdevtk
DEPENDPATH *= $${PWD}/include/cppdevtk/config
DEPENDPATH *= $${PWD}/include/cppdevtk/config/compiler
DEPENDPATH *= $${PWD}/include/cppdevtk/config/platform
DEPENDPATH *= $${PWD}/include/cppdevtk/config/platform_compiler
DEPENDPATH *= $${PWD}/include/cppdevtk/base
DEPENDPATH *= $${PWD}/include/cppdevtk/util
DEPENDPATH *= $${PWD}/include/cppdevtk/jni
DEPENDPATH *= $${PWD}/include/cppdevtk/gui

DEPENDPATH *= $${PWD}/src/cppdevtk/base
DEPENDPATH *= $${PWD}/src/cppdevtk/util
DEPENDPATH *= $${PWD}/src/cppdevtk/jni
DEPENDPATH *= $${PWD}/src/cppdevtk/gui

DEPENDPATH *= $${PWD}/test/test_base
DEPENDPATH *= $${PWD}/test/test_caps_lock_widget
DEPENDPATH *= $${PWD}/test/test_config
DEPENDPATH *= $${PWD}/test/test_disk_space_widget
DEPENDPATH *= $${PWD}/test/test_invisible_widget
DEPENDPATH *= $${PWD}/test/test_slot_throw
DEPENDPATH *= $${PWD}/test/test_util


#****************************************************************************************************************************
# Strip

CPPDEVTK_ENABLE_STRIP_APP = false
CPPDEVTK_ENABLE_STRIP_SHARED_LIB = false
CPPDEVTK_ENABLE_STRIP_STATIC_LIB = false
!debug_and_release|build_pass {
	CONFIG(release, debug|release) {
		unix {
			linux* {
				android {
					CPPDEVTK_ENABLE_STRIP_APP = false
					CPPDEVTK_ENABLE_STRIP_SHARED_LIB = false
					CPPDEVTK_ENABLE_STRIP_STATIC_LIB = false
				}
				else {
					cppdevtk_enable_debuginfo_in_release {
						CPPDEVTK_ENABLE_STRIP_APP = false
						CPPDEVTK_ENABLE_STRIP_SHARED_LIB = false
						CPPDEVTK_ENABLE_STRIP_STATIC_LIB = false
					}
					else {
						CPPDEVTK_ENABLE_STRIP_APP = true
						CPPDEVTK_ENABLE_STRIP_SHARED_LIB = true
						# NOTE: strip does not work for static lib because when called the lib is in OUT_PWD not in DESTDIR (qmake bug?)
						# TODO: investigate
						CPPDEVTK_ENABLE_STRIP_STATIC_LIB = false
					}
				}
				
				CPPDEVTK_STRIP_APP_CMD = strip --strip-all
				CPPDEVTK_STRIP_SHARED_LIB_CMD = strip --strip-unneeded
				CPPDEVTK_STRIP_STATIC_LIB_CMD = strip --strip-debug
			}
			else {
				macx {
					cppdevtk_enable_debuginfo_in_release {
						CPPDEVTK_ENABLE_STRIP_APP = false
						CPPDEVTK_ENABLE_STRIP_SHARED_LIB = false
						CPPDEVTK_ENABLE_STRIP_STATIC_LIB = false
					}
					else {
						CPPDEVTK_ENABLE_STRIP_APP = true
						# NOTE: true does not work for libs on Mac because they are build in subproject dir (qmake bug?)
						# TODO: investigate
						CPPDEVTK_ENABLE_STRIP_SHARED_LIB = false
						CPPDEVTK_ENABLE_STRIP_STATIC_LIB = false
					}
					
					CPPDEVTK_STRIP_APP_CMD = strip
					CPPDEVTK_STRIP_SHARED_LIB_CMD = strip -x
					CPPDEVTK_STRIP_STATIC_LIB_CMD = strip -S
				}
				else {
					ios {
						CPPDEVTK_ENABLE_STRIP_APP = false
						CPPDEVTK_ENABLE_STRIP_SHARED_LIB = false
						CPPDEVTK_ENABLE_STRIP_STATIC_LIB = false
						
						CPPDEVTK_STRIP_APP_CMD = strip
						CPPDEVTK_STRIP_SHARED_LIB_CMD = strip -x
						CPPDEVTK_STRIP_STATIC_LIB_CMD = strip -S
					}
					else {
						error("Unsupported Unix platform!!!")
					}
				}
			}
		}
	}
}


#****************************************************************************************************************************
# JNI
CPPDEVTK_HAVE_JNI = false
!static_and_shared|build_pass {
	CONFIG(shared, static|shared) {
		CPPDEVTK_JAVA_HOME = $$(JAVA_HOME)
		unix {
			linux* {
				CPPDEVTK_HAVE_JNI = true
				!android {
					isEmpty(CPPDEVTK_JAVA_HOME) {
						error("JAVA_HOME is empty!!!")
					}
					
					INCLUDEPATH *= $${CPPDEVTK_JAVA_HOME}/include
					INCLUDEPATH *= $${CPPDEVTK_JAVA_HOME}/include/linux
					
					CPPDEVTK_JAVA_LIB_DIR =
					isEqual(QT_ARCH, "x86_64") {
						CPPDEVTK_JAVA_LIB_DIR = $${CPPDEVTK_JAVA_HOME}/jre/lib/amd64/server
					}
					else {
						CPPDEVTK_JAVA_LIB_DIR = $${CPPDEVTK_JAVA_HOME}/jre/lib/i386/server
					}
					LIBS *= -L$${CPPDEVTK_JAVA_LIB_DIR}
					!static_and_shared|build_pass {
						CONFIG(shared, static|shared) {
							!contains(QMAKE_RPATHDIR, $${CPPDEVTK_JAVA_LIB_DIR}) {
								QMAKE_RPATHDIR = $${QMAKE_RPATHDIR}:$${CPPDEVTK_JAVA_LIB_DIR}/
							}
						}
					}
				}
			}
			else {
				macx {
					CPPDEVTK_HAVE_JNI = true
					
					lessThan(CPPDEVTK_MAC_OS_X_VERSION_MIN_REQUIRED, 1070) {
						CONFIG += cppdevtk_mac_enable_javavm_framework
					}
					
					cppdevtk_mac_enable_javavm_framework {
						INCLUDEPATH = /System/Library/Frameworks/JavaVM.framework/Headers $${INCLUDEPATH}
					}
					else {
						CPPDEVTK_JAVA_HOME = /Library/Java/Home
						
						INCLUDEPATH = $${CPPDEVTK_JAVA_HOME}/include $${INCLUDEPATH}
						INCLUDEPATH = $${CPPDEVTK_JAVA_HOME}/include/darwin $${INCLUDEPATH}
						
						CPPDEVTK_JAVA_LIB_DIR = $${CPPDEVTK_JAVA_HOME}/jre/lib/server
						LIBS = -L$${CPPDEVTK_JAVA_LIB_DIR} $${LIBS}
						!static_and_shared|build_pass {
							CONFIG(shared, static|shared) {
								!contains(QMAKE_RPATHDIR, $${CPPDEVTK_JAVA_LIB_DIR}) {
									#QMAKE_RPATHDIR = $${QMAKE_RPATHDIR}:$${CPPDEVTK_JAVA_LIB_DIR}/
								}
							}
						}
					}
				}
				else {
					ios {
						# nothing to do for now; no jni
					}
					else {
						error("Unsupported Unix platform!!!")
					}
				}
			}
		}
		else {
			win32 {
				CONFIG(shared, static|shared) {
					CPPDEVTK_HAVE_JNI = true
					
					isEmpty(CPPDEVTK_JAVA_HOME) {
						error("JAVA_HOME is empty!!!")
					}
					
					INCLUDEPATH *= $${CPPDEVTK_JAVA_HOME}/include
					INCLUDEPATH *= $${CPPDEVTK_JAVA_HOME}/include/win32
					LIBS *= -L$${CPPDEVTK_JAVA_HOME}/lib
				}
			}
			else {
				error("Unsupported platform!!!")
			}
		}
	}
}


#****************************************************************************************************************************
# QMAKE_RC
win32 {
	QMAKE_RC += -d CPPDEVTK_DETAIL_BUILD
	
	QMAKE_RC += -d _WIN32_WINNT=$${CPPDEVTK_WIN32_WINNT}
	QMAKE_RC += -d NTDDI_VERSION=$${CPPDEVTK_NTDDI_VERSION}
	QMAKE_RC += -d WINVER=$${CPPDEVTK_WINVER}
	QMAKE_RC += -d _WIN32_IE=$${CPPDEVTK_WIN32_IE}
	
	QMAKE_RC += -d _WIN32 -d WIN32
	QMAKE_RC += -d _UNICODE -d UNICODE
	
	!static_and_shared|build_pass {
		CONFIG(shared, static|shared) {
			QMAKE_RC += -d CPPDEVTK_SHARED
		}
	}
	
	!debug_and_release|build_pass {
		CONFIG(debug, debug|release) {
			QMAKE_RC += -d _DEBUG -d DEBUG=1 -d QT_DEBUG
		}
		else {
			QMAKE_RC += -d NDEBUG -d NO_DEBUG -d DEBUG=0 -d QT_NO_DEBUG
		}
	}
	
	QMAKE_RC += -I $${PWD}/include
	QMAKE_RC += -I $${CPPDEVTK_THIRD_PARTY_INCLUDE_DIR}
}


#****************************************************************************************************************************
# Summary
cppdevtk_verbose {
	message("****************************************")
	message("* Begin summary:")
	
	!debug_and_release|build_pass {
		CONFIG(debug, debug|release) {
			message("* Debug pass")
		}
		else {
			message("* Release pass")
		}
	}
	else {
		message("* Auxiliary debug_and_release pass")
	}
	
	!static_and_shared|build_pass {
		CONFIG(static, static|shared) {
			message("* Static pass")
		}
		else {
			message("* Shared pass")
		}
	}
	else {
		message("* Auxiliary static_and_shared pass")
	}
	
	message("QT_CONFIG: $${QT_CONFIG}")
	message("CONFIG: $${CONFIG}")
	message("DEFINES: $${DEFINES}")
	message("CPPDEVTK_THIRD_PARTY_PREFIX: $${CPPDEVTK_THIRD_PARTY_PREFIX}")
	message("CPPDEVTK_PREFIX: $${CPPDEVTK_PREFIX}")
	message("INCLUDEPATH: $${INCLUDEPATH}")
	message("DEPENDPATH: $${DEPENDPATH}")
	message("LIBS: $${LIBS}")
	message("DESTDIR: $${DESTDIR}")
	message("QMAKE_CFLAGS: $${QMAKE_CFLAGS}")
	message("QMAKE_CXXFLAGS: $${QMAKE_CXXFLAGS}")
	message("QMAKE_LFLAGS: $${QMAKE_LFLAGS}")
	message("QMAKE_LFLAGS_RELEASE: $${QMAKE_LFLAGS_RELEASE}")
	unix {
		message("QMAKE_RPATHDIR: $${QMAKE_RPATHDIR}")
		macx {
			message("QMAKE_MACOSX_DEPLOYMENT_TARGET: $${QMAKE_MACOSX_DEPLOYMENT_TARGET}")
		}
		else {
			ios {
				message("QMAKE_IOS_DEPLOYMENT_TARGET: $${QMAKE_IOS_DEPLOYMENT_TARGET}")
			}
		}
	}
	win32 {
		message("QMAKE_RC: $${QMAKE_RC}")
	}
	
	message("* End summary")
	message("****************************************")
}

# print footer.
cppdevtk_verbose {
	message("* Leaving $${_FILE_} included from $${_PRO_FILE_} ($${_PRO_FILE_PWD_})")
	message("********************************************************************************")
}

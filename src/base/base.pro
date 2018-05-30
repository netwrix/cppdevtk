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
QT *= core


TEMPLATE = lib


include(./base_customization.pri)
include(./../../common.pri)


# Build marker.
DEFINES *= CPPDEVTK_DETAIL_BASE_BUILD


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


# Version
unix {
	CONFIG(shared, static|shared) {
		VER_MAJ = $${CPPDEVTK_VERSION_MAJOR}
		VER_MIN = $${CPPDEVTK_VERSION_MINOR}
		VER_PAT = $${CPPDEVTK_VERSION_PATCH}
	}
}


# TARGET + INSTALLS
target.path = $${CPPDEVTK_LIB_DIR}
INSTALLS += target


# LIBS + PRE_TARGETDEPS
!debug_and_release|build_pass {
	win32 {
		LIBS *= -lws2_32
		*msvc* {
			# version.lib and advapi32.lib are needed by third party StackWalker
			# TODO: remove if StackWalker will be replaced
			LIBS *= -lversion -ladvapi32
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


RESOURCES += cppdevtk_base.qrc	\
    cppdevtk_base_customization.qrc

win32 {
	RC_FILE = cppdevtk_base.rc
	HEADERS += cppdevtk_base_res_win.hpp
}


SOURCES += \
	any.cpp	\
	assert.cpp \
	bad_any_cast_exception.cpp	\
	bad_cast_exception.cpp	\
	bad_optional_access_exception.cpp	\
	cancelable.cpp	\
	cloneable.cpp \
	concurrent_run.cpp	\
	condition_variable.cpp	\
	condition_variable_pthread.cpp	\
	condition_variable_qt.cpp	\
	condition_variable_std.cpp	\
	dbc_exceptions.cpp \
	deadlock_exception.cpp \
	dynamic_byte_array.cpp \
	error_category.cpp \
	error_code.cpp \
	error_condition.cpp \
	exception.cpp \
	exception_propagation.cpp \
	future.cpp \
	future_errc.cpp	\
	future_exception.cpp	\
	init_resources.cpp \
	invalid_string_conversion_exception.cpp	\
	ios.cpp \
	ios_failure_exception.cpp	\
	lock_exception.cpp \
	lockables.cpp \
	logger.cpp \
	mutex.cpp \
	mutex_pthread.cpp	\
	mutex_qt.cpp	\
	mutex_std.cpp	\
	non_copyable.cpp \
	qiostream.cpp \
	safe_free.cpp \
	semaphore.cpp	\
	semaphore_pthread.cpp	\
	semaphore_qt.cpp	\
	semaphore_win.cpp	\
	source_code_info.cpp \
	stack_frame.cpp \
	stack_trace.cpp \
	stdexcept.cpp \
	stl_explicit_instantiation.cpp	\
	string_conv.cpp \
	string_utils.cpp \
	stringizable.cpp \
	system_exception.cpp \
	task_canceled_exception.cpp	\
	tfstream.cpp \
	thread.cpp	\
	thread_data.cpp	\
	thread_exception.cpp	\
	thread_local_data_ptr.cpp	\
	throw_lock_exception.cpp	\
	throwable.cpp \
	time_utils.cpp	\
	tios.cpp \
	tiostream.cpp \
	tistream.cpp \
	tostream.cpp \
	tsstream.cpp \
	tstreambuf.cpp \
	tstring.cpp \
	typeinfo.cpp	\
	unknown_exception.cpp	\
	version.c	\
	zeroize.cpp

unix {
	SOURCES += error_code_unx.cpp	\
		error_category_unx.cpp
}
else {
	win32 {
		SOURCES += error_code_win.cpp	\
			error_category_win.cpp
	}
	else {
        error("Unsupported platform!!!")
    }
}

win32 {
	SOURCES += locale_to_lang_id_win.cpp
}

macx:*g++* {
	SOURCES += backtrace_mac_gcc.c	\
			thread_stack_pcs_mac_gcc.c
}

android:*g++* {
	SOURCES += backtrace_android_gcc.c
}

macx|android {
	*g++* {
		SOURCES += backtrace_symbols_mac_android_gcc.c
	}
}

win32:*msvc* {
	SOURCES += stack_walker_win_msvc.cpp	\
		stack_walker_wrapper_win_msvc.cpp
}

unix {
	*g++*|*clang* {
		SOURCES += stack_trace_unx_gcc_clang.cpp
	}
	else {
		error("Unsupported compiler for Unix platform!!!")
	}
	
	linux* {
		*g++* {
			android {
				SOURCES += stack_trace_mac_ios_android_gcc_clang.cpp
			}
			else {
				SOURCES += stack_trace_lnx_gcc.cpp
			}
		}
		else {
			error("Unsupported compiler for Linux platform!!!")
		}
	}
	else {
		macx {
			*g++*|*clang* {
				 SOURCES += stack_trace_mac_ios_android_gcc_clang.cpp
			}
			else {
				error("Unsupported compiler for Mac OS X platform!!!")
			}
		}
		else {
			ios {
				*clang* {
					SOURCES += stack_trace_mac_ios_android_gcc_clang.cpp
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
			SOURCES += stack_trace_win_msvc.cpp
		}
		else {
			error("Unsupported compiler for Windows platform!!!")
		}
	}
	else {
		error("Unsupported platform!!!")
	}
}

*g++*|*clang* {
	SOURCES += name_mangling_gcc_clang.cpp
}
else {
	*msvc* {
		SOURCES += name_mangling_msvc.cpp
	}
	else {
		error("Unsupported compiler!!!")
	}
}

unix {
	SOURCES += thread_unx.cpp	\
		get_current_process_id_unx.cpp
}
else {
	win32 {
		SOURCES += thread_win.cpp	\
			thread_data_win.cpp	\
			get_current_process_id_win.cpp
	}
	else {
		error("Unsupported platform!!!")
	}
}

unix {
	SOURCES += pthread_ext.cpp	\
		posix_signals_watcher_unx.cpp
}

unix {
	SOURCES += socket_pair_unx.cpp
}
else {
	win32 {
		SOURCES += socket_pair_win.cpp
	}
	else {
		error("Unsupported platform!!!")
	}
}

HEADERS += \
	../../include/cppdevtk/base/any.hpp	\
	../../include/cppdevtk/base/architecture_types.h	\
	../../include/cppdevtk/base/architecture_types.hpp	\
	../../include/cppdevtk/base/assert.h	\
	../../include/cppdevtk/base/bad_any_cast_exception.hpp	\
	../../include/cppdevtk/base/bad_cast_exception.hpp	\
	../../include/cppdevtk/base/bad_optional_access_exception.hpp	\
	../../include/cppdevtk/base/call_traits.hpp	\
	../../include/cppdevtk/base/cancelable.hpp	\
	../../include/cppdevtk/base/cassert.hpp	\
	../../include/cppdevtk/base/cerrno.hpp	\
	../../include/cppdevtk/base/cinttypes.hpp	\
	../../include/cppdevtk/base/cloneable.hpp	\
	../../include/cppdevtk/base/concurrent_run.hpp	\
	../../include/cppdevtk/base/condition_variable.hpp	\
	../../include/cppdevtk/base/config.hpp	\
	../../include/cppdevtk/base/cstdint.hpp	\
	../../include/cppdevtk/base/dbc_exceptions.hpp	\
	../../include/cppdevtk/base/dbc.hpp	\
	../../include/cppdevtk/base/deadlock_exception.hpp	\
	../../include/cppdevtk/base/dynamic_byte_array.hpp	\
	../../include/cppdevtk/base/errno.h	\
	../../include/cppdevtk/base/error_category.hpp	\
	../../include/cppdevtk/base/error_code.hpp	\
	../../include/cppdevtk/base/error_condition.hpp	\
	../../include/cppdevtk/base/exception_propagation.hpp	\
	../../include/cppdevtk/base/exception.hpp	\
	../../include/cppdevtk/base/factory.hpp	\
	../../include/cppdevtk/base/future_errc.hpp	\
	../../include/cppdevtk/base/future_exception.hpp	\
	../../include/cppdevtk/base/future.hpp	\
	../../include/cppdevtk/base/generic_locking_algorithms.hpp	\
	../../include/cppdevtk/base/get_current_process_id.hpp	\
	../../include/cppdevtk/base/info.hpp	\
	../../include/cppdevtk/base/info_customization.hpp	\
	../../include/cppdevtk/base/info_tr.hpp	\
	../../include/cppdevtk/base/info_tr_customization.hpp	\
	../../include/cppdevtk/base/init_resources.hpp	\
	../../include/cppdevtk/base/inttypes_detail.h	\
	../../include/cppdevtk/base/inttypes.h	\
	../../include/cppdevtk/base/invalid_string_conversion_exception.hpp	\
	../../include/cppdevtk/base/ios_failure_exception.hpp	\
	../../include/cppdevtk/base/ios.hpp	\
	../../include/cppdevtk/base/join.h	\
	../../include/cppdevtk/base/locale_to_lang_id_win.hpp	\
	../../include/cppdevtk/base/lock_exception.hpp	\
	../../include/cppdevtk/base/lockables.hpp	\
	../../include/cppdevtk/base/locking_levels.hpp	\
	../../include/cppdevtk/base/locks.hpp	\
	../../include/cppdevtk/base/logger.hpp	\
	../../include/cppdevtk/base/mutex.hpp	\
	../../include/cppdevtk/base/name_mangling.hpp	\
	../../include/cppdevtk/base/non_copyable.hpp	\
	../../include/cppdevtk/base/on_block_exit.hpp	\
	../../include/cppdevtk/base/optional.hpp	\
	../../include/cppdevtk/base/qiostream.hpp	\
	../../include/cppdevtk/base/safe_delete.hpp	\
	../../include/cppdevtk/base/safe_free.hpp	\
	../../include/cppdevtk/base/semaphore.hpp	\
	../../include/cppdevtk/base/singletons.hpp	\
	../../include/cppdevtk/base/socket_pair.hpp \
	../../include/cppdevtk/base/source_code_info.hpp	\
	../../include/cppdevtk/base/stack_frame.hpp	\
	../../include/cppdevtk/base/stack_trace.hpp	\
	../../include/cppdevtk/base/static_assert.hpp	\
	../../include/cppdevtk/base/stdexcept.hpp	\
	../../include/cppdevtk/base/stdint_detail.h	\
	../../include/cppdevtk/base/stdint.h	\
	../../include/cppdevtk/base/stl_explicit_instantiation.hpp	\
	../../include/cppdevtk/base/string_conv.hpp	\
	../../include/cppdevtk/base/string_utils.hpp	\
	../../include/cppdevtk/base/stringizable.hpp	\
	../../include/cppdevtk/base/stringize.h	\
	../../include/cppdevtk/base/system_exception.hpp	\
	../../include/cppdevtk/base/task_canceled_exception.hpp	\
	../../include/cppdevtk/base/tchar.h	\
	../../include/cppdevtk/base/tchar.hpp	\
	../../include/cppdevtk/base/tfstream.hpp	\
	../../include/cppdevtk/base/thread_data_types.hpp	\
	../../include/cppdevtk/base/thread_exception.hpp	\
	../../include/cppdevtk/base/thread_shared_obj_locking_ptr.hpp	\
	../../include/cppdevtk/base/thread.hpp	\
	../../include/cppdevtk/base/throwable.hpp	\
	../../include/cppdevtk/base/time_utils.hpp	\
	../../include/cppdevtk/base/tios.hpp	\
	../../include/cppdevtk/base/tiostream.hpp	\
	../../include/cppdevtk/base/tistream.hpp	\
	../../include/cppdevtk/base/tostream.hpp	\
	../../include/cppdevtk/base/tsstream.hpp	\
	../../include/cppdevtk/base/tstreambuf.hpp	\
	../../include/cppdevtk/base/tstring_conv.hpp	\
	../../include/cppdevtk/base/tstring.hpp	\
	../../include/cppdevtk/base/tstringizable.hpp	\
	../../include/cppdevtk/base/tstringize.h	\
	../../include/cppdevtk/base/type_traits.hpp	\
	../../include/cppdevtk/base/typeinfo.hpp	\
	../../include/cppdevtk/base/unknown_exception.hpp	\
	../../include/cppdevtk/base/unused.h	\
	../../include/cppdevtk/base/unused.hpp	\
	../../include/cppdevtk/base/verify.h	\
	../../include/cppdevtk/base/version.h	\
	../../include/cppdevtk/base/wstringize.h	\
	../../include/cppdevtk/base/zeroize.h	\
	../../include/cppdevtk/base/zeroize.hpp

unix {
	HEADERS += ../../include/cppdevtk/base/execinfo_unx.h	\
		../../include/cppdevtk/base/posix_signals_watcher_unx.hpp
}

win32 {
	HEADERS += ../../include/cppdevtk/base/locale_to_lang_id_win.hpp
}

unix {
	linux* {
	   HEADERS += ../../include/cppdevtk/base/error_code_lnx.hpp
	}
	else {
		macx|ios {
			HEADERS += ../../include/cppdevtk/base/error_code_mac.hpp
		}
		else {
			error("Unsupported Unix platform for gcc compiler!!!")
		}
	}
}
else {
	win32 {
		HEADERS += ../../include/cppdevtk/base/error_code_win.hpp
	}
	else {
		error("Unsupported platform!!!")
	}
}


unix {
	HEADERS += pthread_ext.h
}

win32:*msvc* {
	HEADERS += stack_walker_win_msvc.hpp	\
		stack_walker_wrapper_win_msvc.hpp
}

HEADERS += time_utils.h	\
	throw_lock_exception.hpp	\
	thread_local_data_ptr.hpp	\
	thread_data.hpp

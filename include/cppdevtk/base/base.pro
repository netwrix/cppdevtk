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
	any.hpp	\
	architecture_types.h	\
	architecture_types.hpp	\
	assert.h	\
	bad_any_cast_exception.hpp	\
	bad_cast_exception.hpp	\
	bad_optional_access_exception.hpp	\
	call_traits.hpp	\
	cancelable.hpp	\
	cassert.hpp	\
	cerrno.hpp	\
	cinttypes.hpp	\
	cloneable.hpp	\
	concurrent_run.hpp	\
	condition_variable.hpp	\
	config.hpp	\
	cstdint.hpp	\
	dbc_exceptions.hpp	\
	dbc.hpp	\
	deadlock_exception.hpp	\
	dynamic_byte_array.hpp	\
	errno.h	\
	error_category.hpp	\
	error_code.hpp	\
	error_condition.hpp	\
	exception_propagation.hpp	\
	exception.hpp	\
	factory.hpp	\
	future_errc.hpp	\
	future_exception.hpp	\
	future.hpp	\
	generic_locking_algorithms.hpp	\
	get_current_process_id.hpp	\
	info.hpp	\
	info_customization.hpp	\
	info_tr.hpp	\
	info_tr_customization.hpp	\
	init_resources.hpp	\
	inttypes_detail.h	\
	inttypes.h	\
	invalid_string_conversion_exception.hpp	\
	ios_failure_exception.hpp	\
	ios.hpp	\
	join.h	\
	locale_to_lang_id_win.hpp	\
	lock_exception.hpp	\
	lockables.hpp	\
	locking_levels.hpp	\
	locks.hpp	\
	logger.hpp	\
	mutex.hpp	\
	name_mangling.hpp	\
	non_copyable.hpp	\
	on_block_exit.hpp	\
	optional.hpp	\
	qiostream.hpp	\
	safe_delete.hpp	\
	safe_free.hpp	\
	semaphore.hpp	\
	singletons.hpp	\
	source_code_info.hpp	\
	stack_frame.hpp	\
	stack_trace.hpp	\
	static_assert.hpp	\
	stdexcept.hpp	\
	stdint_detail.h	\
	stdint.h	\
	stl_explicit_instantiation.hpp	\
	string_conv.hpp	\
	string_utils.hpp	\
	stringizable.hpp	\
	stringize.h	\
	system_exception.hpp	\
	task_canceled_exception.hpp	\
	tchar.h	\
	tchar.hpp	\
	tfstream.hpp	\
	thread_data_types.hpp	\
	thread_data.hpp	\
	thread_exception.hpp	\
	thread_shared_obj_locking_ptr.hpp	\
	thread.hpp	\
	throwable.hpp	\
	time_utils.hpp	\
	tios.hpp	\
	tiostream.hpp	\
	tistream.hpp	\
	tostream.hpp	\
	tsstream.hpp	\
	tstreambuf.hpp	\
	tstring_conv.hpp	\
	tstring.hpp	\
	tstringizable.hpp	\
	tstringize.h	\
	type_traits.hpp	\
	typeinfo.hpp	\
	unknown_exception.hpp	\
	unused.h	\
	unused.hpp	\
	verify.h	\
	version.h	\
	wstringize.h	\
	zeroize.h	\
	zeroize.hpp

unix {
	HEADERS += execinfo_unx.h
}

win32 {
	HEADERS += locale_to_lang_id_win.hpp
}

unix {
	linux* {
	   HEADERS += error_code_lnx.hpp
	}
	else {
		macx|ios {
			HEADERS += error_code_mac.hpp
		}
		else {
			error("Unsupported Unix platform for gcc compiler!!!")
		}
	}
}
else {
	win32 {
		HEADERS += error_code_win.hpp
	}
	else {
		error("Unsupported platform!!!")
	}
}


headers.files = $${HEADERS}
headers.path = $${CPPDEVTK_INCLUDE_DIR}/base
INSTALLS += headers

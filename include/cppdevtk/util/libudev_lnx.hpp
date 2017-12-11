/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file
///
/// \copyright Copyright (C) 2015 - 2018 CoSoSys Ltd <info@cososys.com>\n
/// Licensed under the Apache License, Version 2.0 (the "License");\n
/// you may not use this file except in compliance with the License.\n
/// You may obtain a copy of the License at\n
/// http://www.apache.org/licenses/LICENSE-2.0\n
/// Unless required by applicable law or agreed to in writing, software\n
/// distributed under the License is distributed on an "AS IS" BASIS,\n
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n
/// See the License for the specific language governing permissions and\n
/// limitations under the License.\n
/// Please see the file COPYING.
///
/// \authors Cristian ANITA <cristian.anita@cososys.com>, <cristian_anita@yahoo.com>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef CPPDEVTK_UTIL_LIBUDEV_LNX_HPP_INCLUDED_
#define CPPDEVTK_UTIL_LIBUDEV_LNX_HPP_INCLUDED_


#include "config.hpp"
#if (!CPPDEVTK_PLATFORM_LINUX || CPPDEVTK_PLATFORM_ANDROID)
#	error "This file is Linux specific!!!"
#endif
#include "dynamic_library.hpp"


struct udev;
struct udev_enumerate;
struct udev_list_entry;
struct udev_device;


namespace cppdevtk {
namespace util {


// NOTE: We need to support both libudev0 and libudev1 in the same build (no multiple binaries)
// so we are forced to use run-time dynamic linking instead of load-time dynamic linking.

// TODO: complete to implement udev API

class CPPDEVTK_UTIL_API LibUDev: public DynamicLibrary {
public:
	struct udev* udev_new(void);
	void udev_unref(struct udev* udev);
	
	struct udev_enumerate* udev_enumerate_new(struct udev* udev);
	void udev_enumerate_unref(struct udev_enumerate* udev_enumerate);
	int udev_enumerate_add_match_subsystem(struct udev_enumerate* udev_enumerate, const char* subsystem);
	int udev_enumerate_add_match_property(struct udev_enumerate* udev_enumerate, const char* property, const char* value);
	int udev_enumerate_scan_devices(struct udev_enumerate* udev_enumerate);
	struct udev_list_entry* udev_enumerate_get_list_entry(struct udev_enumerate* udev_enumerate);
	
	const char* udev_list_entry_get_name(struct udev_list_entry* list_entry);
	const char* udev_list_entry_get_value(struct udev_list_entry* list_entry);
	struct udev_list_entry* udev_list_entry_get_next(struct udev_list_entry* list_entry);
	
	struct udev_device* udev_device_new_from_syspath(struct udev* udev, const char* syspath);
	void udev_device_unref(struct udev_device* udev_device);
	const char* udev_device_get_devtype(struct udev_device* udev_device);
	const char* udev_device_get_property_value(struct udev_device* udev_device, const char* key);
	const char* udev_device_get_devnode(struct udev_device* udev_device);
	const char* udev_device_get_devpath(struct udev_device* udev_device);
	const char* udev_device_get_subsystem(struct udev_device* udev_device);
	const char* udev_device_get_syspath(struct udev_device* udev_device);
	const char* udev_device_get_sysname(struct udev_device* udev_device);
	const char* udev_device_get_sysnum(struct udev_device* udev_device);
	struct udev_list_entry* udev_device_get_properties_list_entry(struct udev_device* udev_device);
	struct udev_list_entry* udev_device_get_sysattr_list_entry(struct udev_device* udev_device);
	struct udev_list_entry* udev_device_get_tags_list_entry(struct udev_device* udev_device);
protected:
	LibUDev(const QString& dynLibName);
private:
	typedef struct udev* (*tpfn_udev_new)(void);
	typedef void (*tpfn_udev_unref)(struct udev* udev);
	
	typedef struct udev_enumerate* (*tpfn_udev_enumerate_new)(struct udev* udev);
	typedef void (*tpfn_udev_enumerate_unref)(struct udev_enumerate* udev_enumerate);
	typedef int (*tpfn_udev_enumerate_add_match_subsystem)(struct udev_enumerate* udev_enumerate, const char* subsystem);
	typedef int (*tpfn_udev_enumerate_add_match_property)(struct udev_enumerate* udev_enumerate, const char* property, const char* value);
	typedef int (*tpfn_udev_enumerate_scan_devices)(struct udev_enumerate* udev_enumerate);
	typedef struct udev_list_entry* (*tpfn_udev_enumerate_get_list_entry)(struct udev_enumerate* udev_enumerate);
	
	typedef const char* (*tpfn_udev_list_entry_get_name)(struct udev_list_entry* list_entry);
	typedef const char* (*tpfn_udev_list_entry_get_value)(struct udev_list_entry* list_entry);
	typedef struct udev_list_entry* (*tpfn_udev_list_entry_get_next)(struct udev_list_entry* list_entry);
	
	typedef struct udev_device* (*tpfn_udev_device_new_from_syspath)(struct udev* udev, const char* syspath);
	typedef void (*tpfn_udev_device_unref)(struct udev_device* udev_device);
	typedef const char* (*tpfn_udev_device_get_devtype)(struct udev_device* udev_device);
	typedef const char* (*tpfn_udev_device_get_property_value)(struct udev_device* udev_device, const char* key);
	typedef const char* (*tpfn_udev_device_get_devnode)(struct udev_device* udev_device);
	typedef const char* (*tpfn_udev_device_get_devpath)(struct udev_device* udev_device);
	typedef const char* (*tpfn_udev_device_get_subsystem)(struct udev_device* udev_device);
	typedef const char* (*tpfn_udev_device_get_syspath)(struct udev_device* udev_device);
	typedef const char* (*tpfn_udev_device_get_sysname)(struct udev_device* udev_device);
	typedef const char* (*tpfn_udev_device_get_sysnum)(struct udev_device* udev_device);
	typedef struct udev_list_entry* (*tpfn_udev_device_get_properties_list_entry)(struct udev_device* udev_device);
	typedef struct udev_list_entry* (*tpfn_udev_device_get_sysattr_list_entry)(struct udev_device* udev_device);
	typedef struct udev_list_entry* (*tpfn_udev_device_get_tags_list_entry)(struct udev_device* udev_device);
	
	
	void LoadSymbols();
	
	
	tpfn_udev_new pfn_udev_new_;
	tpfn_udev_unref pfn_udev_unref_;
	
	tpfn_udev_enumerate_new pfn_udev_enumerate_new_;
	tpfn_udev_enumerate_unref pfn_udev_enumerate_unref_;
	tpfn_udev_enumerate_add_match_subsystem pfn_udev_enumerate_add_match_subsystem_;
	tpfn_udev_enumerate_add_match_property pfn_udev_enumerate_add_match_property_;
	tpfn_udev_enumerate_scan_devices pfn_udev_enumerate_scan_devices_;
	tpfn_udev_enumerate_get_list_entry pfn_udev_enumerate_get_list_entry_;
	
	tpfn_udev_list_entry_get_name pfn_udev_list_entry_get_name_;
	tpfn_udev_list_entry_get_value pfn_udev_list_entry_get_value_;
	tpfn_udev_list_entry_get_next pfn_udev_list_entry_get_next_;
	
	tpfn_udev_device_new_from_syspath pfn_udev_device_new_from_syspath_;
	tpfn_udev_device_unref pfn_udev_device_unref_;
	tpfn_udev_device_get_devtype pfn_udev_device_get_devtype_;
	tpfn_udev_device_get_property_value pfn_udev_device_get_property_value_;
	tpfn_udev_device_get_devnode pfn_udev_device_get_devnode_;
	tpfn_udev_device_get_devpath pfn_udev_device_get_devpath_;
	tpfn_udev_device_get_subsystem pfn_udev_device_get_subsystem_;
	tpfn_udev_device_get_syspath pfn_udev_device_get_syspath_;
	tpfn_udev_device_get_sysname pfn_udev_device_get_sysname_;
	tpfn_udev_device_get_sysnum pfn_udev_device_get_sysnum_;
	tpfn_udev_device_get_properties_list_entry pfn_udev_device_get_properties_list_entry_;
	tpfn_udev_device_get_sysattr_list_entry pfn_udev_device_get_sysattr_list_entry_;
	tpfn_udev_device_get_tags_list_entry pfn_udev_device_get_tags_list_entry_;
};


class CPPDEVTK_UTIL_API LibUDev0: public LibUDev {
public:
	LibUDev0();
};


class CPPDEVTK_UTIL_API LibUDev1: public LibUDev {
public:
	LibUDev1();
};




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Inline functions

inline struct udev* LibUDev::udev_new(void) {
	return pfn_udev_new_();
}

inline void LibUDev::udev_unref(struct udev* udev) {
	return pfn_udev_unref_(udev);
}

inline struct udev_enumerate* LibUDev::udev_enumerate_new(struct udev* udev) {
	return pfn_udev_enumerate_new_(udev);
}

inline void LibUDev::udev_enumerate_unref(struct udev_enumerate* udev_enumerate) {
	return pfn_udev_enumerate_unref_(udev_enumerate);
}

inline int LibUDev::udev_enumerate_add_match_subsystem(struct udev_enumerate* udev_enumerate, const char* subsystem) {
	return pfn_udev_enumerate_add_match_subsystem_(udev_enumerate, subsystem);
}

inline int LibUDev::udev_enumerate_add_match_property(struct udev_enumerate* udev_enumerate, const char* property, const char* value) {
	return pfn_udev_enumerate_add_match_property_(udev_enumerate, property, value);
}

inline int LibUDev::udev_enumerate_scan_devices(struct udev_enumerate* udev_enumerate) {
	return pfn_udev_enumerate_scan_devices_(udev_enumerate);
}

inline struct udev_list_entry* LibUDev::udev_enumerate_get_list_entry(struct udev_enumerate* udev_enumerate) {
	return pfn_udev_enumerate_get_list_entry_(udev_enumerate);
}

inline const char* LibUDev::udev_list_entry_get_name(struct udev_list_entry* list_entry) {
	return pfn_udev_list_entry_get_name_(list_entry);
}

inline const char* LibUDev::udev_list_entry_get_value(struct udev_list_entry* list_entry) {
	return pfn_udev_list_entry_get_value_(list_entry);
}

inline struct udev_list_entry* LibUDev::udev_list_entry_get_next(struct udev_list_entry* list_entry) {
	return pfn_udev_list_entry_get_next_(list_entry);
}

inline struct udev_device* LibUDev::udev_device_new_from_syspath(struct udev* udev, const char* syspath) {
	return pfn_udev_device_new_from_syspath_(udev, syspath);
}

inline void LibUDev::udev_device_unref(struct udev_device* udev_device) {
	return pfn_udev_device_unref_(udev_device);
}

inline const char* LibUDev::udev_device_get_devtype(struct udev_device* udev_device) {
	return pfn_udev_device_get_devtype_(udev_device);
}

inline const char* LibUDev::udev_device_get_property_value(struct udev_device* udev_device, const char* key) {
	return pfn_udev_device_get_property_value_(udev_device, key);
}

inline const char* LibUDev::udev_device_get_devnode(struct udev_device* udev_device) {
	return pfn_udev_device_get_devnode_(udev_device);
}

inline const char* LibUDev::udev_device_get_devpath(struct udev_device* udev_device) {
	return pfn_udev_device_get_devpath_(udev_device);
}

inline const char* LibUDev::udev_device_get_subsystem(struct udev_device* udev_device) {
	return pfn_udev_device_get_devpath_(udev_device);
}

inline const char* LibUDev::udev_device_get_syspath(struct udev_device* udev_device) {
	return pfn_udev_device_get_syspath_(udev_device);
}

inline const char* LibUDev::udev_device_get_sysname(struct udev_device* udev_device) {
	return pfn_udev_device_get_sysname_(udev_device);
}

inline const char* LibUDev::udev_device_get_sysnum(struct udev_device* udev_device) {
	return pfn_udev_device_get_sysnum_(udev_device);
}

inline struct udev_list_entry* LibUDev::udev_device_get_properties_list_entry(struct udev_device* udev_device) {
	return pfn_udev_device_get_properties_list_entry_(udev_device);
}

inline struct udev_list_entry* LibUDev::udev_device_get_sysattr_list_entry(struct udev_device* udev_device) {
	return pfn_udev_device_get_sysattr_list_entry_(udev_device);
}

inline struct udev_list_entry* LibUDev::udev_device_get_tags_list_entry(struct udev_device* udev_device) {
	return pfn_udev_device_get_tags_list_entry_(udev_device);
}


inline LibUDev0::LibUDev0(): LibUDev("libudev.so.0") {}


inline LibUDev1::LibUDev1(): LibUDev("libudev.so.1") {}


}	// namespace util
}	// namespace cppdevtk


#endif	// CPPDEVTK_UTIL_LIBUDEV_LNX_HPP_INCLUDED_

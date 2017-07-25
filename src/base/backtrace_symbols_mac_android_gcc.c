// http://opensource.apple.com/source/Libc/Libc-498.1.7/gen/backtrace.c?txt

/*
 * Copyright (c) 2007 Apple Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */


// NOTE:
// - Do not use CPPDEVTK_ASSERT() because it use StackTrace; use assert() instead
// - the original Mac OS X 10.5 implementation of backtrace_symbols() and _backtrace_snprintf() were buggy
// (discovered on Android were segfaults); Fixed!
// - backtrace_symbols_fd() review: at a glance looks OK, but not tested...


#include <cppdevtk/base/execinfo_unx.h>
#if (!CPPDEVTK_COMPILER_GCC)
#	error "This file is gcc specific!!!"
#endif
#if (!(CPPDEVTK_PLATFORM_MACOSX || CPPDEVTK_PLATFORM_ANDROID))
#	error "This file is Mac OS X or Android specific!!!"
#endif
#include <cppdevtk/base/assert.h>
#include <cppdevtk/base/zeroize.h>

#if (!CPPDEVTK_HAVE_BACKTRACE_SYMBOLS)

#include <sys/uio.h>
#include <dlfcn.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>


#if __LP64__
#define _BACKTRACE_FORMAT "%-4d%-35s 0x%016x %s + %u"
#define _BACKTRACE_FORMAT_SIZE 82
#else
#define _BACKTRACE_FORMAT "%-4d%-35s 0x%08x %s + %u"
#define _BACKTRACE_FORMAT_SIZE 65
#endif
// example: 0   cppdevtk_test_base_debug            0x00008cb5 _ZN8cppdevtk4base10StackTrace9DoCaptureEv + 155

#define _BACKTRACE_UINT_MAX_LEN 19


static int _backtrace_snprintf(char* buf, size_t size, int frame, const void* addr, const Dl_info* info) {
	char symbuf[_BACKTRACE_UINT_MAX_LEN] = {'\0'};
	const char* image = "???";
	const char* symbol = symbuf;
	int retCode = -1;
	
	assert(buf != NULL);
	assert(size >= (_BACKTRACE_FORMAT_SIZE + 1));
	assert(addr != NULL);
	assert(info != NULL);
	
	if (info->dli_fname != NULL) {
		image = strrchr(info->dli_fname, '/');
		if (image != NULL) {
			image += 1;
		}
		if (image == NULL) {
			image = info->dli_fname;
		}
	}
	
	if (info->dli_sname != NULL) {
		symbol = info->dli_sname;
	}
	else {
		retCode = snprintf(symbuf, sizeof(symbuf), "0x%x", (unsigned int)info->dli_saddr);
		assert((0 <= retCode) && (retCode < (int)sizeof(symbuf)));
	}
	
	retCode = snprintf(buf, size, _BACKTRACE_FORMAT, frame, image, (unsigned int)addr, symbol,
			(unsigned int)((addr > info->dli_saddr) ? (addr - info->dli_saddr) : 0));
	assert((0 <= retCode) && (retCode < (int)size));
	
	return retCode;
}

CPPDEVTK_BASE_API char** backtrace_symbols(void* const* buffer, int size) {
	int i = 0;
	size_t total_bytes = 0;
	size_t frames_bytes[size];
	char** result = NULL;
	char** ptrs = NULL;
	char* strs = NULL;
	Dl_info* info = NULL;
	
	info = calloc(size, sizeof(Dl_info));
	if (info == NULL) {
		return NULL;
	}
	
	// Compute the total size for the block that is returned.
	// The block will contain size number of pointers to the
	// symbol descriptions.
	
	total_bytes = sizeof(char*) * size;
	
	// Plus each symbol description
	for (i = 0 ; i < size; ++i) {
		size_t frame_bytes = _BACKTRACE_FORMAT_SIZE;
		assert(buffer[i] != NULL);
		if (dladdr(buffer[i], &info[i]) != 0) {
			if (info[i].dli_sname != NULL) {
				frame_bytes += strlen(info[i].dli_sname);
			}
			else {
				frame_bytes += _BACKTRACE_UINT_MAX_LEN;	// make space for "0x%x", (unsigned int)info->dli_saddr
			}
		}
		else {
			frame_bytes += _BACKTRACE_UINT_MAX_LEN;	// make space for "0x%x", (unsigned int)info->dli_saddr
			ZeroizeMemory(&info[i], sizeof(Dl_info));
		}
		frame_bytes += 1;	// NULL terminator
		
		frames_bytes[i] = frame_bytes;
		total_bytes += frame_bytes;
	}
	
	result = (char**)malloc(total_bytes);
	if (result == NULL) {
		free(info);
		return NULL;
	}
	
	// Fill in the array of pointers and append the strings for
	// each symbol description.
	ptrs = result;
	strs = (char*)(((char*)result) + (sizeof(char*) * size));
	for (i = 0; i < size; ++i) {
		ptrs[i] = (char*)strs;
		strs += _backtrace_snprintf((char*)strs, frames_bytes[i], i, buffer[i], &info[i]) + 1;
	}
	
	free(info);
	
	return result;
}

CPPDEVTK_BASE_API void backtrace_symbols_fd(void* const* buffer, int size, int fd) {
	int i;
	char buf[BUFSIZ];
	Dl_info info;
	struct iovec iov[2];
	
	iov[0].iov_base = buf;
	
	iov[1].iov_base = "\n";
	iov[1].iov_len = 1;
	
	for (i = 0; i < size; ++i) {
		memset(&info, 0, sizeof(info));
		dladdr(buffer[i], &info);
		
		iov[0].iov_len = _backtrace_snprintf(buf, sizeof(buf), i, buffer[i], &info) + 1;
		
		writev(fd, iov, 2);
	}
}

#endif	// (!CPPDEVTK_HAVE_BACKTRACE_SYMBOLS)

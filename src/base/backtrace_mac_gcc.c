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


// NOTE: Do not use CPPDEVTK_ASSERT() because it use StackTrace; use assert() instead


#include <cppdevtk/base/execinfo_unx.h>
#if (!CPPDEVTK_COMPILER_GCC)
#	error "This file is gcc specific!!!"
#endif
#if (!CPPDEVTK_PLATFORM_MACOSX)
#	error "This file is Mac OS X specific!!!"
#endif


#if (!CPPDEVTK_HAVE_BACKTRACE)

#include <mach/vm_types.h>
#include <sys/uio.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


CPPDEVTK_BASE_API int backtrace(void** buffer, int size) {
	extern void _thread_stack_pcs(vm_address_t *buffer, unsigned max, unsigned *nb, unsigned skip);
	unsigned int num_frames;
	_thread_stack_pcs((vm_address_t*)buffer, size, &num_frames, 1);
	while (num_frames >= 1 && buffer[num_frames-1] == NULL) num_frames -= 1;
	return num_frames;
}

#endif	// (!CPPDEVTK_HAVE_BACKTRACE)

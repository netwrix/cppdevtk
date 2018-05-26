// https://github.com/ncm/selectable-socketpair


/* socketpair.c
Copyright 2007, 2010 by Nathan C. Myers <ncm@cantrip.org>
Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    Redistributions of source code must retain the above copyright notice, this
    list of conditions and the following disclaimer.

    Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

    The name of the author must not be used to endorse or promote products
    derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef CPPDEVTK_BASE_SOCKET_PAIR_HPP_INCLUDED_
#define CPPDEVTK_BASE_SOCKET_PAIR_HPP_INCLUDED_


#include "config.hpp"
#if (CPPDEVTK_PLATFORM_WINDOWS)
#	include <windows.h>
#	include <winsock2.h>
#endif


namespace cppdevtk {
namespace base {


#if (CPPDEVTK_PLATFORM_UNIX)
typedef int socket_t;
static const socket_t kInvalidSocket = -1;
#elif (CPPDEVTK_PLATFORM_WINDOWS)
typedef SOCKET socket_t;
static const socket_t kInvalidSocket = INVALID_SOCKET;
#else
#	error "Unsupported platform!!!"
#endif


/// If makeOverlapped is nonzero, both sockets created will be usable for
/// "overlapped" operations via WSASend etc.  If make_overlapped is zero,
/// socks[0] (only) will be usable with regular ReadFile etc., and thus 
/// suitable for use as stdin or stdout of a child process.  Note that the
/// sockets must be closed with closesocket() regardless.
/// \return 0 on success
CPPDEVTK_BASE_API int SocketPair(socket_t socketVector[2], bool makeOverlapped = true);


}	// namespace base
}	// namespace cppdevtk


#endif	// CPPDEVTK_BASE_SOCKET_PAIR_HPP_INCLUDED_

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

/* Changes:
 * 2014-02-12: merge David Woodhouse, Ger Hobbelt improvements
 *     git.infradead.org/users/dwmw2/openconnect.git/commitdiff/bdeefa54
 *     github.com/GerHobbelt/selectable-socketpair
 *   always init the socks[] to -1/INVALID_SOCKET on error, both on Win32/64
 *   and UNIX/other platforms
 * 2013-07-18: Change to BSD 3-clause license
 * 2010-03-31:
 *   set addr to 127.0.0.1 because win32 getsockname does not always set it.
 * 2010-02-25:
 *   set SO_REUSEADDR option to avoid leaking some windows resource.
 *   Windows System Error 10049, "Event ID 4226 TCP/IP has reached
 *   the security limit imposed on the number of concurrent TCP connect
 *   attempts."  Bleah.
 * 2007-04-25:
 *   preserve value of WSAGetLastError() on all error returns.
 * 2007-04-22:  (Thanks to Matthew Gregan <kinetik@flim.org>)
 *   s/EINVAL/WSAEINVAL/ fix trivial compile failure
 *   s/socket/WSASocket/ enable creation of sockets suitable as stdin/stdout
 *     of a child process.
 *   add argument make_overlapped
 */


#include <cppdevtk/base/socket_pair.hpp>
#if (!CPPDEVTK_PLATFORM_UNIX)
#	error "This file is Unix specific!!!"
#endif
#include <cppdevtk/base/cerrno.hpp>
#include <cppdevtk/base/dbc.hpp>
#include <cppdevtk/base/logger.hpp>
#include <cppdevtk/base/system_exception.hpp>
#include <cppdevtk/base/unused.hpp>

#include <cstddef>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>


namespace cppdevtk {
namespace base {


CPPDEVTK_BASE_API int SocketPair(socket_t socketVector[2], bool makeOverlapped) {
	CPPDEVTK_DBC_CHECK_NON_NULL_ARGUMENT(socketVector);
	
	SuppressUnusedWarning(makeOverlapped);
	
    if (socketVector == NULL) {
        errno = EINVAL;
        return -1;
    }
	
    int retCode = socketpair(AF_LOCAL, SOCK_STREAM, 0, socketVector);
	if (retCode != ESUCCESS) {
		CPPDEVTK_LOG_ERROR("socketpair() failed; error code: " << GetLastSystemErrorCode().ToString());
        socketVector[0] = socketVector[1] = -1;
	}
	
    return retCode;
}


}	// namespace base
}	// namespace cppdevtk

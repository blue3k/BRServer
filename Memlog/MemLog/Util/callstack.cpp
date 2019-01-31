////////////////////////////////////////////////////////////////////////////////
//  $Id: callstack.cpp,v 1.20 2006/11/18 03:12:34 dmouldin Exp $
//
//  Visual Leak Detector - CallStack Class Implementations
//  Copyright (c) 2005-2006 Dan Moulding
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
//  See COPYING.txt for the full terms of the GNU Lesser General Public License.
//
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"


#include <cassert>
#include <windows.h>
#include <intrin.h>
//#define __out_xcount(x) // Workaround for the specstrings.h bug in the Platform SDK.
//#define DBGHELP_TRANSLATE_TCHAR
#include <dbghelp.h>    // Provides symbol handling services.
//#define VLDBUILD
#include "callstack.h"  // This class' header.
#include "utility.h"    // Provides various utility functions.


#define MAXSYMBOLNAMELENGTH 512



static LPSTR buildsymbolsearchpath ();

#pragma intrinsic (_InterlockedIncrement, _InterlockedDecrement)


namespace CallStackSystem
{
	LONG lLockCount = 0;
	volatile bool bIsInitialized = false;

	// Global variables.
	HANDLE           currentprocess = NULL; // Pseudo-handle for the current process.
	CRITICAL_SECTION stackwalklock;  // Serializes calls to StackWalk64 from the Debug Help Library.
	CRITICAL_SECTION symbollock;     // Serializes calls to the Debug Help Library symbold handling APIs.
	CRITICAL_SECTION imagelock;


	bool Initialize();
	void Terminate();

};


#pragma comment(lib, "Dbghelp.lib")



bool CallStackSystem::Initialize()
{
	LONG lCurLockCount = _InterlockedIncrement( &lLockCount );

	if( lCurLockCount == 1 ) // first time initialize
	{
		InitializeCriticalSection( &symbollock );
		InitializeCriticalSection( &stackwalklock );
		InitializeCriticalSection( &imagelock );

		bIsInitialized = true;
	}
	else
	{
		while( !bIsInitialized )
		{
			SleepEx( 0, FALSE );
		}

	}


	EnterCriticalSection( &symbollock );

	currentprocess    = GetCurrentProcess();

	// Initialize the symbol handler. We use it for obtaining source file/line
	// number information and function names for the memory leak report.
	LPSTR symbolpath = buildsymbolsearchpath();
	SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_UNDNAME);
	if (!SymInitialize(currentprocess, symbolpath, FALSE)) {
		report(L"WARNING: The symbol handler failed to initialize (error=%lu).\n"
			   L"    File and function names will probably not be available in call stacks.\n", GetLastError());
	}

	ModuleSet *newmodules = new ModuleSet;
//	newmodules->reserve(MODULESETRESERVE);
	if( !EnumerateLoadedModules64( currentprocess, (PENUMLOADED_MODULES_CALLBACK64)addloadedmodule, newmodules ) )
	{
		report( L"Module enumeration failed, error %ud", GetLastError() );
	}
	delete [] symbolpath;

	attachtoloadedmodules( newmodules );

	ModuleSet::iterator   iterMod;
	for ( iterMod = newmodules->begin(); iterMod != newmodules->end(); ++iterMod)
	{
		delete[] iterMod->name;
		delete[] iterMod->path;
	}
	delete newmodules;

	LeaveCriticalSection(&symbollock);


	return true;;
}


//bool g_bInitialized = Initialize();


void CallStackSystem::Terminate()
{
	LONG lCurLockCount = _InterlockedDecrement( &lLockCount );

	if( lCurLockCount == 0 )
	{
		DeleteCriticalSection( &stackwalklock );
		DeleteCriticalSection( &symbollock );
		DeleteCriticalSection( &imagelock );
	}
}



// buildsymbolsearchpath - Builds the symbol search path for the symbol handler.
//   This helps the symbol handler find the symbols for the application being
//   debugged.
//
//  Return Value:
//
//    Returns a string containing the search path. The caller is responsible for
//    freeing the string.
//
static LPSTR buildsymbolsearchpath ()
{
    CHAR   directory [_MAX_DIR];
    CHAR   drive [_MAX_DRIVE];
    LPSTR  env;
    DWORD   envlen;
    SIZE_T  index;
    SIZE_T  length;
    HMODULE module;
    LPSTR  path = new CHAR [MAX_PATH];
    SIZE_T  pos = 0;
    CHAR   system [MAX_PATH];
    CHAR   windows [MAX_PATH];

    // Oddly, the symbol handler ignores the link to the PDB embedded in the
    // executable image. So, we'll manually add the location of the executable
    // to the search path since that is often where the PDB will be located.
    path[0] = '\0';
    module = GetModuleHandle(NULL);
    GetModuleFileNameA(module, path, MAX_PATH);
    _splitpath_s(path, drive, _MAX_DRIVE, directory, _MAX_DIR, NULL, 0, NULL, 0);
    strncpy_s(path, MAX_PATH, drive, _TRUNCATE);
    strapp(&path, directory);

    // When the symbol handler is given a custom symbol search path, it will no
    // longer search the default directories (working directory, system root,
    // etc). But we'd like it to still search those directories, so we'll add
    // them to our custom search path.
    //
    // Append the working directory.
    strapp(&path, ";.\\");

    // Append the Windows directory.
    if (GetWindowsDirectoryA(windows, MAX_PATH) != 0) {
        strapp(&path, ";");
        strapp(&path, windows);
    }

    // Append the system directory.
    if (GetSystemDirectoryA(system, MAX_PATH) != 0) {
        strapp(&path, ";");
        strapp(&path, system);
    }

    // Append %_NT_SYMBOL_PATH%.
    envlen = GetEnvironmentVariableA("_NT_SYMBOL_PATH", NULL, 0);
    if (envlen != 0) {
        env = new CHAR [envlen];
        if (GetEnvironmentVariableA("_NT_SYMBOL_PATH", env, envlen) != 0) {
            strapp(&path, ";");
            strapp(&path, env);
        }
        delete [] env;
    }

    //  Append %_NT_ALT_SYMBOL_PATH%.
    envlen = GetEnvironmentVariableA("_NT_ALT_SYMBOL_PATH", NULL, 0);
    if (envlen != 0) {
        env = new CHAR [envlen];
        if (GetEnvironmentVariableA("_NT_ALT_SYMBOL_PATH", env, envlen) != 0) {
            strapp(&path, ";");
            strapp(&path, env);
        }
        delete [] env;
    }

    //  Append %MEMUTIL_SYMPATH%.
    envlen = GetEnvironmentVariableA("MEMUTIL_SYMPATH", NULL, 0);
    if (envlen != 0) {
        env = new CHAR [envlen];
        if (GetEnvironmentVariableA("MEMUTIL_SYMPATH", env, envlen) != 0) {
            strapp(&path, ";");
            strapp(&path, env);
        }
        delete [] env;
    }

    // Remove any quotes from the path. The symbol handler doesn't like them.
    pos = 0;
    length = strlen(path);
    while (pos < length) {
        if (path[pos] == '\"') {
            for (index = pos; index < length; index++) {
                path[index] = path[index + 1];
            }
        }
        pos++;
    }

    return path;
}


// Constructor - Initializes the CallStack with an initial size of zero and one
//   Chunk of capacity.
//
CallStack::CallStack ()
{
	//if( !g_bInitialized )
	//	Initialize();

    m_capacity   = CALLSTACKCHUNKSIZE;
    m_size       = 0;
    m_status     = 0x0;
    m_store.next = NULL;
    m_topchunk   = &m_store;
    m_topindex   = 0;
}

// Copy Constructor - For efficiency, we want to avoid ever making copies of
//   CallStacks (only pointer passing or reference passing should be performed).
//   The sole purpose of this copy constructor is to ensure that no copying is
//   being done inadvertently.
//
CallStack::CallStack (const CallStack &)
{
    // Don't make copies of CallStacks!
    assert(FALSE);
}

// Destructor - Frees all memory allocated to the CallStack.
//
CallStack::~CallStack ()
{
    CallStack::chunk_t *chunk = m_store.next;
    CallStack::chunk_t *temp;

    while (chunk) {
        temp = chunk;
        chunk = temp->next;
        delete temp;
    }
}

// operator = - Assignment operator. For efficiency, we want to avoid ever
//   making copies of CallStacks (only pointer passing or reference passing
//   should be performed). The sole purpose of this assignment operator is to
//   ensure that no copying is being done inadvertently.
//
CallStack& CallStack::operator = (const CallStack &)
{
    // Don't make copies of CallStacks!
    assert(FALSE);
    return *this;
}

// operator == - Equality operator. Compares the CallStack to another CallStack
//   for equality. Two CallStacks are equal if they are the same size and if
//   every frame in each is identical to the corresponding frame in the other.
//
//  other (IN) - Reference to the CallStack to compare the current CallStack
//    against for equality.
//
//  Return Value:
//
//    Returns true if the two CallStacks are equal. Otherwise returns false.
//
BOOL CallStack::operator == (const CallStack &other) const
{
    const CallStack::chunk_t *chunk = &m_store;
    UINT32                    index;
    const CallStack::chunk_t *otherchunk = &other.m_store;
    const CallStack::chunk_t *prevchunk = NULL;

    if (m_size != other.m_size) {
        // They can't be equal if the sizes are different.
        return FALSE;
    }

    // Walk the chunk list and within each chunk walk the frames array until we
    // either find a mismatch, or until we reach the end of the call stacks.
    while (prevchunk != m_topchunk) {
        for (index = 0; index < ((chunk == m_topchunk) ? m_topindex : CALLSTACKCHUNKSIZE); index++) {
            if (chunk->frames[index] != otherchunk->frames[index]) {
                // Found a mismatch. They are not equal.
                return FALSE;
            }
        }
        prevchunk = chunk;
        chunk = chunk->next;
        otherchunk = otherchunk->next;
    }

    // Reached the end of the call stacks. They are equal.
    return TRUE;
}

// operator [] - Random access operator. Retrieves the frame at the specified
//   index.
//
//   Note: We give up a bit of efficiency here, in favor of efficiency of push
//     operations. This is because walking of a CallStack is done infrequently
//     (only if a leak is found), whereas pushing is done very frequently (for
//     each frame in the program's call stack when the program allocates some
//     memory).
//
//  - index (IN): Specifies the index of the frame to retrieve.
//
//  Return Value:
//
//    Returns the program counter for the frame at the specified index. If the
//    specified index is out of range for the CallStack, the return value is
//    undefined.
//
SIZE_T CallStack::operator [] (UINT32 index) const
{
    UINT32                    count;
    const CallStack::chunk_t *chunk = &m_store;
    UINT32                    chunknumber = index / CALLSTACKCHUNKSIZE;

    for (count = 0; count < chunknumber; count++) {
        chunk = chunk->next;
    }

    return chunk->frames[index % CALLSTACKCHUNKSIZE];
}

// clear - Resets the CallStack, returning it to a state where no frames have
//   been pushed onto it, readying it for reuse.
//
//   Note: Calling this function does not release any memory allocated to the
//     CallStack. We give up a bit of memory-usage efficiency here in favor of
//     performance of push operations.
//
//  Return Value:
//
//    None.
//
VOID CallStack::clear ()
{
    m_size     = 0;
    m_topchunk = &m_store;
    m_topindex = 0;
}

// dump - Dumps a nicely formatted rendition of the CallStack, including
//   symbolic information (function names and line numbers) if available.
//
//   Note: The symbol handler must be initialized prior to calling this
//     function.
//
//  - showinternalframes (IN): If true, then all frames in the CallStack will be
//      dumped. Otherwise, frames internal to the heap will not be dumped.
//
//  Return Value:
//
//    None.
//
VOID CallStack::dump (BOOL showinternalframes) const
{
    DWORD            displacement;
    DWORD64          displacement64;
    BOOL             foundline;
    UINT32           frame;
    SYMBOL_INFO     *functioninfo;
    LPCSTR           functionname;
    SIZE_T           programcounter;
    IMAGEHLP_LINE64  sourceinfo = { 0 };
    BYTE             symbolbuffer [sizeof(SYMBOL_INFO) + (MAXSYMBOLNAMELENGTH * sizeof(WCHAR)) - 1] = { 0 };

    //if (m_status & CALLSTACK_STATUS_INCOMPLETE) {
    //    // This call stack appears to be incomplete. Using StackWalk64 may be
    //    // more reliable.
    //    report(L"    HINT: The following call stack may be incomplete. Setting \"StackWalkMethod\"\n"
    //           L"      in the vld.ini file to \"safe\" instead of \"fast\" may result in a more\n"
    //           L"      complete stack trace.\n");
    //}

	report( "\n\tObject Leak!\n" );

    // Initialize structures passed to the symbol handler.
    functioninfo = (SYMBOL_INFO*)&symbolbuffer;
    functioninfo->SizeOfStruct = sizeof(SYMBOL_INFO);
    functioninfo->MaxNameLen = MAXSYMBOLNAMELENGTH;
    sourceinfo.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

    // Iterate through each frame in the call stack.
    for (frame = 0; frame < m_size; frame++) {
        // Try to get the source file and line number associated with
        // this program counter address.
        programcounter = (*this)[frame];
		EnterCriticalSection(&CallStackSystem::symbollock);
        if ((foundline = SymGetLineFromAddr64( CallStackSystem::currentprocess, programcounter, &displacement, &sourceinfo )) == TRUE) {
            if (!showinternalframes) {
                _strlwr(sourceinfo.FileName );
                if (strstr(sourceinfo.FileName, "afxmem.cpp") ||
                    strstr(sourceinfo.FileName, "dbgheap.c") ||
                    strstr(sourceinfo.FileName, "malloc.c") ||
                    strstr(sourceinfo.FileName, "new.cpp") ||
                    strstr(sourceinfo.FileName, "newaop.cpp") || 
                    strstr(sourceinfo.FileName, "memlog.cpp") || 
                    strstr(sourceinfo.FileName, "memory.cpp") || 
                    strstr(sourceinfo.FileName, "gtest") || 
                    strstr(sourceinfo.FileName, "callstack.cpp")) {
                   // Don't show frames in files internal to the heap.
                    continue;
                }
            }
        }
        else {
			//DWORD dwError = GetLastError();
			//TCHAR strMsgBuf[1024];
			//FormatMessage(
			//	FORMAT_MESSAGE_FROM_SYSTEM,
			//	NULL,
			//	dwError,
			//	MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			//	strMsgBuf,
			//	1024, NULL );


			//report( "SymbolQuery error %d : %s\n", dwError, strMsgBuf );
        }

        // Try to get the name of the function containing this program
        // counter address.
        if (SymFromAddr( CallStackSystem::currentprocess, (*this)[frame], &displacement64, functioninfo ) ) {
            functionname = functioninfo->Name;
        }
        else {
            functionname = "(Function name unavailable)";
        }
        LeaveCriticalSection(&CallStackSystem::symbollock);

        // Display the current stack frame's information.
        if (foundline) {
            report("    %s (%d): %s\n", sourceinfo.FileName, sourceinfo.LineNumber, functionname);
        }
        else {
            report( "    " ADDRESSFORMATA " (File and line number not available): ", (*this)[frame]);
            report( "%s\n", functionname);
        }
    }
}

// push_back - Pushes a frame's program counter onto the CallStack. Pushes are
//   always appended to the back of the chunk list (aka the "top" chunk).
//
//   Note: This function will allocate additional memory as necessary to make
//     room for new program counter addresses.
//
//  - programcounter (IN): The program counter address of the frame to be pushed
//      onto the CallStack.
//
//  Return Value:
//
//    None.
//
VOID CallStack::push_back (const SIZE_T programcounter)
{
    CallStack::chunk_t *chunk;

    if (m_size == m_capacity) {
        // At current capacity. Allocate additional storage.
        chunk = new CallStack::chunk_t;
        chunk->next = NULL;
        m_topchunk->next = chunk;
        m_topchunk = chunk;
        m_topindex = 0;
        m_capacity += CALLSTACKCHUNKSIZE;
    }
    else if (m_topindex == CALLSTACKCHUNKSIZE) {
        // There is more capacity, but not in this chunk. Go to the next chunk.
        // Note that this only happens if this CallStack has previously been
        // cleared (clearing resets the data, but doesn't give up any allocated
        // space).
        m_topchunk = m_topchunk->next;
        m_topindex = 0;
    }

    m_topchunk->frames[m_topindex++] = programcounter;
    m_size++;
}

// getstacktrace - Traces the stack as far back as possible, or until 'maxdepth'
//   frames have been traced. Populates the CallStack with one entry for each
//   stack frame traced.
//
//   Note: This function uses a very efficient method to walk the stack from
//     frame to frame, so it is quite fast. However, unconventional stack frames
//     (such as those created when frame pointer omission optimization is used)
//     will not be successfully walked by this function and will cause the
//     stack trace to terminate prematurely.
//
//  - maxdepth (IN): Maximum number of frames to trace back.
//
//  - framepointer (IN): Frame (base) pointer at which to begin the stack trace.
//      If NULL, then the stack trace will begin at this function.
//
//  Return Value:
//
//    None.
//
VOID FastCallStack::getstacktrace (UINT32 maxdepth, SIZE_T *framepointer)
{
    UINT32  count = 0;
	// STACKFRAME64 frame;

	GETSTACKFRAME( frame );


    //if (framepointer == NULL) {
    //    // Begin the stack trace with the current frame. Obtain the current
    //    // frame pointer.
    //    FRAMEPOINTER(framepointer);
    //}

	framepointer = (SIZE_T*)frame.AddrPC.Offset;

    while (count < maxdepth) {
        if ((SIZE_T*)*framepointer < framepointer) {
            if ((SIZE_T*)*framepointer == NULL) {
                // Looks like we reached the end of the stack.
                break;
            }
            else {
                // Invalid frame pointer. Frame pointer addresses should always
                // increase as we move up the stack.
                m_status |= CALLSTACK_STATUS_INCOMPLETE;
                break;
            }
        }
        if ((SIZE_T)*framepointer & (sizeof(SIZE_T*) - 1)) {
            // Invalid frame pointer. Frame pointer addresses should always
            // be aligned to the size of a pointer. This probably means that
            // we've encountered a frame that was created by a module built with
            // frame pointer omission (FPO) optimization turned on.
            m_status |= CALLSTACK_STATUS_INCOMPLETE;
            break;
        }
        if (IsBadReadPtr((SIZE_T*)*framepointer, sizeof(SIZE_T*))) {
            // Bogus frame pointer. Again, this probably means that we've
            // encountered a frame built with FPO optimization.
            m_status |= CALLSTACK_STATUS_INCOMPLETE;
            break;
        }
        count++;
        push_back(*(framepointer + 1));
        framepointer = (SIZE_T*)*framepointer;
    }
}

// getstacktrace - Traces the stack as far back as possible, or until 'maxdepth'
//   frames have been traced. Populates the CallStack with one entry for each
//   stack frame traced.
//
//   Note: This function uses a documented Windows API to walk the stack. This
//     API is supposed to be the most reliable way to walk the stack. It claims
//     to be able to walk stack frames that do not follow the conventional stack
//     frame layout. However, this robustness comes at a cost: it is *extremely*
//     slow compared to walking frames by following frame (base) pointers.
//
//  - maxdepth (IN): Maximum number of frames to trace back.
//
//  - framepointer (IN): Frame (base) pointer at which to begin the stack trace.
//      If NULL, then the stack trace will begin at this function.
//
//  Return Value:
//
//    None.
//
VOID SafeCallStack::getstacktrace (UINT32 maxdepth, SIZE_T *framepointer)
{
    DWORD        architecture;
//    CONTEXT      context;
    UINT32       count = 0;
   // STACKFRAME64 frame;
//    SIZE_T       programcounter;
//    SIZE_T       stackpointer;
//
//    if (framepointer == NULL) {
//        // Begin the stack trace with the current frame. Obtain the current
//        // frame pointer.
//        FRAMEPOINTER(framepointer);
//    }
//
//    // Get the required values for initialization of the STACKFRAME64 structure
//    // to be passed to StackWalk64(). Required fields are AddrPC and AddrFrame.
//#if defined(_M_IX86) || defined(_M_X64)
//    architecture   = X86X64ARCHITECTURE;
//    programcounter = *(framepointer + 1);
//    stackpointer   = *framepointer;  // An approximation.
//    context.BPREG  = *framepointer;
//    context.IPREG  = programcounter;
//    context.SPREG  = stackpointer;
//#else
//// If you want to retarget Visual Leak Detector to another processor
//// architecture then you'll need to provide architecture-specific code to
//// obtain the program counter and stack pointer from the given frame pointer.
//#error "Not supported on this architecture."
//#endif // _M_IX86 || _M_X64
//
    //// Initialize the STACKFRAME64 structure.
    //memset(&frame, 0x0, sizeof(frame));
    //frame.AddrFrame.Offset = *framepointer;
    //frame.AddrFrame.Mode   = AddrModeFlat;
    //frame.AddrPC.Offset    = programcounter;
    //frame.AddrPC.Mode      = AddrModeFlat;
    //frame.AddrStack.Offset = stackpointer;
    //frame.AddrStack.Mode   = AddrModeFlat;

	architecture   = X86X64ARCHITECTURE;
	GETSTACKFRAME( frame );

	HANDLE currentthread     = GetCurrentThread();

    // Walk the stack.
    EnterCriticalSection(&CallStackSystem::stackwalklock);
    while (count < maxdepth) {
        count++;
		//memset( &context, 0, sizeof(context) );
        if (!StackWalk64(architecture, CallStackSystem::currentprocess, currentthread, &frame, &ctx, NULL,
                         SymFunctionTableAccess64, SymGetModuleBase64, NULL)) {
            // Couldn't trace back through any more frames.
            break;
        }
        if (frame.AddrFrame.Offset == 0) {
            // End of stack.
            break;
        }

        // Push this frame's program counter onto the CallStack.
        push_back((SIZE_T)frame.AddrPC.Offset);

#ifdef _DEBUG
		DWORD            displacement;
		DWORD64          displacement64;
		SYMBOL_INFO     *functioninfo;
		LPCSTR           functionname;
		IMAGEHLP_LINE64  sourceinfo = { 0 };
		BYTE             symbolbuffer [sizeof(SYMBOL_INFO) + (MAXSYMBOLNAMELENGTH * sizeof(WCHAR)) - 1] = { 0 };

		functioninfo = (SYMBOL_INFO*)&symbolbuffer;
		functioninfo->SizeOfStruct = sizeof(SYMBOL_INFO);
		functioninfo->MaxNameLen = MAXSYMBOLNAMELENGTH;
		sourceinfo.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

		BOOL             foundline;
		if ((foundline = SymGetLineFromAddr64( CallStackSystem::currentprocess, frame.AddrPC.Offset, &displacement, &sourceinfo )) == TRUE) {
			strlwr(sourceinfo.FileName );
			if (strstr(sourceinfo.FileName, "afxmem.cpp") ||
				strstr(sourceinfo.FileName, "dbgheap.c") ||
				strstr(sourceinfo.FileName, "malloc.c") ||
				strstr(sourceinfo.FileName, "new.cpp") ||
				strstr(sourceinfo.FileName, "newaop.cpp")) {
				// Don't show frames in files internal to the heap.
				continue;
			}
        }
        else {
			DWORD dwError = GetLastError();
			TCHAR strMsgBuf[1024];
			FormatMessage(
				FORMAT_MESSAGE_FROM_SYSTEM,
				NULL,
				dwError,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				strMsgBuf,
				1024, NULL );

			report( "SymbolQuery error %d : %s\n", dwError, strMsgBuf );
        }

        // Try to get the name of the function containing this program
        // counter address.
        if (SymFromAddr( CallStackSystem::currentprocess, frame.AddrPC.Offset, &displacement64, functioninfo ) ) {
            functionname = functioninfo->Name;
        }
        else {
            functionname = "(Function name unavailable)";
        }
		//report( "Symbol queryd %s\n", functionname );
#endif
    }
    LeaveCriticalSection(&CallStackSystem::stackwalklock);

	//dump( true );
}

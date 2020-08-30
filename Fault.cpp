#include "Fault.h"
#include <assert.h>
#include <iostream>

using std::cout;
using std::endl;

#if WIN32
#include "windows.h"
#endif

//----------------------------------------------------------------------------
// FaultHandler
//----------------------------------------------------------------------------
void
FaultHandler(const char *file, unsigned short line) {
#if WIN32
    // If you hit this line, it means one of the ASSERT macros failed.
    DebugBreak();
#endif
    std::cout << RED << "Error in :" << file << "  at line " << line << RESET
              << std::endl;
    assert(0);
}
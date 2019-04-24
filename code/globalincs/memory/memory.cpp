
#include <core/pstypes.h>
#include "core/error.h"
#include "memory.h"
namespace memory {
const quiet_alloc_t quiet_alloc;
void out_of_memory() {
	core::mprintf(("Memory allocation failed!!!!!!!!!!!!!!!!!!!\n"));
	core::Error(LOCATION, "Out of memory.  Try closing down other applications, increasing your\n"
		"virtual memory size, or installing more physical RAM.\n");
}
}

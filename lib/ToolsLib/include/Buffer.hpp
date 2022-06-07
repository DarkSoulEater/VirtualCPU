#ifndef TOOLSLIB_SRC_TOOLS_BUFFER
#define TOOLSLIB_SRC_TOOLS_BUFFER

#include <ctype.h>

struct Buffer;

Buffer* AllocateBuffer(size_t size);

void DeallocateBuffer(Buffer **buffer);

Buffer* CreateBufferFromFile(const char *fname);

void* BufferGetData(Buffer *buffer);

size_t BufferGetSize(Buffer *buffer);

#endif // TOOLSLIB_SRC_TOOLS_BUFFER
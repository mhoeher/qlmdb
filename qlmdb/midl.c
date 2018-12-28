// On Windows, this ensures we use ANSI versions of Windows API
// calls:
#ifdef UNICODE
#undef UNICODE
#endif

#include "../3rdparty/liblmdb/midl.c"

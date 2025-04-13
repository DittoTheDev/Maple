#include "platform.h"
#if defined(_WIN32) || defined(_WIN64)
const char* get_os_name() { return "Windows"; }
#else
const char* get_os_name() { return "Linux/Unix"; }
#endif

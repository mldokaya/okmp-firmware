#ifndef OKMP_ASSERT

#ifndef ASSERT
    #include "FreeRTOS.h"
    #include "task.h"
    #include "FreeRTOSConfig.h"
    #define ASSERT(x) configASSERT(x)
#endif

#define OKMP_ASSERT
#endif
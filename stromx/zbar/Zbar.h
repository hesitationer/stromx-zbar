#ifndef STROMX_ZBAR_ZBAR_H
#define STROMX_ZBAR_ZBAR_H

#include "stromx/zbar/Config.h"

namespace stromx
{
    namespace runtime
    {
        class Registry;
    }
}

extern "C"
{
    STROMX_ZBAR_API int stromxRegisterZbar(stromx::runtime::Registry* registry);
}

#endif // STROMX_ZBAR_ZBAR_H

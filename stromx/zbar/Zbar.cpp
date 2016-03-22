#include "stromx/zbar/Zbar.h"

#include <iostream>
#include <stromx/runtime/Exception.h>
#include <stromx/runtime/Locale.h>
#include <stromx/runtime/Registry.h>

#include "stromx/zbar/Scan.h"

namespace stromx
{
    namespace zbar
    {
        std::locale locale;
    }
}

STROMX_ZBAR_API int stromxRegisterZbar(stromx::runtime::Registry* registry)
{
    using namespace stromx::zbar;
    
    locale = stromx::runtime::Locale::generate(STROMX_ZBAR_LOCALE_DIR, STROMX_ZBAR_LOCALE_DOMAIN);
    
    try
    {
        registry->registerOperator(new Scan);    
    }
    catch(stromx::runtime::Exception & e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}

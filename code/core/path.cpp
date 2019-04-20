
#include "path.h"
#include "platform.h"
#include <string>
namespace core
{
    namespace path
    {
        const char* clean_filename(const char* filename)
        {
            auto separator = strrchr(filename, DIR_SEPARATOR_CHAR);
            if (separator != nullptr) {
                filename = separator + 1;
            }

            return filename;
        }
    }
}

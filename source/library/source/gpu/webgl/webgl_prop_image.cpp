#include "include/hive.h"

#include <cstring>
#include <vector>

namespace hive
{
    const static int MANDATORY_BORDER_SIZE = 0;

    bool ImageProp::uploadToVRAM()
    {

        if (!IS_IN_VRAM() && data.size() > 0) {

            print data.size();

            glGenTextures(1, &gpu_handle);

            glBindTexture(GL_TEXTURE_2D, gpu_handle);


            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, MANDATORY_BORDER_SIZE, GL_RGB,
                         GL_UNSIGNED_BYTE, data.data());

            // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            //    if (USE_MIP) glGenerateMipmap(GL_TEXTURE_2D);
            //
            //    glBindTexture(GL_TEXTURE_2D, 0);

            return true;
        } else
            return true;
        return false;
    };
} // namespace hive
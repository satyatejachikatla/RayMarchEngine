#pragma once

#include <stdexcept>
#include <sstream>

void GLClearError();
bool GLLogCall();

#define ASSERT(x)                                                                      \
    if (!(x)) {                                                                        \
            std::stringstream err;                                                          \
            err << "GL Call Fail at file : " << __FILE__ << " at line : " <<  __LINE__;\
            std::runtime_error(err.str().c_str());                                          \
    }


#define glCall(call)                                              \
    {                                                             \
        GLClearError();                                           \
        call;                                                     \
        ASSERT(GLLogCall());                                      \
    }


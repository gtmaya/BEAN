#ifndef OBJHANDLER_H
#define OBJHANDLER_H

#include <ngl/Obj.h>
#include "shaderhandler.h"

struct ObjHandler
{
    ObjHandler();
    ~ObjHandler();
    ngl::Obj* m_mesh;
    ShaderProps m_shaderProps;
    glm::vec3 m_position;
};


#endif

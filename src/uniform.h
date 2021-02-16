#ifndef UNIFORM_H
#define UNIFORM_H

#include <gl/glew.h>
#include <glm/glm.hpp>

namespace Uniform
{
    void setValue(GLuint id, const float& x, const float& y, const float& z);
    void setValue(GLuint id, const glm::vec3& value);
    void setValue(GLuint id, const glm::mat4& value);
}

#endif // !UNIFORM_H

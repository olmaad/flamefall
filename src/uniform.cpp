#include "uniform.h"

namespace Uniform
{
    void setValue(GLuint id, const float& x, const float& y, const float& z)
    {
        glUniform3f(id, x, y, z);
    }

    void setValue(GLuint id, const glm::vec3& value)
    {
        glUniform3f(id, value.x, value.y, value.z);
    }

    void setValue(GLuint id, const glm::mat4& value)
    {
        glUniformMatrix4fv(id, 1, GL_FALSE, &value[0][0]);
    }
}
#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <vector>
#include <glm/glm.hpp>

class Scene
{
public:

    const std::vector<glm::vec3>& getVertices() const;
    const std::vector<glm::vec3>& getNormals() const;
    const std::vector<unsigned int>& getIndices() const;

    static Scene import(const std::string& file);

private:
    std::vector<glm::vec3> m_vertices;
    std::vector<glm::vec3> m_normals;
    std::vector<unsigned int> m_indices;

};

#endif // !SCENE_H

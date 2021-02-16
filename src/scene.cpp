#include "scene.h"

#include <algorithm>
#include <iterator>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>  

const std::vector<glm::vec3>& Scene::getVertices() const
{
    return m_vertices;
}

const std::vector<glm::vec3>& Scene::getNormals() const
{
    return m_normals;
}

const std::vector<unsigned int>& Scene::getIndices() const
{
    return m_indices;
}

Scene Scene::import(const std::string& file)
{
    Assimp::Importer ai_importer;

    //const aiScene* scene = model_importer.ReadFile("Asteroid_1_LOW_MODEL_.obj",
        //	aiProcess_CalcTangentSpace |
        //	aiProcess_Triangulate |
        //	aiProcess_JoinIdenticalVertices |
        //	aiProcess_SortByPType);

        //const aiScene* scene = model_importer.ReadFile("flamefall.obj",
        //    aiProcess_CalcTangentSpace |
        //    aiProcess_Triangulate |
        //    aiProcess_SortByPType);

    const aiScene* ai_scene = ai_importer.ReadFile(file,
        aiProcess_Triangulate |
        aiProcess_SortByPType |
        aiProcess_JoinIdenticalVertices
    );

    size_t vertices_count = 0;
    size_t faces_count = 0;
    for (size_t mesh_it = 0; mesh_it < ai_scene->mNumMeshes; ++mesh_it)
    {
        vertices_count += ai_scene->mMeshes[mesh_it]->mNumVertices;
        faces_count += ai_scene->mMeshes[mesh_it]->mNumFaces;
    }

    Scene scene;
    scene.m_vertices.reserve(vertices_count);
    scene.m_normals.reserve(vertices_count);
    scene.m_indices.reserve(faces_count * 3);

    for (size_t mesh_it = 0; mesh_it < ai_scene->mNumMeshes; ++mesh_it)
    {
        auto mesh = ai_scene->mMeshes[mesh_it];

        std::transform(
            mesh->mVertices,
            mesh->mVertices + mesh->mNumVertices,
            std::back_inserter(scene.m_vertices), [](const aiVector3D& value)
        {
            return glm::vec3(value.x, value.y, value.z);
        });

        std::transform(
            mesh->mNormals,
            mesh->mNormals + mesh->mNumVertices,
            std::back_inserter(scene.m_normals), [](const aiVector3D& value)
        {
            return glm::vec3(value.x, value.y, value.z);
        });

        for (size_t face_it = 0; face_it < mesh->mNumFaces; ++face_it)
        {
            scene.m_indices.insert(scene.m_indices.end(), mesh->mFaces[face_it].mIndices, mesh->mFaces[face_it].mIndices + mesh->mFaces[face_it].mNumIndices);
        }
    }

    return scene;
}
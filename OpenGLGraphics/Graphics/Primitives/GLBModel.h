//
//	GLBModel.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 20/02/23
//	Copyright � 2023. All Rights reserved
//

#ifndef _GRAPHIC_LANGUAGE_BINARY_MODEL__H_
#define _GRAPHIC_LANGUAGE_BINARY_MODEL__H_

#include <vector>
#include "GLBMesh.h"
#include "ShaderProgram.h"
#include <Importer.hpp>
#include <postprocess.h>
#include <scene.h>
#include <iostream>
#include <stb_image.h>

namespace Graphics {
	namespace Primitives {
        class GLBModel {
        public:
            // model data 
            std::vector<Asset<Core::Graphics::Texture>> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
            std::vector<Mesh>    meshes;
            std::string directory;
            bool gammaCorrection;

            // constructor, expects a filepath to a 3D model.
            GLBModel(std::string const& path, bool gamma = false) : gammaCorrection(gamma)
            {
                loadModel(path);
            }

            // draws the model, and thus all its meshes
            void Draw(Core::Graphics::ShaderProgram& shader)
            {
                for (unsigned int i = 0; i < meshes.size(); i++)
                    meshes[i].Draw(shader);
            }

        private:
            // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
            void loadModel(std::string const& path)
            {
                // read file via ASSIMP
                Assimp::Importer importer;
                const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
                // check for errors
                if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
                {
                    std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
                    return;
                }
                // retrieve the directory path of the filepath
                directory = path.substr(0, path.find_last_of('/'));

                // process ASSIMP's root node recursively
                processNode(scene->mRootNode, scene);
            }

            // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
            void processNode(aiNode* node, const aiScene* scene)
            {
                // process each mesh located at the current node
                for (unsigned int i = 0; i < node->mNumMeshes; i++)
                {
                    // the node object only contains indices to index the actual objects in the scene. 
                    // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
                    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
                    meshes.push_back(processMesh(mesh, scene));
                }
                // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
                for (unsigned int i = 0; i < node->mNumChildren; i++)
                {
                    processNode(node->mChildren[i], scene);
                }

            }

            Mesh processMesh(aiMesh* mesh, const aiScene* scene)
            {
                // data to fill
                std::vector<Vertex> vertices;
                std::vector<unsigned int> indices;
                std::vector<Asset<Core::Graphics::Texture>> textures;

                // walk through each of the mesh's vertices
                for (unsigned int i = 0; i < mesh->mNumVertices; i++)
                {
                    Vertex vertex;
                    glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
                    // positions
                    vector.x = mesh->mVertices[i].x;
                    vector.y = mesh->mVertices[i].y;
                    vector.z = mesh->mVertices[i].z;
                    vertex.Position = vector;
                    // normals
                    if (mesh->HasNormals())
                    {
                        vector.x = mesh->mNormals[i].x;
                        vector.y = mesh->mNormals[i].y;
                        vector.z = mesh->mNormals[i].z;
                        vertex.Normal = vector;
                    }
                    // texture coordinates
                    if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
                    {
                        glm::vec2 vec;
                        // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                        // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                        vec.x = mesh->mTextureCoords[0][i].x;
                        vec.y = mesh->mTextureCoords[0][i].y;
                        vertex.TexCoords = vec;
                        // tangent
                        vector.x = mesh->mTangents[i].x;
                        vector.y = mesh->mTangents[i].y;
                        vector.z = mesh->mTangents[i].z;
                        vertex.Tangent = vector;
                        // bitangent
                        vector.x = mesh->mBitangents[i].x;
                        vector.y = mesh->mBitangents[i].y;
                        vector.z = mesh->mBitangents[i].z;
                        vertex.Bitangent = vector;
                    }
                    else
                        vertex.TexCoords = glm::vec2(0.0f, 0.0f);

                    vertices.push_back(vertex);
                }
                // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
                for (unsigned int i = 0; i < mesh->mNumFaces; i++)
                {
                    aiFace face = mesh->mFaces[i];
                    // retrieve all indices of the face and store them in the indices vector
                    for (unsigned int j = 0; j < face.mNumIndices; j++)
                        indices.push_back(face.mIndices[j]);
                }
                // process materials
                aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
                // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
                // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
                // Same applies to other texture as the following list summarizes:
                // diffuse: texture_diffuseN
                // specular: texture_specularN
                // normal: texture_normalN

                // 1. diffuse maps
                std::vector<Asset<Core::Graphics::Texture>> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
                textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
                // 2. specular maps
                std::vector<Asset<Core::Graphics::Texture>> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
                textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
                // 3. normal maps
                std::vector<Asset<Core::Graphics::Texture>> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
                textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
                // 4. height maps
                std::vector<Asset<Core::Graphics::Texture>> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
                textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

                // return a mesh object created from the extracted mesh data
                return Mesh(vertices, indices, textures);
            }

            // checks all material textures of a given type and loads the textures if they're not loaded yet.
            // the required info is returned as a Texture struct.
            std::vector<Asset<Core::Graphics::Texture>> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
            {
                std::vector<Asset<Core::Graphics::Texture>> textures;
                for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
                {
                    aiString str;
                    mat->GetTexture(type, i, &str);
                    // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
                    bool skip = false;

                    if (typeName == "texture_diffuse") {
                        Asset<Core::Graphics::Texture> texture = Singleton<ResourceManager>::Instance().GetResource<Core::Graphics::Texture>(str.C_Str());

                        texture->Get()->SetTextureType(Core::Graphics::Texture::eDiffuse);
                        textures.push_back(texture);
                    }
                    else if(typeName == "texture_normal") {
                        Asset<Core::Graphics::Texture> texture = Singleton<ResourceManager>::Instance().GetResource<Core::Graphics::Texture>(str.C_Str());

                        texture->Get()->SetTextureType(Core::Graphics::Texture::eNormal);
                        textures.push_back(texture);
                    }
                }
                return textures;
            }
        };
	}
}

#endif

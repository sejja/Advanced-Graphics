//
//	GLBModel.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 20/02/23
//	Copyright � 2023. All Rights reserved
//

#include "GLBModel.h"
#include <filesystem>

namespace Graphics {
	namespace Primitives {
		GLBModel::GLBModel(std::string const& path) {
			loadModel(path);
		}

		void GLBModel::Draw(Core::Graphics::ShaderProgram& shader) {
			for (unsigned int i = 0; i < meshes.size(); i++)
				meshes[i].Draw(shader);
		}

        void GLBModel::loadModel(std::string const& path)
        {
            std::string directory;
            const size_t last_slash_idx = path.rfind('\\');
            if (std::string::npos != last_slash_idx)
            {
                directory = path.substr(0, last_slash_idx);
            }

            // retrieve the directory path of the filepath
            directory = path.substr(0, path.find_last_of('/'));

            // read file via ASSIMP
            Assimp::Importer importer;
            const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);
            // check for errors
            if(scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
            std::cout << "ERROR::ASSIMP:: INCOMPLETE DATA" << std::endl;

            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) {
                std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
                return;
            }

            if (!scene->mRootNode)
                for (int i = 0; i < scene->mNumMeshes; i++)
                    meshes.push_back(processMesh(scene->mMeshes[i], scene, directory));
            else  // process ASSIMP's root node recursively
                processNode(scene->mRootNode, scene, directory);
        }

        void GLBModel::processNode(aiNode* node, const aiScene* scene, const std::string& dir) {
            // process each mesh located at the current node
            for (unsigned int i = 0; i < node->mNumMeshes; i++)
            {
                // the node object only contains indices to index the actual objects in the scene. 
                // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
                meshes.push_back(processMesh(mesh, scene, dir));
            }
            // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
            for (unsigned int i = 0; i < node->mNumChildren; i++)
            {
                processNode(node->mChildren[i], scene, dir);
            }
        }

        Mesh GLBModel::processMesh(aiMesh* mesh, const aiScene* scene, const std::string& dir)
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
                vertex.mPosition = vector;
                // normals
                if (mesh->HasNormals())
                {
                    vector.x = mesh->mNormals[i].x;
                    vector.y = mesh->mNormals[i].y;
                    vector.z = mesh->mNormals[i].z;
                    vertex.mNormal = vector;
                }
                // texture coordinates
                if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
                {
                    glm::vec2 vec;
                    // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                    // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                    vec.x = mesh->mTextureCoords[0][i].x;
                    vec.y = mesh->mTextureCoords[0][i].y;
                    vertex.mUV = vec;
                    // tangent
                    vector.x = mesh->mTangents[i].x;
                    vector.y = mesh->mTangents[i].y;
                    vector.z = mesh->mTangents[i].z;
                    vertex.mTangent = vector;
                    // bitangent
                    vector.x = mesh->mBitangents[i].x;
                    vector.y = mesh->mBitangents[i].y;
                    vector.z = mesh->mBitangents[i].z;
                    vertex.mBitangent = vector;
                }
                else
                    vertex.mUV = glm::vec2(0.0f, 0.0f);

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
            std::vector<Asset<Core::Graphics::Texture>> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", dir);
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            // 2. specular maps
            std::vector<Asset<Core::Graphics::Texture>> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", dir);
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
            // 3. normal maps
            std::vector<Asset<Core::Graphics::Texture>> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal", dir);
            textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
            // 4. height maps
            std::vector<Asset<Core::Graphics::Texture>> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height", dir);
            textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

            // return a mesh object created from the extracted mesh data
            return Mesh(vertices, indices, textures);
        }

        std::vector<Asset<Core::Graphics::Texture>> GLBModel::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const std::string& dir) {
            std::vector<Asset<Core::Graphics::Texture>> textures;
            for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
            {
                aiString str;
                mat->GetTexture(type, i, &str);
                // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
                bool skip = false;

                if (typeName == "texture_diffuse") {
                    Asset<Core::Graphics::Texture> texture = Singleton<ResourceManager>::Instance().GetResource<Core::Graphics::Texture>((dir + "/" + str.C_Str()).c_str());

                    texture->Get()->SetTextureType(Core::Graphics::Texture::eDiffuse);
                    textures.push_back(texture);
                }
                else if (typeName == "texture_normal") {
                    Asset<Core::Graphics::Texture> texture = Singleton<ResourceManager>::Instance().GetResource<Core::Graphics::Texture>((dir + "/" + str.C_Str()).c_str());

                    texture->Get()->SetTextureType(Core::Graphics::Texture::eNormal);
                    textures.push_back(texture);
                }
            }
            return textures;
        }
	}
}
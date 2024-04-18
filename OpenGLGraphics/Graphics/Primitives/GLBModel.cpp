//
//	GLBModel.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 20/02/23
//	Copyright � 2023. All Rights reserved
//

#include "GLBModel.h"

namespace Graphics {
	namespace Primitives {
        // ------------------------------------------------------------------------
        /*! Default Constructor
        *
        *   Loads a Model from a file
        */ //----------------------------------------------------------------------
		Model::Model(std::string const& path) {
			LoadModel(path);
		}

        // ------------------------------------------------------------------------
        /*! Draw
        *
        *   Draws every mesh in the model
        */ //----------------------------------------------------------------------
		void Model::Draw() {
            std::for_each(mMeshes.begin(), mMeshes.end(), [](Mesh& mesh) { mesh.Draw(); });
		}

        // ------------------------------------------------------------------------
        /*! Load Model
        *
        *   Loads the model from a file, recursively processing each node
        */ //----------------------------------------------------------------------
        void Model::LoadModel(std::string const& path) {
            std::string directory;
            const size_t last_slash_idx = path.rfind('\\');
            if (std::string::npos != last_slash_idx)
                directory = path.substr(0, last_slash_idx);

            // retrieve the directory path of the filepath
            directory = path.substr(0, path.find_last_of('/'));

            // read file via ASSIMP
            Assimp::Importer importer;
            const aiScene* scene = importer.ReadFile(path, aiProcess_CalcTangentSpace | aiProcess_FixInfacingNormals | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph);
            // check for errors
            if(scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
                throw ModelException("ERROR::ASSIMP:: INCOMPLETE DATA");
            
            // check if the scene is null
            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
                throw ModelException(importer.GetErrorString());

            aiMatrix4x4t<float> mat;

            mMeshes.reserve(scene->mNumMeshes);

            // process ASSIMP's root node recursively
            if (!scene->mRootNode)
                for (unsigned i = 0; i < scene->mNumMeshes; i++)
                    mMeshes.push_back(ProcessMesh(scene->mMeshes[i], scene, directory, mat));
            else  // process ASSIMP's root node recursively
                ProcessNode(scene->mRootNode, scene, directory, mat);
        }

        // ------------------------------------------------------------------------
        /*! Process Node
        *
        *  Processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
        */ //----------------------------------------------------------------------
        void Model::ProcessNode(aiNode* node, const aiScene* scene, const std::string& dir, aiMatrix4x4t<float> transform) {
           transform *= node->mTransformation;
           
           mMeshes.reserve(scene->mNumMeshes + mMeshes.capacity());

           // process each mesh located at the current node
            for (unsigned i = 0; i < node->mNumMeshes; i++) {
                // the node object only contains indices to index the actual objects in the scene. 
                // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
                mMeshes.push_back(ProcessMesh(scene->mMeshes[node->mMeshes[i]], scene, dir, transform));
            }
            // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
            for (unsigned i = 0; i < node->mNumChildren; i++)
                ProcessNode(node->mChildren[i], scene, dir, transform);
        }

        // ------------------------------------------------------------------------
        /*! Process Mesh
        *
        *  Processes a mesh and returns a Mesh object, calculating tangent and bitangent vectors
        */ //----------------------------------------------------------------------
        Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene, const std::string& dir, aiMatrix4x4t<float> transform) {
            // data to fill
            std::vector<Mesh::Vertex> vertices;
            std::vector<GLuint> indices;
            vertices.reserve(mesh->mNumVertices);

            // walk through each of the mesh's vertices
            for (unsigned  i = 0; i < mesh->mNumVertices; i++) {
                Mesh::Vertex vertex;
                glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
                // positions
                aiVector3D pos = mesh->mVertices[i];

                aiMatrix4x4t<float> mat = transform;
                aiVector3D pos2 = mat * pos;

                vector.x = pos2.x;
                vector.y = pos2.y;
                vector.z = pos2.z;
                vertex.mPosition = vector;
                // normals
                if (mesh->HasNormals()) {
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
                    //if(vec.x > 1.0f || vec.y > 1.0f)
					//	std::cout << "Whattt????: " << vec.x << ", " << vec.y << std::endl;

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

            indices.reserve(mesh->mNumFaces * 3);

            // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
            for (unsigned  i = 0; i < mesh->mNumFaces; i++) {
                aiFace face = mesh->mFaces[i];
                // retrieve all indices of the face and store them in the indices vector
                for (unsigned  j = 0; j < face.mNumIndices; j++)
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
            Core::Assets::Asset<Core::Graphics::Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, dir);

            // 3. normal maps
            Core::Assets::Asset<Core::Graphics::Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_NORMALS, dir);

            if(diffuseMaps && normalMaps)
                // return a mesh object created from the extracted mesh data
                return Mesh(vertices, indices, diffuseMaps, normalMaps);
            else if(diffuseMaps)
                // return a mesh object created from the extracted mesh data
                return Mesh(vertices, indices, diffuseMaps, nullptr);

            // return a mesh object created from the extracted mesh data
            return Mesh(vertices, indices, nullptr, nullptr);
        }

        // ------------------------------------------------------------------------
        /*! Load Material Textures
        *
        *  Load all of the textures for a material of a given type
        */ //----------------------------------------------------------------------
        Core::Assets::Asset<Core::Graphics::Texture> Model::LoadMaterialTextures(aiMaterial* mat, const aiTextureType type, const std::string& dir) {
            std::vector<Core::Assets::Asset<Core::Graphics::Texture>> textures;
            for (unsigned  i = 0, count = mat->GetTextureCount(type); i < count; i++)  {
                aiString str;
                mat->GetTexture(type, i, &str);
                
                //Swtich depending on the type of texture
                switch (type) {
                case aiTextureType_DIFFUSE:
                {
                    Core::Assets::Asset<Core::Graphics::Texture> texture = Singleton<Core::Assets::ResourceManager>::Instance().GetResource<Core::Graphics::Texture>((dir + "/" + str.C_Str()).c_str());

                    texture->Get()->SetTextureType(Core::Graphics::Texture::eDiffuse);
                    return texture;
                }

                default:
                {
                    Core::Assets::Asset<Core::Graphics::Texture> texture = Singleton<Core::Assets::ResourceManager>::Instance().GetResource<Core::Graphics::Texture>((dir + "/" + str.C_Str()).c_str());

                    texture->Get()->SetTextureType(Core::Graphics::Texture::eNormal);
                    return texture;
                }
                    break;
                }
            }

            return nullptr;
        }
	}
}
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
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <stb_image.h>

namespace Graphics {
	namespace Primitives {
        class Model {
        #pragma region //Declarations
        public:
            CLASS_EXCEPTION(Model)
        #pragma endregion

        #pragma region //Constructor
            // constructor, expects a filepath to a 3D model.
            Model(std::string const& path);
        #pragma endregion

        #pragma region //Methods
            // draws the model, and thus all its meshes
            void Draw();

        private:
            // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
            void loadModel(std::string const& path);

            // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
            void processNode(aiNode* node, const aiScene* scene, const std::string& dir, aiMatrix4x4t<float> transform);

            Mesh processMesh(aiMesh* mesh, const aiScene* scene, const std::string& dir, aiMatrix4x4t<float> transform);
         #pragma endregion

         #pragma region //Members
            std::vector<Mesh> mMeshes;
            // checks all material textures of a given type and loads the textures if they're not loaded yet.
            // the required info is returned as a Texture struct.
            std::vector<Core::Assets::Asset<Core::Graphics::Texture>> loadMaterialTextures(aiMaterial* mat, const aiTextureType type, const std::string& dir);
         #pragma endregion
        };
	}
}

#endif
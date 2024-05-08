//
//	GLBMesh.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 20/02/23
//	Copyright � 2023. All Rights reserved
//

#ifndef _GRAPHIC_LANGUAGE_BINARY_MESH__H_
#define _GRAPHIC_LANGUAGE_BINARY_MESH__H_

#include <glm.hpp>
#include <vector>
#include "ShaderProgram.h"
#include "Texture.h"
#include "Core/Assets/ResourceManager.h"

namespace Graphics {
	namespace Primitives {
        class Mesh {
        #pragma region //Declarations
        public:
            struct Vertex {
                // position
                glm::vec3 mPosition;
                // normal
                glm::vec3 mNormal;
                // texCoords
                glm::vec2 mUV;
                // tangent
                glm::vec3 mTangent;
                // bitangent
                glm::vec3 mBitangent;
            };

        #pragma endregion

        #pragma region //Constructor
            Mesh(const std::vector<Vertex>& vertices, 
                const std::vector<unsigned int>& indices, 
                const std::vector<Core::Assets::Asset<Core::Graphics::Texture>>& textures);
            void Draw();
            void bindTextures();
            GLuint getVao();
            GLuint getVbo();
            GLuint getCount();
                const std::vector<unsigned>& indices, 
                const Core::Assets::Asset<Core::Graphics::Texture>& diffuse,
                const Core::Assets::Asset<Core::Graphics::Texture>& normal);
        #pragma endregion

        #pragma region //Methods
            void Draw() const;
        #pragma endregion

        Core::Assets::Asset<Core::Graphics::Texture> getDiffuse() const { return mDiffuse; }
        Core::Assets::Asset<Core::Graphics::Texture> getNormal() const { return mNormal; }

        void setDiffuse(Core::Assets::Asset<Core::Graphics::Texture> diffuse) { mDiffuse = diffuse; }
        void setNormal(Core::Assets::Asset<Core::Graphics::Texture> normal) { mNormal = normal; }


        #pragma region //Members
        private:
            Core::Assets::Asset<Core::Graphics::Texture> mDiffuse;
            Core::Assets::Asset<Core::Graphics::Texture> mNormal;
            GLsizei mCount;
            GLuint mVao, mVbo, mEbo;
        #pragma endregion
        };
	}
}

#endif

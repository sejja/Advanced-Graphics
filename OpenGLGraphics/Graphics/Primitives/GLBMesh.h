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
#include <string>
#include <vector>
#include "ShaderProgram.h"
#include "Texture.h"

namespace Graphics {
	namespace Primitives {
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

        class Mesh {
        public:
            Mesh(const std::vector<Vertex>& vertices, 
                const std::vector<unsigned int>& indices, 
                const std::vector<Core::Assets::Asset<Core::Graphics::Texture>>& textures);
            void Draw();
            void bindTextures();
            GLuint getVao();
            GLuint getVbo();
            GLuint getCount();

        private:
            std::vector<Core::Assets::Asset<Core::Graphics::Texture>> textures;
            std::size_t mCount;
            GLuint mVao, mVbo, mEbo;
        };
	}
}

#endif

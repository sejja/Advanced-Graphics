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
#include <Graphics/Architecture/SpacePartitioning/octree.hpp>
#include <Dependencies\Assimp\include\assimp\aabb.h>

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

		void setAABB(const aiAABB aiAABBB) { aabb = aiAABBB; }
		aiAABB& getAABB() { return aabb; }

		std::string getMeshID() { return meshID; }


        //SPACE PARTITIONING TEMP PUBLIC
        bool visible = true;

        Octree<Mesh>::node* m_octree_node = nullptr;
        Mesh* m_octree_next_obj = nullptr;
        Mesh* m_octree_prev_obj = nullptr;




        #pragma region //Members
        private:
            Core::Assets::Asset<Core::Graphics::Texture> mDiffuse;
            Core::Assets::Asset<Core::Graphics::Texture> mNormal;
            GLsizei mCount;
            GLuint mVao, mVbo, mEbo;

            aiAABB aabb;

            std::string meshID;
        #pragma endregion
        };
	}
}

#endif

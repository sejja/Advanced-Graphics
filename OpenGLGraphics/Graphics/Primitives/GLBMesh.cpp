﻿//
//	GLBMesh.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 20/02/23
//	Copyright � 2023. All Rights reserved
//

#include "GLBMesh.h"
#include "Graphics/Architecture/InstancedRendering/InstancedRendering.h"


namespace Graphics {
	namespace Primitives {
        // ------------------------------------------------------------------------
        /*! Custom Constructor
        *
        *   Constructs meshes from a bunch of vertices, indices, a diffuse texture and a normal texture
        */ //----------------------------------------------------------------------
		Mesh::Mesh(const std::vector<Vertex>& vertices, 
            const std::vector<unsigned>& indices, 
            const Core::Assets::Asset<Core::Graphics::Texture>& diffuse,
            const Core::Assets::Asset<Core::Graphics::Texture>& normal) :
            mDiffuse(diffuse), mNormal(normal) {

            glGenVertexArrays(1, &mVao);
            glGenBuffers(1, &mVbo);
            glGenBuffers(1, &mEbo);

            glBindVertexArray(mVao);
            // load data into vertex buffers
            glBindBuffer(GL_ARRAY_BUFFER, mVbo);
            // A great thing about structs is that their memory layout is sequential for all its items.
            // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
            // again translates to 3/2 floats which translates to a byte array.
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

            // set the vertex attribute pointers
            // vertex Positions
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
            // vertex normals
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mNormal));
            // vertex texture coords
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mUV));
            // vertex tangent
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mTangent));
            // vertex bitangent
            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, mBitangent));
            mCount = static_cast<GLsizei>(indices.size());
		}

        // ------------------------------------------------------------------------
        /*! Custom Constructor
        *
        *   Constructs meshes from a bunch of vertices, indices, a diffuse texture and a normal texture
        */ //----------------------------------------------------------------------
        void Mesh::Draw() {
            int isInstanced = Singleton<Graphics::Architecture::InstancedRendering::InstanceRenderer>::Instance().is_Instanced(this);

            if (isInstanced) {
                return; //If the mesh ins instanced wo dont draw it again
            }

            bindTextures();

            // draw mesh
            glBindVertexArray(mVao);
            glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(mCount), GL_UNSIGNED_INT, 0);
        }

        void Mesh::bindTextures()
        {
            // bind appropriate textures
            if (mDiffuse) mDiffuse->Get()->Bind();
            // bind normal map
            if (mNormal) mNormal->Get()->Bind();
        }

        GLuint Mesh::getVao()
        {
            return this->mVao;
        }

        GLuint Mesh::getVbo()
        {
            return this->mVbo;
        }

        GLuint Mesh::getCount()
        {
            return this->mCount;
        }
	}
}
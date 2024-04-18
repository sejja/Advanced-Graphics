//
//	DebugPrimitives.cpp
//	OpenGL Graphics
//
//	Created by Diego Revilla on 15/04/24
//	Copyright � 2024 . All Rights reserved
//

#include "DebugPrimitives.h"
#include <gtc/matrix_transform.hpp>

namespace Graphics {
    namespace Debug {
        // ------------------------------------------------------------------------
        /*! Constructor
        *
        *   Constructs an Empty Primitive
        */ //----------------------------------------------------------------------
        DebugPrimitive::DebugPrimitive()
            : mVAO(NULL), mVBO(NULL), mVertexCount(0) {}


        // ------------------------------------------------------------------------
        /*! Destructor
        *
        *   If Initialized, removes the primitive buffers
        */ //----------------------------------------------------------------------
        DebugPrimitive::~DebugPrimitive() {
            if(!mVAO) return;
            // Delete the VBOs
            glDeleteBuffers(1, &mVBO);
            // Delete the VAO
            glDeleteVertexArrays(1, &mVAO);
        }

        // ------------------------------------------------------------------------
        /*! Initialize Buffers
        *
        *   Creates and sets up the internal vertex buffers object and vertex array object
        */ //----------------------------------------------------------------------
        void DebugPrimitive::InitializeBuffers(std::vector<glm::vec3>& vertices) {
            if (mVAO) {
                glDeleteVertexArrays(1, &mVAO); mVAO = 0;
                glDeleteBuffers(1, &mVBO); mVBO = 0;
            }

            // Generate VAO
            glGenVertexArrays(1, &mVAO);

            // Generate VBO
            glGenBuffers(1, &mVBO);

            //Bind buffers and data
            glBindVertexArray(mVAO);
            glBindBuffer(GL_ARRAY_BUFFER, mVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), &vertices[0], GL_DYNAMIC_DRAW);

            //Send vtx. attributes to the array
            //Position
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

            //Unbind after usage
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
            mVertexCount = static_cast<GLsizei>(vertices.size());
        }


        // ------------------------------------------------------------------------
        /*! Initialize Buffers
        *
        *   Initializes the shape as a 3D point
        */ //----------------------------------------------------------------------
        void DebugPrimitive::InitPoint() {
            std::vector<glm::vec3> mVertices;

            mVertices.emplace_back( 0.f );
            InitializeBuffers(mVertices);
        }

        // ------------------------------------------------------------------------
        /*! Initialize Buffers
        *
        *   Initializes the shape as a 3D segment
        */ //----------------------------------------------------------------------
        void DebugPrimitive::InitSegment() {
            std::vector<glm::vec3> mVertices;

            mVertices.emplace_back(0.f);
            mVertices.emplace_back(1.f, 1.0f, 1.0f);
            InitializeBuffers(mVertices);
        }

        // ------------------------------------------------------------------------
        /*! Initialize Buffers
        *
        *   Initializes the shape as an Axis-Aligned Bounding Box
        */ //----------------------------------------------------------------------
        void DebugPrimitive::InitAABB() {
            std::vector<glm::vec3> mVertices;
            const glm::vec3 V1(0.5f, 0.5f, 0.5f);
            const glm::vec3 V2(-0.5f, 0.5f, 0.5f);
            const glm::vec3 V3(-0.5f, -0.5f, 0.5f);
            const glm::vec3 V4(0.5f, -0.5f, 0.5f);
            const glm::vec3 V5(0.5f, 0.5f, -0.5f);
            const glm::vec3 V6(-0.5f, 0.5f, -0.5f);
            const glm::vec3 V7(-0.5f, -0.5f, -0.5f);
            const glm::vec3 V8(0.5f, -0.5f, -0.5f);

            {
                //Front face
                mVertices.push_back(V1);
                mVertices.push_back(V2);
                mVertices.push_back(V2);
                mVertices.push_back(V3);
                mVertices.push_back(V3);
                mVertices.push_back(V4);
                mVertices.push_back(V4);
                mVertices.push_back(V1);

                //Back face
                mVertices.push_back(V5);
                mVertices.push_back(V6);
                mVertices.push_back(V6);
                mVertices.push_back(V7);
                mVertices.push_back(V7);
                mVertices.push_back(V8);
                mVertices.push_back(V8);
                mVertices.push_back(V5);

                //Horizontal unions
                mVertices.push_back(V1);
                mVertices.push_back(V5);
                mVertices.push_back(V2);
                mVertices.push_back(V6);
                mVertices.push_back(V3);
                mVertices.push_back(V7);
                mVertices.push_back(V4);
                mVertices.push_back(V8);

            }

            InitializeBuffers(mVertices);
        }


        // ------------------------------------------------------------------------
        /*! Initialize Buffers
        *
        *   Initializes the shape as a triangle
        */ //----------------------------------------------------------------------
        void DebugPrimitive::InitTriangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) {
            std::vector<glm::vec3> mVertices;
            mVertices.push_back(a);
            mVertices.push_back(b);
            mVertices.push_back(c);
            InitializeBuffers(mVertices);
        }

        // ------------------------------------------------------------------------
        /*! Initialize Buffers
        *
        *   Initializes the shape as a plane
        */ //----------------------------------------------------------------------
        void DebugPrimitive::InitPlane() {
            std::vector<glm::vec3> mVertices;
            //4 points to build a quad
            const glm::vec3 V1(0.5f, 0.5f, 0.0f);
            const glm::vec3 V2(-0.5f, 0.5f, 0.0f);
            const glm::vec3 V3(0.5f, -0.5f, 0.0f);
            const glm::vec3 V4(-0.5f, -0.5f, 0.0f);

            mVertices.push_back(V1);
            mVertices.push_back(V2);
            mVertices.push_back(V3);
            mVertices.push_back(V2);
            mVertices.push_back(V4);
            mVertices.push_back(V3);

            InitializeBuffers(mVertices);
        }

        // ------------------------------------------------------------------------
        /*! Initialize Buffers
        *
        *  Initializes the shape as a sphere disc
        */ //----------------------------------------------------------------------
        void DebugPrimitive::InitSphereDisc() {
            std::vector<glm::vec3> mVertices;
            //Draw a circle on the XY plane
            const glm::mat3 Rotate = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

            //Loop until we have a complete circle
            for (float i = 0; i < 360; i += 1) {
                mVertices.push_back({ 0.5f * cos(glm::radians(i)), 0.5f * sin(glm::radians(i)), 0.f });
                mVertices.push_back({ 0.5f * cos(glm::radians(i + 1)), 0.5f * sin(glm::radians(i + 1)), 0.f });
            }

            InitializeBuffers(mVertices);
        }

        // ------------------------------------------------------------------------
        /*! Initialize Buffers
        *
        *   Initializes the shape as a camera frustum
        */ //----------------------------------------------------------------------
        void DebugPrimitive::InitFrustum() {
            std::vector<glm::vec3> mVertices;
            //24 vertices in total (8 vertices, each used on 3 faces)
            const glm::vec3 V1(1.f, 1.f, 1.f);
            const glm::vec3 V2(-1.f, 1.f, 1.f);
            const glm::vec3 V3(-1.f, -1.f, 1.f);
            const glm::vec3 V4(1.f, -1.f, 1.f);
            const glm::vec3 V5(1.f, 1.f, -1.f);
            const glm::vec3 V6(-1.f, 1.f, -1.f);
            const glm::vec3 V7(-1.f, -1.f, -1.f);
            const glm::vec3 V8(1.f, -1.f, -1.f);

            //Same as with the AABB, but using lines
            //Front face
                //Front face
            mVertices.push_back(V1);
            mVertices.push_back(V2);
            mVertices.push_back(V2);
            mVertices.push_back(V3);
            mVertices.push_back(V3);
            mVertices.push_back(V4);
            mVertices.push_back(V4);
            mVertices.push_back(V1);

            //Back face
            mVertices.push_back(V5);
            mVertices.push_back(V6);
            mVertices.push_back(V6);
            mVertices.push_back(V7);
            mVertices.push_back(V7);
            mVertices.push_back(V8);
            mVertices.push_back(V8);
            mVertices.push_back(V5);

            //Horizontal unions
            mVertices.push_back(V1);
            mVertices.push_back(V5);
            mVertices.push_back(V2);
            mVertices.push_back(V6);
            mVertices.push_back(V3);
            mVertices.push_back(V7);
            mVertices.push_back(V4);
            mVertices.push_back(V8);

            InitializeBuffers(mVertices);
        }

        // ------------------------------------------------------------------------
        /*! Draw
        *
        *   Draws the primitive
        */ //----------------------------------------------------------------------
        void DebugPrimitive::Draw() const {
            glBindVertexArray(mVAO);
            glDrawArrays(GL_LINES, 0, mVertexCount);
        }
    }
}
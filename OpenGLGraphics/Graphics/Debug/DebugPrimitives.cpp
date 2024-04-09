#include "DebugPrimitives.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

Primitive::Primitive()
{
}

/**********************************************
* fn	~DebugShape
* brief
*   Destructor. Frees memory allocated by the shape
**********************************************/
Primitive::~Primitive() {
    // Delete the VBOs
    glDeleteBuffers(1, &mVBO);
    // Delete the VAO
    glDeleteVertexArrays(1, &mVAO);
}

/**********************************************
* fn	InitializeBuffers
* brief
*   Creates and sets up the internal vertex
buffers object and vertex array object
**********************************************/
void Primitive::InitializeBuffers()
{
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mVertices.size(), &mVertices[0], GL_DYNAMIC_DRAW);

    //Send vtx. attributes to the array
    //Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

    //Unbind after usage
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

/**********************************************
* fn	LoadShader
* brief
*   Loads, compiles and links the vertex shader
and fragment shader used by a shape when drawing
* param vtxShaderPath : path to the vertex shader file
* param fragShaderPath : path to the fragment shader file
**********************************************/
void Primitive::LoadShader(const char* shaderpath)
{

    mShader = Singleton<Core::Assets::ResourceManager>::Instance().GetResource<Core::Graphics::ShaderProgram>(shaderpath);

}

/**********************************************
* fn	InitPoint
* brief
*   Initializes the shape as a 3D point
**********************************************/
void Primitive::InitPoint()
{
    //Just a point
    glm::vec3 P{ 0.f };
    mVertices.push_back(P);
}

/**********************************************
* fn	InitSegment
* brief
*   Initializes the shape as a 3D segment
**********************************************/
void Primitive::InitSegment()
{
    //2 points: start and end
    glm::vec3 Start(0.f);
    glm::vec3 End(1.f, 1.0f, 1.0f);

    mVertices.push_back(Start);
    mVertices.push_back(End);
}

/**********************************************
* fn	InitAABB
* brief
*   Initializes the shape as an Axis-Aligned
* Bounding Box
**********************************************/
void Primitive::InitAABB()
{
    glm::vec3 V1(0.5f, 0.5f, 0.5f);
    glm::vec3 V2(-0.5f, 0.5f, 0.5f);
    glm::vec3 V3(-0.5f, -0.5f, 0.5f);
    glm::vec3 V4(0.5f, -0.5f, 0.5f);
    glm::vec3 V5(0.5f, 0.5f, -0.5f);
    glm::vec3 V6(-0.5f, 0.5f, -0.5f);
    glm::vec3 V7(-0.5f, -0.5f, -0.5f);
    glm::vec3 V8(0.5f, -0.5f, -0.5f);

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
}

/**********************************************
* fn	InitTriangle
* brief
*   Initializes the shape as a triangle
* param a, b, c : vertices in world space
**********************************************/
void Primitive::InitTriangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c)
{
    mVertices.push_back(a);
    mVertices.push_back(b);
    mVertices.push_back(c);
}

/**********************************************
* fn	InitPlane
* brief
*   Initializes the shape as a plane
**********************************************/
void Primitive::InitPlane()
{
    //4 points to build a quad
    glm::vec3 V1(0.5f, 0.5f, 0.0f);
    glm::vec3 V2(-0.5f, 0.5f, 0.0f);
    glm::vec3 V3(0.5f, -0.5f, 0.0f);
    glm::vec3 V4(-0.5f, -0.5f, 0.0f);

    mVertices.push_back(V1);
    mVertices.push_back(V2);
    mVertices.push_back(V3);

    mVertices.push_back(V2);
    mVertices.push_back(V4);
    mVertices.push_back(V3);

}

/**********************************************
* fn	InitSphereDisc
* brief
*   Initializes the shape as a sphere disc
**********************************************/
void Primitive::InitSphereDisc()
{
    //Draw a circle on the XY plane
    glm::mat3 Rotate = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

    //Loop until we have a complete circle
    for (unsigned i = 0; i < 360; i++)
    {
        mVertices.push_back({ 0.5f * cos(glm::radians(static_cast<float>(i))), 0.5f * sin(glm::radians(static_cast<float>(i))), 0.f });
        mVertices.push_back({ 0.5f * cos(glm::radians(static_cast<float>(i + 1))), 0.5f * sin(glm::radians(static_cast<float>(i + 1))), 0.f });
    }
}

/**********************************************
* fn	InitFrustum
* brief
*   Initializes the shape as a camera frustum
**********************************************/
void Primitive::InitFrustum()
{
    //24 vertices in total (8 vertices, each used on 3 faces)
    glm::vec3 V1(1.f, 1.f, 1.f);
    glm::vec3 V2(-1.f, 1.f, 1.f);
    glm::vec3 V3(-1.f, -1.f, 1.f);
    glm::vec3 V4(1.f, -1.f, 1.f);
    glm::vec3 V5(1.f, 1.f, -1.f);
    glm::vec3 V6(-1.f, 1.f, -1.f);
    glm::vec3 V7(-1.f, -1.f, -1.f);
    glm::vec3 V8(1.f, -1.f, -1.f);

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
}

/**********************************************
* fn	Clear
* brief
*   Removes all vertices of a shape
**********************************************/
void Primitive::Clear()
{
    mVertices.clear();
}

/**********************************************
* fn	VtxCount
* brief
*  Returns the amount of vertices stored in the
* shape
**********************************************/
unsigned Primitive::VtxCount()
{
    return (unsigned)mVertices.size();
}

/**********************************************
* fn	BindForDrawing
* brief
*   Prepares the shape's VAO for drawing
**********************************************/
void Primitive::BindForDrawing()
{
    glBindVertexArray(mVAO);
}
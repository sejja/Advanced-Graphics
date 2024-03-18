#include <stdexcept>
#include "DebugShapes.h"

// ------------------------------------------------------------------------
/*! Upadte
*
*   Updates the Perspective Matrix
*/ //----------------------------------------------------------------------
debug_system::debug_system(Core::Primitives::Camera const* c) {
    mCamera = c;

    mPoint.InitPoint();
    mPoint.InitializeBuffers();
    mPoint.LoadShader("Content/Shaders/DebugLines.shader");

    //Segment
    mSegment.InitSegment();
    mSegment.InitializeBuffers();
    mSegment.LoadShader("Content/Shaders/DebugLines.shader");

    //Triangle
    mTriangle.LoadShader("Content/Shaders/DebugLines.shader");

    //AABB
    mAABB.InitAABB();
    mAABB.InitializeBuffers();
    mAABB.LoadShader("Content/Shaders/DebugLines.shader");

    //Plane
    mPlane.InitPlane();
    mPlane.InitializeBuffers();
    mPlane.LoadShader("Content/Shaders/DebugLines.shader");

    //Sphere (just 1 circle)
    mSphere.InitSphereDisc();
    mSphere.InitializeBuffers();
    mSphere.LoadShader("Content/Shaders/DebugLines.shader");

    //Frustum
    mFrustum.InitFrustum();
    mFrustum.InitializeBuffers();
    mFrustum.LoadShader("Content/Shaders/DebugLines.shader");
}

// ------------------------------------------------------------------------
/*! Upadte
*
*   Updates the Perspective Matrix
*/ //----------------------------------------------------------------------
void debug_system::draw_point(glm::vec3 pt, glm::vec4 color) {
    //Enable shader
    mPoint.mShader->Get()->Bind();

    //Set uniforms:
    {
        //Build the model to viewport matrix
        //Model->world: apply translation to the point
        glm::mat4 Model = glm::translate(glm::mat4(1.0f), glm::vec3(pt));
        glm::mat4 MVP = mCamera->GetProjectionMatrix() * mCamera->GetViewMatrix() * Model;//proj * cam * Model; //= mCamera->GetProjMtx() * mCamera->GetCamMtx() * Model;

        //Locate the MVP uniform in the shader
        GLuint Location = glGetUniformLocation(mPoint.mShader->Get()->GetHandle(), "uniform_mvp");
        if (Location >= 0)
            glUniformMatrix4fv(Location, 1, GL_FALSE, &MVP[0][0]);
        else
            throw std::runtime_error("RUNTIME ERROR: 'uniform_mvp' not found");

        //Locate the color uniform in the fragment shader
        Location = glGetUniformLocation(mPoint.mShader->Get()->GetHandle(), "uniform_color");
        if (Location >= 0)
            glUniform4f(Location, color.r, color.g, color.b, color.a);
        else
            throw std::runtime_error("RUNTIME ERROR: 'uniform_color' not found");
    }

    mPoint.BindForDrawing();
    glDrawArrays(GL_LINES, 0, mPoint.VtxCount());
}

// ------------------------------------------------------------------------
/*! Upadte
*
*   Updates the Perspective Matrix
*/ //----------------------------------------------------------------------
void debug_system::draw_segment(glm::vec3 s, glm::vec3 e, glm::vec4 color) {
    //Enable shader
    mSegment.mShader->Get()->Bind();

    //Set uniforms:
    {
        //Build the model to viewport matrix
        //Model->world: transform based on position of starting point
        glm::mat4 Model = glm::translate(glm::mat4(1.0f), s) * glm::scale(glm::mat4(1.0f), e - s);
        glm::mat4 MVP = mCamera->GetProjectionMatrix() * mCamera->GetViewMatrix() * Model; //= mCamera->GetProjMtx() * mCamera->GetCamMtx() * Model;

        //Locate the MVP uniform in the shader
        GLuint Location = glGetUniformLocation(mSegment.mShader->Get()->GetHandle(), "uniform_mvp");
        if (Location >= 0)
            glUniformMatrix4fv(Location, 1, GL_FALSE, &MVP[0][0]);
        else
            throw std::runtime_error("RUNTIME ERROR: 'uniform_mvp' not found");

        //Locate the color uniform in the fragment shader
        Location = glGetUniformLocation(mSegment.mShader->Get()->GetHandle(), "uniform_color");
        if (Location >= 0)
            glUniform4f(Location, color.r, color.g, color.b, color.a);
        else
            throw std::runtime_error("RUNTIME ERROR: 'uniform_color' not found");
    }

    mSegment.BindForDrawing();
    glDrawArrays(GL_LINES, 0, mSegment.VtxCount());
}

// ------------------------------------------------------------------------
/*! Upadte
*
*   Updates the Perspective Matrix
*/ //----------------------------------------------------------------------
void debug_system::draw_triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec4 color) {
    mTriangle.Clear();
    mTriangle.InitTriangle(a, b, c);
    mTriangle.InitializeBuffers();

    //Enable shader
    mTriangle.mShader->Get()->Bind();

    //Set uniforms:
    {
        //Model = identity (not required)
        glm::mat4 MVP = mCamera->GetProjectionMatrix() * mCamera->GetViewMatrix();

        //Locate the MVP uniform in the shader
        int Location = glGetUniformLocation(mTriangle.mShader->Get()->GetHandle(), "uniform_mvp");
        if (Location >= 0)
            glUniformMatrix4fv(Location, 1, GL_FALSE, &MVP[0][0]);
        else
            throw std::runtime_error("RUNTIME ERROR: 'uniform_mvp' not found");

        //Locate the color uniform in the fragment shader
        Location = glGetUniformLocation(mTriangle.mShader->Get()->GetHandle(), "uniform_color");
        if (Location >= 0)
            glUniform4f(Location, color.r, color.g, color.b, color.a);
        else
            throw std::runtime_error("RUNTIME ERROR: 'uniform_color' not found");
    }
    mTriangle.BindForDrawing();
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

// ------------------------------------------------------------------------
/*! Upadte
*
*   Updates the Perspective Matrix
*/ //----------------------------------------------------------------------
void debug_system::draw_aabb(glm::vec3 c, glm::vec3 size, glm::vec4 color) {
    //Enable shader
    mAABB.mShader->Get()->Bind();

    //Set uniforms:
    {
        //Model: translate center, then scale in the 3 main axis
        glm::mat4 Model = glm::translate(glm::mat4(1.0f), c) * glm::scale(glm::mat4(1.0f), size);

        //Build the model to viewport matrix
        glm::mat4 MVP = mCamera->GetProjectionMatrix() * mCamera->GetViewMatrix() * Model; //= mCamera->GetProjMtx() * mCamera->GetCamMtx() * Model;

        //Locate the MVP uniform in the shader
        int Location = glGetUniformLocation(mAABB.mShader->Get()->GetHandle(), "uniform_mvp");
        if (Location >= 0)
            glUniformMatrix4fv(Location, 1, GL_FALSE, &MVP[0][0]);
        else
            throw std::runtime_error("RUNTIME ERROR: 'uniform_mvp' not found");

        //Locate the color uniform in the fragment shader
        Location = glGetUniformLocation(mAABB.mShader->Get()->GetHandle(), "uniform_color");
        if (Location >= 0)
            glUniform4f(Location, color.r, color.g, color.b, color.a);
        else
            throw std::runtime_error("RUNTIME ERROR: 'uniform_color' not found");
    }

    mAABB.BindForDrawing();
    glDrawArrays(GL_LINES, 0, mAABB.VtxCount());
}

// ------------------------------------------------------------------------
/*! Upadte
*
*   Updates the Perspective Matrix
*/ //----------------------------------------------------------------------
void debug_system::draw_plane(glm::vec3 pt, glm::vec3 n, float scale, glm::vec4 color) {
    //Enable shader
    mPlane.mShader->Get()->Bind();

    // Model: scale 2D, translate to the point, lookAt
    // following the normal
    glm::vec3 Normal = glm::normalize(n);
    glm::vec3 Right;

    //Sanity check to avoid cross product with the same vector
    if (Normal == glm::vec3(0.f, 1.0f, 0.0f))
        Right = glm::cross(Normal, glm::vec3(1.0f, 0.0f, 0.0f));

    else
        Right = glm::cross(Normal, glm::vec3(0.0f, 1.0f, 0.0f));

    glm::vec3 Up = glm::cross(Right, Normal);

    glm::mat4 ScaleMtx = glm::scale(glm::mat4(1.0f), glm::vec3(scale));
    glm::mat4 Model = glm::translate(glm::mat4(1.0f), pt) * glm::lookAt(pt, Normal, Up) * ScaleMtx;

    //Set uniforms:
    {
        //Model = identity (not required)
        glm::mat4 MVP = mCamera->GetProjectionMatrix() * mCamera->GetViewMatrix() * Model; // = mCamera->GetProjMtx() * mCamera->GetCamMtx() * Model;

        //Locate the MVP uniform in the shader
        int Location = glGetUniformLocation(mPlane.mShader->Get()->GetHandle(), "uniform_mvp");
        if (Location >= 0)
            glUniformMatrix4fv(Location, 1, GL_FALSE, &MVP[0][0]);
        else
            throw std::runtime_error("RUNTIME ERROR: 'uniform_mvp' not found");

        //Locate the color uniform in the fragment shader
        Location = glGetUniformLocation(mPlane.mShader->Get()->GetHandle(), "uniform_color");
        if (Location >= 0)
            glUniform4f(Location, color.r, color.g, color.b, color.a);
        else
            throw std::runtime_error("RUNTIME ERROR: 'uniform_color' not found");
    }

    //Draw
    mPlane.BindForDrawing();
    glDrawArrays(GL_TRIANGLES, 0, mPlane.VtxCount());
}

// ------------------------------------------------------------------------
/*! Upadte
*
*   Updates the Perspective Matrix
*/ //----------------------------------------------------------------------
void debug_system::draw_sphere(glm::vec3 c, float r, glm::vec4 color) {
    //Enable shader
    mSphere.mShader->Get()->Bind();

    //Set uniforms based on which disc to draw
    //1st disc
    {
        //Model: translate by the center, scale time the radius
        glm::mat4 Model = glm::translate(glm::mat4(1.0f), c) * glm::scale(glm::mat4(1.0f), glm::vec3(r));

        //Build the model to viewport matrix
        glm::mat4 MVP = mCamera->GetProjectionMatrix() * mCamera->GetViewMatrix() * Model; // = mCamera->GetProjMtx() * mCamera->GetCamMtx();

        //Locate the MVP uniform in the shader
        int Location = glGetUniformLocation(mSphere.mShader->Get()->GetHandle(), "uniform_mvp");
        if (Location >= 0)
            glUniformMatrix4fv(Location, 1, GL_FALSE, &MVP[0][0]);
        else
            throw std::runtime_error("RUNTIME ERROR: 'uniform_mvp' not found");

        //Locate the color uniform in the fragment shader
        Location = glGetUniformLocation(mSphere.mShader->Get()->GetHandle(), "uniform_color");
        if (Location >= 0)
            glUniform4f(Location, color.r, color.g, color.b, color.a);
        else
            throw std::runtime_error("RUNTIME ERROR: 'uniform_color' not found");

        mSphere.BindForDrawing();
        glDrawArrays(GL_LINES, 0, mSphere.VtxCount());
    }


    //2nd disc
    {
        //Rotate to aligh with XZ plane
        glm::mat4 Model = glm::translate(glm::mat4(1.0f), c) * glm::rotate(glm::mat4(1.0f), glm::radians(90.f), glm::vec3(0.f, 1.0f, 0.f)) * glm::scale(glm::mat4(1.0f), glm::vec3(r));
        glm::mat4 MVP = mCamera->GetProjectionMatrix() * mCamera->GetViewMatrix() * Model; // = mCamera->GetProjMtx() * mCamera->GetCamMtx();

        //Locate the MVP uniform in the shader
        int Location = glGetUniformLocation(mSphere.mShader->Get()->GetHandle(), "uniform_mvp");
        if (Location >= 0)
            glUniformMatrix4fv(Location, 1, GL_FALSE, &MVP[0][0]);
        else
            throw std::runtime_error("RUNTIME ERROR: 'uniform_mvp' not found");

        //Locate the color uniform in the fragment shader
        Location = glGetUniformLocation(mSphere.mShader->Get()->GetHandle(), "uniform_color");
        if (Location >= 0)
            glUniform4f(Location, color.r, color.g, color.b, color.a);
        else
            throw std::runtime_error("RUNTIME ERROR: 'uniform_color' not found");

        //Draw
        mSphere.BindForDrawing();
        glDrawArrays(GL_LINES, 0, mSphere.VtxCount());
    }


    //3rd disc
    {
        //Rotate to align with YZ plane
        glm::mat4 Model = glm::translate(glm::mat4(1.0f), c) * glm::rotate(glm::mat4(1.0f), glm::radians(90.f), glm::vec3(1.f, 0.0f, 0.f)) * glm::scale(glm::mat4(1.0f), glm::vec3(r));
        glm::mat4 MVP = mCamera->GetProjectionMatrix() * mCamera->GetViewMatrix() * Model; // = mCamera->GetProjMtx() * mCamera->GetCamMtx();

        //Locate the MVP uniform in the shader
        int Location = glGetUniformLocation(mSphere.mShader->Get()->GetHandle(), "uniform_mvp");
        if (Location >= 0)
            glUniformMatrix4fv(Location, 1, GL_FALSE, &MVP[0][0]);
        else
            throw std::runtime_error("RUNTIME ERROR: 'uniform_mvp' not found");

        //Locate the color uniform in the fragment shader
        Location = glGetUniformLocation(mSphere.mShader->Get()->GetHandle(), "uniform_color");
        if (Location >= 0)
            glUniform4f(Location, color.r, color.g, color.b, color.a);
        else
            throw std::runtime_error("RUNTIME ERROR: 'uniform_color' not found");

        //Draw
        mSphere.BindForDrawing();
        glDrawArrays(GL_LINES, 0, mSphere.VtxCount());
    }

    //Horizon disc
    //4th: horizon disk
    // Calculate the horizon ring's radius
    // and center
    glm::vec3 View = { 0.f, 1.f, 0.f };// = mCamera->mPosition - c;
    float Dist = length(View);
    float L = sqrt(Dist * Dist - r * r);
    float HorizonR = (r * L) / Dist;
    float T = sqrt(r * r - HorizonR * HorizonR);

    //Compute lookAt matrix so it constantly looks to the camera
    glm::vec3 Right = glm::cross(View, glm::vec3(0.f, 1.0f, 0.f));
    glm::vec3 Up = glm::cross(View, Right);

    glm::mat4 LookMtx = glm::inverse(glm::lookAt(glm::vec3(0.0f), View, Up));

    //Set uniforms:
    {
        //Model: translate by the center, scale time the radius
        //and apply look matrix
        glm::mat4 Model = glm::translate(glm::mat4(1.0f), c + View * T) * LookMtx * glm::scale(glm::mat4(1.0f), glm::vec3(HorizonR));

        //Build the model to viewport matrix
        glm::mat4 MVP = mCamera->GetProjectionMatrix() * mCamera->GetViewMatrix() * Model; // = mCamera->GetProjMtx() * mCamera->GetCamMtx();

        int Location = glGetUniformLocation(mSphere.mShader->Get()->GetHandle(), "uniform_mvp");
        if (Location >= 0)
            glUniformMatrix4fv(Location, 1, GL_FALSE, &MVP[0][0]);
        else
            throw std::runtime_error("RUNTIME ERROR: 'uniform_mvp' not found");

        //Locate the color uniform in the fragment shader
        Location = glGetUniformLocation(mSphere.mShader->Get()->GetHandle(), "uniform_color");
        if (Location >= 0)
            glUniform4f(Location, color.r, color.g, color.b, color.a);
        else
            throw std::runtime_error("RUNTIME ERROR: 'uniform_color' not found");
    }
    mSphere.BindForDrawing();
    glDrawArrays(GL_LINES, 0, mSphere.VtxCount());
}

// ------------------------------------------------------------------------
/*! Upadte
*
*   Updates the Perspective Matrix
*/ //----------------------------------------------------------------------
void debug_system::draw_frustum_lines(glm::mat4 const& vp, glm::vec4 color) {
    //Enable shader
    mFrustum.mShader->Get()->Bind();

    //Set uniforms:
    {
        //Build the model to viewport matrix
        //Model: use inverse of givel world-to-perspective matrix
        const glm::mat4 MVP = mCamera->GetProjectionMatrix() * mCamera->GetViewMatrix() * glm::inverse(vp);

        //Locate the MVP uniform in the shader
        int Location = glGetUniformLocation(mFrustum.mShader->Get()->GetHandle(), "uniform_mvp");
        if (Location >= 0)
            glUniformMatrix4fv(Location, 1, GL_FALSE, &MVP[0][0]);
        else
            throw std::runtime_error("RUNTIME ERROR: 'uniform_mvp' not found");

        //Locate the color uniform in the fragment shader
        Location = glGetUniformLocation(mFrustum.mShader->Get()->GetHandle(), "uniform_color");
        if (Location >= 0)
            glUniform4f(Location, color.r, color.g, color.b, color.a);
        else
            throw std::runtime_error("RUNTIME ERROR: 'uniform_color' not found");
    }

    //Draw
    mFrustum.BindForDrawing();
    glDrawArrays(GL_LINES, 0, mFrustum.VtxCount());
}

void debug_system::change_camera(Core::Primitives::Camera const* c) {
    mCamera = c;
}
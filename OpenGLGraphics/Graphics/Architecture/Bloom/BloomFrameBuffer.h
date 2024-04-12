//
//	BloomFrameBuffer.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 1/03/24
//	Copyright © 2024. All Rights reserved
//

#ifndef _BLOOM_FRAME_BUFFER__H_
#define _BLOOM_FRAME_BUFFER__H_

#include <glm.hpp>
#include <array>
#include <glew.h>
#include "CommonDefines.h"

namespace Graphics {
	namespace Architecture {
		namespace Bloom {
            class BloomFramebuffer {
            #pragma region //Declarations 
            public:
                struct BloomMip {
                #pragma region //Constructors
                    BloomMip(const glm::u16vec2 size);
                    ~BloomMip();
                #pragma endregion

                #pragma region //Methods
                    constexpr glm::u16vec2 inline GetSize() const;
					GLuint inline GetTexture() const;
                #pragma endregion

                #pragma region //Members
                private:
                    glm::u16vec2 mSize;
                    GLuint mTexture;
                #pragma endregion
                };

                CLASS_EXCEPTION(BloomFramebuffer)

                constexpr static std::size_t cNumMips = 5;
                using MipChain = std::array< BloomMip, cNumMips>;
            #pragma endregion

            #pragma region //Constructor & Destructor
                BloomFramebuffer(const glm::u16vec2 size);
                ~BloomFramebuffer();
            #pragma endregion

            #pragma region //Methods
                void Bind() const;
                const MipChain& GetMipChain() const;
            #pragma endregion

            #pragma region //Members
            private:
                GLuint mTexture;
                MipChain mMipChain;
            #pragma endregion
            };

            // ------------------------------------------------------------------------
            /*! Get Size
            *
            *   Returns the size of the MIP
            */ //----------------------------------------------------------------------
            constexpr glm::u16vec2 BloomFramebuffer::BloomMip::GetSize() const {
                return mSize;
            }
            
            // ------------------------------------------------------------------------
            /*! Get Texture
            *
            *   Returns the handle of the MIP
            */ //----------------------------------------------------------------------
            GLuint BloomFramebuffer::BloomMip::GetTexture() const {
                return mTexture;
            }
		}
	}
}

#endif
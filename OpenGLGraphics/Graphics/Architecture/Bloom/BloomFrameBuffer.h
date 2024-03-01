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
#include <vector>
#include "CommonDefines.h"

namespace Graphics {
	namespace Architecture {
		namespace Bloom {
            class BloomFBO {
                struct bloomMip {
                    glm::vec2 size;
                    glm::ivec2 intSize;
                    unsigned int texture;
                };
            public:
                CLASS_EXCEPTION(BloomFBO)
                BloomFBO();
                ~BloomFBO();
                bool Init(unsigned int windowWidth, unsigned int windowHeight, unsigned int mipChainLength);
                void Destroy();
                void BindForWriting();
                const std::vector<bloomMip>& MipChain() const;

            private:
                bool mInit;
                unsigned int mFBO;
                std::vector<bloomMip> mMipChain;
            };
		}
	}
}

#endif
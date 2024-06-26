//
//	TextureImporter.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 01/03/24
//	Copyright � 2024. All Rights reserved
//

#ifndef _TEXTURE_IMPORTER__H_
#define _TEXTURE_IMPORTER__H_

#include "Core/Assets/ResourceManager.h"

namespace Core {
	namespace Assets {
		class TextureImporter : public IResourceImporter {
		public:
			DONTDISCARD std::shared_ptr<IResource> ImportFromFile(const std::string_view& filename) const override;
		};
	}
}

#endif
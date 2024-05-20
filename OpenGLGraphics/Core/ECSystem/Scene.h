//
//	Scene.h
//	OpenGL Graphics
//
//	Created by Diego Revilla on 01/03/24
//	Copyright © 2024. All Rights reserved
//

#ifndef _SCENE__H_
#define _SCENE__H_

#include <algorithm>
#include <execution>
#include "Parser.h"
#include "Graphics/Primitives/Renderables.h"
#include "Core/ParticleSystem/ParticleManager.h"
#include <iostream>
#include "Graphics/OpenGLPipeline.h"
#include "Graphics\Primitives\GLBMesh.h"

namespace Core {
	class Renderable;
	class Scene {
#pragma region //Functions
	public:
		void CreateScene(const std::string_view& file, std::function<void(const std::shared_ptr<Object>& obj)>);
		void Tick();
		void Save(const std::string_view& file);
		void ClearScene();

		const std::vector<std::shared_ptr<Core::Object>>& GetObjects() const {
			return mObjects;
		}

		void addObject(const std::shared_ptr<Object>& obj) {
			//mObjects.push_back(obj);
			mObjects.emplace_back(std::move(obj));
		}

		void removeObject(const std::shared_ptr<Object>& obj) {
			mObjects.erase(std::remove(mObjects.begin(), mObjects.end(), obj), mObjects.end());
		}

		
		Octree<::Graphics::Primitives::Mesh>& GetOctree() {
			return m_octree;
		}
	


		void CreateOctree(int levels, int sizebit);
		void AddObjToNode(::Graphics::Primitives::Mesh& mesh, Octree<::Graphics::Primitives::Mesh>::node* node);
		void EraseObjFromPrevNode(::Graphics::Primitives::Mesh& mesh, Octree<::Graphics::Primitives::Mesh>& tree);
		void CheckFrustrumObjectCollisions(Octree<::Graphics::Primitives::Mesh>::node* node, frustrum const& frus);
		void OctreeCheck(frustrum const& frustum);
		void FrustumCheck(frustrum const& frustum);
		void AABBDrawAll(Core::Graphics::OpenGLPipeline& pipeline);

		// Stats
		int stat_draw_calls = 0;
		int stat_frustum_aabb_checks = 0;
		int stat_frustum_aabb_positive = 0;

		bool debugDrawALL = false;

#pragma endregion	

#pragma region //Variables
	private:
		SceneParser mParser;
		std::vector<std::shared_ptr<Object>> mObjects;

		Octree<::Graphics::Primitives::Mesh> m_octree;


#pragma endregion
	};
}

#endif
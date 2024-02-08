#include "AnimationComponent.h"
#include "Parser.h"
#include <SDL.h>

namespace Behaviors {
	void AnimationComponent::Update() {
		if (!GetParent().expired()) {
			auto parent = GetParent().lock();
		
			parent->SetPosition({
				cos(mElapsed + PI / 2) * 50 + mOriginal.x,
				sin(mElapsed / 2 + PI / 2) * 10 + mOriginal.y,
				sin(mElapsed + PI / 2) * 50 + mOriginal.z
				});

			mElapsed += 0.016;
		}
	}

	void AnimationComponent2::Update() {
		if (!GetParent().expired()) {
			auto parent = GetParent().lock();

			parent->SetPosition({
				cos(mElapsed) * 50 + mOriginal.x,
				sin(mElapsed / 2) * 10 + mOriginal.y,
				sin(mElapsed) * 50 + mOriginal.z
				});

			mElapsed += 0.016;
		}
	}

	void AnimationComponent3::Update() {
		if (!GetParent().expired()) {
			auto parent = GetParent().lock();

			parent->SetPosition({
				mOriginal.x,
				sin(mElapsed) * 50 + mOriginal.y,
				mOriginal.z
				});

			mElapsed += 0.016;
		}
	}

	void AnimationComponent4::Update() {
		if (!GetParent().expired()) {
			auto parent = GetParent().lock();

			parent->SetPosition({
				cos(mElapsed) * 50 + mOriginal.y,
				sin(mElapsed / 2) * 10 + mOriginal.x,
				sin(mElapsed) * 50 + mOriginal.z
				});

			mElapsed += 0.016;
		}
	}
}
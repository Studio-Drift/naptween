/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

// External Includes
#include <nap/core.h>
#include <nap/logger.h>
#include <iostream>
#include <utility/stringutils.h>

// Local Includes
#include "tweenservice.h"
#include "tween.h"

RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::TweenService)
RTTI_CONSTRUCTOR(nap::ServiceConfiguration*)
RTTI_END_CLASS

namespace nap
{
	TweenService::TweenService(ServiceConfiguration* configuration) :
		Service(configuration)
	{ }


	TweenService::~TweenService()
	{ }


	bool TweenService::init(nap::utility::ErrorState& errorState)
	{
        mMainThreadId = std::this_thread::get_id();

		return true;
	}


	void TweenService::update(double deltaTime)
	{
		// remove any killed tweens
        while(!mTweensToRemove.empty())
        {
            auto* tween = mTweensToRemove.front();
            mTweensToRemove.pop();

            // if the tween is not complete, call the killed signal
            if(!tween->mComplete)
                tween->KilledSignal();

            // remove the tween from the vector
            mTweens.erase(std::remove_if(mTweens.begin(), mTweens.end(),
                                         [&tween](const std::unique_ptr<TweenBase>& t) { return t.get() == tween; }), mTweens.end());
        }

        // update tweens
        for(auto& tween : mTweens)
            tween->update(deltaTime);
	}


	void TweenService::shutdown()
	{
        mTweens.clear();
	}


	void TweenService::removeTween(TweenBase* tween)
	{
        assert(std::this_thread::get_id() == mMainThreadId); // ensure this is called from the main thread

        mTweensToRemove.emplace(tween);
	}
}
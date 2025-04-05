/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "tweenhandle.h"
#include "tweenservice.h"
#include "tween.h"

//////////////////////////////////////////////////////////////////////////
// RTTI
//////////////////////////////////////////////////////////////////////////

RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::TweenHandleFloat)
        RTTI_CONSTRUCTOR(nap::TweenService&, nap::TweenFloat*)
RTTI_END_CLASS

RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::TweenHandleDouble)
        RTTI_CONSTRUCTOR(nap::TweenService&, nap::TweenDouble*)
RTTI_END_CLASS

RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::TweenHandleVec2)
        RTTI_CONSTRUCTOR(nap::TweenService&, nap::TweenVec2*)
RTTI_END_CLASS

RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::TweenHandleVec3)
        RTTI_CONSTRUCTOR(nap::TweenService&, nap::TweenVec3*)
RTTI_END_CLASS

namespace nap
{
	TweenHandleBase::TweenHandleBase(TweenService& tweenService)
		: mService(tweenService)
	{ }

	TweenHandleBase::~TweenHandleBase()
	{
		mService.removeTween(mTweenBase);
	}
}
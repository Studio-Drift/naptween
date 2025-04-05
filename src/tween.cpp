/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "tween.h"

//////////////////////////////////////////////////////////////////////////
// RTTI
//////////////////////////////////////////////////////////////////////////

RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::TweenFloat)
        RTTI_CONSTRUCTOR(float, float, float)
RTTI_END_CLASS

RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::TweenDouble)
        RTTI_CONSTRUCTOR(double, double, float)
RTTI_END_CLASS

RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::TweenVec2)
        RTTI_CONSTRUCTOR(glm::vec2, glm::vec2, float)
RTTI_END_CLASS

RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::TweenVec3)
        RTTI_CONSTRUCTOR(glm::vec3, glm::vec3, float)
RTTI_END_CLASS

namespace nap
{
}
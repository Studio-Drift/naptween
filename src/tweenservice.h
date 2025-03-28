/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

// External Includes
#include <nap/service.h>
#include <rtti/factory.h>
#include <concurrentqueue.h>

// local includes
#include "tweeneasing.h"
#include "tween.h"
#include "tweenhandle.h"
#include "tweenmode.h"

namespace nap
{
	//////////////////////////////////////////////////////////////////////////

	/**
	 * The TweenService is responsible for creating, updating and retaining Tweens created by the TweenService
	 * Once you call createTween<T> on the TweenService. It will construct a new Tween and keep the unique_ptr to the Tween stored internally.
	 * Then, the TweenService will make a handle to that tween and return a unique_ptr to the TweenHandle which needs to be managed outside the TweenService ( typically, by the class from where you call the createTween<T> method )
	 * The function of the Handle is to provide the user access to the Tween without having to worry about memory managament. Once the unique_ptr of the handle is out of scope, the created handle will be deconstructed and notify the TweenService to mark the Tween for deletion
	 * This is to prevent memory access violations that can occur when you dispose a Tween during a call to its Update, Killed or Complete signal
	 */
	class NAPAPI TweenService : public Service
	{
		friend class TweenHandleBase;

		RTTI_ENABLE(Service)
	public:

		/**
		 * Constructor
		 */
		TweenService(ServiceConfiguration* configuration);

		/**
		 * Deconstructor
		 */
		virtual ~TweenService();

		/**
		 * creates a Tween, service retains unique_ptr to tween and returns a TweenHandle that enables the user to have access to the Tweens functionality outside the TweenService
		 * Once the handle is deconstructed, it will notify the service that the Tween can be deleted. The tween will then be deleted during the update loop but outside the scope of any Update, Complete or Killed signals
		 * Will assert if duration is negative or when called from outside the main thread
		 * @tparam T the value type to tween
		 * @param startValue the start value of the tween
		 * @param endValue the end value of the tween
		 * @param duration the duration of the tween
		 */
		template<typename T>
		std::unique_ptr<TweenHandle<T>> createTween(T startValue, T endValue, float duration);
	protected:

		/**
		 * registers all objects that need a specific way of construction
		 * @param factory the factory to register the object creators with
		 */
		virtual void registerObjectCreators(rtti::Factory& factory) override;

		/**
		 * initializes service
		 * @param errorState contains any errors
		 * @return returns true on successful initialization
		 */
		virtual bool init(nap::utility::ErrorState& errorState) override;

		/**
		 * updates any tweens
		 * @param deltaTime deltaTime
		 */
		virtual void update(double deltaTime) override;

		/**
		 * called when service is shutdown, deletes all tweens
		 */
		void shutdown() override;
	private:
		/**
		 * removes a tween, called by tween handle, thread safe
		 * @param pointer to tween
		 */
		void removeTween(TweenBase* tween);

		// vector holding the tweens
		std::vector<std::unique_ptr<TweenBase>> mTweens;

		// vector holding tweens that need to be removed
		moodycamel::ConcurrentQueue<TweenBase*> mTweensToRemove;

        // used to check if tween is created on main thread
        std::thread::id mMainThreadId;
	};

	//////////////////////////////////////////////////////////////////////////
	// template definition
	//////////////////////////////////////////////////////////////////////////

	template<typename T>
	std::unique_ptr<TweenHandle<T>> TweenService::createTween(T startValue, T endValue, float duration)
	{
        // check if duration is negative
        assert(duration > 0.0f);

        // check if called from main thread
        assert(std::this_thread::get_id() != mMainThreadId);

		// construct tween
		std::unique_ptr<Tween<T>> tween = std::make_unique<Tween<T>>(startValue, endValue, duration);

		// construct handle
		std::unique_ptr<TweenHandle<T>> tween_handle = std::make_unique<TweenHandle<T>>(*this, tween.get());

		// move ownership of tween
        mTweens.emplace_back(std::move(tween));

		// return unique_ptr to handle
		return std::move(tween_handle);
	}
}

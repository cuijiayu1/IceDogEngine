#pragma once
#include "../../Utils/Common/UtilBasic.h"
#include "Components/ActorComponent.h"
#include "Components/EventComponent.h"
#include "../../Core/VectorSpace.h"

namespace IceDogGameplay
{
	// actor will auto regist to the engine, we don`t need to
	using IceDogCore::float3;

	class Actor
	{
	public:
		Actor();
		~Actor();

		/* move the actor forward */
		void MoveForward(float distance);

		/* move right in current direction */
		void MoveRight(double distance);

		/* get the world location of this actor */
		float3 GetActorLocation();

		/* set the world location of this actor */
		void SetActorLocation(const float3& newLocation);

		/* get the scale of this actor */
		float3 GetActorScale();

		/* get the look at location */
		float3 GetActorLookAt();

		/* get the up vector of this actor */
		float3 GetActorUpVector();

		/* set the scale of this actor */
		void SetActorScale(const float3& newScale);

		/* get the rotation of this actor */
		float3 GetActorRotation();

		/* set the rotation of this actor */
		void SetActorRotation(const float3& newRotate);

		/* set the actor enable, this will enable all the component to work */
		void SetEnable();

		/* set the actor to disable, so the actor will have nearly no runtime cost */
		void SetDisable();

		/*
			after component creation, the component should regist to the actor so that the 
			actor could have the authority to control his components. But this function should
			not be called manully, except for special usage. The component would call this auto
			matilly
		*/
		void RegistComponentToActor(std::shared_ptr<Component> comp);

	protected:
		/* the heart of this actor */
		virtual void Tick(float deltaTime);


	private:
		void LayerTick(float deltaTime);
		void BeforTick(float deltaTime);
		void AfterTick(float deltaTime);

	protected:
		// the event handle component that bridge useful event here
		EventComponent r_defaultEventComponent;

	private:
		// the vector coordinate of this obj
		IceDogCore::BasicSpace* r_defaultActorCoordinate;
		// the actor component that every actor have one. just the default one
		ActorComponent r_defaultActorComponent;
		// hold the components
		std::vector<std::shared_ptr<Component>> r_holdComponents;
	};
}


#include "GamepadControlComponent.h"
#include <Input.h>
#include <Helpers.h>

dae::GamepadControlComponent::GamepadControlComponent( GameObject* pParent, float speed )
	: Component( pParent )
	, m_Speed( speed )
{
}

void dae::GamepadControlComponent::Update()
{
	DoLeftAxis();
	DoRightAxis(); // In case the controler we'll be using for the exam is secretly a right joystick
}

void dae::GamepadControlComponent::DoLeftAxis()
{
	if ( !m_TransformRef.Validate() )
	{
		m_TransformRef = GetParent()->GetComponent<TransformComponent>();
		assert( m_TransformRef.Validate() && "Parent must have transform" );
	}

	auto leftAxis{ InputManager::GetInstance().PollAxis( Gamepad::Axis::lStick ) };

	constexpr float deadZone{ 0.15f };
	const float axisPercent{ static_cast<float>( leftAxis.first ) / INT16_MAX };

	if ( axisPercent < deadZone && axisPercent > -deadZone )
	{
		return;
	}

	// Exponential response curve
	const float axisExponential{ std::pow( 2.f, std::abs( axisPercent * 5.f ) - 5.f ) *
								 ( axisPercent / std::abs( axisPercent ) ) };
	const float movementX{ m_Speed * axisExponential * Timer::GetInstance().GetElapsed() };
	m_TransformRef->Move( movementX, 0.f );
}

void dae::GamepadControlComponent::DoRightAxis()
{
	if ( !m_TransformRef.Validate() )
	{
		m_TransformRef = GetParent()->GetComponent<TransformComponent>();
		assert( m_TransformRef.Validate() && "Parent must have transform" );
	}

	auto rightAxis{ InputManager::GetInstance().PollAxis( Gamepad::Axis::rStick ) };

	constexpr float deadZone{ 0.15f };
	const float axisPercent{ static_cast<float>( rightAxis.first ) / INT16_MAX };

	if ( axisPercent < deadZone && axisPercent > -deadZone )
	{
		return;
	}

	// Exponential response curve
	const float axisExponential{ std::pow( 2.f, std::abs( axisPercent * 5.f ) - 5.f ) *
								 ( axisPercent / std::abs( axisPercent ) ) };
	const float movementX{ m_Speed * axisExponential * Timer::GetInstance().GetElapsed() };
	m_TransformRef->Move( movementX, 0.f );
}

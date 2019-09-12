#include "car.hpp"
#include <SDL2_gfxPrimitives.h>
#include <vector>

Car::~Car() { }

void Car::Update(float dt)
{
	direction.Rotate(currentDirection * turnSpeed * dt);
	currentDirection = None;

	speed += currentAcceleration * dt;
	if (speed > terminalVelocity) speed = terminalVelocity;
	if (speed < -terminalVelocity) speed = -terminalVelocity;
	
	position += direction * speed * dt;
	currentAcceleration = 0;

	for (int i = 0; i < 4; i++)
	{
		transformedCarPoints[i] = carPoints[i];
		transformedCarPoints[i].Rotate(direction.Angle());
		transformedCarPoints[i] += position;
	}
}

void Car::Render(SDL_Renderer* renderer)
{
	filledTrigonRGBA(
		renderer,
		(Sint16)transformedCarPoints[0].x,
		(Sint16)transformedCarPoints[0].y,
		(Sint16)transformedCarPoints[1].x,
		(Sint16)transformedCarPoints[1].y,
		(Sint16)transformedCarPoints[2].x,
		(Sint16)transformedCarPoints[2].y,
		255, 0, 0, 255);

	filledTrigonRGBA(
		renderer,
		(Sint16)transformedCarPoints[0].x,
		(Sint16)transformedCarPoints[0].y,
		(Sint16)transformedCarPoints[2].x,
		(Sint16)transformedCarPoints[2].y,
		(Sint16)transformedCarPoints[3].x,
		(Sint16)transformedCarPoints[3].y,
		255, 0, 0, 255);
}

void Car::Respawn()
{
	position = spawnpoint;
	direction = spawndirection;
}

void Car::TurnDirection(CarDirection direction)
{
	currentDirection = direction;
}

void Car::Accelerate()
{
	currentAcceleration = acceleration;
}

void Car::Deccelerate()
{
	currentAcceleration = -acceleration;
}

const Vector2D Car::GetPosition()
{
	return position;
}

Vector2D* Car::GetTransformedVertices(int* size)
{
	if (size)
		(*size) = 4;

	return transformedCarPoints;
}
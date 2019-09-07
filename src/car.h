#pragma once
#include "math/vector2d.h"
#include <SDL2/SDL.h>

enum CarDirection
{
	Left = -1,
	None = 0,
	Right = 1
};

class Car
{
private:
	Vector2D spawnpoint, spawndirection;
	Vector2D direction;
	Vector2D position;
	float speed;
	float currentAcceleration;
	CarDirection currentDirection;

	static constexpr const float acceleration = 100;
	static constexpr const float turnSpeed = 2;
	static constexpr const float terminalVelocity = 100;

	Vector2D transformedCarPoints[4];
	Vector2D carPoints[4] =
	{
		{-10, 5},
		{10, 5},
		{10, -5},
		{-10, -5}
	};

public:
	Car(Vector2D spawnpoint, Vector2D spawndirection)
		: spawnpoint(spawnpoint), spawndirection(spawndirection) 
	{
		position = spawnpoint;
		direction = spawndirection;
		speed = 0;
		currentAcceleration = 0;
		currentDirection = None;
	}

	~Car();

	void Update(float dt);
	void Render(SDL_Renderer* renderer);
	void Respawn();

	void TurnDirection(CarDirection direction);
	void Accelerate();
	void Deccelerate();

	Vector2D* GetTransformedVertices(int* size);
	const Vector2D GetPosition();
};

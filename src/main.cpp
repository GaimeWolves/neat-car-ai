#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <iostream>
#include <string>
#include <cfloat>
#include <ctime>
#include <cstdlib>

#include "car.hpp"
#include "math/spline.hpp"

#include "NEAT/neat.hpp"

NEAT neat = NEAT(3, 1, {"Bias", "A", "B", "O"}, 100);

Spline spline, borderLeft, borderRight;
Car* car;
float fitness = 0;

float trackWidth = 50;
int selectedPoint = -1;

float dt = 0.2f;

SDL_Window* window = nullptr;
SDL_Surface* screenSurface = nullptr;
SDL_Renderer* renderer = nullptr;

Uint64 NOW = SDL_GetPerformanceCounter();
Uint64 LAST = 0;
double deltaTime = 0;

void DrawTrack()
{
	float res = 0.25f;
	Vector2D pL1;
	Vector2D pR1;
	Vector2D pL2;
	Vector2D pR2;
	for (float t = 0; t < spline.Points.size(); t += res)
	{
		pL1 = borderLeft.GetSplinePoint(t, true);
		pR1 = borderRight.GetSplinePoint(t, true);
		pL2 = borderLeft.GetSplinePoint(fmod((t + res), spline.Points.size()), true);
		pR2 = borderRight.GetSplinePoint(fmod((t + res), spline.Points.size()), true);

		filledTrigonRGBA(renderer, pL1.x, pL1.y, pR1.x, pR1.y, pL2.x, pL2.y, 255, 255, 255, 255);
		filledTrigonRGBA(renderer, pL2.x, pL2.y, pR1.x, pR1.y, pR2.x, pR2.y, 255, 255, 255, 255);
	}

	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	Vector2D old = spline.GetSplinePoint(dt, true);
	for (float t = dt; t < spline.Points.size(); t += dt)
	{
		Vector2D point = spline.GetSplinePoint(t, true);
		SDL_RenderDrawLine(renderer, old.x, old.y, point.x, point.y);
		old = point;
	}

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	old = borderLeft.GetSplinePoint(0, true);
	for (float t = dt; t < borderLeft.Points.size(); t += dt)
	{
		Vector2D point = borderLeft.GetSplinePoint(t, true);
		SDL_RenderDrawLine(renderer, old.x, old.y, point.x, point.y);
		old = point;
	}

	old = borderRight.GetSplinePoint(0, true);
	for (float t = dt; t < borderRight.Points.size(); t += dt)
	{
		Vector2D point = borderRight.GetSplinePoint(t, true);
		SDL_RenderDrawLine(renderer, old.x, old.y, point.x, point.y);
		old = point;
	}

	Vector2D p = spline.GetSplinePoint(0, true);
	Vector2D g = spline.GetSplineGradient(0, true);
	float r = atan2(-g.y, g.x);
	thickLineRGBA(
		renderer, 
		(int)(trackWidth / 2 * sin(r) + p.x),
		(int)(trackWidth / 2 * cos(r) + p.y),
		(int)(-trackWidth / 2 * sin(r) + p.x),
		(int)(-trackWidth / 2 * cos(r) + p.y),
		4,
		0, 255, 0, 255
	);

	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_Rect rect;
	for (int i = 0; i < spline.Points.size(); i++)
	{
		p = spline.Points[i];
		rect = {(int)p.x - 2, (int)p.y - 2, 4, 4};
		SDL_RenderFillRect(renderer, &rect);
	}
}

void UpdatePaths()
{
	borderLeft.Points.clear();
	borderRight.Points.clear();

	for (int i = 0; i < spline.Points.size(); i++)
	{
		Vector2D g = spline.GetSplineGradient(i, true);
		Vector2D p = spline.Points[i];

		float r = atan2(-g.y, g.x);
		borderLeft.Points.push_back({(float)(trackWidth / 2 * sin(r) + p.x), (float)(trackWidth / 2 * cos(r) + p.y)});
		borderRight.Points.push_back({(float)(-trackWidth / 2 * sin(r) + p.x), (float)(-trackWidth / 2 * cos(r) + p.y)});
	}
}

void HandleInput(SDL_Event event)
{
	if (event.type == SDL_MOUSEMOTION)
	{
		if (selectedPoint != -1)
		{
			int x, y;
			SDL_GetMouseState(&x, &y);
			spline.Points[selectedPoint] = {(float)x, (float)y};
			UpdatePaths();
		}
	}
	else if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		float x, y;
		int tX, tY;
		uint32_t btn = SDL_GetMouseState(&tX, &tY);
		x = (float)tX;
		y = (float)tY;

		if (selectedPoint == -1 && btn & SDL_BUTTON(SDL_BUTTON_LEFT))
		{
			for (int i = 0; i < spline.Points.size(); i++)
			{
				Vector2D p = spline.Points[i];
				if (euclidDist(p, {x, y}) < 10)
					selectedPoint = i;
			}

		//Still no selected point? Check if new point will be placed!
		if (selectedPoint == -1)
		{
			int lowestIndex = -1, shortestDist = INT32_MAX;

				for (int i = 0; i < spline.Points.size(); i++)
				{
					int index = (i + 1) % (spline.Points.size());

					Vector2D a = spline.Points[i];
					Vector2D b = spline.Points[index];

					float dstToPoints = euclidDist(a, {x, y}) + euclidDist(b, {x, y});

					if (dstToPoints < euclidDist(a, b) * 1.1f && dstToPoints < shortestDist)
					{
						shortestDist = dstToPoints;
						lowestIndex = index;
					}
				}

				if (lowestIndex != -1)
				{
					spline.Points.insert(spline.Points.begin() + lowestIndex, {x, y});
					UpdatePaths();
				}
			}
		}
		else if (btn & SDL_BUTTON(SDL_BUTTON_RIGHT))
		{
			for (int i = 0; i < spline.Points.size(); i++)
			{
				Vector2D p = spline.Points[i];
				if (euclidDist(p, {x, y}) < 10)
				{
					spline.Points.erase(spline.Points.begin() + i);
					UpdatePaths();
				}
			}
		}
	}
	else if (event.type == SDL_MOUSEBUTTONUP)
	{
		int x, y;
		uint32_t btn = SDL_GetMouseState(&x, &y);

		if (selectedPoint != -1 && !(btn & SDL_BUTTON(SDL_BUTTON_LEFT)))
			selectedPoint = -1;
		}
		else if (event.type == SDL_KEYDOWN)
		{
			if (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_DELETE])
			{
				float x, y;
				int tX, tY;
				uint32_t btn = SDL_GetMouseState(&tX, &tY);
				x = (float)tX;
				y = (float)tY;

				for (int i = 0; i < spline.Points.size(); i++)
				{
					Vector2D p = spline.Points[i];
					if (euclidDist(p, {x, y}) < 10)
					{
						spline.Points.erase(spline.Points.begin() + i);
						UpdatePaths();
					}
				}
			}
		}
}

int main(int argc, char* argv[])
{
	rand();
	srand(time(nullptr));

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "SDL2 could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return -1;
	}

	window = SDL_CreateWindow("Q-Learning Car AI Project", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 720, SDL_WINDOW_SHOWN);
	if (!window)
	{
		std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return -1;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer)
	{
		std::cerr << "Renderer could not be initialited! SDL_Error: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		return -1;
	}

	spline.Points = {
		{100, 350}, //Goal/Start
		{100, 550}, {100, 600}, {105, 625}, {112, 638}, {125, 645}, {150, 650}, {200, 650},	//Curve 1,
		{475, 650}, //Point for long straight line
		{750, 650}, {800, 650}, {825, 645}, {838, 638}, {845, 625}, {850, 600}, {850, 550},	//Curve 2,
					{850, 500}, {845, 475}, {838, 462}, {825, 455}, {800, 450}, {750, 450},	//Curve 3 ...
								{725, 445}, {712, 438}, {705, 425}, {700, 400}, {700, 350},
								{705, 325}, {712, 312}, {725, 305}, {750, 300}, {800, 300},
								{825, 295}, {838, 288}, {845, 275}, {850, 250}, {850, 200},
					{850, 150}, {845, 125}, {838, 112}, {825, 105}, {800, 100}, {750, 100},
		{662.5, 075}, {587.5, 125}, {512.5, 075}, {437.5, 125}, {362.5, 075}, {287.5, 125},	//Squiggly line
		{200, 100}, {150, 100}, {125, 105}, {112, 112}, {105, 125}, {100, 150}, {100, 200}	//Last curve
		};
	UpdatePaths();

	car = new Car(spline.Points[0], spline.GetSplineGradient(0, true).Normalize());

	neat.Networks[0].SetInput("Bias", -1);
	neat.Networks[0].SetInput("A", 0);
	neat.Networks[0].SetInput("B", 1);
	neat.Networks[0].Propagate();
	std::cout << neat.Networks[0].GetOutput("O");

	for (int i = 0; i < 10; i++)
	{
		for (auto n : neat.Networks)
			n.SetFitness(rand() & 100);

		neat.DoEvolutionCycle();
	}

	std::cout << neat.Networks[0] << std::endl;

	bool quit = false;
	SDL_Event event;
	while (!quit)
	{
		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();

		deltaTime = (double)((NOW - LAST) / (double)SDL_GetPerformanceFrequency());
		SDL_SetWindowTitle(window, (std::string("Q-Learning Car AI Project - FPS: " + std::to_string((int)(1 / deltaTime))).c_str()));

		while (SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT)
			{
				quit = true;
			}
			HandleInput(event);
		}

		const Uint8* keyboard = SDL_GetKeyboardState(nullptr);

		if (keyboard[SDL_SCANCODE_A])
			car->TurnDirection(Left);
		else if (keyboard[SDL_SCANCODE_D])
			car->TurnDirection(Right);

		if (keyboard[SDL_SCANCODE_W])
			car->Accelerate();
		else if (keyboard[SDL_SCANCODE_S])
			car->Deccelerate();

		Vector2D position = car->GetPosition();
		car->Update(deltaTime);
		Vector2D newPosition = car->GetPosition();

		int index1 = -1, index2 = -1;
		float distance1 = FLT_MAX, distance2 = FLT_MAX;

		for (int i = 0; i < spline.Points.size(); i++)
		{
			float dst = euclidDist(newPosition, spline.Points[i]);
			if (dst < distance1)
			{
				index1 = i;
				distance1 = dst;
			}
			else if (dst < distance2)
			{
				index2 = i;
				distance2 = dst;
			}
		}

		int iA = index1 < index2 ? index1 : index2;
		int iB = index1 > index2 ? index1 : index2;

		if (iB == spline.Points.size() - 1 && iA == 0)
		{
			iA = iB;
			iB = 0;
		}

		Vector2D newP = GetClosestPointToLine(spline.Points[iA], spline.Points[iB], newPosition);
		Vector2D oldP = GetClosestPointToLine(spline.Points[iA], spline.Points[iB], position);

		float dist = euclidDist(newP, oldP);
		if (euclidDist(oldP, spline.Points[iB]) < euclidDist(newP, spline.Points[iB]))
			dist *= -1;

		fitness += dist;

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		stringRGBA(renderer, 10, 10, std::to_string(fitness).c_str(), 255, 255, 255, 255);

		DrawTrack();
		car->Render(renderer);

		SDL_RenderPresent(renderer);
		SDL_UpdateWindowSurface(window);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	std::cin;
	return 0;
}
#pragma once
#include "Engine/olcPixelGameEngine.h"
#include <memory>
#include <cassert>
#include <algorithm>
#include <utility>
#include "Utility/Random.hpp"
#include <random>

#define PI 3.14159265358979323846f

constexpr const int N = 128; // Width & Height of square window (width, height)
constexpr const int ITERATIONS = 16; // Number of iterations
constexpr const int SCALE = 4;

constexpr const float VESCOSITY = 0.0000001f;
constexpr const float DIFFUSION = 0.0f;
constexpr const float MOTION_SPEED = 0.2f;

namespace {
	static float map(float value, float start1, float stop1, float start2, float stop2) 
	{
		return start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
	}

	//2D to 1D | x,y into index
	//#define IX(x, y) (x + y * N)
	static int IX(int x, int y)
	{
		x = std::clamp(x, 0, N - 1); // avoid array out of bounds
		y = std::clamp(y, 0, N - 1);

		return x + (y * N);
	}
}


struct Fluid
{
	int size;  
	float dt;   // delta time (affects motion speed)
	float diff; // diffusion amount (velocity of fluid)
	float visc; // viscosity (thickness of fluit)

	float* s;
	float* density;

	float* Vx; // velocityX
	float* Vy; // velocityY

	float* Vx0; // previous velocityX
	float* Vy0; // previous velocityY

	
	Fluid(float dt, float diffusion, float viscosity)
		:
		dt(dt),
		diff(diffusion),
		visc(viscosity)
	{
		size = N;
		
		this->s = new float[N * N];
		this->density = new float[N * N];

		this->Vx = new float[N * N];
		this->Vy = new float[N * N];

		this->Vx0 = new float[N * N];
		this->Vy0 = new float[N * N];

		assert(s && density && Vx && Vy && Vx0 && Vy0);

		memset(s, 0, N * N);
		memset(density, 0, N * N);
		memset(Vx, 0, N * N);
		memset(Vy, 0, N * N);
		memset(Vx0, 0, N * N);
		memset(Vy0, 0, N * N);
		
	}


	void Step()
	{
		float visc = this->visc;
		float diff = this->diff;
		float dt = this->dt;

		float* Vx = this->Vx;
		float* Vy = this->Vy;

		float* Vx0 = this->Vx0;
		float* Vy0 = this->Vy0;

		float* s = this->s;
		float* density = this->density;

		Diffuse(1, Vx0, Vx, visc, dt);
		Diffuse(2, Vy0, Vy, visc, dt);

		Project(Vx0, Vy0, Vx, Vy);

		Advect(1, Vx, Vx0, Vx0, Vy0, dt);
		Advect(2, Vy, Vy0, Vx0, Vy0, dt);

		Project(Vx, Vy, Vx0, Vy0);

		Diffuse(0, s, density, diff, dt);
		Advect(0, density, s, Vx, Vy, dt);
	}


	// Adds density into a position
	void AddDensity(int x, int y, float amount)
	{
		int index = IX(x, y);
		this->density[index] += amount;
	}

	// Adds velocity into a position
	void AddVelocity(int x, int y, float amountX, float amountY)
	{
		int index = IX(x, y);

		this->Vx[index] += amountX;
		this->Vy[index] += amountY;
	}


	/*
	Diffuse is really simple; it just precalculates a value and passes everything off to LinearSolve.
	So that means, while I know what it does, I don't really know how,
	since all the work is in that mysterious function.
	*/
	//Speading out
	void Diffuse(int b, float* x, float* x0, float diff, float dt)
	{
		float a = dt * diff * (N - 2) * (N - 2);
		LinearSolve(b, x, x0, a, 1 + SCALE * a);
	}

	// Solves linear equation (e.g: 2x + y = 3) 
	/*
	this function is mysterious, but it does some kind of solving.
	this is done by running through the whole array and setting each
	cell to a combination of its neighbors. It does this several times;
	the more iterations it does, the more accurate the results,
	but the slower things run. In the step function above,
	four iterations are used. After each iteration, it resets the
	boundaries so the calculations don't explode.
	*/
	void LinearSolve(int b, float* x, float* x0, float a, float c)
	{
		float cRecip = 1.0f / c;
		for (int k = 0; k < ITERATIONS; k++) 
		{
			for (int j = 1; j < N - 1; j++) 
			{
				for (int i = 1; i < N - 1; i++) 
				{
					x[IX(i, j)] =
						(x0[IX(i, j)]
							+ a * (x[IX(i + 1, j)]
								+ x[IX(i - 1, j)]
								+ x[IX(i, j + 1)]
								+ x[IX(i, j - 1)]
								)) * cRecip;
				}
			}

			SetBoundary(b, x);
		}
	}




	/*
	As noted above, this function sets the boundary cells at the outer edges of the this so they perfectly counteract their neighbors.
	There's a bit of oddness here which is, what is this b pramaeter? It can be 0, 1, 2, or 3, and each value has a special meaning which is not at all obvious. The answer lies is what kind of data can be passed into this function.

	We have four different kinds of data (x, y, and z velocities, plus density) floating around, and any of those four can be passed in to set_bnd. You may notice that this is exactly the number of values this parameter can have, and this is not a coincidence.

	Let's look at a boundary cell (horrible ASCII art warning):

	+---+---+
	|   |^  |
	|   |  |
	|   |  |
	+---+---+
	Here we're at the left edge of the this. The left cell is the boundary cell that needs to counteract its neighbor, the right cell. The right cell has a velocity that's up and to the left.
	The boundary cell's x velocity needs to be opposite its neighbor, but its y velocity needs to be equal to its neighbor. This will produce a result like so:

	+---+---+
	|  ^|^  |
	| / |  |
	|/  |  |
	+---+---+
	This counteracts the motion of the fluid which would take it through the wall, and preserves the rest of the motion. So you can see that what action is taken depends on which array we're dealing with; if we're dealing with x velocities, then we have to set the boundary cell's value to the opposite of its neighbor, but for everything else we set it to be the same.
	That is the answer to the mysterious b. It tells the function which array it's dealing with, and so whether each boundary cell should be set equal or opposite its neighbor's value.

	This function also sets corners. This is done very simply, by setting each corner cell equal to the average of its three neighbors.
	*/
	//Wraps Fluid & applies bouncing
	//b param tells which wall is it right left..
	void SetBoundary(int b, float* x)
	{
		
		for (int i = 1; i < N - 1; i++) 
		{
			x[IX(i, 0)] = b == 2 ? -x[IX(i, 1)] : x[IX(i, 1)];
			x[IX(i, N - 1)] = b == 2 ? -x[IX(i, N - 2)] : x[IX(i, N - 2)];
		}
		for (int j = 1; j < N - 1; j++) 
		{
			x[IX(0, j)] = b == 1 ? -x[IX(1, j)] : x[IX(1, j)];
			x[IX(N - 1, j)] = b == 1 ? -x[IX(N - 2, j)] : x[IX(N - 2, j)];
		}

		x[IX(0, 0)] = 0.5f * (x[IX(1, 0)] + x[IX(0, 1)]);
		x[IX(0, N - 1)] = 0.5f * (x[IX(1, N - 1)] + x[IX(0, N - 2)]);
		x[IX(N - 1, 0)] = 0.5f * (x[IX(N - 2, 0)] + x[IX(N - 1, 1)]);
		x[IX(N - 1, N - 1)] = 0.5f * (x[IX(N - 2, N - 1)] + x[IX(N - 1, N - 2)]);

	}






	/*
	This function is also somewhat mysterious as to exactly how it works, 
	but it does some more running through the dataand setting values, 
	with some calls to lin_solve thrown in for fun.
	*/
	void Project(float* velocX, float* velocY, float* p, float* div)
	{
		for (int j = 1; j < N - 1; j++) {
			for (int i = 1; i < N - 1; i++) {
				div[IX(i, j)] = -0.5f * (
					velocX[IX(i + 1, j)]
					- velocX[IX(i - 1, j)]
					+ velocY[IX(i, j + 1)]
					- velocY[IX(i, j - 1)]
					) / N;
				p[IX(i, j)] = 0;
			}
		}

		SetBoundary(0, div);
		SetBoundary(0, p);
		LinearSolve(0, p, div, 1, 4);

		for (int j = 1; j < N - 1; j++) {
			for (int i = 1; i < N - 1; i++) {
				velocX[IX(i, j)] -= 0.5f * (p[IX(i + 1, j)]
					- p[IX(i - 1, j)]) * N;
				velocY[IX(i, j)] -= 0.5f * (p[IX(i, j + 1)]
					- p[IX(i, j - 1)]) * N;
			}
		}
		SetBoundary(1, velocX);
		SetBoundary(2, velocY);
		
	}





	/*
	This function is responsible for actually moving things around.
	To that end, it looks at each cell in turn.In that cell, 
	it grabs the velocity, follows that velocity back in time, 
	and sees where it lands.It then takes a weighted average
	of the cells around the spot where it lands, then applies 
	that value to the current cell.
	*/
	//Motion associated with velocities
	void Advect(int b, float* d, float* d0, float* velocX, float* velocY, float dt)
	{
		float i0, i1, j0, j1;

		float dtx = dt * (N - 2);
		float dty = dt * (N - 2);

		float s0, s1, t0, t1;
		float tmp1, tmp2, x, y;

		float Nfloat = N;
		float ifloat, jfloat;
		int i, j;

			for (j = 1, jfloat = 1; j < N - 1; j++, jfloat++) 
			{
				for (i = 1, ifloat = 1; i < N - 1; i++, ifloat++)
				{
					tmp1 = dtx * velocX[IX(i, j)];
					tmp2 = dty * velocY[IX(i, j)];

					x = ifloat - tmp1;
					y = jfloat - tmp2;

					if (x < 0.5f) x = 0.5f;
					if (x > Nfloat + 0.5f) x = Nfloat + 0.5f;
					i0 = floorf(x);
					i1 = i0 + 1.0f;
					if (y < 0.5f) y = 0.5f;
					if (y > Nfloat + 0.5f) y = Nfloat + 0.5f;
					j0 = floorf(y);
					j1 = j0 + 1.0f;


					s1 = x - i0;
					s0 = 1.0f - s1;
					t1 = y - j0;
					t0 = 1.0f - t1;


					int i0i = static_cast<int>(i0);
					int i1i = static_cast<int>(i1);
					int j0i = static_cast<int>(j0);
					int j1i = static_cast<int>(j1);

					d[IX(i, j)] =
						s0 * (t0 * d0[IX(i0i, j0i)] + t1 * d0[IX(i0i, j1i)]) +
						s1 * (t0 * d0[IX(i1i, j0i)] + t1 * d0[IX(i1i, j1i)]);

			}
		}
		SetBoundary(b, d);
	}



	//Fade fluid's speading
	void Fade()
	{
		for (int i = 0; i < N * N; ++i)
		{ 
			float d = density[i];
			density[i] -= std::clamp(d - 0.1f, 0.0f, 255.0f);
		}
	}

	~Fluid()
	{			 
		delete[] this->s;
		delete[] this->density;
		delete[] this->Vx;
		delete[] this->Vy;
		delete[] this->Vx0;
		delete[] this->Vy0;
	}
};

class FluidSimulation final : public olc::PixelGameEngine
{
private:
	Fluid* fluid;
	olc::vf2d previousMousePos;

public:
	FluidSimulation(const char* title,
		int width,
		int height,
		int pixel_width, 
		int pixel_height,
		bool full_screen,
		bool vsync);
	virtual ~FluidSimulation() = default;

public:
	virtual bool OnUserCreate() override;
	virtual bool OnUserUpdate(float fElapsedTime) override;
	virtual void OnUserInput(const float& dt);
	virtual bool OnUserDestroy() override;

	
private:
	//const olc::Pixel CLEAR_COLOR = { 58, 73, 97, 255 };
	const olc::Pixel CLEAR_COLOR = olc::BLACK;
	const olc::Pixel FLUID_COLOR = olc::WHITE;


};

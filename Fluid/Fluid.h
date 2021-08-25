#pragma once
#include "../Constants.h"

class Fluid
{
public:
	Fluid();
	~Fluid();

public:
	// Update Fluid each frame
	void Update() noexcept;

	// Adds density into a position
	void AddDensity(int x, int y, float amount) noexcept;

	// Adds velocity into a position
	void AddVelocity(int x, int y, float amountX, float amountY) noexcept;

	/**
	*	Diffuse is really simple; it just precalculates a value and passes everything off to LinearSolve.
	*	So that means, while I know what it does, I don't really know how,
	*	since all the work is in that mysterious function.
	*/
	void Diffuse(int b, float* x, float* x0, float diff, float dt) noexcept;

	/**
	*	this function is mysterious, but it does some kind of solving.
	*	this is done by running through the whole array and setting each
	*	cell to a combination of its neighbors. It does this several times;
	*	the more iterations it does, the more accurate the results,
	*	but the slower things run. In the step function above,
	*	four iterations are used. After each iteration, it resets the
	*	boundaries so the calculations don't explode.
	*/
	void LinearSolve(int b, float* x, float* x0, float a, float c) noexcept;

	/**
	*	As noted above, this function sets the boundary cells at the outer edges of the this so they perfectly counteract their neighbors.
	*	There's a bit of oddness here which is, what is this b pramaeter? It can be 0, 1, 2, or 3, and each value has a special meaning which is not at all obvious. The answer lies is what kind of data can be passed into this function.
	*	
	*	We have four different kinds of data (x, y, and z velocities, plus density) floating around, and any of those four can be passed in to set_bnd. You may notice that this is exactly the number of values this parameter can have, and this is not a coincidence.
	*	
	*	Let's look at a boundary cell (horrible ASCII art warning):
	*	
	*	+---+---+
	*	|   |^  |
	*	|   |  |
	*	|   |  |
	*	+---+---+
	*	Here we're at the left edge of the this. The left cell is the boundary cell that needs to counteract its neighbor, the right cell. The right cell has a velocity that's up and to the left.
	*	The boundary cell's x velocity needs to be opposite its neighbor, but its y velocity needs to be equal to its neighbor. This will produce a result like so:
	*	
	*	+---+---+
	*	|  ^|^  |
	*	| / |  |
	*	|/  |  |
	*	+---+---+
	*	This counteracts the motion of the fluid which would take it through the wall, and preserves the rest of the motion. So you can see that what action is taken depends on which array we're dealing with; if we're dealing with x velocities, then we have to set the boundary cell's value to the opposite of its neighbor, but for everything else we set it to be the same.
	*	That is the answer to the mysterious b. It tells the function which array it's dealing with, and so whether each boundary cell should be set equal or opposite its neighbor's value.
	*	
	*	This function also sets corners. This is done very simply, by setting each corner cell equal to the average of its three neighbors.
	*/
	 void SetBoundary(int b, float* x) noexcept;

	/**
	*	This function is also somewhat mysterious as to exactly how it works,
	*	but it does some more running through the dataand setting values,
	*	with some calls to LinearSolve thrown in for fun.
	*/
	 void Project(float* velocX, float* velocY, float* p, float* div) noexcept;

	/**
	*	This function is responsible for actually moving things around.
	*	To that end, it looks at each cell in turn.In that cell,
	*	it grabs the velocity, follows that velocity back in time,
	*	and sees where it lands.It then takes a weighted average
	*	of the cells around the spot where it lands, then applies
	*	that value to the current cell.
	**/
	 void Advect(int b, float* d, float* d0, float* velocX, float* velocY, float dt) noexcept;

private: // No stack over flow please OS!
	 std::array<float, N * N> s;
	 std::array<float, N * N> density;

	 std::array<float, N * N> Vx; // velocityX
	 std::array<float, N * N> Vy; // velocityY

	 std::array<float, N * N> Vx0; // previous velocityX
	 std::array<float, N * N> Vy0; // previous velocityY

public:
	friend class FluidSimulation; // to access private members
};


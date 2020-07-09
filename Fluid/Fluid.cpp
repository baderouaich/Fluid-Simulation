#include "Fluid.h"

Fluid::Fluid(float dt, float diffusion, float viscosity)
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

void Fluid::Step()
{
	/*float visc = this->visc;
	float diff = this->diff;
	float dt = this->dt;

	float* Vx = this->Vx;
	float* Vy = this->Vy;

	float* Vx0 = this->Vx0;
	float* Vy0 = this->Vy0;

	float* s = this->s;
	float* density = this->density;*/

	Diffuse(1, Vx0, Vx, visc, dt);
	Diffuse(2, Vy0, Vy, visc, dt);

	Project(Vx0, Vy0, Vx, Vy);

	Advect(1, Vx, Vx0, Vx0, Vy0, dt);
	Advect(2, Vy, Vy0, Vx0, Vy0, dt);

	Project(Vx, Vy, Vx0, Vy0);

	Diffuse(0, s, density, diff, dt);
	Advect(0, density, s, Vx, Vy, dt);
}

void Fluid::AddDensity(int x, int y, float amount)
{
	int index = IX(x, y);
	this->density[index] += amount;
}

void Fluid::AddVelocity(int x, int y, float amountX, float amountY)
{
	int index = IX(x, y);

	this->Vx[index] += amountX;
	this->Vy[index] += amountY;
}

void Fluid::Diffuse(int b, float* x, float* x0, float diff, float dt)
{
	float a = dt * diff * (N - 2) * (N - 2);
	LinearSolve(b, x, x0, a, 1 + SCALE * a);
}

void Fluid::LinearSolve(int b, float* x, float* x0, float a, float c)
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

void Fluid::SetBoundary(int b, float* x)
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

void Fluid::Project(float* velocX, float* velocY, float* p, float* div)
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

void Fluid::Advect(int b, float* d, float* d0, float* velocX, float* velocY, float dt)
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





Fluid::~Fluid()
{
	delete[] this->s;
	delete[] this->density;
	delete[] this->Vx;
	delete[] this->Vy;
	delete[] this->Vx0;
	delete[] this->Vy0;
}

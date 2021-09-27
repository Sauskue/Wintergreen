#include "pch.h"
#include "Demos.h"

ParticleFX::Particle::Particle(int x, int y):
	x(x),
	y(y),
	v_x((std::rand() % 2) - 1),
	v_y((std::rand() % 10) + .01)
{}

void ParticleFX::OnCreate()
{
	SetSize(width, height);
	SetTitle(title);
}

void ParticleFX::OnUpdate()
{

}

void ParticleFX::OnRender()
{

}

void ParticleFX::OnDestroy()
{

}
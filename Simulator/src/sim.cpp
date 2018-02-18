/*
 * This file contains the main simulation loop for FEM.
 */

#include "sim.h"

Sim::Sim(std::shared_ptr<Tetrahedrons> tetrahedronList, std::shared_ptr<Particles> particleList) : tetras(tetrahedronList), vertices(particleList)
{}

void Sim::init()
{
	// Precompute rest deformation (Dm), volume, inverse Dm for each tetrahedron
	for(int i=0; i<tetras->numTetra; i++)
	{
		tetras->computeRestDeformation( i, vertices );
		tetras->computeInvRestDeformation( i );
		tetras->computeUndeformedVolume( i );
	}
}

void Sim::clean()
{
	//Clear all hash tables
	collisionForceMap.clear();
	elasticForceMap.clear();
	//Set forces for all vertices/particles to zero
	for(int i=0; i<vertices->numParticles; i++)
	{
		vertices->force[i] = Eigen::Matrix<T, 3, 1>::Zero();
	}
}

void Sim::eulerIntegration()
{
// TODO
}

bool Sim::checkHashElasticForces( int i, int j, Eigen::Matrix<T,3,1>& force )
{
	long key1 = 1000000000 * i + j;
	long key2 = 1000000000 * j + i;

	if( elasticForceMap.find(key1) != elasticForceMap.end() || 
		elasticForceMap.find(key2) != elasticForceMap.end() )
	{
		force = elasticForceMap.find(key1)->second;
		return true;
	}
	return false;
}

void Sim::computeElasticForce( int tetraIndex )
{
	Eigen::Matrix<T,3,3> newDeformation = Eigen::Matrix<T, 3, 3>::Zero(); //Ds
	tetras->computeNewDeformation( newDeformation, tetraIndex, vertices );

	//      Compute force = Ds(Dm inv)
	//      Compute Piola (P) -- Need a table of values
	//      Compute Energy (H)
	//      Add energy to forces (f += h)
	//      f4 += -(h1 + h2 + h3)
}

void Sim::update()
{
	// TODO
	// ***** BETTER DOUBLE CHECK WITH LADISLAV'S VIDEO BOI *****

	clean(); //clears forces and hash tables
	checkCollisions(); //Apply Forces to particles that occur through collision
	
	// Loop through tetras

	//		since 2 tetrahedrons can share one or more edges, create a hash table to check if the force along a certain edge has already been calculated.

	//      Compute new deformation (Ds)
	//      Compute force = Ds(Dm inv)
	//      Compute Piola (P) -- Need a table of values
	//      Compute Energy (H)
	//      Add energy to forces (f += h)
	//      f4 += -(h1 + h2 + h3)

	//      eulerIntegration()

	for(int i=0; i<tetras->numTetra; i++)
	{
		computeElasticForce(i);
		eulerIntegration();
	}
}

void Sim::checkCollisions()
{
	// TODO
	// First do brute force SDF for primitives
	// Later do interobject collisions with a grid+bounding box or BVH

	for(int i=0; i<vertices->numParticles; i++)
	{
		//Apply force to particle once not per tetrahedron
		//use hash table for this
	}
}

// Reads all the required files and stores stuff in respective arrays
//void tetRead()
//{
//    // read node file and store list of points (id, pos, attributes)
//    // read .ele file and store tetrahedra (id, nodes)
//    // can read more files for faces, edges, etc.
//}
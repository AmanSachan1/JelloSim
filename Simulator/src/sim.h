#pragma once
#include <Eigen/Core>
#include <Eigen/Dense>

#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <time.h>

#include <memory>

#include "tetrahedrons.h"
#include "particles.h"
#include "sdf.h"

class Sim
{
public:
	std::shared_ptr<Tetrahedrons> tetras;
	std::shared_ptr<Particles> vertices;

	Sim(std::shared_ptr<Tetrahedrons> tetrahedronList, std::shared_ptr<Particles> particleList);

	void init();
	void eulerIntegration();
	void update();
	void checkCollisions();
};
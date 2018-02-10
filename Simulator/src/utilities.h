#pragma once
#include <Eigen/Core>
#include <Eigen/Dense>

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <stdlib.h>
#include <time.h>

#include "particles.h"
#include "triangles.h"

using namespace std;
using T = double;

namespace Utils
{
	template <class T, int dim>
	void inline writePartio(std::string& particleFile, std::shared_ptr<Particles> vertices, int numVertices)
	{
		Partio::ParticlesDataMutable* parts = Partio::create();
		Partio::ParticleAttribute posH, vH, mH;
		mH = parts->addAttribute("m", Partio::VECTOR, 1);
		posH = parts->addAttribute("position", Partio::VECTOR, 3);
		vH = parts->addAttribute("v", Partio::VECTOR, 3);

		for (int i=0; i<numVertices; i++)
		{
			int idx = parts->addParticle();
			float* m = parts->dataWrite<float>(mH, idx);
			float* p = parts->dataWrite<float>(posH, idx);
			float* v = parts->dataWrite<float>(vH, idx);

			m[0] = vertices->mass[0];
			for (uint k = 0; k < 3; k++)
			{
				p[k] = vertices->pos[i][k];
				v[k] = vertices->vel[i][k];
			}
		}

		Partio::write(particleFile.c_str(), *parts);
		parts->release();
	};

	void inline create_objFile(std::string file_name, std::shared_ptr<Particles> vertices, std::shared_ptr<Triangles> triangles)
	{
		ofstream objfile;
		objfile.open (file_name);
		objfile << "# " + file_name + "\n"; //comment with file name

		objfile << "# vertices\n";
		for(int i=0; i<vertices->numParticles; i++)
		{
			objfile << "v  ";
			for (uint k = 0; k < 3; k++)
			{
				objfile << std::to_string(vertices->pos[i][k]) + "  ";
			}
			objfile << "\n";
		}

		objfile << "# faces\n";
		for(int i=0; i<triangles->numTriangles; i++)
		{
			objfile << "f  " + std::to_string(triangles->triFaceList[i][0] + 1) + "//  "
							 + std::to_string(triangles->triFaceList[i][1] + 1) + "//  "
							 + std::to_string(triangles->triFaceList[i][2] + 1) + "//  " + "\n";
		}

		objfile << "# end of obj file\n";
		objfile.close();
	}

	void inline generateTrianglesFromParticles( T deltaTime, std::shared_ptr<Particles> particles, std::string fileName )
	{
		//generate obj file
		std::shared_ptr<Triangles> triangles = std::make_shared<Triangles>();

		for(int i=0; i<particles->numParticles-2; i+=2)
		{
			triangles->addTriangle(i, i+1, i+2);
		}
		Utils::create_objFile(fileName, particles, triangles);
	}

	template<typename T>
	std::vector<T> split(const std::string& line) 
	{
		std::istringstream is(line);
		return std::vector<T>(std::istream_iterator<T>(is), std::istream_iterator<T>());
	}
};
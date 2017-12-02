#pragma once
#include <vector>
#include <array>
#include <fstream>
#include <glm/mat4x4.hpp>
#include <GL/glew.h>

//3d geometry and how to load it from disk

using namespace glm;

namespace cgj {
	
	class Mesh {
	public:
		Mesh();
		~Mesh();
		void load(std::string filename);
		void draw();
	private:
		std::vector<vec3> Vertices;
		std::vector<vec2> Texcoords;
		std::vector<vec3> Normals;

		GLuint VaoId;
		GLuint VboVertices, VboTexcoords, VboNormals;

		void createBufferObjects(bool TexcoordsLoaded, bool NormalsLoaded);
	};

	class MeshParser {
	public:
		MeshParser(std::string filename);
		~MeshParser();
		void parse();

		bool TexcoordsLoaded;
		bool NormalsLoaded;
		std::vector<vec3> vertexData;
		std::vector<vec2> texcoordData;
		std::vector<vec3> normalData;
		std::vector<unsigned int> vertexIdx, texcoordIdx, normalIdx;
	private:
		void parseLine(std::stringstream& sin);
		void parseVertex(std::stringstream& sin);
		void parseTexcoord(std::stringstream& sin);
		void parseNormal(std::stringstream& sin);
		void parseFace(std::stringstream& sin);

		std::ifstream input;
	};
}
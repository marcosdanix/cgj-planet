#pragma once
#include <vector>
#include <array>
#include <fstream>
#include <glm/mat4x4.hpp>
#include <GL/glew.h>

//3d geometry and how to load it from disk

using namespace glm;

namespace cgj {
	
	class MeshParser;
	class MeshFilter;

	class Mesh {
	public:
		Mesh();
		~Mesh();
		Mesh& operator=(Mesh& m);
		void load(std::string filename);
		void load(std::string filename, MeshFilter& filter);
		void draw();
	private:
		std::vector<vec3> Vertices;
		std::vector<vec2> Texcoords;
		std::vector<vec3> Normals;

		GLuint VaoId;
		GLuint VboVertices, VboTexcoords, VboNormals;

		size_t count;

		void createBufferObjects(bool TexcoordsLoaded, bool NormalsLoaded);
		
		MeshFilter& filter_;
		static MeshFilter unitfilter;
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

	class MeshFilter {
	public:
		virtual ~MeshFilter() {}
		virtual void filter() { vertexData = parser_->vertexData; normalData = parser_->normalData; }
		void parser(MeshParser* parser) { parser_ = parser; }
		std::vector<vec3> vertexData;
		std::vector<vec3> normalData;
	protected:
		MeshParser* parser_;
	};

	class PerlinFilter : public MeshFilter {
	public:
		PerlinFilter(float freq, float amplitude, float add, int iterations=1, float decay=2.0f);
		void filter();
	private:
		float freq_;
		float amplitude_;
		float add_;
		int iterations_;
		float decay_;
		std::vector<vec3> extendVertices();
		std::vector<vec3> recalculateNormals();
	};
}
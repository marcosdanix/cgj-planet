#include "mesh.h"
#include "error.h"
#include "util.h"
#include <sstream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/fast_square_root.hpp>

using namespace cgj;

#define VERTICES 0
#define TEXCOORDS 1
#define NORMALS 2

/////////////////////////////////////////////////////////////////////// Mesh

MeshFilter cgj::Mesh::unitfilter;

cgj::Mesh::Mesh(): VaoId(0), VboVertices(0), VboTexcoords(0), VboNormals(0), count(0), filter_(Mesh::unitfilter)
{
}

Mesh::~Mesh()
{
	glBindVertexArray(VaoId);
	glDisableVertexAttribArray(VERTICES);
	glDisableVertexAttribArray(TEXCOORDS);
	glDisableVertexAttribArray(NORMALS);
	glDeleteBuffers(1, &VboVertices);
	glDeleteBuffers(1, &VboTexcoords);
	glDeleteBuffers(1, &VboNormals);
	glDeleteVertexArrays(1, &VaoId);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Mesh & cgj::Mesh::operator=(Mesh & m)
{
	Vertices = m.Vertices;
	Texcoords = m.Texcoords;
	Normals = m.Normals;

	VaoId = m.VaoId;
	VboVertices = m.VboVertices;
	VboTexcoords = m.VboTexcoords;
	VboNormals = m.VboNormals;

	count = m.count;
	filter_ = m.filter_;

	return *this;
}

void Mesh::load(std::string filename)
{
	load(filename, filter_);
}

void Mesh::load(std::string filename, MeshFilter& filter)
{
	MeshParser parser(filename);
	filter_ = filter;
	
	parser.parse();
	
	filter.parser(&parser);
	filter.filter();

	for (unsigned int i = 0; i < parser.vertexIdx.size(); i++) {
		unsigned int vi = parser.vertexIdx[i];
		vec3 v = filter.vertexData[vi - 1];
		Vertices.push_back(v);

		if (parser.TexcoordsLoaded) {
			unsigned int ti = parser.texcoordIdx[i];
			vec2 t = parser.texcoordData[ti - 1];
			Texcoords.push_back(t);
		}

		if (parser.NormalsLoaded) {
			unsigned int ni = parser.normalIdx[i];
			vec3 n = filter.normalData[ni - 1];
			Normals.push_back(n);
		}
	}

	createBufferObjects(parser.TexcoordsLoaded, parser.NormalsLoaded);

	count = Vertices.size();

	Vertices.clear();
	Texcoords.clear();
	Normals.clear();
}

void Mesh::draw()
{
	if (count == 0) return; //Don't draw if there's no data to load

	glBindVertexArray(VaoId);
	glDrawArrays(GL_TRIANGLES, 0, count);
	glBindVertexArray(0);
	PEEK_OPENGL_ERROR("Failed drawing Vertices")
}

void Mesh::createBufferObjects(bool TexcoordsLoaded, bool NormalsLoaded)
{
	glGenVertexArrays(1, &VaoId);
	PEEK_OPENGL_ERROR("Failed generating Vertex Array")

	glBindVertexArray(VaoId);
	{
		glGenBuffers(1, &VboVertices);
		glBindBuffer(GL_ARRAY_BUFFER, VboVertices);
		glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(vec3), &Vertices[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(VERTICES);
		glVertexAttribPointer(VERTICES, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0);

		PEEK_OPENGL_ERROR("Failed adding Vertex Buffer")

		if (TexcoordsLoaded)
		{
			glGenBuffers(1, &VboTexcoords);
			glBindBuffer(GL_ARRAY_BUFFER, VboTexcoords);
			glBufferData(GL_ARRAY_BUFFER, Texcoords.size() * sizeof(vec2), &Texcoords[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(TEXCOORDS);
			glVertexAttribPointer(TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), 0);

			PEEK_OPENGL_ERROR("Failed adding Texcoords Buffer")
		}
		
		if (NormalsLoaded)
		{
			glGenBuffers(1, &VboNormals);
			glBindBuffer(GL_ARRAY_BUFFER, VboNormals);
			glBufferData(GL_ARRAY_BUFFER, Normals.size() * sizeof(vec3), &Normals[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(NORMALS);
			glVertexAttribPointer(NORMALS, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0);

			PEEK_OPENGL_ERROR("Failed adding Normals Buffer")
		}
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/////////////////////////////////////////////////////////////////////// MeshParser

cgj::MeshParser::MeshParser(std::string filename): input(std::ifstream(filename))
{
}

cgj::MeshParser::~MeshParser()
{
	vertexData.clear();
	texcoordData.clear();
	normalData.clear();
	vertexIdx.clear();
	texcoordIdx.clear();
	normalIdx.clear();
}

void cgj::MeshParser::parse()
{
	std::string line;
	while (std::getline(input, line)) {
		parseLine(std::stringstream(line));
	}
	TexcoordsLoaded = (texcoordIdx.size() > 0);
	NormalsLoaded = (normalIdx.size() > 0);
}

void cgj::MeshParser::parseLine(std::stringstream & sin)
{
	std::string s;
	sin >> s;
	if (s.compare("v") == 0) parseVertex(sin);
	else if (s.compare("vt") == 0) parseTexcoord(sin);
	else if (s.compare("vn") == 0) parseNormal(sin);
	else if (s.compare("f") == 0) parseFace(sin);
}

void cgj::MeshParser::parseVertex(std::stringstream & sin)
{
	float x, y, z;
	sin >> x >> y >> z;
	vertexData.push_back(vec3(x, y, z));
}

void cgj::MeshParser::parseTexcoord(std::stringstream & sin)
{
	float u, v;
	sin >> u >> v;
	texcoordData.push_back(vec2(u, v));
}

void cgj::MeshParser::parseNormal(std::stringstream & sin)
{
	float nx, ny, nz;
	sin >> nx >> ny >> nz;
	normalData.push_back(vec3(nx, ny, nz));
}

void cgj::MeshParser::parseFace(std::stringstream & sin)
{
	std::string token;
	for (int i = 0; i < 3; i++) {
		std::getline(sin, token, '/');
		if (token.size() > 0) vertexIdx.push_back(std::stoi(token));
		std::getline(sin, token, '/');
		if (token.size() > 0) texcoordIdx.push_back(std::stoi(token));
		std::getline(sin, token, ' ');
		if (token.size() > 0) normalIdx.push_back(std::stoi(token));
	}
}


cgj::PerlinFilter::PerlinFilter(float freq, float amplitude, int iterations, float decay):
	freq_(freq),
	amplitude_(amplitude),
	iterations_(iterations),
	decay_(decay)
{
}

void cgj::PerlinFilter::filter()
{
	this->vertexData = extendVertices();
	this->normalData = recalculateNormals();
}


std::vector<vec3> cgj::PerlinFilter::extendVertices()
{
	if (!parser_->NormalsLoaded) return std::vector<vec3>();

	std::vector<vec3> modifiedVertices(parser_->vertexData.size());
	std::vector<bool> isVertexModified(parser_->vertexData.size(), false);

	for (int i = 0; i < parser_->vertexIdx.size(); ++i) {
		unsigned int j = parser_->vertexIdx[i] - 1;
		if (!isVertexModified[j]) {
			isVertexModified[j] = true;
			vec3 normal = parser_->normalData[parser_->normalIdx[i] - 1];
			vec3 pos = parser_->vertexData[j];
			pos += normal * amplitude_*perlin(pos, freq_, iterations_, decay_);
			modifiedVertices[j] = pos;
		}
	}

	return modifiedVertices;
}

std::vector<vec3> cgj::PerlinFilter::recalculateNormals()
{
	std::vector<vec3> modifiedNormals(parser_->normalData.size(), vec3(0.0));

	for (int i = 0; i < parser_->vertexIdx.size(); i += 3) {
		vec3 v0 = vertexData[parser_->vertexIdx[i] - 1];
		vec3 v1 = vertexData[parser_->vertexIdx[i + 1] - 1];
		vec3 v2 = vertexData[parser_->vertexIdx[i + 2] - 1];
		vec3 u = v1 - v0;
		vec3 v = v2 - v0;
		vec3 n = fastNormalize(cross(u, v));
		modifiedNormals[parser_->normalIdx[i] - 1] += n;
		modifiedNormals[parser_->normalIdx[i + 1] - 1] += n;
		modifiedNormals[parser_->normalIdx[i + 2] - 1] += n;
	}

	for (int i = 0; i < modifiedNormals.size(); ++i) {
		modifiedNormals[i] = normalize(modifiedNormals[i]);
	}

	return modifiedNormals;
}
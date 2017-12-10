#pragma once
#include <string>
#include <stack>
#include <functional>
#include <glm/fwd.hpp>
#include "transform.h"
#include "shader.h"
#include "mesh.h"
#include "util.h"
#include "camera.h"

//Scene and Node related code

namespace cgj {

	const std::string ModelAttributeName = "Model";
	const std::string ViewAttributeName = "View";
	const std::string ProjectionAttributeName = "Projection";
	//Normal matrix is in eye space (View * Model)
	const std::string NormalAttributeName = "Normal";
	
	class Node;
	class Scene;
	class Camera;
	
	class Node {
		friend class NodeIterator;
	public:
		Node();
		Node(Transform& transform);
		
		//if this Node has a child, does addNodeBack to that child
		Node& addChild(Node* child);
		Node* getChildren();
		Node* removeAllChildren();
		Node* next();
		
		//adds a node to the end of the list
		//returns itself
		Node& addNodeBack(Node* node);
		//removes the node from its list, and fixes the connection with its parent
		void removeNodeList();

		void draw(Camera& camera);
		void update();

		Transform& transform();
		Node& transform(Transform& t);
		Node& mesh(Mesh& mesh);
		Node& shader(ShaderProgram& shader);
		Node& beforeDraw(std::function<void()> before);
		Node& afterDraw(std::function<void()> after);
		Node& updateFunc(std::function<void(Node&)> update);
	private:
		//Intrusive lists are not just a meme (quoting Game Engine Architecture)
		//This list is also circular FYI
		Node* next_;
		Node* prev_;
		Node* parent_;
		Node* child_; //this points to another list of node
					  
		//The stuff that's actually useful!!
		Transform transform_;
		Mesh mesh_;
		ShaderProgram shader_;
		//Is this game engine-y?
		//They are just function pointers... right?
		std::function<void()> before_;
		std::function<void()> after_;
		std::function<void(Node&)> update_;
	};

	class Scene {
	public:
		Scene();
		Scene(Camera& camera, Transform& transform = Transform());
		Node* root();
		void draw();
		void update();
		Camera& camera();
	private:
		Node root_;
		Camera camera_;
	};


	//Walk through the tree depth first
	//Uses the matrix stack to calculate the world transform of the current node
	class NodeIterator {
	public:
		NodeIterator(Node* start);
		Node* get();
		mat4 matrix();
		mat4 inverse();
		void next();
		bool isEnd();
	private:
		std::stack<Node*> start_;
		TransformStack stack_;
		Node* node_;
	};


}

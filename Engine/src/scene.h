#pragma once
#include <string>
#include "transform.h"
#include <glm/fwd.hpp>
#include <stack>
#include "util.h"

//Scene and Node related code

namespace cgj {

	const std::string ModelAttributeName = "Model";
	const std::string ViewAttributeName = "View";
	const std::string ProjectionAttributeName = "Projection";
	const std::string NormalAttributeName = "Normal";
	
	class Node;
	class Scene;
	class Camera;
	
	class Scene {
	public:
		Scene();
		Scene(Camera& camera);
	private:
		Node root_;
		Camera camera_;
	};


	class Node {
	public:
		Node();
		Node(Transform transform);
		
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

		void draw();

		Transform transform();
		Node& transform(Transform& t);
		Node& mesh(Mesh& mesh);
		Node& shader(ShaderProgram& shader);
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
		mat4 worldMatrix();

	};

	//Walk through the tree depth first
	//Uses the matrix stack to calculate the world transform of the current node
	class NodeIterator {
	public:
		NodeIterator(Node* start);
		Node* get();
		mat4 matrix();
		void next();
		bool isEnd();
	private:
		std::stack<Node*> start_;
		MatrixStack matrix_;
		Node* node_;

	};

	class Camera {

	};

}

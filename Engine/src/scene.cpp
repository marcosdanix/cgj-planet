#include "scene.h"
#include <glm/mat4x4.hpp>

using namespace cgj;

/////////////////////////////////////////////////////////////////////// Scene

Scene::Scene(): root_(), camera_()
{
}

Scene::Scene(Transform & transform): root_(transform), camera_()
{
}

Scene::Scene(Camera & camera, Transform& transform): root_(transform), camera_(camera)
{
}

Node* Scene::root()
{
	return &root_;
}

void cgj::Scene::draw()
{
	root_.draw(camera_);
}

/////////////////////////////////////////////////////////////////////// Node


cgj::Node::Node():
	next_(this),
	prev_(this),
	parent_(nullptr),
	child_(nullptr)
{
}

cgj::Node::Node(Transform & transform):
	next_(this),
	prev_(this),
	parent_(nullptr),
	child_(nullptr),
	transform_(transform)
{
}

Node& cgj::Node::addChild(Node * child)
{
	if (child_ == nullptr) {
		child_ = child;
	}
	else {
		child_->addNodeBack(child);
	}

	child_->parent_ = this;
	return *this;
}

Node * cgj::Node::getChildren()
{
	return child_;
}

Node * cgj::Node::removeAllChildren()
{
	Node* children = child_;
	child_->parent_ = nullptr;
	child_ = nullptr;
	return children;
}

Node * cgj::Node::next()
{
	return next_;
}

Node & cgj::Node::addNodeBack(Node * node)
{
	Node* prev = this->prev_;
	this->prev_ = node;
	node->prev_ = prev;
	prev->next_ = node;
	node->next_ = this;
	return *this;
}

void cgj::Node::removeNodeList()
{
	if (next_ != prev_) {
		prev_->next_ = next_;
		next_->prev_ = prev_;
	}
}

void cgj::Node::draw(Camera& camera)
{
	NodeIterator it(this);

	while (!it.isEnd()) {
		Node* node = it.get();
		//don't draw with empty shaders.
		if (!node->shader_.empty()) {	
			node->shader_.use();			
				//uniform attributes
			mat4 normal = transpose(it.inverse() * camera.inverseView());
			node->shader_
				.uniform(ModelAttributeName, it.matrix())
				.uniform(ViewAttributeName, camera.view())
				.uniform(ProjectionAttributeName, camera.projection())				
				.uniform(NormalAttributeName, normal);
			
			node->mesh_.draw();
			node->shader_.stop();
		}
		
		it.next();
	}
}

Transform& cgj::Node::transform()
{
	return transform_;
}

Node & cgj::Node::transform(Transform & t)
{
	transform_ = t;
}

Node & cgj::Node::mesh(Mesh & mesh)
{
	mesh_ = mesh;
}

Node & cgj::Node::shader(ShaderProgram & shader)
{
	shader_ = shader;
}

/////////////////////////////////////////////////////////////////////// NodeIterator

cgj::NodeIterator::NodeIterator(Node * start)
{
	start_.push(start);
	stack_.push(start->transform());
	node_ = start;
}

Node * cgj::NodeIterator::get()
{
	return node_;
}

mat4 cgj::NodeIterator::matrix()
{
	return stack_.result();
}

mat4 cgj::NodeIterator::inverse()
{
	return stack_.inverse();
}

void cgj::NodeIterator::next()
{
	Node* child = node_->child_;
	Node* next = node_->next_;

	if (child != nullptr) {
		start_.push(child);
		stack_.push(child->transform_);
		node_ = child;
	}        //end of the list
	else if (next == start_.top()) {
		do {
			//going back back back back...
			start_.pop();
			stack_.pop();
			node_ = node_->parent_;
			if (node_ == nullptr) return; //this ends the iteration in isEnd()
			next = node_->next_;
		} while (next != nullptr || next != start_.top());

		node_ = next;
		stack_.push(node_->transform_);

	}
	else {
		stack_.pop();
		node_ = node_->next_;
		stack_.push(node_->transform_);
	}
}

bool cgj::NodeIterator::isEnd()
{
	return node_ == nullptr;
}

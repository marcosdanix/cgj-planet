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
				node->shader_.uniform(ModelAttributeName, it.matrix());
				node->shader_.uniform(ViewAttributeName, camera.view());
				node->shader_.uniform(ProjectionAttributeName, camera.projection());
				mat4 normal = transpose(it.inverse() * camera.inverseView());
				node->shader_.uniform(NormalAttributeName, normal);
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

#include "scene.h"
#include <glm/mat4x4.hpp>

using namespace cgj;

/////////////////////////////////////////////////////////////////////// Scene

cgj::Scene::Scene(): camera_()
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

void cgj::Scene::update()
{
	root_.update();
}

Camera & cgj::Scene::camera()
{
	return camera_;
}

/////////////////////////////////////////////////////////////////////// Node


cgj::Node::Node():
	next_(this),
	prev_(this),
	parent_(nullptr),
	child_(nullptr),
	texture_(nullptr)
{
}

cgj::Node::Node(Transform & transform):
	next_(this),
	prev_(this),
	parent_(nullptr),
	child_(nullptr),
	texture_(nullptr),
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
			if (node->before_) node->before_();


			mat3 normal = mat3(transpose(it.inverse() * camera.inverseView()));
			node->shader_
				.uniform(ModelAttributeName, it.matrix())
				.uniform(ViewAttributeName, camera.view())
				.uniform(ProjectionAttributeName, camera.projection())
				.uniform(NormalAttributeName, normal);

			if (texture_ != nullptr) node->shader_.uniform(TextureAttributeName, *texture_);
			
			node->mesh_.draw();

			if (node->after_) node->after_();

			node->shader_.stop();
		}
		
		it.next();
	}
}

void cgj::Node::update()
{
	NodeIterator it(this);

	while (!it.isEnd()) {
		Node* node = it.get();
		if (node->update_) node->update_(*node);
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
	return *this;
}

Node & cgj::Node::mesh(Mesh & mesh)
{
	mesh_ = mesh;
	return *this;
}

Node & cgj::Node::shader(ShaderProgram & shader)
{
	shader_ = shader;
	return *this;
}

Node & cgj::Node::texture(Texture * texture)
{
	texture_ = texture;
	return *this;
}

Node & cgj::Node::beforeDraw(std::function<void()> before)
{
	before_ = before;
	return *this;
}

Node & cgj::Node::afterDraw(std::function<void()> after)
{
	after_ = after;
	return *this;
}

Node & cgj::Node::updateFunc(std::function<void(Node&)> update)
{
	update_ = update;
	return *this;
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
		} while (next == start_.top());

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

#include "Node.h"

Node::Node(SNodeType type, Node *parent)
{
	this->type = type;
	this->parent = parent;
    if (parent)
        parent->addChild(this);
    //this->name = "";
}

Node::~Node()
{
	for (auto child : children)
		deleteChild(child);
}

void Node::addChild(Node *child)
{
	children.push_back(child);
}

void Node::deleteChild(Node *child)
{
	delete child;
	children.erase(std::remove(children.begin(), children.end(), child),
		children.end());
}

void Node::removeChild(Node* child)
{
	children.erase(std::remove(children.begin(), children.end(), child),
		children.end());
}

void Node::removeChild(int childIndex)
{
	if (childIndex < 0 || childIndex >= children.size())
		return;

	children.erase(children.begin() + childIndex);
}

void Node::serialize(QDataStream& buffer)
{
    buffer << (qint32) type;
    buffer << (qint32) (name.size() + 1);
    buffer.writeRawData(name.c_str(), name.size() + 1);
}

void Node::deserialize(QDataStream& buffer)
{
    qint32 nameLength;
    buffer >> nameLength;
    char *nameBuffer = new char[nameLength];
    buffer.readRawData(nameBuffer, nameLength);
    name.assign(nameBuffer);
}

int Node::getIndex() const
{
	if (parent) {
		std::vector<Node *> siblings = parent->children;
		std::vector<Node *>::iterator index = std::find(siblings.begin(), siblings.end(), this);

		if (index != siblings.end())
			return std::distance(siblings.begin(), index);

		return -1;
	}
	return -1;
}

int Node::getChildCount() const
{
	return children.size();
}

Node* Node::getChild(int index) const
{
	if (index < 0 || index >= children.size())
		return nullptr;

	return children[index];
}

Node* Node::getParent() const
{
	return parent;
}

SNodeType Node::getType() const
{
	return type;
}
#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <QByteArray>
#include <QDataStream>

enum SNodeType
{
	SNodeType_ReturnMarker,
	SNodeType_Point,
	SNodeType_Prop,
	SNodeType_PointLight
};

class Node
{
public:
	Node(SNodeType type, Node *parent = nullptr);
	~Node();


	void addChild(Node* child);
	void deleteChild(Node *child);		// todo - recursive
	// When you get back add moveto new node that makes more sense.
	void removeChild(Node *child);
	void removeChild(int childIndex);

	// Generates a serialization of the node, first element must be the node type
	virtual void serialize(QDataStream &buffer);
    virtual void deserialize(QDataStream &buffer);

	// Retrieve index in parent's child vector
	int getIndex() const;
	int getChildCount() const;
	Node* getChild(int index) const;
	Node* getParent() const;
	SNodeType getType() const;

	SNodeType type;						// Type of node
	Node *parent;
	std::vector<Node *> children;		// todo - set up smart pointers
    std::string name;
};
#include "Map.h"

Map::Map()
{
    root = new Node(SNodeType_Point, nullptr);
    Node *light0 = new Node(SNodeType_PointLight, root);
    Node *scaffold0 = new Node(SNodeType_Point, root);
    Node *scaffold1 = new Node(SNodeType_Point, root);
    Node *truss0 = new Node(SNodeType_Prop, scaffold0);
    Node *truss1 = new Node(SNodeType_Prop, scaffold0);
    light0->name = "light0";
    scaffold0->name = "scaffold0";
    scaffold1->name = "scaffold1";
    truss0->name = "truss0";
    truss1->name = "truss1";
}

Map::~Map()
{
    delete root;
}

Node *Map::getRoot() const
{
    return root;
}

void Map::serializeTree(Node *root, QDataStream &buffer)
{
    if (root == nullptr)
        return;

    root->serialize(buffer);
    for (int i = 0; i < root->children.size(); i++) {
        serializeTree(root->children[i], buffer);
    }

    buffer << (qint32) SNodeType_ReturnMarker;
}

void Map::deserializeTree(Node* parent, QDataStream &buffer)
{
    if (buffer.atEnd())
        return;

    qint32 nodeType;
    buffer >> nodeType;

    switch (nodeType) {

    case (SNodeType_ReturnMarker):
        if (parent->getParent() == nullptr)
            return;
        deserializeTree(parent->getParent(), buffer);
        break;

    case (SNodeType_Point):
    case (SNodeType_Prop):
    case (SNodeType_PointLight):
        Node *child = new Node((SNodeType) nodeType, parent);
        child->deserialize(buffer);
        deserializeTree(child, buffer);
        break;
    }
}
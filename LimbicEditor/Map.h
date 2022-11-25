#pragma once

#include <glm/glm.hpp>
#include <QDataStream>
#include "Node.h"

class Map
{
public:
    Node *root;

    Map();
    ~Map();

    Node *getRoot() const;

    static void serializeTree(Node *root, QDataStream &buffer);
    static void deserializeTree(Node *parent, QDataStream &buffer);
};
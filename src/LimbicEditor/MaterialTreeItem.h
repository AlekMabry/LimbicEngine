#ifndef LIMBICEDITOR_MATERIALTREEITEM_H
#define LIMBICEDITOR_MATERIALTREEITEM_H


#include <QtWidgets/QTreeWidgetItem>
#include "Material.h"

class MaterialTreeItem : public QTreeWidgetItem
{

public:
    Material material;
};


#endif //LIMBICEDITOR_MATERIALTREEITEM_H

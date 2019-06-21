#ifndef LIMBICEDITOR_MATERIALTREEITEM_H
#define LIMBICEDITOR_MATERIALTREEITEM_H


#include <QtWidgets/QTreeWidgetItem>
#include "Material.h"
#include <QString>

class MaterialTreeItem : public QTreeWidgetItem
{

public:
    Material material;
    std::string file_path;
    std::string new_file_path;
};


#endif //LIMBICEDITOR_MATERIALTREEITEM_H

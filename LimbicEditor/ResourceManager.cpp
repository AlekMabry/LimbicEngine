#include "ResourceManager.h"

ResourceManager::ResourceManager(QObject* parent)
    : QObject(parent)
{

}

ResourceManager::~ResourceManager()
{

}

TextureGroup *ResourceManager::newTextureGroup() {
    QString name = "Untitled";
    int suffix = 0;
    while (true)
    {
        QString name = "Untitled" + QString::number(suffix);
        if (!textureGroups.contains(name)) {
            auto newTextureGroup = std::make_unique<TextureGroup>();
            newTextureGroup->editorPath = name;
            TextureGroup *rawPtr = newTextureGroup.get();
            textureGroups.emplace(name, std::move(newTextureGroup));
            updateAlphabeticalTextureGroups();
            return rawPtr;
        }
        suffix++;
    }
}

void ResourceManager::deleteTextureGroup(QString &editorName)
{
    std::map<QString, std::unique_ptr<TextureGroup>>::iterator it;
    it = textureGroups.find(editorName);
    if (it != textureGroups.end()) {
        textureGroups.erase(it);
        updateAlphabeticalTextureGroups();
    }
}

int ResourceManager::getTextureGroupCount()
{
    return textureGroups.size();
}

int ResourceManager::getTextureGroupPath(int index, QString &editorPath) {
    if (index < alphabeticalTextureGroups.size()) {
        editorPath = alphabeticalTextureGroups[index]->editorPath;
        return 0;
    }
    return 1;
}

void ResourceManager::updateAlphabeticalTextureGroups()
{
    alphabeticalTextureGroups.clear();
    for (std::map<QString, std::unique_ptr<TextureGroup>>::iterator it = textureGroups.begin();
        it != textureGroups.end(); ++it) {
        alphabeticalTextureGroups.push_back(it->second.get());
    }
    emit updatedTextureGroups();
}
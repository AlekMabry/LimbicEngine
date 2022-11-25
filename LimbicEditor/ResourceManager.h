#pragma once

#include <QObject>
#include <QSettings>
#include <QString>
#include <QPixmap>
#include <QDebug>
#include <stdexcept>
#include <map>

enum TextureType {
    TEXTURE_ALBEDO = 0,
    TEXTURE_NORMAL = 1,
    TEXTURE_OCCLUSIONROUGHNESSMETALLIC = 2 };

struct Texture
{
    TextureType type;
    QString importPath;
    QString resourcePath;
    QPixmap thumbnail;
    int mipmaps;
};

struct TextureGroup
{
    QString editorPath;
    std::unique_ptr<Texture[]> textures[3];
};

/*
    @brief  Manages importing, caching, and exporting compiled resources for the game.

            Editor UI classes should interface with resource state through this.
*/
class ResourceManager : public QObject
{
    Q_OBJECT

public:
    ResourceManager(QObject *parent);
    ~ResourceManager();
    
    /*
        @brief  Generate new untitled and empty texture group.

        @return Texture group handle.
    */
    TextureGroup *newTextureGroup();

    /*
        @brief Deletes texture group.
    */
    void deleteTextureGroup(QString &editorPath);

    /*
        @brief  Get number of texture groups.

        @return Number of texture groups
    */
    int getTextureGroupCount();

    /*
        @brief  Get name of texture group by index (alphabetical order)

        @param[in]  index       Index (alphabetical order) of requested texture group.
        @param[out] editorPath  Editor path of texture group if found.
        @return                 Zero if found, one if not found.
    */
    int getTextureGroupPath(int index, QString& editorPath);

signals:
    void updatedTextureGroups();

private:
    /*
        @brief  Updates alphabetical texture group vector and signals the change so that
                QAbstractItemModels can request texture groups by indexing.
    */
    void updateAlphabeticalTextureGroups();

    std::unique_ptr<QSettings> settings;

    std::map<QString, std::unique_ptr<TextureGroup>> textureGroups;
    std::vector<TextureGroup *> alphabeticalTextureGroups;
};

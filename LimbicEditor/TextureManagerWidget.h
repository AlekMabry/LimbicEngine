#pragma once

#include <QWidget>
#include <assert.h>

#include "ui_TextureManager.h";
#include "ResourceManager.h";
#include "TextureGroupListModel.h";

class TextureManagerWidget : public QWidget
{
    Q_OBJECT;

public:
    TextureManagerWidget(ResourceManager *resources, QWidget *parent = nullptr);

private slots:
    void addNewTextureGroup();
    void deleteTextureGroup();

private:
    std::unique_ptr<TextureGroupListModel> textureGroupListModel;
    ResourceManager *resources;
    TextureGroup *currentTextureGroup;
    Texture *currentTextures[3];

    Ui::TextureManager ui;
    QPixmap unusedTextureThumbnail;
    QPixmap missingTextureThumbnail;

    void disableAlbedoGroup();
    void disableNormalGroup();
    void disableOcclusionRoughnessMetallicGroup();
    void deselectTextureGroup();
};

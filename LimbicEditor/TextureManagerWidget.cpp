#include "TextureManagerWidget.h"

TextureManagerWidget::TextureManagerWidget(ResourceManager *resources, QWidget *parent)
    : QWidget(parent)
{
    this->resources = resources;
    currentTextureGroup = nullptr;
    for (int i = 0; i < 3; i++)
        currentTextures[i] = nullptr;

    unusedTextureThumbnail = QIcon(":/Thumbnails/Texture-Unused").pixmap(QSize(128, 128));
    missingTextureThumbnail = QIcon(":/Thumbnails/Texture-Missing").pixmap(QSize(128, 128));

    ui.setupUi(this);

    disableAlbedoGroup();
    disableNormalGroup();
    disableOcclusionRoughnessMetallicGroup();

    textureGroupListModel = std::make_unique<TextureGroupListModel>(resources, this);
    ui.textureListView->setModel(textureGroupListModel.get());

    connect(ui.pushButton_newTexture, SIGNAL(clicked()), this, SLOT(addNewTextureGroup()));
    connect(ui.pushButton_deleteTexture, SIGNAL(clicked()), this, SLOT(deleteTextureGroup()));
}

void TextureManagerWidget::addNewTextureGroup() {
    currentTextureGroup = resources->newTextureGroup();
    if (currentTextureGroup != nullptr) {
        ui.lineEdit_textureGroupName->setEnabled(true);
        ui.lineEdit_textureGroupName->setText(currentTextureGroup->editorPath);
    }
}

void TextureManagerWidget::deleteTextureGroup() {
    assert(currentTextureGroup != nullptr);
    resources->deleteTextureGroup(currentTextureGroup->editorPath);
    deselectTextureGroup();
}

void TextureManagerWidget::disableAlbedoGroup() {
    ui.label_albedoThumbnail->setPixmap(unusedTextureThumbnail);
    ui.label_albedoDimensions->setText("");
    ui.lineEdit_albedo->setText("");
    ui.lineEdit_albedo->setDisabled(true);
    ui.toolButton_pickAlbedo->setDisabled(true);
}

void TextureManagerWidget::disableNormalGroup() {
    ui.label_normalThumbnail->setPixmap(unusedTextureThumbnail);
    ui.label_normalDimensions->setText("");
    ui.lineEdit_normal->setText("");
    ui.lineEdit_normal->setDisabled(true);
    ui.toolButton_pickNormal->setDisabled(true);
}

void TextureManagerWidget::disableOcclusionRoughnessMetallicGroup() {
    ui.label_occlusionRoughnessMetallicThumbnail->setPixmap(unusedTextureThumbnail);
    ui.label_occlusionRoughnessMetallicDimensions->setText("");
    ui.lineEdit_occlusionRoughnessMetallic->setText("");
    ui.lineEdit_occlusionRoughnessMetallic->setDisabled(true);
    ui.toolButton_pickOcclusionRoughnessMetallic->setDisabled(true);
}

void TextureManagerWidget::deselectTextureGroup() {
    ui.lineEdit_textureGroupName->setText("");
    ui.lineEdit_textureGroupName->setEnabled(false);
    disableAlbedoGroup();
    disableNormalGroup();
    disableOcclusionRoughnessMetallicGroup();
}
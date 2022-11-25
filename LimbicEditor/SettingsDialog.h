#pragma once

#include <QDialog>
#include <QFileDialog>
#include <QSettings>
#include <map>

#include "ui_Settings.h"

class SettingsDialog : public QDialog
{
	Q_OBJECT

public:
    SettingsDialog(QWidget *parent = nullptr);

private slots:
    void setAssetImportDirectory();
    void setAssetExportDirectory();
    void saveSettings();

private:
    Ui::Settings ui;
};

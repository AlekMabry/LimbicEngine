#pragma once

#include "ui_Settings.h"

#include <QDialog>
#include <QFileDialog>
#include <QSettings>
#include <map>

class SettingsDialog : public QDialog
{
	Q_OBJECT

public:
	SettingsDialog(QWidget* parent = nullptr);

private slots:
	void setResourceImportDirectory();
	void setResourceExportDirectory();
	void saveSettings();

private:
	Ui::Settings ui;
};

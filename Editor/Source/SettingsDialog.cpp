#include <SettingsDialog.h>

SettingsDialog::SettingsDialog(QWidget* parent) : QDialog(parent)
{
	ui.setupUi(this);

	QSettings settings;

	if (settings.contains("Editor/resourceImportDirectory"))
		ui.lineEdit_resourceImportDirectory->setText(settings.value("Editor/resourceImportDirectory").toString());

	connect(ui.toolButton_resourceImportDirectory, SIGNAL(clicked()), this, SLOT(setResourceImportDirectory()));

	if (settings.contains("Editor/resourceExportDirectory"))
		ui.lineEdit_resourceExportDirectory->setText(settings.value("Editor/resourceExportDirectory").toString());

	connect(ui.toolButton_resourceExportDirectory, SIGNAL(clicked()), this, SLOT(setResourceExportDirectory()));

	connect(ui.pushButton_save, SIGNAL(clicked()), this, SLOT(saveSettings()));
	connect(ui.pushButton_close, SIGNAL(clicked()), this, SLOT(close()));
}

void SettingsDialog::setResourceImportDirectory()
{
	QString dir = QFileDialog::getExistingDirectory(
		this, tr("Resource Import Directory"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

	ui.lineEdit_resourceImportDirectory->setText(dir);
	ui.pushButton_save->setEnabled(true);
}

void SettingsDialog::setResourceExportDirectory()
{
	QString dir = QFileDialog::getExistingDirectory(
		this, tr("Resource Export Directory"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

	ui.lineEdit_resourceExportDirectory->setText(dir);
	ui.pushButton_save->setEnabled(true);
}

void SettingsDialog::saveSettings()
{
	QSettings settings;

	settings.setValue("Editor/ResourceImportDirectory", ui.lineEdit_resourceImportDirectory->text());
	settings.setValue("Editor/ResourceExportDirectory", ui.lineEdit_resourceExportDirectory->text());

	ui.pushButton_save->setEnabled(false);
}
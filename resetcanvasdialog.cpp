#include "resetcanvasdialog.h"
#include "ui_resetcanvasdialog.h"

resetCanvasDialog::resetCanvasDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::resetCanvasDialog)
{
    ui->setupUi(this);
}

resetCanvasDialog::~resetCanvasDialog()
{
    delete ui;
}

int resetCanvasDialog::getWidth()
{
    return ui->spinBoxWidth->text().toInt();
}

int resetCanvasDialog::getHeight()
{
    return ui->spinBoxHeight->text().toInt();
}

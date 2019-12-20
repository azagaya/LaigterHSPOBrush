#include "hspobrushgui.h"
#include "ui_hspobrushgui.h"

HSPOBrushGui::HSPOBrushGui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HSPOBrushGui)
{
  ui->setupUi(this);
}

HSPOBrushGui::~HSPOBrushGui()
{
  delete ui;
}

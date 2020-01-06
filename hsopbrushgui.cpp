#include "hsopbrushgui.h"
#include "ui_hsopbrushgui.h"
#include <QPainter>

HSOPBrushGui::HSOPBrushGui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HSOPBrushGui)
{
  ui->setupUi(this);
}

HSOPBrushGui::~HSOPBrushGui()
{
  delete ui;
}

void HSOPBrushGui::on_SelectButton_clicked()
{

}

void HSOPBrushGui::on_SelectButton_toggled(bool checked)
{
  selected_changed(checked);
}

void HSOPBrushGui::brush_sprite_updated(QImage sprite){
  QImage fill(QSize(200,200),QImage::Format_RGBA8888);
  fill.fill(QColor(51,51,77,255));

  QPainter p(&fill);
  if (sprite.width() > 200 || sprite.height() > 200) sprite = sprite.scaled(200,200);
  p.drawImage(QPointF(100-sprite.width()/2.0,100-sprite.height()/2.0),sprite);

  p.setPen(QColor(153,153,255));
  p.drawEllipse(100-sprite.width()/2.0,100-sprite.height()/2.0,sprite.width(),sprite.height());
  ui->imageLabel->resize(sprite.size());
  ui->imageLabel->setPixmap(QPixmap::fromImage(fill));
}

void HSOPBrushGui::on_sizeSlider_valueChanged(int value)
{
  size_changed(value);
}

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



bool HSOPBrushGui::get_height_enabled(){
  return ui->checkBoxNormal->isChecked();
}

bool HSOPBrushGui::get_parallax_enabled(){
  return ui->checkBoxParallax->isChecked();
}

bool HSOPBrushGui::get_specular_enabled(){
  return ui->checkBoxSpecular->isChecked();
}

bool HSOPBrushGui::get_occlussion_enabled(){
  return ui->checkBoxOcclussion->isChecked();
}

void HSOPBrushGui::brush_sprite_updated(QImage sprite){
  QImage fill(QSize(200,200),QImage::Format_RGBA8888);
  fill.fill(QColor(51,51,77,255));

  QPainter p(&fill);
  if (sprite.width() > 200 || sprite.height() > 200) sprite = sprite.scaled(200,200);
  p.drawImage(QPointF(100-sprite.width()/2.0,100-sprite.height()/2.0),sprite);

//  p.setPen(QColor(153,153,255));
//  p.drawEllipse(100-sprite.width()/2.0,100-sprite.height()/2.0,sprite.width(),sprite.height());
  ui->imageLabel->resize(sprite.size());
  ui->imageLabel->setPixmap(QPixmap::fromImage(fill));
}

void HSOPBrushGui::on_sizeSlider_valueChanged(int value)
{
  size_changed(value);
}

void HSOPBrushGui::on_hardnessSlider_valueChanged(int value)
{
  hardness_changed(value);
}

void HSOPBrushGui::on_sliderNormal_valueChanged(int value)
{
  height_changed(value);
}

void HSOPBrushGui::on_mixSlider_valueChanged(int value)
{
  mix_changed(value);
}

void HSOPBrushGui::on_sliderSpecular_valueChanged(int value)
{
  specular_changed(value);
}

void HSOPBrushGui::on_sliderOcclussion_valueChanged(int value)
{
  occlussion_changed(value);
}

void HSOPBrushGui::on_sliderParallax_valueChanged(int value)
{
  parallax_changed(value);
}

void HSOPBrushGui::unselect_all(){
  ui->buttonPeak->setChecked(false);
  ui->buttonRounded->setChecked(false);
  ui->buttonEraser->setChecked(false);
}

void HSOPBrushGui::on_buttonRounded_toggled(bool checked)
{
  if (checked){
    ui->buttonPeak->setChecked(false);
    ui->buttonEraser->setChecked(false);
  }
  selected_changed(checked);
}

void HSOPBrushGui::on_buttonPeak_toggled(bool checked)
{
  if (checked){
    ui->buttonRounded->setChecked(false);
    ui->buttonEraser->setChecked(false);
  }
  selected_changed(checked);
}

void HSOPBrushGui::on_buttonEraser_toggled(bool checked)
{
  if (checked){
    ui->buttonRounded->setChecked(false);
    ui->buttonPeak->setChecked(false);
  }
  selected_changed(checked);
}

bool HSOPBrushGui::get_button_eraser(){
  return ui->buttonEraser->isChecked();
}
bool HSOPBrushGui::get_button_rounded(){
  return ui->buttonRounded->isChecked();
}
bool HSOPBrushGui::get_button_peak(){
  return ui->buttonPeak->isChecked();
}

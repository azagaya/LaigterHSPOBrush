#ifndef HSOPBRUSHGUI_H
#define HSOPBRUSHGUI_H

#include <QWidget>

namespace Ui {
class HSOPBrushGui;
}

class HSOPBrushGui : public QWidget
{
  Q_OBJECT

  public:
  explicit HSOPBrushGui(QWidget *parent = nullptr);
  ~HSOPBrushGui();

  signals:
      void selected_changed(bool s);
      void size_changed(int size);
      void hardness_changed(int hardness);
      void height_changed(int height);
      void specular_changed(int spec);
      void occlussion_changed(int occ);
      void parallax_changed(int par);
      void mix_changed(int mix);

  public slots:
      void brush_sprite_updated(QImage sprite);
      bool get_height_enabled();
      bool get_parallax_enabled();
      bool get_specular_enabled();
      bool get_occlussion_enabled();

  private slots:
  void on_SelectButton_clicked();

  void on_SelectButton_toggled(bool checked);

  void on_sizeSlider_valueChanged(int value);

  void on_hardnessSlider_valueChanged(int value);

  void on_sliderNormal_valueChanged(int value);

  void on_mixSlider_valueChanged(int value);

  void on_sliderSpecular_valueChanged(int value);

  void on_sliderOcclussion_valueChanged(int value);

  void on_sliderParallax_valueChanged(int value);

  private:
  Ui::HSOPBrushGui *ui;
};

#endif // HSOPBRUSHGUI_H

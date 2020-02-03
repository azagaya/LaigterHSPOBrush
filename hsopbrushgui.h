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
      bool get_button_rounded();
      bool get_button_peak();
      bool get_button_eraser();
      void unselect_all();

  private slots:

  void on_sizeSlider_valueChanged(int value);

  void on_hardnessSlider_valueChanged(int value);

  void on_sliderNormal_valueChanged(int value);

  void on_mixSlider_valueChanged(int value);

  void on_sliderSpecular_valueChanged(int value);

  void on_sliderOcclussion_valueChanged(int value);

  void on_sliderParallax_valueChanged(int value);

  void on_buttonRounded_toggled(bool checked);

  void on_buttonPeak_toggled(bool checked);

  void on_buttonEraser_toggled(bool checked);

  private:
  Ui::HSOPBrushGui *ui;
};

#endif // HSOPBRUSHGUI_H

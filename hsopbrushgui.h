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

  public slots:
      void brush_sprite_updated(QImage sprite);

  private slots:
  void on_SelectButton_clicked();

  void on_SelectButton_toggled(bool checked);

  void on_sizeSlider_valueChanged(int value);

  private:
  Ui::HSOPBrushGui *ui;
};

#endif // HSOPBRUSHGUI_H

#ifndef HSPOBRUSHGUI_H
#define HSPOBRUSHGUI_H

#include <QWidget>

namespace Ui {
class HSPOBrushGui;
}

class HSPOBrushGui : public QWidget
{
  Q_OBJECT

  public:
  explicit HSPOBrushGui(QWidget *parent = nullptr);
  ~HSPOBrushGui();

  private:
  Ui::HSPOBrushGui *ui;
};

#endif // HSPOBRUSHGUI_H

#ifndef HSPOBRUSH_H
#define HSPOBRUSH_H

#include "hspobrushgui.h"

#include <QWidget>
#include <brushinterface.h>
#include <imageprocessor.h>

class HSPOBrush : public QObject, public BrushInterface
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "org.azagaya.laigter.plugins.BrushInterface")
  Q_INTERFACES(BrushInterface)
  public:
  void mousePress(const QPoint &pos) override;
  void mouseMove(const QPoint &oldPos, const QPoint &newPos) override;
  void mouseRelease(const QPoint &pos) override;
  bool get_selected() override;
  void set_selected(bool s) override;
  QWidget *loadGUI(QWidget *parent = nullptr) override;
  void setProcessor(ImageProcessor **processor) override;
  QIcon getIcon() override;
  QString getName() override;
  QImage getBrushSprite() override;
  void drawAt(QPoint point, QPainter *p, float alpha_mod = 1.0);
  void updateOverlay(int xmin, int xmax, int ymin, int ymax);
  void updateBrushSprite();
  QObject * getObject() override;
  signals:
  void selected_changed(BrushInterface *brush);

  public slots:
  void set_radius(int r);
  void set_max(int m);
  void set_min(int m);
  void set_mix(int m);
  void set_hardness(int h);
  void set_lineSelected(bool l);
  void set_eraserSelected(bool e);
  void set_brushSelected(bool b);

  private:
  QImage *m_heightmap, auxHeight, oldHeight;
  QImage *m_specular, auxSpecular, oldSpecular;
  QImage *m_parallax, auxParallax, oldParallax;
  QImage *m_occlussion, auxOcclussion, oldOcclussion;
  HSPOBrushGui *gui;
  QImage brushSprite;
  int radius = 20;
  int maxV = 255, minV = 0;
  ImageProcessor **processorPtr;
  ImageProcessor *m_processor;
  float alpha = 1.0;
  float hardness = 0;

  bool linesSelected = true, brushSelected = true, eraserSelected = false, selected = true;
  QString m_name = tr("HSPO Plugin");
};

#endif // HSPOBRUSH_H

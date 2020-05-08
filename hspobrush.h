#ifndef HSPOBRUSH_H
#define HSPOBRUSH_H

#include "hsopbrushgui.h"

#include <QWidget>
#include <brush_interface.h>
#include <image_processor.h>
class Overlay {
  public:
  Overlay(QImage *o, int c, QString t){
    ov = o;
    color = c;
    type = t;
  }
  QImage *ov;
  int color;
  QString type;
};

class HSPOBrush : public QObject, public BrushInterface
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID "org.azagaya.laigter.plugins.BrushInterface" FILE "metadata.json")
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
  void drawAt(QPoint point, QPainter *p, float alpha_mod = 1.0, bool tile_x = false, bool tile_y = false);
  QImage updateOverlay(int xmin, int xmax, int ymin, int ymax, QImage ov, QImage old, QImage aux);
  void updateBrushSprite();
  QObject * getObject() override;
  int WrapCoordinate(int coord, int interval);
  QPoint WorldToLocal(QPoint world);
  signals:
  void selected_changed(BrushInterface *brush);
  void brush_sprite_updated(QImage sprite);

  public slots:
  void set_radius(int r);
  void set_max(int m);
  void set_min(int m);
  void set_mix(int m);
  void set_hardness(int h);
  void set_lineSelected(bool l);
  void set_eraserSelected(bool e);
  void set_brushSelected(bool b);
  void set_height(int heigt);
  void set_specular(int s);
  void set_parallax(int p);
  void set_occlussion(int o);

  private:
  QImage *m_heightmap, auxHeight, oldHeight;
  QImage *m_specular, auxSpecular, oldSpecular;
  QImage *m_parallax, auxParallax, oldParallax;
  QImage *m_occlussion, auxOcclussion, oldOcclussion;
  QImage auxOv;
  HSOPBrushGui *gui;
  QImage brushSprite;
  int radius = 15;
  int maxV = 128, minV = 0, height = 128, spec = 255, occ = 0, parallax = 128;
  ImageProcessor **processorPtr;
  ImageProcessor *m_processor;
  float alpha = 0.5;
  float hardness = 0;

  bool linesSelected = true, brushSelected = true, eraserSelected = false, selected = true;
  QString m_name = tr("HSPO Plugin");
};

#endif // HSPOBRUSH_H

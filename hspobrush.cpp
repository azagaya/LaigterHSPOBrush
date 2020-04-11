#include "hspobrush.h"
#include <algorithm>
#include <QDebug>
#include <QRadialGradient>
#include <QtConcurrent/QtConcurrent>
#include <QIcon>
#include <QList>


QImage HSPOBrush::updateOverlay(int xmin, int xmax, int ymin, int ymax, QImage ov, QImage old, QImage aux){
  QPoint topLeft;
  QPoint botRight;

  topLeft = QPoint(xmin,ymin);
  botRight = QPoint(xmax,ymax);

  xmin -= radius;
  ymin -= radius;
  xmax += radius;
  ymax += radius;

  ymin = ymin < 0 ? 0 : ymin;
  ymin = ymin > ov.height() ? ov.height() : ymin;
  ymax = ymax < 0 ? 0 : ymax;
  ymax = ymax > ov.height() ? ov.height() : ymax;

  xmin = xmin < 0 ? 0 : xmin;
  xmin = xmin > ov.width() ? ov.width() : xmin;
  xmax = xmax < 0 ? 0 : xmax;
  xmax = xmax > ov.width() ? ov.width() : xmax;

  for (int x = xmin; x < xmax; x++){
    for (int y =ymin; y < ymax; y++){
      QColor oldColor = old.pixelColor(x,y);
      QColor auxColor = aux.pixelColor(x,y);
      //      auxColor.setAlphaF(auxColor.alphaF()*auxOv.pixelColor(x,y).redF());
      QColor newColor(0,0,0,0);

      if (auxColor.alphaF() <= 1e-6 || m_processor->get_texture()->pixelColor(x,y).alphaF() == 0){
        newColor = oldColor;
      } else {
        float outA = alpha*auxColor.alphaF()+oldColor.alphaF()*(1-alpha*auxColor.alphaF());
        int r = auxColor.red()*alpha*auxColor.alphaF()/outA+oldColor.red()*oldColor.alphaF()*(1-alpha*auxColor.alphaF())/outA;

        int g = auxColor.green()*alpha*auxColor.alphaF()/outA+oldColor.green()*oldColor.alphaF()*(1-alpha*auxColor.alphaF())/outA;

        int b = auxColor.blue()*alpha*auxColor.alphaF()/outA+oldColor.blue()*oldColor.alphaF()*(1-alpha*auxColor.alphaF())/outA;

        newColor = QColor(r,g,b);
        newColor.setAlphaF(outA);

      }

      ov.setPixelColor(x,y,newColor);
    }
  }
  return ov;
}

void HSPOBrush::drawAt(QPoint point, QPainter *p, float alpha_mod, bool invert){
  QRadialGradient gradient(point, radius);
  //  if (p->device() == (QPaintDevice *) &auxParallax){
  //    invert = true;
  //  } else {
  //    invert = false;
  //  }
  invert = false;
  if (!invert){
    gradient.setColorAt(0,QColor(maxV,maxV,maxV,1.0*255));
    gradient.setColorAt(hardness,QColor(maxV,maxV,maxV,1.0*255));
    for (int i=100*(hardness); i<100; i++){
      float x = i/100.0;
      if (gui->get_button_rounded()){
        gradient.setColorAt(x,QColor(maxV,maxV,maxV,(sqrt(1-(x-hardness)*(x-hardness)/(1-hardness)/(1-hardness)))*255));
      } else if (gui->get_button_peak()){
        gradient.setColorAt(x,QColor(maxV,maxV,maxV,(-sqrt(1-(x-1)*(x-1)/(1-hardness)/(1-hardness))+1)*255));
      }

    }
    gradient.setColorAt(1,QColor(maxV,maxV,maxV,0.0));
  }

  QBrush brush(gradient);

  p->setPen(QPen(brush, radius, Qt::SolidLine, Qt::RoundCap,
                 Qt::MiterJoin));

  p->drawEllipse(point.x()-radius/2,point.y()-radius/2,radius,radius);


}

void HSPOBrush::mouseMove(const QPoint &oldPos, const QPoint &newPos){

  m_processor = *processorPtr;

  if (!selected)
    return;


  int w = m_processor->get_texture()->width();
  int h = m_processor->get_texture()->height();

  bool tilex = m_processor->get_tile_x();
  bool tiley = m_processor->get_tile_y();

  QPoint in(oldPos);
  QPoint fi(newPos);

  int xmin = std::min(in.x(),fi.x());
  int xmax = std::max(in.x(),fi.x());

  int ymin = std::min(in.y(),fi.y());
  int ymax = std::max(in.y(),fi.y());


  if (!gui->get_button_eraser()){

    QList <Overlay> imageList;
    if (gui->get_height_enabled()){
      imageList.append(Overlay(&auxHeight,height,"height"));
    }
    if (gui->get_parallax_enabled()){
      imageList.append(Overlay(&auxParallax,parallax,"parallax"));
    }
    if (gui->get_specular_enabled()){
      imageList.append(Overlay(&auxSpecular,spec,"specular"));
    }
    if (gui->get_occlussion_enabled()){
      imageList.append(Overlay(&auxOcclussion,occ,"occlussion"));
    }

    foreach(Overlay overlay, imageList){

      QImage *aux = overlay.ov;
      maxV = overlay.color;
      QPainter p(aux);

      QPainterPath path;
      path.moveTo(in);
      path.lineTo(fi);
      qreal length = path.length();
      qreal pos = 0;


      while (pos < length) {
        qreal percent = path.percentAtLength(pos);
        QPoint point = path.pointAtPercent(percent).toPoint();

        if (tilex){
          point.setX(point.x() % w);
          xmin = std::min(xmin,point.x());
          xmax = std::max(xmax,point.x());
        }
        if (tiley){
          point.setY(point.y() % h);
          ymin = std::min(ymin,point.y());
          ymax = std::max(ymax,point.y());
        }

        if (tilex){
          if (point.x() + radius >= w){
            drawAt(QPoint(point.x()-w,point.y()),&p);
            xmax = w;
            xmin = 0;
          } else if (point.x() - radius <= 0){
            drawAt(QPoint(point.x() + w, point.y()), &p);
            xmax = w;
            xmin = 0;

          }
        }

        if (tiley){
          if (point.y() + radius >= h){
            drawAt(QPoint(point.x(),point.y()-h),&p);
            ymax = h;
            ymin = 0;
          } else if (point.y() - radius <= 0){
            drawAt(QPoint(point.x(), point.y()+h), &p);
            ymax = h;
            ymin = 0;
          }
        }

        drawAt(point,&p);
        pos += radius/4.0;
      }

    }

    if (gui->get_height_enabled())
      m_processor->set_heightmap_overlay(updateOverlay(xmin,xmax, ymin, ymax, m_processor->get_heightmap_overlay(), oldHeight, auxHeight));
    if (gui->get_parallax_enabled())
      m_processor->set_parallax_overlay(updateOverlay(xmin,xmax, ymin, ymax, m_processor->get_parallax_overlay(), oldParallax, auxParallax));
    if (gui->get_specular_enabled())
      m_processor->set_specular_overlay(updateOverlay(xmin,xmax,ymin,ymax,m_processor->get_specular_overlay(), oldSpecular, auxSpecular));
    if (gui->get_occlussion_enabled())
      m_processor->set_occlussion_overlay(updateOverlay(xmin,xmax,ymin,ymax,m_processor->get_occlusion_overlay(), oldOcclussion, auxOcclussion));
  } else {
    //TODO make it work in tiles
    QImage erased(auxHeight.size(),QImage::Format_RGBA8888_Premultiplied);
    erased.fill(QColor(0,0,0,0));
    QPainter p(&erased);

    QPen pen(QColor(1,1,1,1));
    pen.setWidth(2*radius);
    pen.setStyle(Qt::SolidLine);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::MiterJoin);
    p.setPen(pen);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.drawLine(in,fi);

    QList <Overlay> imageList;
    if (gui->get_height_enabled()){
      imageList.append(Overlay(&oldHeight,height,"height"));
    }
    if (gui->get_parallax_enabled()){
      imageList.append(Overlay(&oldParallax,parallax,"parallax"));
    }
    if (gui->get_specular_enabled()){
      imageList.append(Overlay(&oldSpecular,spec,"specular"));
    }
    if (gui->get_occlussion_enabled()){
      imageList.append(Overlay(&oldOcclussion,occ,"occlussion"));
    }
    foreach(Overlay ov, imageList){
      QImage *overlay = ov.ov;
      for (int x=0; x < overlay->width(); x++){
        for (int y=0; y < overlay->height(); y++){
          if (erased.pixelColor(x,y).alphaF() != 0){
            overlay->setPixelColor(x,y,QColor(0,0,0,0));
          }
        }
      }
      if (ov.type == "height"){
        m_processor->set_heightmap_overlay(*overlay);
      } else if (ov.type == "parallax"){
        m_processor->set_parallax_overlay(*overlay);
      } else if (ov.type == "specular") {
        m_processor->set_specular_overlay(*overlay);
      } else if (ov.type == "occlussion"){
        m_processor->set_occlussion_overlay(*overlay);
      }
    }

  }
  //}
  //  }

  xmin = std::min(in.x(),fi.x());
  xmax = std::max(in.x(),fi.x());

  ymin = std::min(in.y(),fi.y());
  ymax = std::max(in.y(),fi.y());

  QRect r(QPoint(xmin-radius,ymin-radius),QPoint(xmax+radius,ymax+radius));

  if (gui->get_specular_enabled())
    m_processor->calculate_specular();
//    QtConcurrent::run(m_processor,&ImageProcessor::calculate_specular);
  if (gui->get_parallax_enabled())
    QtConcurrent::run(m_processor,&ImageProcessor::calculate_parallax);
  if(gui->get_occlussion_enabled())
    QtConcurrent::run(m_processor,&ImageProcessor::calculate_occlusion);
  if (gui->get_height_enabled())
    QtConcurrent::run(m_processor,&ImageProcessor::generate_normal_map,false,false,false,r);
}

void HSPOBrush::mousePress(const QPoint &pos){
  m_processor = *processorPtr;

  oldHeight = m_processor->get_heightmap_overlay();
  auxHeight = QImage(oldHeight.width(), oldHeight.height(), QImage::Format_RGBA8888_Premultiplied);
  auxHeight.fill(QColor(0,0,0,0));

  oldParallax = m_processor->get_parallax_overlay();
  auxParallax = QImage(oldParallax.width(), oldParallax.height(), QImage::Format_RGBA8888_Premultiplied);
  auxParallax.fill(QColor(0,0,0,0));

  oldSpecular = m_processor->get_specular_overlay();
  auxSpecular = QImage(oldSpecular.width(), oldSpecular.height(), QImage::Format_RGBA8888_Premultiplied);
  auxSpecular.fill(QColor(0,0,0,0));

  oldOcclussion = m_processor->get_occlusion_overlay();
  auxOcclussion = QImage(oldOcclussion.width(), oldOcclussion.height(), QImage::Format_RGBA8888_Premultiplied);
  auxOcclussion.fill(QColor(0,0,0,0));

  int w = m_processor->get_texture()->width();
  int h = m_processor->get_texture()->height();

  bool tilex = m_processor->get_tile_x();
  bool tiley = m_processor->get_tile_y();


  if (!gui->get_button_eraser()){
    QList <Overlay> imageList;
    if (gui->get_height_enabled()){
      imageList.append(Overlay(&auxHeight,height,"height"));
    }
    if (gui->get_parallax_enabled()){
      imageList.append(Overlay(&auxParallax,parallax,"parallax"));
    }
    if (gui->get_specular_enabled()){
      imageList.append(Overlay(&auxSpecular,spec,"specular"));
    }
    if (gui->get_occlussion_enabled()){
      imageList.append(Overlay(&auxOcclussion,occ,"occlussion"));
    }

    QPoint fi(pos);
    QPoint point;
    foreach(Overlay overlay, imageList)

    {
      /* Draw the point */
      QImage *aux = overlay.ov;
      QPainter p(aux);
      maxV = overlay.color;

      QRadialGradient gradient(fi, radius);
      gradient.setColorAt(0,QColor(maxV,maxV,maxV,1*255));
      gradient.setColorAt(1,QColor(maxV,maxV,maxV,hardness*255));

      QBrush brush(gradient);
      p.setRenderHint(QPainter::Antialiasing, true);
      if (brushSelected)
        p.setPen(QPen(brush, radius, Qt::SolidLine, Qt::RoundCap,
                      Qt::MiterJoin));

      point = QPoint(fi.x(), fi.y());

      if (tilex){
        point.setX(point.x() % w);
      }
      if (tiley){
        point.setY(point.y() % h);
      }

      if (tilex){
        if (point.x() + radius >= w){
          drawAt(QPoint(point.x()-w,point.y()),&p);
        } else if (point.x() - radius <= 0){
          drawAt(QPoint(point.x() + w, point.y()), &p);
        }
      }

      if (tiley){
        if (point.y() + radius >= h){
          drawAt(QPoint(point.x(),point.y()-h),&p);
        } else if (point.y() - radius <= 0){
          drawAt(QPoint(point.x(), point.y()+h), &p);
        }
      }

      drawAt(point, &p);

    }
    int xmin = point.x()-radius, xmax = point.x()+radius, ymin = point.y()-radius, ymax = point.y()+radius;

    if (gui->get_height_enabled())
      m_processor->set_heightmap_overlay(updateOverlay(xmin,xmax, ymin, ymax, m_processor->get_heightmap_overlay(), oldHeight, auxHeight));
    if (gui->get_parallax_enabled())
      m_processor->set_parallax_overlay(updateOverlay(xmin,xmax, ymin, ymax, m_processor->get_parallax_overlay(), oldParallax, auxParallax));
    if (gui->get_specular_enabled())
      m_processor->set_specular_overlay(updateOverlay(xmin,xmax,ymin,ymax,m_processor->get_specular_overlay(), oldSpecular, auxSpecular));
    if (gui->get_occlussion_enabled())
      m_processor->set_occlussion_overlay(updateOverlay(xmin,xmax,ymin,ymax,m_processor->get_occlusion_overlay(), oldOcclussion, auxOcclussion));

    QRect r(QPoint(xmin, ymin),QPoint(xmax, ymax));
    //  QtConcurrent::run(m_processor,&ImageProcessor::calculate_specular);
    //  QtConcurrent::run(m_processor,&ImageProcessor::calculate_parallax);
    //  QtConcurrent::run(m_processor,&ImageProcessor::calculate_occlusion);
    QtConcurrent::run(m_processor,&ImageProcessor::generate_normal_map,false,false,false,r);
  }
}

void HSPOBrush::mouseRelease(const QPoint &pos){

}

void HSPOBrush::setProcessor(ImageProcessor **processor){

  processorPtr = processor;
}

QWidget *HSPOBrush::loadGUI(QWidget *parent){
  radius = 15;
  gui = new HSOPBrushGui(parent);
  connect(gui,SIGNAL(selected_changed(bool)),this,SLOT(set_lineSelected(bool)));
  connect(this,SIGNAL(brush_sprite_updated(QImage)),gui,SLOT(brush_sprite_updated(QImage)));
  connect(gui,SIGNAL(size_changed(int)),this,SLOT(set_radius(int)));
  connect(gui,SIGNAL(hardness_changed(int)),this,SLOT(set_hardness(int)));

  connect(gui,SIGNAL(height_changed(int)),this,SLOT(set_height(int)));
  connect(gui,SIGNAL(specular_changed(int)),this,SLOT(set_specular(int)));
  connect(gui,SIGNAL(occlussion_changed(int)),this,SLOT(set_occlussion(int)));
  connect(gui,SIGNAL(parallax_changed(int)),this,SLOT(set_parallax(int)));

  connect(gui,SIGNAL(mix_changed(int)),this,SLOT(set_mix(int)));
  updateBrushSprite();
  return gui;
}

void HSPOBrush::set_height(int height){
  this->height = height;
  updateBrushSprite();
}

void HSPOBrush::set_parallax(int p){
  parallax = p;
}

void HSPOBrush::set_specular(int s){
  spec = s;
}

void HSPOBrush::set_occlussion(int o){
  occ = o;
}

void HSPOBrush::set_radius(int r){
  radius = r;
  updateBrushSprite();
}

void HSPOBrush::set_hardness(int h){
  hardness = h/100.0;
  updateBrushSprite();
}

void HSPOBrush::set_max(int m){
  maxV = m;
  updateBrushSprite();
}

void HSPOBrush::set_min(int m){
  minV = m;
  updateBrushSprite();
}

void HSPOBrush::set_mix(int m){
  alpha = m/100.0;
  updateBrushSprite();
}

void HSPOBrush::set_lineSelected(bool l){
  linesSelected = l;
  set_selected(l);
  updateBrushSprite();
}

void HSPOBrush::set_eraserSelected(bool e){
  eraserSelected = e;
  if (e && !selected) set_selected(e);
  else if (!e && !brushSelected) set_selected(e);
}

void HSPOBrush::set_brushSelected(bool b){
  brushSelected = b;
  if (b && !selected) set_selected(b);
  else if (!b && !eraserSelected) set_selected(b);
}

QIcon HSPOBrush::getIcon(){
  return QIcon(QPixmap(":/icons/icon.png"));
}

QString HSPOBrush::getName(){
  return m_name;
}

bool HSPOBrush::get_selected(){
  return selected;
}

void HSPOBrush::set_selected(bool s){
  selected = s;
  if (s){
    selected_changed(this);
  } else {
    // TODO uncheck button
  }
  //  if (!s){
  //    gui->unselect_all();
  //  }
}

QImage HSPOBrush::getBrushSprite(){
  return brushSprite;
}

void HSPOBrush::updateBrushSprite(){
  brushSprite = QImage(2*radius,2*radius,QImage::Format_RGBA8888);
  brushSprite.fill(0.0);
  QPainter p(&brushSprite);
  maxV = 255;
  drawAt(QPoint(radius,radius), &p, alpha);

  brush_sprite_updated(brushSprite);
}

QObject * HSPOBrush::getObject(){
  return this;
}

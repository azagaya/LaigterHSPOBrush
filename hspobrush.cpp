#include "hspobrush.h"
#include <algorithm>
#include <QDebug>
#include <QRadialGradient>
#include <QtConcurrent/QtConcurrent>
#include <QIcon>


void HSPOBrush::updateOverlay(int xmin, int xmax, int ymin, int ymax){
  QImage *overlay = m_processor->get_specular_overlay();

  QPoint topLeft;
  QPoint botRight;

  topLeft = QPoint(xmin,ymin);
  botRight = QPoint(xmax,ymax);

  xmin -= radius;
  ymin -= radius;
  xmax += radius;
  ymax += radius;

  ymin = ymin < 0 ? 0 : ymin;
  ymin = ymin > overlay->height() ? overlay->height() : ymin;
  ymax = ymax < 0 ? 0 : ymax;
  ymax = ymax > overlay->height() ? overlay->height() : ymax;

  xmin = xmin < 0 ? 0 : xmin;
  xmin = xmin > overlay->width() ? overlay->width() : xmin;
  xmax = xmax < 0 ? 0 : xmax;
  xmax = xmax > overlay->width() ? overlay->width() : xmax;

  for (int x = xmin; x < xmax; x++){
    for (int y =ymin; y < ymax; y++){
      QColor oldColor = oldHeight.pixelColor(x,y);
      QColor auxColor = auxHeight.pixelColor(x,y);
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

      overlay->setPixelColor(x,y,newColor);
    }
  }
}

void HSPOBrush::drawAt(QPoint point, QPainter *p, float alpha_mod){
  QRadialGradient gradient(point, radius);
  gradient.setColorAt(0,QColor(maxV,maxV,maxV,1.0*255));
  for (int i=1; i<=100; i++){
    float x = i/100.0;
    gradient.setColorAt(x,QColor(maxV,maxV,maxV,(-sqrt(1-(x-1)*(x-1))+1)*255*((1-x)+x*hardness)));
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

  if (brushSelected){

    QRect rect = m_processor->get_texture()->rect();

    QPainter p(&auxHeight);

    if (!linesSelected){
      QRadialGradient gradient(fi, radius);
      gradient.setColorAt(0,QColor(maxV,maxV,maxV,1.0*255));
      gradient.setColorAt(1,QColor(maxV,maxV,maxV,hardness*255));
      QBrush brush(gradient);
      p.setRenderHint(QPainter::Antialiasing, true);
      if (brushSelected)
        p.setPen(QPen(brush, radius, Qt::SolidLine, Qt::RoundCap,
                      Qt::MiterJoin));

      QPoint point(fi.x(), fi.y());

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

      drawAt(point, &p);
    }else{

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

    updateOverlay(xmin,xmax, ymin, ymax);

  } else {
    QImage *overlay = m_processor->get_specular_overlay();
    QPainter p(&auxHeight);
    QPen pen(QColor(1,1,1,1));
    pen.setWidth(2*radius);
    pen.setStyle(Qt::SolidLine);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::MiterJoin);
    p.setPen(pen);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.drawLine(in,fi);

    for (int x=0; x < overlay->width(); x++){
      for (int y=0; y < overlay->height(); y++){
        if (auxHeight.pixelColor(x,y).alphaF() != 0){
          overlay->setPixelColor(x,y,QColor(0,0,0,0));
        }
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

  QtConcurrent::run(m_processor,&ImageProcessor::calculate_specular);
  QtConcurrent::run(m_processor,&ImageProcessor::generate_normal_map,false,false,false,r);
}

void HSPOBrush::mousePress(const QPoint &pos){
  m_processor = *processorPtr;
  QImage *overlay = m_processor->get_specular_overlay();
  oldHeight = QImage(overlay->width(),overlay->height(),QImage::Format_RGBA8888_Premultiplied);
  oldHeight = *overlay;
  auxHeight = QImage(oldHeight.width(), oldHeight.height(), QImage::Format_RGBA8888_Premultiplied);
  auxHeight.fill(QColor(255,255,255,0));

  /* Draw the point */
  QPoint fi(pos);
  QPainter p(&auxHeight);

  int w = m_processor->get_texture()->width();
  int h = m_processor->get_texture()->height();

  bool tilex = m_processor->get_tile_x();
  bool tiley = m_processor->get_tile_y();

  QRadialGradient gradient(fi, radius);
  gradient.setColorAt(0,QColor(maxV,maxV,maxV,1*255));
  gradient.setColorAt(1,QColor(maxV,maxV,maxV,hardness*255));

  QBrush brush(gradient);
  p.setRenderHint(QPainter::Antialiasing, true);
  if (brushSelected)
    p.setPen(QPen(brush, radius, Qt::SolidLine, Qt::RoundCap,
                  Qt::MiterJoin));

  QPoint point(fi.x(), fi.y());

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
  updateOverlay(fi.x()-radius, fi.x()+radius, fi.y()-radius, fi.y()+radius);

  QRect r(QPoint(fi.x()-radius,fi.y()-radius),QPoint(fi.x()+radius,fi.y()+radius));

  QtConcurrent::run(m_processor,&ImageProcessor::generate_normal_map,false,false,false,r);
}

void HSPOBrush::mouseRelease(const QPoint &pos){

}

void HSPOBrush::setProcessor(ImageProcessor **processor){

  processorPtr = processor;
}

QWidget *HSPOBrush::loadGUI(QWidget *parent){
  radius = 15;
  gui = new HSPOBrushGui(parent);
  updateBrushSprite();
  return gui;
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
  return QIcon(QPixmap(":/icons/icons/icon.png"));
}

QString HSPOBrush::getName(){
  return m_name;
}

bool HSPOBrush::get_selected(){
  return selected;
}

void HSPOBrush::set_selected(bool s){
  selected = s;
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
  drawAt(QPoint(radius,radius), &p, alpha);
}

QObject * HSPOBrush::getObject(){
  return this;
}
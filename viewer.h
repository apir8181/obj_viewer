#ifndef VIEWER_H
#define VIEWER_H

#include <QGLWidget>
#include <QSize>
#include <QAction>
#include <GL/glut.h>
#include "parser.h"

class Viewer: public QGLWidget{
  Q_OBJECT
 public:
  Viewer(Parser *parser,QString file_name,QWidget *parent);
  ~Viewer();
  QAction *windowMenuAction() const;
  QSize minimumSizeHint() const;
  QSize sizeHint() const;
  DRAW_TYPE getDrawType();

  //scale
  void enableRotate();
  void setScaleX(double num);
  float getScaleX();
  void setScaleY(double num);
  float getScaleY();
  void setScaleZ(double num);
  float getScaleZ();
  //light
  void setShowNormals(bool t);
  bool getShowNormals();
  void enableLight();
  void disableLight();
  bool getShowLight();
  void setLightPositionX(double x);
  float getLightPositionX();
  void setLightPositionY(double y);
  float getLightPositionY();
  void setLightPositionZ(double z);
  float getLightPositionZ();

  void enableTranslate();
  //loop subdivision,texture,scale

  void setDrawType(DRAW_TYPE t);
  void enablePoint();
  void enableLine();
  void enablePoly();

 protected:
  void initializeGL();
  void resizeGL(int width,int height);
  void paintGL();
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void wheelEvent(QWheelEvent *event);

 private:
  Parser *parser;
  QAction *action;
  QString file_name;  
  //operation state
  bool is_rotate;
  bool is_translate;
  bool is_light;
  DRAW_TYPE draw_type;

  QPoint last_pos;
  //scroll view
  GLfloat wheel_num;
  //translate
  GLfloat translate_x, translate_y;
  void translatePress(QMouseEvent *event);
  void translateMove(QMouseEvent *event);
  void translate();
  //rotation
  GLfloat rotation_x, rotation_y, rotation_z;
  void rotatePress(QMouseEvent *event);
  void rotateMove(QMouseEvent *event);
  void rotate();
  //scale
  GLfloat scale_x,scale_y,scale_z;
  void scale();
  //light
  bool show_normals;
  GLfloat light_ambient[4];
  GLfloat light_diffuse[4];
  GLfloat light_specular[4];
  GLfloat light_position[4];
  GLfloat light_model_ambient[4];
  GLfloat material_ambient[4];
  GLfloat material_diffuse[4];
  GLfloat material_specular[4];
  GLfloat material_emission[4];
  GLfloat material_shininess;
  void initLight();

  void drawSystemCoordinate();
};

#endif

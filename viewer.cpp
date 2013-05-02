#include "viewer.h"
#include "parser.h"
#include <QtGui>
#include <QtOpenGL>
#include <QSize>
#include <GL/glut.h>
#include <QString>

Viewer::Viewer(Parser *_parser,QString _file_name,QWidget *parent)
  :QGLWidget(parent)
{
  parser = _parser;
  parser->printInfo();
  file_name = _file_name;
  action = new QAction(this);
  action->setCheckable(true);
  action->setText(QFileInfo(file_name).fileName());
  connect(action,SIGNAL(triggered()),this,SLOT(show()));
  connect(action,SIGNAL(triggered()),this,SLOT(setFocus()));

  setAttribute(Qt::WA_DeleteOnClose);
  setWindowTitle(QFileInfo(file_name).fileName());
  setFormat(QGLFormat(QGL::DoubleBuffer|QGL::DepthBuffer));

  draw_type = DRAW_LINE;
  //scale
  scale_x = scale_y = scale_z = 1.0;
  //translate
  wheel_num = 0;
  is_translate = false;
  translate_x = translate_y = 0;
  //rotate
  rotation_x = rotation_y = rotation_z = 0;
  is_rotate = true;
  //light
  show_normals = true;
  is_light = true;
  light_ambient[0]=light_ambient[1]=light_ambient[2]=0.0;
  light_ambient[3]=1.0;
  light_diffuse[0]=light_diffuse[1]=light_diffuse[2]=
    light_diffuse[3]=1.0;
  light_specular[0]=light_specular[1]=light_specular[2]=
    light_specular[3]=1.0;
  light_position[0]=light_position[1]=0.0;light_position[2]=20.0;
  light_position[3]=1.0;
  light_model_ambient[0]=light_model_ambient[1]=
    light_model_ambient[2]=0.1;light_model_ambient[3]=1.0;
  material_ambient[0]=material_ambient[1]=material_ambient[2]=0.2;
  material_ambient[3]=1.0;
  material_diffuse[0]=material_diffuse[1]=material_diffuse[2]=0.8;
  material_diffuse[3]=1.0;
  material_specular[0]=material_specular[1]=material_specular[2]=0.0;
  material_specular[3]=1.0;
  material_emission[0]=material_emission[1]=material_emission[2]=0.0;
  material_emission[3]=1.0;
  material_shininess = 100.0;
}

Viewer::~Viewer(){
  if (parser != NULL) delete parser;
}

void Viewer::initializeGL(){
  glClearColor(0.4,0.4,0.4,1.0);
  glShadeModel(GL_SMOOTH);
  if (is_light){
    initLight();
  }
  glEnable(GL_DEPTH_TEST);
}

void Viewer::resizeGL(int width,int height){
  glViewport(0,0,width,height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(90,1.0,0.3,50.0);
}

void Viewer::paintGL(){
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);

  glLoadIdentity();
  gluLookAt(0.0,0.0,3.0 + wheel_num,
            0.0,0.0,0.0,
            0.0,1.0,0.0);

  if (parser->getSuccess()){
    translate();
    rotate();
    drawSystemCoordinate();
    scale();
    parser->drawInGL(draw_type,is_light,show_normals);
    glLightfv(GL_LIGHT0,GL_POSITION,light_position);
  }
}

void Viewer::mousePressEvent(QMouseEvent *event){
  if (is_rotate) rotatePress(event);
  else if (is_translate) translatePress(event);
}

void Viewer::mouseMoveEvent(QMouseEvent *event){
  if (is_rotate) rotateMove(event);
  else if (is_translate) translateMove(event);
}

void Viewer::wheelEvent(QWheelEvent *event){
  wheel_num -= GLfloat(event->delta()) / 90;
  wheel_num = wheel_num < -3.0 ? -3.0 : wheel_num;
  updateGL();
}

void Viewer::translatePress(QMouseEvent *event){
  last_pos = event->pos();
}

void Viewer::translateMove(QMouseEvent *event){
  GLfloat dx = GLfloat(event->x() - last_pos.x())/width();
  GLfloat dy = GLfloat(event->y() - last_pos.y())/height();
  
  if (event->buttons() & Qt::LeftButton){
    translate_x += dx * 5;
    translate_y += dy * 5;
    updateGL();
  }
  last_pos = event->pos();
}


void Viewer::translate(){
  glTranslatef(translate_x,translate_y,0);
}

void Viewer::rotatePress(QMouseEvent *event){
  last_pos = event->pos();
}

void Viewer::rotateMove(QMouseEvent *event){
  GLfloat dx = GLfloat(event->x() - last_pos.x())/width();
  GLfloat dy = GLfloat(event->y() - last_pos.y())/height();

  if (event->buttons() & Qt::LeftButton){
    rotation_x += 180 * dy;
    rotation_y += 180 * dx;
    updateGL();
  }
  last_pos = event->pos();
}

void Viewer::rotate(){
  glRotatef(rotation_x,1.0,0.0,0.0);
  glRotatef(rotation_y,0.0,1.0,0.0);
  glRotatef(rotation_z,0.0,0.0,1.0);
}

void Viewer::scale(){
  glScalef(scale_x,scale_y,scale_z);
}

QSize Viewer::minimumSizeHint() const{
  return QSize(400,400);
}

QSize Viewer::sizeHint() const{
  return QSize(800,800);
}

QAction *Viewer::windowMenuAction() const{
  return action;
}

void Viewer::enableRotate(){
  is_rotate = true;
  is_translate = false;
}

void Viewer::enableTranslate(){
  is_translate = true;
  is_rotate = false;
}

void Viewer::enableLight(){
  is_light = true;
  updateGL();
}

void Viewer::disableLight(){
  is_light = false;
  updateGL();
}

void Viewer::drawSystemCoordinate(){
  glDisable(GL_LIGHTING);
  glDisable(GL_LIGHT0);
  glBegin(GL_LINES);
  glColor3f(1.0,0.0,0.0);
  glVertex3f(0.0,0.0,0.0);
  glVertex3f(5.0,0.0,0.0);
  glColor3f(0.0,1.0,0.0);
  glVertex3f(0.0,0.0,0.0);
  glVertex3f(0.0,5.0,0.0);
  glColor3f(0.0,0.0,1.0);
  glVertex3f(0.0,0.0,0.0);
  glVertex3f(0.0,0.0,5.0);
  glEnd();
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
}

void Viewer::enablePoint(){
  draw_type = DRAW_POINT;
  updateGL();
}

void Viewer::enableLine(){
  draw_type = DRAW_LINE;
  updateGL();
}

void Viewer::enablePoly(){
  draw_type = DRAW_POLY;
  updateGL();
}

DRAW_TYPE Viewer::getDrawType(){
  return draw_type;
}

void Viewer::setDrawType(DRAW_TYPE t){
  draw_type = t;
}

void Viewer::setScaleX(double num){
  scale_x = float(num);
  updateGL();
}

float Viewer::getScaleX(){
  return scale_x;
}

void Viewer::setScaleY(double num){
  scale_y = float(num);
  updateGL();
}

float Viewer::getScaleY(){
  return scale_y;
}

void Viewer::setScaleZ(double num){
  scale_z = float(num);
  updateGL();
}

float Viewer::getScaleZ(){
  return scale_z;
}

void Viewer::initLight(){
  glLightfv(GL_LIGHT0,GL_AMBIENT,light_ambient);
  glLightfv(GL_LIGHT0,GL_DIFFUSE,light_diffuse);

  glLightModelfv(GL_LIGHT_MODEL_AMBIENT,light_model_ambient);
  glMaterialfv(GL_FRONT,GL_AMBIENT,material_ambient);
  glMaterialfv(GL_FRONT,GL_DIFFUSE,material_diffuse);
  glMaterialfv(GL_FRONT,GL_SPECULAR,material_specular);
  glMaterialfv(GL_FRONT,GL_EMISSION,material_emission);
  glMaterialf(GL_FRONT,GL_SHININESS,material_shininess);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_NORMALIZE);
}

void Viewer::setShowNormals(bool t){
  show_normals = t;
  updateGL();
}

bool Viewer::getShowNormals(){
  return show_normals;
}

bool Viewer::getShowLight(){
  return is_light;
}

void Viewer::setLightPositionX(double x){
  light_position[0] = float(x);
  updateGL();
}

float Viewer::getLightPositionX(){
  return light_position[0];
}

void Viewer::setLightPositionY(double y){
  light_position[1] = y;
  updateGL();
}

float Viewer::getLightPositionY(){
  return light_position[1];
}

void Viewer::setLightPositionZ(double z){
  light_position[2] = z;
  updateGL();
}

float Viewer::getLightPositionZ(){
  return light_position[2];
}

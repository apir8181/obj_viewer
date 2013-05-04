#include "viewer.h"
#include "parser.h"
#include <QtGui>
#include <QtOpenGL>
#include <QSize>
#include <GL/glut.h>
#include <QString>
#include <QColor>

Viewer::Viewer(Parser *_parser,QString _file_name,QWidget *parent)
  :QGLWidget(parent)
{
  parser = _parser;
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
  light_ambient[0]=light_ambient[1]=light_ambient[2]=1.0;
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
  material_specular[0]=material_specular[1]=material_specular[2]=0.6;
  material_specular[3]=1.0;
  material_emission[0]=material_emission[1]=material_emission[2]=0.0;
  material_emission[3]=1.0;
  material_shininess = 100.0;
}

Viewer::~Viewer(){
  if (parser != NULL) delete parser;
  if (action != NULL) delete action;
  if (texture_image != NULL) delete texture_image;
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
  return QSize(600,600);
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
  //x axis
  glColor3f(1.0,0.0,0.0);
  glVertex3f(0.0,0.0,0.0);
  glVertex3f(1.0,0.0,0.0);
  glVertex3f(1.0,0.0,0.0);
  glVertex3f(0.9,0.0,-0.05);
  glVertex3f(1.0,0.0,0.0);
  glVertex3f(0.9,0.0,0.05);
  //y axis
  glColor3f(0.0,1.0,0.0);
  glVertex3f(0.0,0.0,0.0);
  glVertex3f(0.0,1.0,0.0);
  glVertex3f(0.0,1.0,0.0);
  glVertex3f(0.0,0.9,0.05);
  glVertex3f(0.0,1.0,0.0);
  glVertex3f(0.0,0.9,-0.05);
  //z axis
  glColor3f(0.0,0.0,1.0);
  glVertex3f(0.0,0.0,0.0);
  glVertex3f(0.0,0.0,1.0);
  glVertex3f(0.0,0.0,1.0);
  glVertex3f(0.0,0.05,0.9);
  glVertex3f(0.0,0.0,1.0);
  glVertex3f(0.0,-0.05,0.9);
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

void Viewer::setLightAmbient(int r,int g,int b){
  light_ambient[0] = float(r) / 255;
  light_ambient[1] = float(g) / 255;
  light_ambient[2] = float(b) / 255;
  glLightfv(GL_LIGHT0,GL_AMBIENT,light_ambient);
  updateGL();
}

QColor Viewer::getLightAmbient(){
  return QColor(int(light_ambient[0] * 255),
                int(light_ambient[1] * 255),
                int(light_ambient[2] * 255),
                1);
}

void Viewer::setLightDiffuse(int r,int g,int b){
  light_diffuse[0] = float(r) / 255;
  light_diffuse[1] = float(g) / 255;
  light_diffuse[2] = float(b) / 255;
  glLightfv(GL_LIGHT0,GL_DIFFUSE,light_diffuse);
  updateGL();
}

QColor Viewer::getLightDiffuse(){
  return QColor(int(light_diffuse[0] * 255),
                int(light_diffuse[1] * 255),
                int(light_diffuse[2] * 255),
                1);
}

void Viewer::setLightSpecular(int r,int g,int b){
  light_specular[0] = float(r) / 255;
  light_specular[1] = float(g) / 255;
  light_specular[2] = float(b) / 255;
  glLightfv(GL_LIGHT0,GL_SPECULAR,light_specular);
  updateGL();
}

QColor Viewer::getLightSpecular(){
  return QColor(int(light_specular[0] * 255),
                int(light_specular[1] * 255),
                int(light_specular[2] * 255),
                1);
}

void Viewer::setMaterialAmbient(int r,int g,int b){
  material_ambient[0] = float(r) / 255;
  material_ambient[1] = float(g) / 255;
  material_ambient[2] = float(b) / 255;
  glMaterialfv(GL_FRONT,GL_AMBIENT,material_ambient);
  updateGL();
}

QColor Viewer::getMaterialAmbient(){
  return QColor(int(material_ambient[0] * 255),
                int(material_ambient[1] * 255),
                int(material_ambient[2] * 255),
                1);
}

void Viewer::setMaterialDiffuse(int r,int g,int b){
  material_diffuse[0] = float(r) / 255;
  material_diffuse[1] = float(g) / 255;
  material_diffuse[2] = float(b) / 255;
  glMaterialfv(GL_FRONT,GL_DIFFUSE,material_diffuse);
  updateGL();
}

QColor Viewer::getMaterialDiffuse(){
  return QColor(int(material_diffuse[0] * 255),
                int(material_diffuse[1] * 255),
                int(material_diffuse[2] * 255),
                1);
}

void Viewer::setMaterialSpecular(int r,int g,int b){
  material_specular[0] = float(r) / 255;
  material_specular[1] = float(g) / 255;
  material_specular[2] = float(b) / 255;
  glMaterialfv(GL_FRONT,GL_SPECULAR,material_specular);
  updateGL();
}

QColor Viewer::getMaterialSpecular(){
  return QColor(int(material_specular[0] * 255),
                int(material_specular[1] * 255),
                int(material_specular[2] * 255),
                1);
}

void Viewer::setMaterialEmission(int r,int g,int b){
  material_emission[0] = float(r) / 255;
  material_emission[1] = float(g) / 255;
  material_emission[2] = float(b) / 255;
  glMaterialfv(GL_FRONT,GL_EMISSION,material_emission);
  updateGL();
}

QColor Viewer::getMaterialEmission(){
  return QColor(int(material_emission[0] * 255),
                int(material_emission[1] * 255),
                int(material_emission[2] * 255),
                1);
}

void Viewer::setMaterialShininess(double s){
  material_shininess = s;
  glMaterialf(GL_FRONT,GL_SHININESS,material_shininess);
  updateGL();
}

float Viewer::getMaterialShininess(){
  return material_shininess;
}

void Viewer::enableTexture(){
  is_texture = true;
  updateGL();
}

void Viewer::disableTexture(){
  is_texture = false;
  updateGL();
}

void Viewer::setTextureImage(QImage *image){
  if (texture_image != NULL && texture_image != image)
    delete texture_image;
  texture_image = image;
  updateGL();
}

QImage* Viewer::getTextureImage(){
  return texture_image;
}

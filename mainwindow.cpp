#include "mainwindow.h"
#include "viewer.h"
#include <QtGui>
#include <QIcon>
#include <QAction>
#include <QMenuBar>
#include <QStatusBar>
#include <QMenu>
#include <QMdiSubWindow>
#include <QFileDialog>
#include <QActionGroup>
#include <QToolBar>
#include <QSlider>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFont>
#include <QCheckBox>
#include <QSize>

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent){
  mdi_area = new QMdiArea();
  setCentralWidget(mdi_area);
  connect(mdi_area,SIGNAL(subWindowActivated(QMdiSubWindow*)),
          this,SLOT(updateActions()));

  createResources();
  createActions();
  createMenuBar();
  createToolBar();
  createLeftToolBar();
  //setWindowIcon()
  setWindowTitle("Qt Obj Viewer");
}

MainWindow::~MainWindow(){
  delete open_pix;
  delete close_pix;
  delete close_all_pix;
  delete quit_pix;
  delete next_pix;
  delete previous_pix;
  delete rotate_pix;
  delete translate_pix;
  delete point_pix;
  delete line_pix;
  delete polygon_pix;
  delete scale_pix;
  delete light_pix;
  delete material_pix;
  delete texture_pix;
  delete divide_pix;
}

void MainWindow::createMenuBar(){
  file_menu = menuBar()->addMenu("&File");
  file_menu->addAction(open_action);
  file_menu->addAction(close_action);
  file_menu->addAction(close_all_action);
  file_menu->addSeparator();
  file_menu->addAction(quit_action);
  
  window_menu = menuBar()->addMenu("&Window");  
  window_menu->addAction(next_action);
  window_menu->addAction(previous_action);
  window_menu->addSeparator();

  operation_menu = menuBar()->addMenu("&Operation");
  operation_menu->addAction(rotate_action);
  operation_menu->addAction(translate_action);
  operation_menu->addSeparator();
  operation_menu->addAction(point_action);
  operation_menu->addAction(line_action);
  operation_menu->addAction(polygon_action);
  operation_menu->addSeparator();
  operation_menu->addAction(divide_action);
}

void MainWindow::createToolBar(){
  toolbar = addToolBar("Toolbar");
  toolbar->addAction(open_action);
  toolbar->addAction(close_action);
  toolbar->addAction(close_all_action);
  toolbar->addAction(quit_action);

  toolbar->addSeparator();
  toolbar->addAction(previous_action);
  toolbar->addAction(next_action);

  toolbar->addSeparator();
  toolbar->addAction(rotate_action);
  toolbar->addAction(translate_action);

  toolbar->addSeparator();
  toolbar->addAction(point_action);
  toolbar->addAction(line_action);
  toolbar->addAction(polygon_action);

  toolbar->addSeparator();
  toolbar->addAction(divide_action);
}

void MainWindow::createLeftToolBar(){
  
  QToolBar* temp_toolbar = new QToolBar(this);
  addToolBar(Qt::LeftToolBarArea,temp_toolbar);
  QScrollArea* scroll_area = new QScrollArea(this);
  temp_toolbar->addWidget(scroll_area);
  scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  QWidget* temp_widget = new QWidget(this);
  left_toolbar = new QVBoxLayout();
  createScaleWidget();
  createLightWidget();
  createMaterialWidget();
  createTextureWidget();
  temp_widget->setLayout(left_toolbar);
  scroll_area->setWidget(temp_widget);
}

void MainWindow::createResources(){
  open_pix = new QPixmap("images/open.png");
  close_pix = new QPixmap("images/close.png");
  close_all_pix = new QPixmap("images/closeAll.png");
  quit_pix = new QPixmap("images/quit.png");
  next_pix = new QPixmap("images/next.png");
  previous_pix = new QPixmap("images/previous.png");
  rotate_pix = new QPixmap("images/rotate.png");
  translate_pix = new QPixmap("images/translate.png");
  point_pix = new QPixmap("images/point.png");
  line_pix = new QPixmap("images/line.png");
  polygon_pix = new QPixmap("images/polygon.png");
  scale_pix = new QPixmap("images/scale.png");
  light_pix = new QPixmap("images/light.png");
  material_pix = new QPixmap("images/material.png");
  texture_pix = new QPixmap("images/texture.png");
  divide_pix = new QPixmap("images/divide.png");
}

void MainWindow::createActions(){
  //file action
  open_action = new QAction(*open_pix,"&Open",this);
  close_action = new QAction(*close_pix,"&Close",this);
  close_all_action = new QAction(*close_all_pix,"Close &All",this);
  quit_action = new QAction(*quit_pix,"&Quit",this);
  connect(open_action,SIGNAL(triggered()),this,SLOT(open()));
  connect(close_action,SIGNAL(triggered()),mdi_area,
          SLOT(closeActiveSubWindow()));
  connect(close_all_action,SIGNAL(triggered()),
          mdi_area,SLOT(closeAllSubWindows()));
  connect(quit_action,SIGNAL(triggered()),qApp,SLOT(quit()));

  //window action
  next_action = new QAction(*next_pix,"&Next",this);
  previous_action = new QAction(*previous_pix,"&Previous",this);
  connect(next_action,SIGNAL(triggered()),mdi_area,
          SLOT(activateNextSubWindow()));
  connect(previous_action,SIGNAL(triggered()),mdi_area,
          SLOT(activatePreviousSubWindow()));
  window_action_group = new QActionGroup(this);

  //operation action
  rotate_action = new QAction(*rotate_pix,"&Rotate",this);
  translate_action = new QAction(*translate_pix,"&Translate",this);
  rotate_action->setCheckable(true);
  rotate_action->setChecked(true);
  translate_action->setCheckable(true);
  operation_action_group = new QActionGroup(this);
  operation_action_group->addAction(translate_action);
  operation_action_group->addAction(rotate_action);
  connect(rotate_action,SIGNAL(triggered()),this,SLOT(rotateAction()));
  connect(translate_action,SIGNAL(triggered()),
          this,SLOT(translateAction()));
  
  //shape action
  point_action = new QAction(*point_pix,"Point",this);
  line_action = new QAction(*line_pix,"Line",this);
  polygon_action = new QAction(*polygon_pix,"Polygon",this);
  point_action->setCheckable(true);
  line_action->setCheckable(true);
  polygon_action->setCheckable(true);
  polygon_action->setChecked(true);
  shape_action_group = new QActionGroup(this);
  shape_action_group->addAction(point_action);
  shape_action_group->addAction(line_action);
  shape_action_group->addAction(polygon_action);
  connect(point_action,SIGNAL(triggered()),this,SLOT(pointAction()));
  connect(line_action,SIGNAL(triggered()),this,SLOT(lineAction()));
  connect(polygon_action,SIGNAL(triggered()),
          this,SLOT(polygonAction()));

  divide_action = new QAction(*divide_pix,"Subdivision",this);
  connect(divide_action,SIGNAL(triggered()),
          this,SLOT(divideAction()));
}

void MainWindow::updateActions(){
  Viewer* now = activeViewer();
  if (now && now->windowMenuAction()){
    now->windowMenuAction()->setChecked(true);
    //update operation state
    if (rotate_action->isChecked()) now->enableRotate();
    else if (translate_action->isChecked()) now->enableTranslate();

    //update shape state
    if (now->getDrawType() == DRAW_POINT)
      point_action->setChecked(true);
    else if (now->getDrawType() == DRAW_LINE)
      line_action->setChecked(true);
    else if (now->getDrawType() == DRAW_POLY)
      polygon_action->setChecked(true);

    //update scale num
    scale_spinbox_x->setValue(double(now->getScaleX()));
    scale_spinbox_y->setValue(double(now->getScaleY()));
    scale_spinbox_z->setValue(double(now->getScaleZ()));
    //update light pos
    light_pos_spinbox_x->setValue(now->getLightPositionX());
    light_pos_spinbox_y->setValue(now->getLightPositionY());
    light_pos_spinbox_z->setValue(now->getLightPositionZ());
    //update light color
    buttonSetColor(light_ambient_button,now->getLightAmbient());
    buttonSetColor(light_diffuse_button,now->getLightDiffuse());
    buttonSetColor(light_specular_button,now->getLightSpecular());
    //update material
    buttonSetColor(material_ambient_button,now->getMaterialAmbient());
    buttonSetColor(material_diffuse_button,now->getMaterialDiffuse());
    buttonSetColor(material_specular_button,
                   now->getMaterialSpecular());
    buttonSetColor(material_emission_button,
                   now->getMaterialEmission());
    material_shininess_spinbox->setValue(now->getMaterialShininess());
    //texture
    if (now->getTextureEnabled())
      texture_checkbox->setChecked(Qt::Checked);
    else texture_checkbox->setChecked(Qt::Unchecked);
    if (now->getTextureEnabled()) enableTextureWidget();
    else disableTextureWidget();

    QPixmap pix;
    if (now->getTextureImage() && pix.convertFromImage(*(now->getTextureImage()))){
      QIcon icon(pix);
      texture_select_button->setIconSize(QSize(texture_select_button->width(),texture_select_button->height()));
      texture_select_button->setIcon(icon);
    }
    else{
      QIcon icon;
      texture_select_button->setIcon(icon);
    }

    //update light
    if (now->getShowNormals())
      show_normals_box->setCheckState(Qt::Checked);
    else show_normals_box->setCheckState(Qt::Unchecked);
    if (now->getShowLight()){
      show_light_box->setCheckState(Qt::Checked);
      enableLightWidget();
    }
    else{
      show_light_box->setCheckState(Qt::Unchecked);
      disableLightWidget();
    }
  }
}

void MainWindow::open(){
  QString file_name = QFileDialog::getOpenFileName(this,"Open",".",
                                                   QString("*.obj"));
  if (file_name.isEmpty()) return;
  else{
    Parser *parser = new Parser(file_name.toStdString().c_str());
    Viewer *viewer = new Viewer(parser,file_name,this);
    //set default draw type
    if (line_action->isChecked()) viewer->enableLine();
    else if (point_action->isChecked()) viewer->enablePoint();
    else if (polygon_action->isChecked()) viewer->enablePoly();
    //set default scale num
    scale_spinbox_x->setValue(viewer->getScaleX());
    scale_spinbox_y->setValue(viewer->getScaleY());
    scale_spinbox_z->setValue(viewer->getScaleZ());
    //set default light pos
    light_pos_spinbox_x->setValue(viewer->getLightPositionX());
    light_pos_spinbox_y->setValue(viewer->getLightPositionY());
    light_pos_spinbox_z->setValue(viewer->getLightPositionZ());
    //set default light color
    buttonSetColor(light_ambient_button,viewer->getLightAmbient());
    buttonSetColor(light_diffuse_button,viewer->getLightDiffuse());
    buttonSetColor(light_specular_button,viewer->getLightSpecular());
    //set default material color
    buttonSetColor(material_ambient_button,
                   viewer->getMaterialAmbient());
    buttonSetColor(material_diffuse_button,
                   viewer->getMaterialDiffuse());
    buttonSetColor(material_specular_button,
                   viewer->getMaterialSpecular());
    buttonSetColor(material_emission_button,
                   viewer->getMaterialEmission());
    material_shininess_spinbox->setValue(viewer->getMaterialShininess());
    if (viewer->getShowLight()) enableLightWidget();
    else disableLightWidget();
    //texture
    if (viewer->getTextureEnabled())
      texture_checkbox->setChecked(Qt::Checked);
    else texture_checkbox->setChecked(Qt::Unchecked);
    if (viewer->getTextureEnabled()) enableTextureWidget();
    else disableTextureWidget();
    texture_select_button->setIcon(QIcon());
    
    QMdiSubWindow *sub_window = mdi_area->addSubWindow(viewer);
    window_menu->addAction(viewer->windowMenuAction());
    window_action_group->addAction(viewer->windowMenuAction());
    mdi_area->activateNextSubWindow();
    sub_window->show();

  }
}

void MainWindow::rotateAction(){
  Viewer* v = activeViewer();
  if (v) v->enableRotate();
}

void MainWindow::translateAction(){
  Viewer* v = activeViewer();
  if (v) v->enableTranslate();
}

void MainWindow::scaleXAction(double num){
  Viewer* v = activeViewer();
  if (v) v->setScaleX(num);
}

void MainWindow::scaleYAction(double num){
  Viewer* v = activeViewer();
  if (v) v->setScaleY(num);
}

void MainWindow::scaleZAction(double num){
  Viewer* v = activeViewer();
  if (v) v->setScaleZ(num);
}

void MainWindow::showNormals(int state){
  Viewer* v = activeViewer();
  if (v){
    if (state == Qt::Checked){
      v->setShowNormals(true);
    }
    else v->setShowNormals(false);
  }
}

void MainWindow::lightPosXAction(double num){
  Viewer* v = activeViewer();
  if (v) v->setLightPositionX(num);
}

void MainWindow::lightPosYAction(double num){
  Viewer* v = activeViewer();
  if (v) v->setLightPositionY(num);
}

void MainWindow::lightPosZAction(double num){
  Viewer* v = activeViewer();
  if (v) v->setLightPositionZ(num);
}

void MainWindow::lightAmbientAction(){
  Viewer* v = activeViewer();
  if (!v) return;

  QColor color = QColorDialog::getColor(v->getLightAmbient(),this);
  if (color.isValid()){
    buttonSetColor(light_ambient_button,color);
    v->setLightAmbient(color.red(),color.green(),color.blue());
  }
}

void MainWindow::lightDiffuseAction(){
  Viewer* v = activeViewer();
  if (!v) return;

  QColor color = QColorDialog::getColor(v->getLightDiffuse(),this);
  if (color.isValid()){
    buttonSetColor(light_diffuse_button,color);
    v->setLightDiffuse(color.red(),color.green(),color.blue());
  }
}

void MainWindow::lightSpecularAction(){
  Viewer* v = activeViewer();
  if (!v) return;

  QColor color = QColorDialog::getColor(v->getLightSpecular(),this);
  if (color.isValid()){
    buttonSetColor(light_specular_button,color);
    v->setLightSpecular(color.red(),color.green(),color.blue());
  }
}

void MainWindow::materialAmbientAction(){
  Viewer* v = activeViewer();
  if (!v) return;

  QColor color = QColorDialog::getColor(v->getMaterialAmbient(),this);
  if (color.isValid()){
    buttonSetColor(material_ambient_button,color);
    v->setMaterialAmbient(color.red(),color.green(),color.blue());
  }
}

void MainWindow::materialDiffuseAction(){
  Viewer* v = activeViewer();
  if (!v) return;

  QColor color = QColorDialog::getColor(v->getMaterialDiffuse(),this);
  if (color.isValid()){
    buttonSetColor(material_diffuse_button,color);
    v->setMaterialDiffuse(color.red(),color.green(),color.blue());
  }
}

void MainWindow::materialSpecularAction(){
  Viewer* v = activeViewer();
  if (!v) return;

  QColor color = QColorDialog::getColor(v->getMaterialSpecular(),this);
  if (color.isValid()){
    buttonSetColor(material_specular_button,color);
    v->setMaterialSpecular(color.red(),color.green(),color.blue());
  }
}

void MainWindow::materialEmissionAction(){
  Viewer* v = activeViewer();
  if (!v) return;

  QColor color = QColorDialog::getColor(v->getMaterialEmission(),this);
  if (color.isValid()){
    buttonSetColor(material_emission_button,color);
    v->setMaterialEmission(color.red(),color.green(),color.blue());
  }
}

void MainWindow::materialShininessAction(double num){
  Viewer* v = activeViewer();
  if (v){
    v->setMaterialShininess(num);
  }
}

void MainWindow::showLight(int state){
  Viewer* v = activeViewer();
  if (v){
    if (state == Qt::Checked){
      v->enableLight();
      enableLightWidget();
    }
    else{
      v->disableLight();
      disableLightWidget();
    }
  }
}

void MainWindow::textureShowAction(int state){
  Viewer* v = activeViewer();
  if (v){
    if (state == Qt::Checked){
      v->enableTexture();
      enableTextureWidget();
    }
    else{
      v->disableTexture();
      disableTextureWidget();
    }
  }
}

void MainWindow::pointAction(){
  Viewer* v= activeViewer();
  if (v) v->enablePoint();
}

void MainWindow::lineAction(){
  Viewer* v= activeViewer();
  if (v) v->enableLine();
}

void MainWindow::polygonAction(){
  Viewer* v= activeViewer();
  if (v) v->enablePoly();
}


void MainWindow::textureSelectAction(){
  Viewer *v = activeViewer();
  if (!v) return;
  QString file_name = QFileDialog::getOpenFileName(this,"Open",".");
  if (file_name.isEmpty()) return;
  QImage* image = new QImage(file_name);
  if (!image && image->isNull()) return;
  v->setTextureImage(image);

  QPixmap pix;
  if (pix.convertFromImage(*image)){
    QIcon icon(pix);
    texture_select_button->setIconSize(QSize(texture_select_button->width(),texture_select_button->height()));
    texture_select_button->setIcon(icon);
  }
}

void MainWindow::divideAction(){
  Viewer *v = activeViewer();
  if (!v) return;
  bool ok = false;
  int number = \
    QInputDialog::getInteger(this,QString("Subdivision"),
                             QString("Input iteration times"),
                             0,0,10,1,&ok);
  if (ok)
    v->subDivide(number);
}

Viewer* MainWindow::activeViewer(){
  QMdiSubWindow *sub_window = mdi_area->activeSubWindow();
  if (sub_window)
    return qobject_cast<Viewer *>(sub_window->widget());
  return 0;
}

void MainWindow::createScaleWidget(){
  QHBoxLayout *scale_top = new QHBoxLayout();
  QLabel *scale_label = new QLabel(QString("Scale:"),this);
  QLabel *scale_icon = new QLabel(this);
  scale_icon->setPixmap(scale_pix->scaled(25,25));
  scale_top->addWidget(scale_icon);
  scale_top->addWidget(scale_label);

  QHBoxLayout *scale_box_x = new QHBoxLayout();
  QLabel *scale_label_x = new QLabel(QString("x:"),this);
  scale_spinbox_x = new QDoubleSpinBox(this);
  scale_spinbox_x->setRange(-100.0,100.0);
  scale_spinbox_x->setSingleStep(0.1);
  scale_box_x->addWidget(scale_label_x);
  scale_box_x->addWidget(scale_spinbox_x);

  QHBoxLayout *scale_box_y = new QHBoxLayout();
  QLabel *scale_label_y = new QLabel(QString("y:"),this);
  scale_spinbox_y = new QDoubleSpinBox(this);
  scale_spinbox_y->setRange(-100.0,100.0);
  scale_spinbox_y->setSingleStep(0.1);
  scale_box_y->addWidget(scale_label_y);
  scale_box_y->addWidget(scale_spinbox_y);

  QHBoxLayout *scale_box_z = new QHBoxLayout();
  QLabel *scale_label_z = new QLabel(QString("z:"),this);
  scale_spinbox_z = new QDoubleSpinBox(this);
  scale_spinbox_z->setRange(-100.0,100.0);
  scale_spinbox_z->setSingleStep(0.1);
  scale_box_z->addWidget(scale_label_z);
  scale_box_z->addWidget(scale_spinbox_z);

  QVBoxLayout *scale_box_layout = new QVBoxLayout();
  scale_box_layout->addLayout(scale_top);
  scale_box_layout->addLayout(scale_box_x);
  scale_box_layout->addLayout(scale_box_y);
  scale_box_layout->addLayout(scale_box_z);

  QWidget *scale_box_widget = new QWidget(this);
  scale_box_widget->setLayout(scale_box_layout);

  connect(scale_spinbox_x,SIGNAL(valueChanged(double)),
          this,SLOT(scaleXAction(double)));
  connect(scale_spinbox_y,SIGNAL(valueChanged(double)),
          this,SLOT(scaleYAction(double)));
  connect(scale_spinbox_z,SIGNAL(valueChanged(double)),
          this,SLOT(scaleZAction(double)));
  left_toolbar->addWidget(scale_box_widget);
}

void MainWindow::createLightWidget(){
  QVBoxLayout *light_layout = new QVBoxLayout();
  //light icon
  QHBoxLayout *light_top = new QHBoxLayout();
  QLabel *light_label = new QLabel(QString("Light:"),this);
  QLabel *light_icon = new QLabel(this);
  light_icon->setPixmap(light_pix->scaled(25,25));
  light_top->addWidget(light_icon);
  light_top->addWidget(light_label);
  light_layout->addLayout(light_top);
  //show normal
  show_normals_box = new QCheckBox("normals",this);
  show_normals_box->setCheckState(Qt::Checked);
  connect(show_normals_box,SIGNAL(stateChanged(int)),
          this,SLOT(showNormals(int)));
  light_layout->addWidget(show_normals_box);
  //show light
  show_light_box = new QCheckBox("light",this);
  show_light_box->setCheckState(Qt::Checked);
  connect(show_light_box,SIGNAL(stateChanged(int)),
          this,SLOT(showLight(int)));
  light_layout->addWidget(show_light_box);
  //light position
  QLabel *light_pos_label = new QLabel(QString("Light pos:"),this);
  light_layout->addWidget(light_pos_label);
  QHBoxLayout *light_pos_box_x = new QHBoxLayout();
  QLabel *light_pos_label_x = new QLabel(QString("x:"),this);
  light_pos_spinbox_x = new QDoubleSpinBox(this);
  light_pos_spinbox_x->setRange(-1000.0,1000.0);
  light_pos_spinbox_x->setSingleStep(0.1);
  light_pos_box_x->addWidget(light_pos_label_x);
  light_pos_box_x->addWidget(light_pos_spinbox_x);
  connect(light_pos_spinbox_x,SIGNAL(valueChanged(double)),
          this,SLOT(lightPosXAction(double)));
  light_layout->addLayout(light_pos_box_x);
  QHBoxLayout *light_pos_box_y = new QHBoxLayout();
  QLabel *light_pos_label_y = new QLabel(QString("y:"),this);
  light_pos_spinbox_y = new QDoubleSpinBox(this);
  light_pos_spinbox_y->setRange(-1000.0,1000.0);
  light_pos_spinbox_y->setSingleStep(0.1);
  light_pos_box_y->addWidget(light_pos_label_y);
  light_pos_box_y->addWidget(light_pos_spinbox_y);
  connect(light_pos_spinbox_y,SIGNAL(valueChanged(double)),
          this,SLOT(lightPosYAction(double)));
  light_layout->addLayout(light_pos_box_y);
  QHBoxLayout *light_pos_box_z = new QHBoxLayout();
  QLabel *light_pos_label_z = new QLabel(QString("z:"),this);
  light_pos_spinbox_z = new QDoubleSpinBox(this);
  light_pos_spinbox_z->setRange(-1000.0,1000.0);
  light_pos_spinbox_z->setSingleStep(0.1);
  light_pos_box_z->addWidget(light_pos_label_z);
  light_pos_box_z->addWidget(light_pos_spinbox_z);
  connect(light_pos_spinbox_z,SIGNAL(valueChanged(double)),
          this,SLOT(lightPosZAction(double)));
  light_layout->addLayout(light_pos_box_z);
  //light pos end
  //light pro start
  QHBoxLayout *light_ambient_box = new QHBoxLayout();
  QLabel *light_ambient_label = new QLabel(QString("ambient:"),this);
  light_ambient_button = new QPushButton(this);
  light_ambient_button->setFixedSize(35,25);
  connect(light_ambient_button,SIGNAL(clicked()),
          this,SLOT(lightAmbientAction()));
  light_ambient_box->addWidget(light_ambient_label);
  light_ambient_box->addWidget(light_ambient_button);
  light_layout->addLayout(light_ambient_box);
  QHBoxLayout *light_diffuse_box = new QHBoxLayout();
  QLabel *light_diffuse_label = new QLabel(QString("diffuse:"),this);
  light_diffuse_button = new QPushButton(this);
  light_diffuse_button->setFixedSize(35,25);
  connect(light_diffuse_button,SIGNAL(clicked()),
          this,SLOT(lightDiffuseAction()));
  light_diffuse_box->addWidget(light_diffuse_label);
  light_diffuse_box->addWidget(light_diffuse_button);
  light_layout->addLayout(light_diffuse_box);  
  QHBoxLayout *light_specular_box = new QHBoxLayout();
  QLabel *light_specular_label = new QLabel(QString("specular:"),this);
  light_specular_button = new QPushButton(this);
  light_specular_button->setFixedSize(35,25);
  connect(light_specular_button,SIGNAL(clicked()),
          this,SLOT(lightSpecularAction()));
  light_specular_box->addWidget(light_specular_label);
  light_specular_box->addWidget(light_specular_button);
  light_layout->addLayout(light_specular_box);
  //light pro end
  QWidget *light_box_widget = new QWidget(this);
  light_box_widget->setLayout(light_layout);
  left_toolbar->addWidget(light_box_widget);
}

void MainWindow::createMaterialWidget(){
  QVBoxLayout *material_layout = new QVBoxLayout();
  //light icon
  QHBoxLayout *material_top = new QHBoxLayout();
  QLabel *material_label = new QLabel(QString("Material:"),this);
  QLabel *material_icon = new QLabel(this);
  material_icon->setPixmap(material_pix->scaled(25,25));
  material_top->addWidget(material_icon);
  material_top->addWidget(material_label);
  material_layout->addLayout(material_top);
  //ambient box
  QHBoxLayout *material_ambient_box = new QHBoxLayout();
  QLabel *material_ambient_label=new QLabel(QString("ambient:"),this);
  material_ambient_button = new QPushButton(this);
  material_ambient_button->setFixedSize(35,25);
  connect(material_ambient_button,SIGNAL(clicked()),
          this,SLOT(materialAmbientAction()));
  material_ambient_box->addWidget(material_ambient_label);
  material_ambient_box->addWidget(material_ambient_button);
  material_layout->addLayout(material_ambient_box);
  //diffuse box
  QHBoxLayout *material_diffuse_box = new QHBoxLayout();
  QLabel *material_diffuse_label=new QLabel(QString("diffuse:"),this);
  material_diffuse_button = new QPushButton(this);
  material_diffuse_button->setFixedSize(35,25);
  connect(material_diffuse_button,SIGNAL(clicked()),
          this,SLOT(materialDiffuseAction()));
  material_diffuse_box->addWidget(material_diffuse_label);
  material_diffuse_box->addWidget(material_diffuse_button);
  material_layout->addLayout(material_diffuse_box);
  //specular box
  QHBoxLayout *material_specular_box = new QHBoxLayout();
  QLabel *material_specular_label = 
    new QLabel(QString("specular:"),this);
  material_specular_button = new QPushButton(this);
  material_specular_button->setFixedSize(35,25);
  connect(material_specular_button,SIGNAL(clicked()),
          this,SLOT(materialSpecularAction()));
  material_specular_box->addWidget(material_specular_label);
  material_specular_box->addWidget(material_specular_button);
  material_layout->addLayout(material_specular_box);
  //emission box
  QHBoxLayout *material_emission_box = new QHBoxLayout();
  QLabel *material_emission_label = 
    new QLabel(QString("emission:"),this);
  material_emission_button = new QPushButton(this);
  material_emission_button->setFixedSize(35,25);
  connect(material_emission_button,SIGNAL(clicked()),
          this,SLOT(materialEmissionAction()));
  material_emission_box->addWidget(material_emission_label);
  material_emission_box->addWidget(material_emission_button);
  material_layout->addLayout(material_emission_box);
  
  QHBoxLayout *material_shininess_box = new QHBoxLayout();
  QLabel *material_shininess_label = 
    new QLabel(QString("shininess:"),this);
  material_shininess_spinbox = new QDoubleSpinBox(this);
  material_shininess_spinbox->setRange(0,100.0);
  material_shininess_spinbox->setSingleStep(1);
  material_shininess_box->addWidget(material_shininess_label);
  material_shininess_box->addWidget(material_shininess_spinbox);
  connect(material_shininess_spinbox,SIGNAL(valueChanged(double)),
          this,SLOT(materialShininessAction(double)));
  material_layout->addLayout(material_shininess_box);

  QWidget *material_box_widget = new QWidget(this);
  material_box_widget->setLayout(material_layout);
  left_toolbar->addWidget(material_box_widget);  
}

void MainWindow::enableLightWidget(){
  light_pos_spinbox_x->setEnabled(true);
  light_pos_spinbox_y->setEnabled(true);
  light_pos_spinbox_z->setEnabled(true);
  light_ambient_button->setEnabled(true);
  light_diffuse_button->setEnabled(true);
  light_specular_button->setEnabled(true);
  material_ambient_button->setEnabled(true);
  material_diffuse_button->setEnabled(true);
  material_specular_button->setEnabled(true);
  material_emission_button->setEnabled(true);
  material_shininess_spinbox->setEnabled(true);
}

void MainWindow::disableLightWidget(){
  light_pos_spinbox_x->setEnabled(false);
  light_pos_spinbox_y->setEnabled(false);
  light_pos_spinbox_z->setEnabled(false);
  light_ambient_button->setEnabled(false);
  light_diffuse_button->setEnabled(false);
  light_specular_button->setEnabled(false);
  material_ambient_button->setEnabled(false);
  material_diffuse_button->setEnabled(false);
  material_specular_button->setEnabled(false);
  material_emission_button->setEnabled(false);
  material_shininess_spinbox->setEnabled(false);
}

void MainWindow::createTextureWidget(){
  QVBoxLayout *texture_layout = new QVBoxLayout();

  QHBoxLayout *texture_top = new QHBoxLayout();
  QLabel *texture_icon = new QLabel(this);
  QLabel *texture_label = new QLabel(QString("Texture:"),this);
  texture_icon->setPixmap(texture_pix->scaled(25,25));
  texture_top->addWidget(texture_icon);
  texture_top->addWidget(texture_label);
  texture_layout->addLayout(texture_top);

  texture_checkbox = new QCheckBox(QString("enable"),this);
  texture_checkbox->setChecked(true);
  texture_layout->addWidget(texture_checkbox);
  connect(texture_checkbox,SIGNAL(stateChanged(int)),
          this,SLOT(textureShowAction(int)));

  QLabel *texture_select_label = new QLabel(QString("select"),this);
  texture_layout->addWidget(texture_select_label);
  
  texture_select_button = new QPushButton();
  texture_select_button->setMinimumHeight(100);
  texture_layout->addWidget(texture_select_button);
  connect(texture_select_button,SIGNAL(clicked()),
          this,SLOT(textureSelectAction()));

  QWidget *texture_widget = new QWidget();
  texture_widget->setLayout(texture_layout);
  left_toolbar->addWidget(texture_widget);
}

void MainWindow::enableTextureWidget(){
  texture_select_button->setEnabled(true);
}

void MainWindow::disableTextureWidget(){
  texture_select_button->setEnabled(false);
}

void MainWindow::buttonSetColor(QPushButton *button,QColor color){
    const QString COLOR_STYLE("QPushButton {background-color: %1;}");
    button->setStyleSheet(COLOR_STYLE.arg(color.name()));
}

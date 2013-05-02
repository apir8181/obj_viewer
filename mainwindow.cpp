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
}

void MainWindow::createLeftToolBar(){
  left_toolbar = new QToolBar(this);
  addToolBar(Qt::LeftToolBarArea,left_toolbar);
  createScaleWidget();
  createLightWidget();
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
  line_action->setChecked(true);
  polygon_action->setCheckable(true);
  shape_action_group = new QActionGroup(this);
  shape_action_group->addAction(point_action);
  shape_action_group->addAction(line_action);
  shape_action_group->addAction(polygon_action);
  connect(point_action,SIGNAL(triggered()),this,SLOT(pointAction()));
  connect(line_action,SIGNAL(triggered()),this,SLOT(lineAction()));
  connect(polygon_action,SIGNAL(triggered()),
          this,SLOT(polygonAction()));
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

    //update light
    if (now->getShowNormals())
      show_normals_box->setCheckState(Qt::Checked);
    else show_normals_box->setCheckState(Qt::Unchecked);
    if (now->getShowLight())
      show_light_box->setCheckState(Qt::Checked);
    else show_light_box->setCheckState(Qt::Unchecked);
  }
}

void MainWindow::open(){
  QString file_name = QFileDialog::getOpenFileName(this,"Open",".");
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

void MainWindow::showLight(int state){
  Viewer* v = activeViewer();
  if (v){
    if (state == Qt::Checked){
      v->enableLight();
    }
    else v->disableLight();
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
  QVBoxLayout *light_layout = new QVBoxLayout(this);
  //light icon
  QHBoxLayout *light_top = new QHBoxLayout(this);
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
  QLable *light_pos_label = new QLabel(QString("Light pos:"),this);
  light_layout->addWidget(light_pos_label);
  QHBoxLayout *light_pos_box_x = new QHBoxLayout(this);
  QLabel *light_pos_label_x = new QLabel(QString("x:"),this);
  light_pos_spinbox_x = new QDoubleSpinBox(this);
  light_pos_spinbox_x->setRange(-1000.0,1000.0);
  light_pos_spinbox_x->setSingleStep(0.1);
  light_pos_box_x->addWidget(light_pos_label_x);
  light_pos_box_x->addWidget(light_pos_spinbox_x);
  connect(light_pos_spinbox_x,SIGNAL(valueChanged(double)),
          this,SLOT(lightPosXAction(double)));
  light_layout->addLayout(light_pos_box_x);
  QHBoxLayout *light_pos_box_y = new QHBoxLayout(this);
  QLabel *light_pos_label_y = new QLabel(QString("y:"),this);
  light_pos_spinbox_y = new QDoubleSpinBox(this);
  light_pos_spinbox_y->setRange(-1000.0,1000.0);
  light_pos_spinbox_y->setSingleStep(0.1);
  light_pos_box_y->addWidget(light_pos_label_y);
  light_pos_box_y->addWidget(light_pos_spinbox_y);
  connect(light_pos_spinbox_y,SIGNAL(valueChanged(double)),
          this,SLOT(lightPosYAction(double)));
  light_layout->addLayout(light_pos_box_y);
  QHBoxLayout *light_pos_box_z = new QHBoxLayout(this);
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

  QWidget *light_box_widget = new QWidget(this);
  light_box_widget->setLayout(light_layout);
  left_toolbar->addWidget(light_box_widget);
}
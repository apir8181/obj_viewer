#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QMdiArea>
#include <QActionGroup>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include "viewer.h"

class MainWindow:public QMainWindow{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void updateActions();
  void open();

  void rotateAction();
  void translateAction();

  void scaleXAction(double num);
  void scaleYAction(double num);
  void scaleZAction(double num);

  void showNormals(int state);
  void showLight(int state);

  void lightPosXAction(double num);
  void lightPosYAction(double num);
  void lightPosZAction(double num);

  void pointAction();
  void lineAction();
  void polygonAction();


private:
  QPixmap *open_pix;
  QPixmap *close_pix;
  QPixmap *close_all_pix;
  QPixmap *quit_pix;
  QPixmap *next_pix;
  QPixmap *previous_pix;
  QPixmap *rotate_pix;
  QPixmap *translate_pix;
  QPixmap *point_pix;
  QPixmap *line_pix;
  QPixmap *polygon_pix;
  QPixmap *scale_pix;
  QPixmap *light_pix;

  QAction *open_action;
  QAction *close_action;
  QAction *close_all_action;
  QAction *quit_action;

  QActionGroup *window_action_group;  
  QAction *next_action;
  QAction *previous_action;

  QActionGroup *operation_action_group;
  QAction *rotate_action;
  QAction *translate_action;

  QActionGroup *shape_action_group;
  QAction *point_action;
  QAction *line_action;
  QAction *polygon_action;

  QMdiArea *mdi_area;
  QMenu *file_menu;
  QMenu *window_menu;
  QMenu *operation_menu;
  QToolBar *toolbar;
  QToolBar *left_toolbar;
  //scale widget
  QDoubleSpinBox *scale_spinbox_x;
  QDoubleSpinBox *scale_spinbox_y;
  QDoubleSpinBox *scale_spinbox_z;
  //light widget
  QCheckBox *show_normals_box;
  QCheckBox *show_light_box;
  //light position widget
  QDoubleSpinBox *light_pos_spinbox_x;
  QDoubleSpinBox *light_pos_spinbox_y;
  QDoubleSpinBox *light_pos_spinbox_z;

  void createMenuBar();
  void createToolBar();
  void createLeftToolBar();
  void createScaleWidget();
  void createLightWidget();
  void createResources();
  void createActions();
  Viewer *activeViewer();
};

#endif

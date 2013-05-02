#include "mainwindow.h"
#include <GL/glut.h>

int main(int argc,char **argv){
  glutInit(&argc,argv);
  QApplication app(argc,argv);
  MainWindow window;
  window.resize(1024,800);
  window.move(0,0);
  window.setWindowTitle("Small OPENGL Reader");
  window.show();
  return app.exec();
}

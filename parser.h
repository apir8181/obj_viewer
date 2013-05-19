#ifndef PARSER_H
#define PARSER_H

#include "parserstruct.h"
#include "hedgemesh.h"
#include <string>
#include <vector>
#include <map>

using namespace std;


class Parser{
 public:
  //assume the file_name is right and 
  //the file has correct obj file format
  Parser(const char* file_name);
  ~Parser();
  void drawInGL(DRAW_TYPE t,bool is_light,bool show_normals,
                bool show_texture);
  void printInfo();
  bool getSuccess();
  HEdgeMesh *mesh;
 private:
  void resizeVector();
  bool success;
  objInfo object;

  float vertice_scale_x, vertice_scale_y, vertice_scale_z;
  float vertice_transform_x, vertice_transform_y, vertice_transform_z;

};

#endif

#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

using namespace std;

struct faceInfo{
  vector<int> vertices_index;
  vector<int> text_coords_index;
  vector<int> normals_index;
  int smooth_index;
};

struct objInfo{
  string name;
  vector<float*> vertices4f;
  vector<float*> text_coords3f;
  vector<float*> normals3f;
  vector<faceInfo> faces;
};

enum DRAW_TYPE{
  DRAW_POINT,
  DRAW_LINE,
  DRAW_POLY
};

class Parser{
 public:
  //assume the file_name is right and 
  //the file has correct obj file format
  Parser(const char* file_name);
  ~Parser();
  void drawInGL(DRAW_TYPE t,bool is_light,bool show_normals);
  void printInfo();
  bool getSuccess();
 private:
  void resizeVector();
  bool success;
  objInfo object;

  float vertice_scale_x, vertice_scale_y, vertice_scale_z;
  float vertice_transform_x, vertice_transform_y, vertice_transform_z;
};

#endif

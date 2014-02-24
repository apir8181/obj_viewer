#ifndef PARSERSTRUCT_H
#define PARSERSTRUCT_H

#include <vector>
#include <string>
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


struct Vertex4f{
  float x, y, z, w;
  Vertex4f(float _x,float _y,float _z, float _w = 1){
    x = _x; y = _y; z = _z; w = _w;
  }
};

struct Vertex3f{
  float x, y, z;
  Vertex3f(float _x, float _y, float _z = 0){
    x = _x; y = _y; z = _z;
  }
};


struct Triangle{
  int a, b, c, at, bt, ct;
  Triangle(int _a,int _b,int _c,int _at = 0,int _bt = 0,int _ct = 0){
    a = _a; b = _b; c = _c;
    at = _at; bt = _bt; ct = _ct;
  }
};

#endif

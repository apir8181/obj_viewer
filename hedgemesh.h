#ifndef HEDGEMESH_H
#define HEDGEMESH_H

#include "parserstruct.h"
#include <map>
#include <vector>

using namespace std;

class HEdgeMesh{
 public:
  HEdgeMesh(objInfo& info);
  ~HEdgeMesh();
  void loopDivision(int scale_number);
  void printInfo();
  void drawInGL(DRAW_TYPE t,bool light,bool normal,bool texture);
  
 private:
  vector<Vertex4f> vertices4f;
  vector<Vertex3f> normals3f;
  vector<Vertex3f> texture3f;
  vector<Triangle> triangles;

  void vertices4fInit(vector<float*>& v);
  void normals3fInit(vector<float*>& v);
  void texture3fInit(vector<float*>& v,vector<float*>& v2);

  void breakIntoTriangles(vector<faceInfo> &faces);
  void buildPreprocess();
  void build();
  Vertex3f calculateNormal(int a,int b,int c);
  void divide();
  Vertex4f updateOdd(int start,int end,int tri_index);
  Vertex3f updateOddTexture(int a,int b,int at,int bt,int tri_index);
  Vertex4f updateEven(int index,vector<int>* edge);
  Vertex3f updateEvenTexture(int index,vector<int>*edge);
  int getExculdeTriIndex(int a,int b,int tri_index);

  /*
  vector<Vertex4f> odd_vertices4f;
  vector<Vertex4f> even_vertices4f;
  map< pair<int,int>, int> m_edge_to_odd_index;
  int getEdgeToOddIndex(int start,int end);
  void setEdgeToOddIndex(int start,int end,int index);

  void addOdd();
  void updateEven();
  Vertex4f __updateEven(int index,vector<int>* edge);
  void updateOdd();
  Vertex4f __updateOdd(int start,int end,int tri_index);
  int getExculdeTriIndex(int a,int b,int tri_index);
  void mergeOddAndEven();
  */
  map< pair<int,int>, vector<int> > m_edge_to_triangle_index;
  int getAnotherTriangleIndex(int start_index,int end_index,int now_face);
  void setTriangleIndex(int start_index,int end_index,int index);

};

#endif

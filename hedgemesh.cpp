#include "hedgemesh.h"
#include "parserstruct.h"
#include <assert.h>
#include <utility>
#include <map>
#include <set>
#include <math.h>
#include <vector>
#include <queue>
#include <stdio.h>
#include <GL/glut.h>
#define MP(a,b) (make_pair(a,b))
#define NOTEXIST -1

using namespace std;

HEdgeMesh::HEdgeMesh(objInfo& info){
  vertices4f.clear();
  normals3f.clear();
  triangles.clear();
  texture3f.clear();
  m_edge_to_triangle_index.clear();

  vertices4fInit(info.vertices4f);
  //normals3fInit(info.normals3f);
  texture3fInit(info.text_coords3f,info.vertices4f);
  breakIntoTriangles(info.faces);
  buildPreprocess();
  build();
}

HEdgeMesh::~HEdgeMesh(){
  vertices4f.clear();
  normals3f.clear();
  triangles.clear();
  m_edge_to_triangle_index.clear();
}


void HEdgeMesh::vertices4fInit(vector<float*>& v){
  float* temp;
  for (vector<float*>::iterator it = v.begin();
       it != v.end(); ++ it){
    temp = *it;
    vertices4f.push_back(Vertex4f(temp[0],temp[1],temp[2],temp[3]));
  }
}

void HEdgeMesh::normals3fInit(vector<float*>& v){
  float *temp;
  for (vector<float*>::iterator it = v.begin();
       it != v.end(); ++ it){
    temp = *it;
    normals3f.push_back(Vertex3f(temp[0],temp[1],temp[2]));
  }
}

void HEdgeMesh::texture3fInit(vector<float*>& v,vector<float*>& v2){
  float *temp;
  for (unsigned int i = 0; i < v.size(); ++i){
    temp = v[i];
    texture3f.push_back(Vertex3f(temp[0],temp[1],temp[2]));
  }
  if (v.size() != v2.size()){
    for (unsigned int i = v.size(); i < v2.size(); ++i)
      texture3f.push_back(Vertex3f(0,0,0));
  }
}

void HEdgeMesh::breakIntoTriangles(vector<faceInfo> &faces){
  for (vector<faceInfo>::iterator it = faces.begin();
       it != faces.end(); ++ it){
    vector<int> temp = it->vertices_index;
    vector<int> temp2 = it->text_coords_index;
    for (unsigned int i = 2; i < temp.size(); i ++){
      Triangle t = Triangle(temp[0]-1,temp[i-1]-1,temp[i]-1);
      if (temp2.size() == temp.size()){
        t.at = temp2[0]-1; t.bt = temp2[i-1]-1; t.ct = temp2[i]-1;
      }
      triangles.push_back(t);
    }
  }
}

void HEdgeMesh::buildPreprocess(){
  m_edge_to_triangle_index.clear();
  for (unsigned int i = 0; i < triangles.size(); i ++){
    setTriangleIndex(triangles[i].a,triangles[i].b,i);
    setTriangleIndex(triangles[i].b,triangles[i].c,i);
    setTriangleIndex(triangles[i].c,triangles[i].a,i);
  }
}

void HEdgeMesh::build(){
  bool* triangle_is_used = NULL;
  if (triangles.size() != 0) triangle_is_used = new bool[triangles.size()];
  for (unsigned int i = 0; i < triangles.size(); i ++)
    triangle_is_used[i] = false;


  set< pair<int,int> > hedge_used; hedge_used.clear();
  queue<int> triangle_queue;
  while (true){
    bool flag = true;
    for (unsigned int i = 0; i < triangles.size(); i ++){
      if (triangle_is_used[i] == false){
        triangle_queue.push(i);
        triangle_is_used[i] = true;
        flag = false;
        break;
      }
    }
    if (flag) break;

    int now, another, temp;
    while (!triangle_queue.empty()){
      now = triangle_queue.front();
      triangle_queue.pop();
      
      if (hedge_used.find(MP(triangles[now].a,triangles[now].b)) !=
          hedge_used.end() ||
          hedge_used.find(MP(triangles[now].b,triangles[now].c)) !=
          hedge_used.end() ||
          hedge_used.find(MP(triangles[now].c,triangles[now].a)) !=
          hedge_used.end() ){
        temp = triangles[now].a;
        triangles[now].a = triangles[now].c;
        triangles[now].c = temp;
        temp = triangles[now].at;
        triangles[now].at = triangles[now].ct;
        triangles[now].ct = temp;
      }
      hedge_used.insert(MP(triangles[now].a,triangles[now].b));
      hedge_used.insert(MP(triangles[now].b,triangles[now].c));
      hedge_used.insert(MP(triangles[now].c,triangles[now].a));

      another = getAnotherTriangleIndex(triangles[now].a,triangles[now].b,now);
      if (another != NOTEXIST && triangle_is_used[another] == false){
        triangle_queue.push(another);
        triangle_is_used[another] = true;
      }
      another = getAnotherTriangleIndex(triangles[now].b,triangles[now].c,now);
      if (another != NOTEXIST && triangle_is_used[another] == false){
        triangle_queue.push(another);
        triangle_is_used[another] = true;
      }
      another = getAnotherTriangleIndex(triangles[now].c,triangles[now].a,now);
      if (another != NOTEXIST && triangle_is_used[another] == false){
        triangle_queue.push(another);
        triangle_is_used[another] = true;
      }
    }
  }

  if (triangle_is_used != NULL) delete[] triangle_is_used;

  normals3f.clear();
  for (unsigned int i = 0; i < triangles.size(); i ++)
    normals3f.push_back(calculateNormal(triangles[i].a,triangles[i].b,triangles[i].c));
}

Vertex3f HEdgeMesh::calculateNormal(int a,int b,int c){
  Vertex3f m(vertices4f[b].x-vertices4f[a].x, vertices4f[b].y-vertices4f[a].y , vertices4f[b].z-vertices4f[a].z);
  Vertex3f n(vertices4f[c].x-vertices4f[b].x, vertices4f[c].y-vertices4f[b].y , vertices4f[c].z-vertices4f[b].z);
  Vertex3f normal(m.y*n.z-m.z*n.y, m.z*n.x-m.x*n.z, m.x*n.y-m.y*n.x);
  float scale = sqrt(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z);
  normal.x /= scale; normal.y /= scale; normal.z /= scale;
  return normal;
}

int HEdgeMesh::getAnotherTriangleIndex(int start_index,int end_index,int now_face){
  if (start_index > end_index){
    int temp = start_index;
    start_index = end_index;
    end_index = temp;
  }
  vector<int> v = m_edge_to_triangle_index[MP(start_index,end_index)];
  if (v.size() > 0 && v[0] != now_face) return v[0];
  else if (v.size() > 1 && v[1] != now_face) return v[1];
  else return NOTEXIST;
}

void HEdgeMesh::setTriangleIndex(int start_index,int end_index,int index){
  if (start_index > end_index){
    int temp = start_index;
    start_index = end_index;
    end_index = temp;
  }
  m_edge_to_triangle_index[MP(start_index,end_index)].push_back(index);
}

void HEdgeMesh::printInfo(){
  for (unsigned int i = 0; i < triangles.size(); i ++){
    printf("triangle:%d\n",i);
    printf("vertex:\n");
    Vertex4f temp = vertices4f[triangles[i].a];
    printf("%f,%f,%f\n",temp.x,temp.y,temp.z);
    temp = vertices4f[triangles[i].b];
    printf("%f,%f,%f\n",temp.x,temp.y,temp.z);
    temp = vertices4f[triangles[i].c];
    printf("%f,%f,%f\n",temp.x,temp.y,temp.z);
    printf("normal:\n");
    printf("%f,%f,%f\n",normals3f[i].x,normals3f[i].y,normals3f[i].z);
  }
}

void HEdgeMesh::drawInGL(DRAW_TYPE t,bool light,bool normal,bool texture){
  glPushMatrix();  

  Vertex4f v4f(0,0,0);
  Vertex3f v3f(0,0,0);
  if (t == DRAW_POLY){
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.3,0.3,0.3);
    /*for (unsigned int i = 0; i < triangles.size(); ++i){
      glBegin(GL_LINE_LOOP);
      v4f = vertices4f[triangles[i].a];
      glVertex4f(v4f.x,v4f.y,v4f.z,v4f.w);
      v4f = vertices4f[triangles[i].b];
      glVertex4f(v4f.x,v4f.y,v4f.z,v4f.w);
      v4f = vertices4f[triangles[i].c];
      glVertex4f(v4f.x,v4f.y,v4f.z,v4f.w);
      glEnd();
      }*/    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_TEXTURE_2D);
  }

  if (t == DRAW_POINT || t == DRAW_LINE || !light){
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
  }
  if (t == DRAW_POINT || t == DRAW_LINE || !texture) 
    glDisable(GL_TEXTURE_2D); 

  for (unsigned int i = 0; i < triangles.size(); ++ i){
    if (t == DRAW_POINT){ 
      glBegin(GL_POINTS); 
      glColor3f(0,0,0); 
    }
    else if (t == DRAW_LINE){
      glBegin(GL_LINE_LOOP); 
      glColor3f(0,0,0);
    }
    else{
      if (!light) glColor3f(0.5,0.5,0.5);
      glBegin(GL_TRIANGLES);
    }
    
    v3f = normals3f[i];
    glNormal3f(v3f.x,v3f.y,v3f.z);

    if (texture){
      v3f = texture3f[triangles[i].at];
      glTexCoord2f(v3f.x,v3f.y);
    }
    v4f = vertices4f[triangles[i].a];
    glVertex4f(v4f.x,v4f.y,v4f.z,v4f.w);

    if (texture){
      v3f = texture3f[triangles[i].bt];
      glTexCoord2f(v3f.x,v3f.y);
    }
    v4f = vertices4f[triangles[i].b];
    glVertex4f(v4f.x,v4f.y,v4f.z,v4f.w);

    if (texture){
      v3f = texture3f[triangles[i].ct];
      glTexCoord2f(v3f.x,v3f.y);
    }
    v4f = vertices4f[triangles[i].c];
    glVertex4f(v4f.x,v4f.y,v4f.z,v4f.w);

    glEnd();
  }

  glDisable(GL_TEXTURE_2D); 
  if (normal){
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glBegin(GL_LINES);
    glColor3f(0.0,0.717647,0.921568);//cyan
    for (unsigned int i = 0; i < triangles.size(); ++ i){
      v4f = vertices4f[triangles[i].a];
      v3f = normals3f[i];
      glVertex4f(v4f.x,v4f.y,v4f.z,v4f.w);
      glVertex4f(v4f.x+v3f.x*0.1,v4f.y+v3f.y*0.1,
                 v4f.z+v3f.z*0.1,v4f.w);
      v4f = vertices4f[triangles[i].b];
      glVertex4f(v4f.x,v4f.y,v4f.z,v4f.w);
      glVertex4f(v4f.x+v3f.x*0.1,v4f.y+v3f.y*0.1,
                 v4f.z+v3f.z*0.1,v4f.w);
      v4f = vertices4f[triangles[i].c];
      glVertex4f(v4f.x,v4f.y,v4f.z,v4f.w);
      glVertex4f(v4f.x+v3f.x*0.1,v4f.y+v3f.y*0.1,
                 v4f.z+v3f.z*0.1,v4f.w);
    }
    glEnd();
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
  }

  glPopMatrix();
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
}

void HEdgeMesh::loopDivision(int scale_number){
  for (int i = 0; i < scale_number; i ++)
    divide();
}

void HEdgeMesh::divide(){
  //update odd
  vector<Vertex4f> odd_vertices4f; odd_vertices4f.clear();
  vector<Vertex3f> odd_texture3f; odd_texture3f.clear();
  map< pair<int,int>,int > odd_m; odd_m.clear();
  for (unsigned int i = 0; i < triangles.size(); ++i){
    int a = triangles[i].a, b = triangles[i].b, c = triangles[i].c,
      at = triangles[i].at, bt = triangles[i].bt,ct = triangles[i].ct;
    if (odd_m.find(MP(a,b)) == odd_m.end() &&
        odd_m.find(MP(b,a)) == odd_m.end()){
      int index = odd_vertices4f.size();
      odd_m[MP(a,b)] = odd_m[MP(b,a)] = index;
      odd_vertices4f.push_back(updateOdd(a,b,i));
      odd_texture3f.push_back(updateOddTexture(a,b,at,bt,i));
    }
    if (odd_m.find(MP(a,c)) == odd_m.end() &&
        odd_m.find(MP(c,a)) == odd_m.end()){
      int index = odd_vertices4f.size();
      odd_m[MP(a,c)] = odd_m[MP(c,a)] = index;
      odd_vertices4f.push_back(updateOdd(a,c,i));
      odd_texture3f.push_back(updateOddTexture(a,c,at,ct,i));
    }
    if (odd_m.find(MP(b,c)) == odd_m.end() &&
        odd_m.find(MP(c,b)) == odd_m.end()){
      int index = odd_vertices4f.size();
      odd_m[MP(b,c)] = odd_m[MP(c,b)] = index;
      odd_vertices4f.push_back(updateOdd(b,c,i));
      odd_texture3f.push_back(updateOddTexture(b,c,bt,ct,i));
    }
  }
  //update even
  vector<Vertex4f> even_vertices4f; even_vertices4f.clear();
  vector<Vertex3f> even_texture3f; even_texture3f.clear();
  vector<int>* edge = NULL;
  vector<int>* t_edge = NULL;
  if (vertices4f.size() != 0)
    edge = new vector<int>[vertices4f.size()];
  if (texture3f.size() != 0)
    t_edge = new vector<int>[texture3f.size()];
  for (unsigned int i = 0; i < triangles.size(); ++i){
    int a = triangles[i].a, b = triangles[i].b, c = triangles[i].c,
      at = triangles[i].at, bt = triangles[i].bt,ct = triangles[i].ct;
    edge[a].push_back(b);
    edge[b].push_back(a);
    edge[a].push_back(c);
    edge[c].push_back(a);
    edge[b].push_back(c);
    edge[c].push_back(b);
    t_edge[at].push_back(bt);
    t_edge[bt].push_back(at);
    t_edge[at].push_back(ct);
    t_edge[ct].push_back(at);
    t_edge[bt].push_back(ct);
    t_edge[ct].push_back(bt);
  }
  for (unsigned int i = 0; i < vertices4f.size(); ++i){
    even_vertices4f.push_back(updateEven(i,edge));
    even_texture3f.push_back(updateEvenTexture(i,t_edge));
  }
  delete[] edge;
  delete[] t_edge;

  vertices4f.clear();
  texture3f.clear();
  for (unsigned int i = 0; i < even_vertices4f.size(); ++i){
    vertices4f.push_back(even_vertices4f[i]);
    texture3f.push_back(even_texture3f[i]);
  }
  for (unsigned int i = 0; i < odd_vertices4f.size(); ++i){
    vertices4f.push_back(odd_vertices4f[i]);
    texture3f.push_back(odd_texture3f[i]);
  }

  vector<Triangle> temp_tri; temp_tri.clear();
  int offset = even_vertices4f.size();
  for (unsigned int i = 0; i < triangles.size(); ++i){
    int a = triangles[i].a, b = triangles[i].b, c = triangles[i].c,
      at = triangles[i].at,bt = triangles[i].bt,ct = triangles[i].ct,
      ab = odd_m[MP(a,b)] + offset, 
      ac = odd_m[MP(a,c)] + offset,
      bc = odd_m[MP(b,c)] + offset;
    temp_tri.push_back(Triangle(a,ab,ac,at,ab,ac));
    temp_tri.push_back(Triangle(ab,b,bc,ab,bt,bc));
    temp_tri.push_back(Triangle(ac,bc,c,ac,bc,ct));
    temp_tri.push_back(Triangle(ac,ab,bc,ac,ab,bc));
  }
  triangles = temp_tri;

  buildPreprocess();
  build();
}

Vertex3f HEdgeMesh::updateOddTexture(int a,int b,
                                     int at,int bt,int tri_index){
  Vertex3f v(0,0,0);
  float r1 = 0.5, r2 = 0.125;
  int ano_tri_index = getAnotherTriangleIndex(a,b,tri_index);
  if (ano_tri_index != NOTEXIST){
    int up1, up2;
    if (triangles[tri_index].at!=at && triangles[tri_index].at!=bt)
      up1 = triangles[tri_index].at;
    else if (triangles[tri_index].bt!=at&&triangles[tri_index].bt!=bt)
      up1 = triangles[tri_index].bt;
    else up1 = triangles[tri_index].ct;
    if (triangles[ano_tri_index].at!=at&&
        triangles[ano_tri_index].at!=bt) 
      up2 = triangles[ano_tri_index].at;
    else if (triangles[ano_tri_index].bt!=at&&
             triangles[ano_tri_index].bt!=bt)
      up2 = triangles[ano_tri_index].bt;
    else up2  = triangles[ano_tri_index].ct;
    r1 = 0.375;
    v.x += texture3f[up1].x * r2;
    v.y += texture3f[up1].y * r2;
    v.z += texture3f[up1].z * r2;
    v.x += texture3f[up2].x * r2;
    v.y += texture3f[up2].y * r2;
    v.z += texture3f[up2].z * r2;
  }
  v.x += texture3f[at].x * r1;
  v.y += texture3f[at].y * r1;
  v.z += texture3f[at].z * r1;
  v.x += texture3f[at].x * r1;
  v.y += texture3f[at].y * r1;
  v.z += texture3f[at].z * r1;
  return v;
}

Vertex4f HEdgeMesh::updateOdd(int start,int end,int tri_index){
  Vertex4f v(0,0,0); v.w = 0;
  float r1 = 0.5, r2 = 0.125;
  int ano_tri_index = getAnotherTriangleIndex(start,end,tri_index);
  if (ano_tri_index != NOTEXIST){
    int up1 = getExculdeTriIndex(start,end,tri_index),
      up2 = getExculdeTriIndex(start,end,ano_tri_index);
    r1 = 0.375;
    v.x += vertices4f[up1].x * r2;
    v.y += vertices4f[up1].y * r2;
    v.z += vertices4f[up1].z * r2;
    v.w += vertices4f[up1].w * r2;
    v.x += vertices4f[up2].x * r2;
    v.y += vertices4f[up2].y * r2;
    v.z += vertices4f[up2].z * r2;
    v.w += vertices4f[up2].w * r2;
  }
  v.x += vertices4f[start].x * r1;
  v.y += vertices4f[start].y * r1;
  v.z += vertices4f[start].z * r1;
  v.w += vertices4f[start].w * r1;
  v.x += vertices4f[end].x * r1;
  v.y += vertices4f[end].y * r1;
  v.z += vertices4f[end].z * r1;
  v.w += vertices4f[end].w * r1;
  return v;
}

Vertex3f HEdgeMesh::updateEvenTexture(int index,vector<int> *t_edge){
  float PI = acos(-1);
  int n = t_edge[index].size();
  float beta = (0.625-pow((0.375+0.25*cos(2*PI/n)),2))/n;
  if (n == 2) beta = 0.125;
  Vertex3f v3f(0,0,0);
  for (unsigned int j = 0; j < t_edge[index].size(); ++j){
    Vertex3f temp = texture3f[t_edge[index][j]];
    v3f.x += temp.x * beta;
    v3f.y += temp.y * beta;
    v3f.z += temp.z * beta;
  }
  v3f.x += (1-n*beta)*texture3f[index].x;
  v3f.y += (1-n*beta)*texture3f[index].y;
  v3f.z += (1-n*beta)*texture3f[index].z;
  return v3f;  
}

Vertex4f HEdgeMesh::updateEven(int index,vector<int>* edge){
  float PI = acos(-1);
  int n = edge[index].size();
  float beta = (0.625-pow((0.375+0.25*cos(2*PI/n)),2))/n;
  if (n == 2) beta = 0.125;
  Vertex4f v4f(0,0,0);
  v4f.w = 0;
  for (unsigned int j = 0; j < edge[index].size(); ++j){
    Vertex4f temp = vertices4f[edge[index][j]];
    v4f.x += temp.x * beta;
    v4f.y += temp.y * beta;
    v4f.z += temp.z * beta;
    v4f.w += temp.w * beta;
  }
  v4f.x += (1-n*beta)*vertices4f[index].x;
  v4f.y += (1-n*beta)*vertices4f[index].y;
  v4f.z += (1-n*beta)*vertices4f[index].z;
  v4f.w += (1-n*beta)*vertices4f[index].w;
  return v4f;  
}

int HEdgeMesh::getExculdeTriIndex(int a,int b,int tri_index){
  if (triangles[tri_index].a != a && triangles[tri_index].a != b)
    return triangles[tri_index].a;
  if (triangles[tri_index].b != a && triangles[tri_index].b != b)
    return triangles[tri_index].b;
  return triangles[tri_index].c;
}

/*
int HEdgeMesh::getEdgeToOddIndex(int start,int end){
  if (start > end){ int temp = start; start = end; end = temp; }
  if (m_edge_to_odd_index.find(MP(start,end)) ==
      m_edge_to_odd_index.end())
    return NOTEXIST;
  return m_edge_to_odd_index[MP(start,end)];
}

void HEdgeMesh::setEdgeToOddIndex(int start,int end,int index){
  if (start > end) {int temp = start; start = end; end = temp;}
  m_edge_to_odd_index[MP(start,end)] = index;
}

void HEdgeMesh::loopDivision(int scale_number){
  for (int i = 0; i < scale_number; i ++){
    addOdd();
    printf("add odd success\n");
    updateEven();
    printf("update even success\n");
    updateOdd();
    printf("update odd success\n");
    mergeOddAndEven();
    printf("merge success\n");
  }
}

void HEdgeMesh::addOdd(){
  Vertex4f v4fa(0,0,0), v4fb(0,0,0), v4fc(0,0,0), v4f(0,0,0);
  int a, b, c, index;
  odd_vertices4f.clear();
  m_edge_to_odd_index.clear();
  for (unsigned int i = 0; i < triangles.size(); i ++){
    a = triangles[i].a;
    b = triangles[i].b;
    c = triangles[i].c;
    v4fa = vertices4f[a];
    v4fb = vertices4f[b];
    v4fc = vertices4f[c];
    
    if (getEdgeToOddIndex(a,b) == NOTEXIST){
      index = odd_vertices4f.size();
      v4f = Vertex4f(0.5*v4fa.x+0.5*v4fb.x, 0.5*v4fa.y+0.5*v4fb.y,
                   0.5*v4fa.z+0.5*v4fb.z, 0.5*v4fa.w+0.5*v4fb.w);
      odd_vertices4f.push_back(v4f);
      setEdgeToOddIndex(a,b,index);
    }
    if (getEdgeToOddIndex(b,c) == NOTEXIST){
      index = odd_vertices4f.size();
      v4f = Vertex4f(0.5*v4fb.x+0.5*v4fc.x, 0.5*v4fb.y+0.5*v4fc.y,
                   0.5*v4fb.z+0.5*v4fc.z, 0.5*v4fb.w+0.5*v4fc.w);
      odd_vertices4f.push_back(v4f);
      setEdgeToOddIndex(b,c,index);
    }
    if (getEdgeToOddIndex(a,c) == NOTEXIST){
      index = odd_vertices4f.size();
      v4f = Vertex4f(0.5*v4fa.x+0.5*v4fc.x, 0.5*v4fa.y+0.5*v4fc.y,
                   0.5*v4fa.z+0.5*v4fc.z, 0.5*v4fa.w+0.5*v4fc.w);
      odd_vertices4f.push_back(v4f);
      setEdgeToOddIndex(a,c,index);
    }
  }

}

Vertex4f HEdgeMesh::__updateEven(int index, vector<int>* edge){
  float PI = acos(-1);
  int n = edge[index].size();
  float beta = (0.625-pow((0.375+0.25*cos(2*PI/n)),2))/n;
  if (n == 2) beta = 0.125;
  Vertex4f v4f(0,0,0);
  v4f.w = 0;
  for (unsigned int j = 0; j < edge[index].size(); ++j){
    Vertex4f temp = odd_vertices4f[getEdgeToOddIndex(index,j)];
    v4f.x += temp.x * beta;
    v4f.y += temp.y * beta;
    v4f.z += temp.z * beta;
    v4f.w += temp.w * beta;
  }
  v4f.x += (1-n*beta)*vertices4f[index].x;
  v4f.y += (1-n*beta)*vertices4f[index].y;
  v4f.z += (1-n*beta)*vertices4f[index].z;
  v4f.w += (1-n*beta)*vertices4f[index].w;
  return v4f;
}

void HEdgeMesh::updateEven(){
  vector<int> *edge = NULL;
  set< pair<int,int> > s_edge; s_edge.clear();
  if (vertices4f.size() != 0) 
    edge = new vector<int>[vertices4f.size()];
  for (unsigned int i = 0; i < triangles.size(); ++i){
    int a = triangles[i].a, b = triangles[i].b, c = triangles[i].c;
    if (s_edge.find(MP(a,b)) == s_edge.end()){
      edge[a].push_back(b);
      edge[b].push_back(a);
      s_edge.insert(MP(a,b));
      s_edge.insert(MP(b,a));
    }
    if (s_edge.find(MP(b,c)) == s_edge.end()){
      edge[b].push_back(c);
      edge[c].push_back(b);
      s_edge.insert(MP(b,c));
      s_edge.insert(MP(c,b));
    }
    if (s_edge.find(MP(a,c)) == s_edge.end()){
      edge[a].push_back(c);
      edge[c].push_back(a);
      s_edge.insert(MP(a,c));
      s_edge.insert(MP(c,a));
    }
  }

  
  bool *v_used = NULL;
  if (vertices4f.size() != 0) v_used = new bool[vertices4f.size()];
  for (unsigned int i = 0; i < vertices4f.size(); ++i){
    v_used[i] = false;
  }

  even_vertices4f = vertices4f;
  for (unsigned int i = 0; i < triangles.size(); ++i){
    int a = triangles[i].a, b = triangles[i].b, c = triangles[i].c;
    if (v_used[a] == false){
      v_used[a] = true;
      even_vertices4f[a] = __updateEven(a,edge);
    }
    if (v_used[b] == false){
      v_used[b] = true;
      even_vertices4f[b] = __updateEven(b,edge);
    }
    if (v_used[c] == false){
      v_used[c] = true;
      even_vertices4f[c] = __updateEven(c,edge);
    }
  }

  if (edge != NULL) delete[] edge;
  if (v_used != NULL) delete[] v_used;
}


Vertex4f HEdgeMesh::__updateOdd(int start,int end,int tri_index){
  int odd_index = getEdgeToOddIndex(start,end);
  assert(odd_index != NOTEXIST);
  int ano_tri_index = getAnotherTriangleIndex(start,end,tri_index);

  Vertex4f v(0,0,0);
  v.w = 0;
  float r1 = 0.5, r2 = 0.5;
  if (ano_tri_index != NOTEXIST){
    r1 = r2 = 0.375;
    int u1 = getExculdeTriIndex(start,end,tri_index), 
      u2 = getExculdeTriIndex(start,end,ano_tri_index);
    v.x += even_vertices4f[u1].x * 0.125;
    v.y += even_vertices4f[u1].y * 0.125;
    v.z += even_vertices4f[u1].z * 0.125;
    v.w += even_vertices4f[u1].w * 0.125;
    v.x += even_vertices4f[u2].x * 0.125;
    v.y += even_vertices4f[u2].y * 0.125;
    v.z += even_vertices4f[u2].z * 0.125;
    v.w += even_vertices4f[u2].w * 0.125;
  }
  v.x += even_vertices4f[start].x * r1;
  v.y += even_vertices4f[start].y * r1;
  v.z += even_vertices4f[start].z * r1;
  v.w += even_vertices4f[start].w * r1;
  v.x += even_vertices4f[end].x * r2;
  v.y += even_vertices4f[end].y * r2;
  v.z += even_vertices4f[end].z * r2;
  v.w += even_vertices4f[end].w * r2;
  return v;
}

void HEdgeMesh::updateOdd(){
  bool* odd_used = NULL;
  if (odd_vertices4f.size() != 0) 
    odd_used = new bool[odd_vertices4f.size()];
  for (unsigned int i = 0; i < odd_vertices4f.size(); ++i)
    odd_used[i] = false;

  int index;
  for (unsigned int i = 0; i < triangles.size(); ++i){
    int a = triangles[i].a, b = triangles[i].b, c = triangles[i].c;
    index = getEdgeToOddIndex(a,b);
    if (odd_used[index] == false){
      odd_used[index] = true;
      odd_vertices4f[index] = __updateOdd(a,b,i);
    }
    index = getEdgeToOddIndex(b,c);
    if (odd_used[index] == false){
      odd_used[index] = true;
      odd_vertices4f[index] = __updateOdd(b,c,i);
    }
    index = getEdgeToOddIndex(a,c);
    if (odd_used[index] == false){
      odd_used[index] = true;
      odd_vertices4f[index] = __updateOdd(a,c,i);
    }
  }

  if (odd_used != NULL) delete[] odd_used;
}

void HEdgeMesh::mergeOddAndEven(){
  vector<Triangle> temp_tris; temp_tris.clear();
  int offset = even_vertices4f.size();
  for (unsigned int i = 0; i < triangles.size(); i ++){
    int a = triangles[i].a, b = triangles[i].b, c = triangles[i].c;
    int ab = getEdgeToOddIndex(a,b) + offset,
      ac = getEdgeToOddIndex(a,c) + offset,
      bc = getEdgeToOddIndex(b,c) + offset;
    temp_tris.push_back(Triangle(a,ab,ac));
    temp_tris.push_back(Triangle(ab,b,bc));
    temp_tris.push_back(Triangle(ac,bc,c));
    temp_tris.push_back(Triangle(ab,bc,ac));
  }
  triangles.clear();
  for (unsigned int i = 0; i < temp_tris.size(); ++i)
    triangles.push_back(temp_tris[i]);

  vertices4f.clear();
  normals3f.clear();
  for (unsigned int i = 0; i < even_vertices4f.size(); ++i)
    vertices4f.push_back(even_vertices4f[i]);
  for (unsigned int i = 0; i < odd_vertices4f.size(); ++i)
    vertices4f.push_back(odd_vertices4f[i]);
  
  buildPreprocess();
  build();
  odd_vertices4f.clear();
  even_vertices4f.clear();
  m_edge_to_odd_index.clear();
}
*/

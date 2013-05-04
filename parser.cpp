#include "parser.h"

#include <stdio.h>
#include <string.h>
#include <fstream>
#include <vector>
#include <iostream>
#include <GL/glut.h>

using namespace std;

static float strToFloat(char str[]){
  float result = 0;
  bool isfact = false;
  bool negative = false;
  float factor = 10;

  int length = strlen(str);
  for (int i = 0; i < length; i ++){
    if (str[i] == '.') isfact = true;
    else if (i == 0 && str[i] == '-') negative = true;
    else if (!isfact){
      result *= 10;
      result += int(str[i] - '0');
    }
    else{
      result += int(str[i] - '0')/factor;
      factor *= 10;
    }
  }

  if (negative) result *= -1;
  return result;
}

static int strToUnsignedInt(char *str){
  int result = 0;
  int length = strlen(str);
  for (int i = 0; i < length; i ++){
    result *= 10;
    result += int(str[i] - '0');
  }
  return result;
}

Parser::Parser(const char* file_name){
  object.vertices4f.clear();
  object.text_coords3f.clear();
  object.normals3f.clear();
  object.faces.clear();

  ifstream input(file_name);
  if(!input){
    success = false;
    return;
  }

  char str[1024];
  string temp_str;
  int smooth_index = 0;

  //if a line characters exceeded 1023 characters,
  //remain to do
  while (getline(input,temp_str)){
    strcpy(str,temp_str.c_str());

    int length = strlen(str);
    if (length == 0) {continue;}
    else if (str[0] == '#') {continue;}
    else if (str[0] == 'v' && length > 1 && str[1] == ' '){
      char *pch = strtok(str," ");
      pch = strtok(NULL," ");
      int count = 0;
      float* temp = new float[4];
      temp[0] = temp[1] = temp[2] = 0.0;
      temp[3] = 1.0;

      while (pch){
        temp[count++] = strToFloat(pch);
        if (count == 3) break;
        pch = strtok(NULL," ");
      }

      object.vertices4f.push_back(temp);
    }
    else if (str[0] == 'v' && length > 1 && str[1] == 't'){
      char *pch = strtok(str," ");
      pch = strtok(NULL," ");
      int count = 0;
      float* temp = new float[3];
      temp[0] = temp[1] = temp[2] = 0;

      while (pch){
        temp[count++] = strToFloat(pch);
        if (count == 2) break;
        pch = strtok(NULL," ");
      }

      object.text_coords3f.push_back(temp);
    }
    else if (str[0] == 'v' && length > 1 && str[1] == 'n'){
      char *pch = strtok(str," ");
      pch = strtok(NULL," ");
      int count = 0;
      float* temp = new float[3];
      temp[0] = temp[1] = temp[2] = 0;

      while (pch){
        temp[count++] = strToFloat(pch);
        if (count == 3) break;
        pch = strtok(NULL," ");
      }
      
      object.normals3f.push_back(temp);
    }
    else if (str[0] == 'f'){
      char *pch = strtok(str," ");
      pch = strtok(NULL," ");

      vector<string> record;
      while (pch){
        record.push_back(string(pch));
        pch = strtok(NULL," ");
      }

      faceInfo face;
      face.vertices_index.clear();
      face.text_coords_index.clear();
      face.normals_index.clear();
      face.smooth_index = smooth_index;

      for (unsigned int i = 0; i < record.size(); i ++){
        int count = 0;

        for (unsigned int j = 0; j < record[i].size(); j ++)
          if (record[i][j] == '/') count++;


        char temp[1024]; 
        strcpy(temp,record[i].c_str());
        pch = strtok(temp,"/");
        int num = strToUnsignedInt(pch);
        face.vertices_index.push_back(num);
        if (count == 1){
          pch = strtok(NULL,"/");
          num = strToUnsignedInt(pch);
          face.text_coords_index.push_back(num);
        }
        else if (count == 2){
          pch = strtok(NULL,"/");
          int num1 = strToUnsignedInt(pch);
          pch = strtok(NULL,"/");
          if (pch){
            int num2 = strToUnsignedInt(pch);
            face.text_coords_index.push_back(num1);
            face.normals_index.push_back(num2);
          }
          else face.normals_index.push_back(num1);
        }
        
      }

      object.faces.push_back(face);
    }
    else if (str[0] == 'o'){
      //something to be done
    }
    else if (str[0] == 's'){
      char *pch = strtok(str," ");
      pch = strtok(NULL," ");
      if (strcmp(pch,"off") == 0) smooth_index = 0;
      else smooth_index = strToUnsignedInt(pch);
    }
    else continue;
       
  }

  input.close();
  success = true;
  resizeVector();
}

void Parser::drawInGL(DRAW_TYPE t,bool is_light,bool show_normals){
  glPushMatrix();
  glScalef(vertice_scale_x,vertice_scale_y,vertice_scale_z);
  glTranslatef(vertice_transform_x,vertice_transform_y,
               vertice_transform_z);

  for (vector<faceInfo>::iterator face = object.faces.begin();
       face != object.faces.end(); face ++){

    
    if (t == DRAW_POLY){
      glDisable(GL_LIGHTING);
      glDisable(GL_LIGHT0);
      glColor3f(0.0,0.0,0.0);
      glBegin(GL_LINE_LOOP);
    
      for (vector<int>::iterator 
             index = face->vertices_index.begin();
           index != face->vertices_index.end(); index ++){
        float* vertice4f = object.vertices4f[*index - 1];
        glVertex4f(vertice4f[0],vertice4f[1],
                   vertice4f[2],vertice4f[3]);
      }
      glEnd();      
      glEnable(GL_LIGHTING);
      glEnable(GL_LIGHT0);
    }

    if (t == DRAW_POINT || t == DRAW_LINE || is_light == false){
      glColor3f(0.0,0.0,0.0);
      glDisable(GL_LIGHTING);
      glDisable(GL_LIGHT0);
    }

    if (t == DRAW_POINT) glBegin(GL_POINTS);
    else if (t == DRAW_LINE) glBegin(GL_LINE_LOOP);
    else{
      if (!is_light) glColor3f(0.5,0.5,0.5);
      glBegin(GL_POLYGON);
    }

    unsigned int count = 0;
    for (vector<int>::iterator 
           index = face->vertices_index.begin();
         index != face->vertices_index.end(); index ++){
      float* vertice4f = object.vertices4f[*index - 1];
      //set normal vector
      if (is_light && count < face->normals_index.size()){
        float* normal3f = object.normals3f[face->normals_index[count] 
                                        - 1];


        glNormal3f(normal3f[0],normal3f[1],normal3f[2]);
      }
      count ++;
      //set vertices

      glVertex4f(vertice4f[0],vertice4f[1],
                 vertice4f[2],vertice4f[3]);
    }
    glEnd();
     

    //show normal vector
    if (show_normals){
      glDisable(GL_LIGHTING);
      glDisable(GL_LIGHT0);
      for (unsigned int i = 0; i < face->vertices_index.size(); i ++){
        float* vertice4f = object.vertices4f[face->vertices_index[i]-1];
        if (i < face->normals_index.size()){
          float* normal3f = object.normals3f[face->normals_index[i]-1];
          glColor3f(0.0,0.717647,0.921568);//cyan
          glBegin(GL_LINES);
          glVertex4f(vertice4f[0],vertice4f[1],
                     vertice4f[2],vertice4f[3]);
          glVertex4f(vertice4f[0]+normal3f[0]*0.1,//make it beautiful
                     vertice4f[1]+normal3f[1]*0.1,
                     vertice4f[2]+normal3f[2]*0.1,
                     vertice4f[3]);
          glEnd();
        }
      }
      glEnable(GL_LIGHTING);
      glEnable(GL_LIGHT0);
    }
    //end show normal vector

  }//end for faces

  glPopMatrix();
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  //remian to set texture vectors
}


Parser::~Parser(){
  for (unsigned int i = 0; i < object.vertices4f.size(); i ++)
    delete object.vertices4f[i];
  for (unsigned int i = 0; i < object.text_coords3f.size(); i ++)
    delete object.text_coords3f[i];
  for (unsigned int i = 0; i < object.normals3f.size(); i ++)
    delete object.normals3f[i];

  object.vertices4f.clear();
  object.text_coords3f.clear();
  object.normals3f.clear();

}

void Parser::printInfo(){

  objInfo* it = &object;
  
  cout << it->name << endl;
  cout << "Vertices:" << endl;
  for (unsigned int i = 0; i < it->vertices4f.size();i ++)
    cout << "v:" <<  i << " " << it->vertices4f[i][0] << " "
         << it->vertices4f[i][1] << " " << it->vertices4f[i][2]
         << " " << it->vertices4f[i][3] << endl;

  cout << "Textures:" << endl;
  for (unsigned int i = 0; i < it->text_coords3f.size(); i ++)
    cout << "vt:" << i << " " << it->text_coords3f[i][0] << " "
         << it->text_coords3f[i][1] << " " 
         << it->text_coords3f[i][2] << endl;

  cout << "Normals:" << endl;
  for (unsigned int i = 0; i < it->normals3f.size(); i ++)
    cout << "vn:" << i << " " << it->normals3f[i][0] << " "
         << it->normals3f[i][1] << " " << it->normals3f[i][2]
         << endl;

  cout << "Faces:" << endl;
  for (unsigned int i = 0; i < it->faces.size(); i ++){
    cout << "f:" << i << " ";
    for (unsigned int j = 0; j < it->faces[i].vertices_index.size(); j ++){
      cout << it->faces[i].vertices_index[j] << "/";
      if (it->faces[i].text_coords_index.size() > j)
        cout << it->faces[i].text_coords_index[j];
      cout << "/";
      if (it->faces[i].normals_index.size() > j)
        cout << it->faces[i].normals_index[j];
      cout << " ";
    }
    cout << endl;
  }
    
}

bool Parser::getSuccess(){
  return success;
}


static float max(float a,float b){
  return a > b ? a : b;
}

static float min(float a,float b){
  return a > b ? b : a;
}


void Parser::resizeVector(){
  vertice_scale_x = vertice_scale_y = vertice_scale_z = 1.0;
  float x_max = -1e7, y_max = -1e7, z_max = -1e7,
    x_min = 1e7, y_min = 1e7, z_min = 1e7;

  for (unsigned int i = 0; i < object.vertices4f.size(); i ++){
    float normal = object.vertices4f[i][3];
    x_max = max(x_max,object.vertices4f[i][0]) / normal;
    x_min = min(x_min,object.vertices4f[i][0]) / normal;
    y_max = max(y_max,object.vertices4f[i][1]) / normal;
    y_min = min(y_min,object.vertices4f[i][1]) / normal;
    z_max = max(z_max,object.vertices4f[i][2]) / normal;
    z_min = min(z_min,object.vertices4f[i][2]) / normal;
  }


  vertice_scale_x /= (x_max - x_min);
  vertice_scale_y /= (y_max - y_min);
  vertice_scale_z /= (z_max - z_min);
  vertice_scale_x = vertice_scale_y = vertice_scale_z = 
    min(vertice_scale_x,min(vertice_scale_y,vertice_scale_z)) * 3;
  vertice_transform_x = (x_max + x_min) / 2 * -1;
  vertice_transform_y = (y_max + y_min) / 2 * -1;
  vertice_transform_z = (z_max + z_min) / 2 * -1;
}

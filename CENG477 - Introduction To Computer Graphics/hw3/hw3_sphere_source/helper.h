#ifndef __HELPER__H__
#define __HELPER__H__

#include <iostream>
#include <string>
#include <fstream>
#include <jpeglib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

extern GLuint idProgramShader;
extern GLuint idFragmentShader;
extern GLuint idVertexShader;
extern GLuint idJpegTexture;
extern GLuint idJpegHeightMap;

using namespace std;

void createShaders(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

bool readDataFromFile(const string& fileName, string &data);

void initTexture(char *filename,int *w, int *h, int mode);

#endif

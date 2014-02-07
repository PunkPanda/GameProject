#pragma once
#ifndef GL_H
	#define GL_H
	#include <gl\gl.h>                                // Header File For The OpenGL32 Library
	#include <gl\glu.h>                               // Header File For The GLu32 Library
	#include <gl\freeglut.h>
	#include "BMP.h"								  // Header File For The BMP Library
	#include "SOIL.h"
#endif

BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag);
GLvoid ReSizeGLScene(GLsizei width, GLsizei height);
int LoadGLTextures(void);
int InitGL(GLvoid);
int DrawGLScene(GLvoid);
GLvoid KillGLWindow(GLvoid);
bool GraphicsInit(void);
void GraphicsUpdate(void);
GLuint LoadTexture(const char* tex);

extern bool    fullscreen;                            // Fullscreen Flag Set To Fullscreen Mode By Default

extern GLfloat     xrot;                               // X Rotation ( NEW )
extern GLfloat     yrot;                               // Y Rotation ( NEW )
extern GLfloat     zrot;                               // Z Rotation ( NEW )

extern GLfloat		x;
extern GLfloat		y;
extern GLfloat		z;
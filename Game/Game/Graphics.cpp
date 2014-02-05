#include "WndProc.h"
#include <stdio.h>

#include "Graphics.h"

#include "Player.h"
#include "Region.h"
#include "Collision.h"

HGLRC           hRC=NULL;                           // Permanent Rendering Context
HDC             hDC=NULL;                           // Private GDI Device Context
HWND            hWnd=NULL;                          // Holds Our Window Handle
HINSTANCE       hInstance;                          // Holds The Instance Of The Application

GLuint      texture[3];                         // Storage For One Texture ( NEW )

bool		fullscreen;

GLfloat     xrot;                               // X Rotation ( NEW )
GLfloat     yrot;                               // Y Rotation ( NEW )
GLfloat     zrot;                               // Z Rotation ( NEW )

GLfloat		x;
GLfloat		y;
GLfloat		z;

bool door1;
bool door2;
bool door3;
bool door4;

BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint      PixelFormat;                        // Holds The Results After Searching For A Match

	WNDCLASS    wc;									// Windows Class Structure

	DWORD       dwExStyle;							// Window Extended Style
	DWORD       dwStyle;							// Window Style

	RECT WindowRect;								// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;                        // Set Left Value To 0
	WindowRect.right=(long)width;                   // Set Right Value To Requested Width
	WindowRect.top=(long)0;                         // Set Top Value To 0
	WindowRect.bottom=(long)height;                 // Set Bottom Value To Requested Height

	fullscreen=fullscreenflag;                      // Set The Global Fullscreen Flag

	hInstance       = GetModuleHandle(NULL);					// Grab An Instance For Our Window
	wc.style        = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;       // Redraw On Move, And Own DC For Window
	wc.lpfnWndProc      = (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra       = 0;									// No Extra Window Data
	wc.cbWndExtra       = 0;									// No Extra Window Data
	wc.hInstance        = hInstance;							// Set The Instance
	wc.hIcon        = LoadIcon(NULL, IDI_WINLOGO);				// Load The Default Icon
	wc.hCursor      = LoadCursor(NULL, IDC_ARROW);				// Load The Arrow Pointer
	wc.hbrBackground    = NULL;									// No Background Required For GL
	wc.lpszMenuName     = NULL;									// We Don't Want A Menu
	wc.lpszClassName    = "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))                        // Attempt To Register The Window Class
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;                           // Exit And Return FALSE
	}

	if (fullscreen)                             // Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;                   // Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));       // Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);			// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth    = width;					// Selected Screen Width
		dmScreenSettings.dmPelsHeight   = height;					// Selected Screen Height
		dmScreenSettings.dmBitsPerPel   = bits;						// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Run In A Window.
			if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				fullscreen=FALSE;               // Select Windowed Mode (Fullscreen=FALSE)
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
				return FALSE;                   // Exit And Return FALSE
			}
		}
	}

	if (fullscreen)                             // Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;              // Window Extended Style
		dwStyle=WS_POPUP;                       // Windows Style
		ShowCursor(FALSE);                      // Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;           // Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);     // Adjust Window To True Requested Size

	if (!(hWnd=CreateWindowEx(  dwExStyle,              // Extended Style For The Window
								"OpenGL",               // Class Name
								title,                  // Window Title
								WS_CLIPSIBLINGS |           // Required Window Style
								WS_CLIPCHILDREN |           // Required Window Style
								dwStyle,                // Selected Window Style
								0, 0,					// Window Position
								WindowRect.right-WindowRect.left,   // Calculate Adjusted Window Width
								WindowRect.bottom-WindowRect.top,   // Calculate Adjusted Window Height
								NULL,                   // No Parent Window
								NULL,                   // No Menu
								hInstance,              // Instance
								NULL)))                 // Don't Pass Anything To WM_CREATE
	{
		KillGLWindow();                         // Reset The Display
		MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;                           // Return FALSE
	}

	static  PIXELFORMATDESCRIPTOR pfd=                  // pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),                  // Size Of This Pixel Format Descriptor
		1,                              // Version Number
		PFD_DRAW_TO_WINDOW |                        // Format Must Support Window
		PFD_SUPPORT_OPENGL |                        // Format Must Support OpenGL
		PFD_DOUBLEBUFFER,                       // Must Support Double Buffering
		PFD_TYPE_RGBA,                          // Request An RGBA Format
		bits,                               // Select Our Color Depth
		0, 0, 0, 0, 0, 0,                       // Color Bits Ignored
		0,                              // No Alpha Buffer
		0,                              // Shift Bit Ignored
		0,                              // No Accumulation Buffer
		0, 0, 0, 0,                         // Accumulation Bits Ignored
		16,                             // 16Bit Z-Buffer (Depth Buffer)
		0,                              // No Stencil Buffer
		0,                              // No Auxiliary Buffer
		PFD_MAIN_PLANE,                         // Main Drawing Layer
		0,                              // Reserved
		0, 0, 0                             // Layer Masks Ignored
	};

	if (!(hDC=GetDC(hWnd)))                         // Did We Get A Device Context?
	{
		KillGLWindow();                         // Reset The Display
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;                           // Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))             // Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();                         // Reset The Display
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;                           // Return FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))               // Are We Able To Set The Pixel Format?
	{
		KillGLWindow();                         // Reset The Display
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;                           // Return FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))                   // Are We Able To Get A Rendering Context?
	{
		KillGLWindow();                         // Reset The Display
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;                           // Return FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))                        // Try To Activate The Rendering Context
	{
		KillGLWindow();                         // Reset The Display
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;                           // Return FALSE
	}

	ShowWindow(hWnd,SW_SHOW);                       // Show The Window
	SetForegroundWindow(hWnd);                      // Slightly Higher Priority
	SetFocus(hWnd);                             // Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);                       // Set Up Our Perspective GL Screen

	if (!InitGL())                              // Initialize Our Newly Created GL Window
	{
		KillGLWindow();                         // Reset The Display
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;                           // Return FALSE
	}

	return TRUE;                                // Success
}

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)					// Resize And Initialize The GL Window
{
    if (height==0)                              // Prevent A Divide By Zero By
    {
        height=1;								// Making Height Equal One
    }
 
    glViewport(0, 0, width, height);                    // Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);                        // Select The Projection Matrix
    glLoadIdentity();									// Reset The Projection Matrix
 
    // Calculate The Aspect Ratio Of The Window
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
 
    glMatrixMode(GL_MODELVIEW);                     // Select The Modelview Matrix
    glLoadIdentity();								// Reset The Modelview Matrix
}

int LoadGLTextures()                                    // Load Bitmaps And Convert To Textures
{
    /* load an image file directly as a new OpenGL texture */
    texture[0] = SOIL_load_OGL_texture
        (
        tileImage.c_str(),
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
        );
     texture[1] = SOIL_load_OGL_texture
        (
        backgroundImage.c_str(),
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
        );
	 texture[2] = SOIL_load_OGL_texture
        (
        doorImage.c_str(),
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
        );
    if(texture[0] == 0 || texture[1] == 0 || texture[2] == 0)
        return false;
 
 
    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
 
    return true;                                        // Return Success
}

int InitGL(GLvoid)                              // All Setup For OpenGL Goes Here
{
    /*if (!LoadGLTextures())                          // Jump To Texture Loading Routine ( NEW )
    {
        return FALSE;                           // If Texture Didn't Load Return FALSE ( NEW )
    }*/
 
    glEnable(GL_TEXTURE_2D);                        // Enable Texture Mapping ( NEW )
    glShadeModel(GL_SMOOTH);                        // Enable Smooth Shading
    glClearColor(0.12f, 0.4f, 0.6f, 0.5f);                   // Black Background
    glClearDepth(1.0f);                         // Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);                        // Enables Depth Testing
    glDepthFunc(GL_LEQUAL);                         // The Type Of Depth Testing To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);          // Really Nice Perspective Calculations
    return TRUE;                                // Initialization Went OK
}


int DrawGLScene(GLvoid)                             // Here's Where We Do All The Drawing
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);         // Clear Screen And Depth Buffer
    glLoadIdentity();                           // Reset The Current Matrix
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
	x = -regionWidth/2;
	y = -regionHeight/2;
    glTranslatef(-P1->pos.x, -P1->pos.y, z-25.0f);                      // Move Into The Screen 5 Units

	glRotatef(xrot,1.0f,0.0f,0.0f);                     // Rotate On The X Axis
	glRotatef(yrot,0.0f,1.0f,0.0f);                     // Rotate On The Y Axis
	glRotatef(zrot,0.0f,0.0f,1.0f);                     // Rotate On The Z Axis

	//draw the background
	glBindTexture(GL_TEXTURE_2D, texture[1]);               // Select Our Texture
	glBegin(GL_QUADS);
		glColor3f(1.0f, 1.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(0, 0, -0.002f);  // Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(100, 0, -0.002f);  // Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(100, 30, -0.002f);  // Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(0, 30, -0.002f);  // Top Left Of The Texture and Quad
	glEnd();

	//draw each tile
	for(short i = 0; i < regionWidth; ++i)
	{
		if(abs(i - P1->pos.x) > 18)
			continue;
		for(short j = 0; j < regionHeight; ++j)
		{
			if(abs(j - P1->pos.y) > 18)
				continue;
			switch(currentRegion[i][j])
			{
				case ' ':
					break;
				case '-':
					glBindTexture(GL_TEXTURE_2D, texture[0]);               // Select Our Texture
					glBegin(GL_QUADS);
						glColor3f(1.0f, 1.0f, 1.0f);
						glTexCoord2f(0.0f, 0.0f); glVertex2f(i, j);  // Bottom Left Of The Texture and Quad
						glTexCoord2f(1.0f, 0.0f); glVertex2f(i + 1.0f, j);  // Bottom Right Of The Texture and Quad
						glTexCoord2f(1.0f, 1.0f); glVertex2f(i + 1.0f, j + 1.0f);  // Top Right Of The Texture and Quad
						glTexCoord2f(0.0f, 1.0f); glVertex2f(i, j + 1.0f);  // Top Left Of The Texture and Quad
					glEnd();
					break;
				case '<':
					glBindTexture(GL_TEXTURE_2D, texture[0]);               // Select Our Texture
					glBegin(GL_TRIANGLES);
						glColor3f(1.0f, 1.0f, 1.0f);
						glTexCoord2f(0.0f, 0.0f); glVertex2f(i, j);  // Bottom Left Of The Texture and Quad
						glTexCoord2f(1.0f, 0.0f); glVertex2f(i + 1.0f, j);  // Bottom Right Of The Texture and Quad
						glTexCoord2f(1.0f, 1.0f); glVertex2f(i + 1.0f, j + 1.0f);  // Top Right Of The Texture and Quad
					glEnd();
					break;
				case '>':
					glBindTexture(GL_TEXTURE_2D, texture[0]);               // Select Our Texture
					glBegin(GL_TRIANGLES);
						glColor3f(1.0f, 1.0f, 1.0f);
						glTexCoord2f(0.0f, 0.0f); glVertex2f(i, j);  // Bottom Left Of The Texture and Quad
						glTexCoord2f(1.0f, 0.0f); glVertex2f(i + 1.0f, j);  // Bottom Right Of The Texture and Quad
						glTexCoord2f(0.0f, 1.0f); glVertex2f(i + 0.0f, j + 1.0f);  // Top Right Of The Texture and Quad
					glEnd();
					break;
				case '1': 
					if(!door1)
					{
						glBindTexture(GL_TEXTURE_2D, texture[2]); 
						glBegin(GL_QUADS);
							glColor3f(1.0f, 1.0f, 1.0f);
							glTexCoord2f(0.0f, 0.0f); glVertex3f(i, j, -0.001);  // Bottom Left Of The Texture and Quad
							glTexCoord2f(1.0f, 0.0f); glVertex3f(i + 3.0f, j, -0.001);  // Bottom Right Of The Texture and Quad
							glTexCoord2f(1.0f, 1.0f); glVertex3f(i + 3.0f, j + 4.0f, -0.001);  // Top Right Of The Texture and Quad
							glTexCoord2f(0.0f, 1.0f); glVertex3f(i, j + 4.0f, -0.001);  // Top Left Of The Texture and Quad
						glEnd();
						door1 = true;
					}
					break;
				case '2':
					if(!door2)
					{
						glBindTexture(GL_TEXTURE_2D, texture[2]); 
						glBegin(GL_QUADS);
							glColor3f(1.0f, 1.0f, 1.0f);
							glTexCoord2f(0.0f, 0.0f); glVertex3f(i, j, -0.001);  // Bottom Left Of The Texture and Quad
							glTexCoord2f(1.0f, 0.0f); glVertex3f(i + 3.0f, j, -0.001);  // Bottom Right Of The Texture and Quad
							glTexCoord2f(1.0f, 1.0f); glVertex3f(i + 3.0f, j + 4.0f, -0.001);  // Top Right Of The Texture and Quad
							glTexCoord2f(0.0f, 1.0f); glVertex3f(i, j + 4.0f, -0.001);  // Top Left Of The Texture and Quad
						glEnd();
						door2 = true;
					}
					break;
				case '3': 
					if(!door3)
					{
						glBindTexture(GL_TEXTURE_2D, texture[2]); 
						glBegin(GL_QUADS);
							glColor3f(1.0f, 1.0f, 1.0f);
							glTexCoord2f(0.0f, 0.0f); glVertex3f(i, j, -0.001);  // Bottom Left Of The Texture and Quad
							glTexCoord2f(1.0f, 0.0f); glVertex3f(i + 3.0f, j, -0.001);  // Bottom Right Of The Texture and Quad
							glTexCoord2f(1.0f, 1.0f); glVertex3f(i + 3.0f, j + 4.0f, -0.001);  // Top Right Of The Texture and Quad
							glTexCoord2f(0.0f, 1.0f); glVertex3f(i, j + 4.0f, -0.001);  // Top Left Of The Texture and Quad
						glEnd();
						door3 = true;
					}
					break;
				case '4':
					if(!door4)
					{
						glBindTexture(GL_TEXTURE_2D, texture[2]); 
						glBegin(GL_QUADS);
							glColor3f(1.0f, 1.0f, 1.0f);
							glTexCoord2f(0.0f, 0.0f); glVertex3f(i, j, -0.001);  // Bottom Left Of The Texture and Quad
							glTexCoord2f(1.0f, 0.0f); glVertex3f(i + 3.0f, j, -0.001);  // Bottom Right Of The Texture and Quad
							glTexCoord2f(1.0f, 1.0f); glVertex3f(i + 3.0f, j + 4.0f, -0.001);  // Top Right Of The Texture and Quad
							glTexCoord2f(0.0f, 1.0f); glVertex3f(i, j + 4.0f, -0.001);  // Top Left Of The Texture and Quad
						glEnd();
						door4 = true;
					}
					break;
				default:
					break;
			}
		}
	}

	for(std::list<Object*>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		if ((**it).classType == 7)
		{
			Object i = **it;

			glBindTexture(GL_TEXTURE_2D, i.texture[0]); 
			glBegin(GL_QUADS);
			//draw all objects
				glColor3f(i.rgb.r, i.rgb.g, i.rgb.b);          // Set The Color
				glTexCoord2f(0.0f, 0.0f); glVertex3f(i.pos.x - i.width/2, i.pos.y - i.height/2, i.pos.z);  // Bottom Left Of The Texture and Quad
				glTexCoord2f(1.0f, 0.0f); glVertex3f(i.pos.x + i.width/2, i.pos.y - i.height/2, i.pos.z);  // Bottom Right Of The Texture and Quad
				glTexCoord2f(1.0f, 1.0f); glVertex3f(i.pos.x + i.width/2, i.pos.y + i.height/2, i.pos.z);  // Top Right Of The Texture and Quad
				glTexCoord2f(0.0f, 1.0f); glVertex3f(i.pos.x - i.width/2, i.pos.y + i.height/2, i.pos.z);  // Top Left Of The Texture and Quad
			glEnd();
		}
	}

	for(std::list<Dood*>::iterator it = doods.begin(); it != doods.end(); ++it)
	{
		Dood i = **it;

		if(!i.active)
			continue;

		glBindTexture(GL_TEXTURE_2D, i.texture[0]); 
		glBegin(GL_QUADS);
		//draw all objects
			glColor3f(i.rgb.r, i.rgb.g, i.rgb.b);          // Set The Color
			glTexCoord2f(0.0f, 0.0f); glVertex3f(i.pos.x - i.width/2, i.pos.y - i.height/2, i.pos.z);  // Bottom Left Of The Texture and Quad
			glTexCoord2f(1.0f, 0.0f); glVertex3f(i.pos.x + i.width/2, i.pos.y - i.height/2, i.pos.z);  // Bottom Right Of The Texture and Quad
			glTexCoord2f(1.0f, 1.0f); glVertex3f(i.pos.x + i.width/2, i.pos.y + i.height/2, i.pos.z);  // Top Right Of The Texture and Quad
			glTexCoord2f(0.0f, 1.0f); glVertex3f(i.pos.x - i.width/2, i.pos.y + i.height/2, i.pos.z);  // Top Left Of The Texture and Quad
		glEnd();
	}

	//hurtbox/hitbox visualization
	//disable texturing and set polygon mode to draw outlines
	glDisable(GL_TEXTURE_2D);
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	for(std::vector<Hurtbox*>::iterator it = hurtboxes.begin(); it != hurtboxes.end(); ++it)
	{
		Hurtbox i = **it;

		glBegin(GL_QUADS);
		//draw all objects
			glColor3f(0, 0.6, 1.0);          // Set The Color
			glVertex3f(i.pos.x - i.width/2, i.pos.y - i.height/2, i.pos.z + 0.1);  // Bottom Left Of The Texture and Quad
			glVertex3f(i.pos.x + i.width/2, i.pos.y - i.height/2, i.pos.z + 0.1);  // Bottom Right Of The Texture and Quad
			glVertex3f(i.pos.x + i.width/2, i.pos.y + i.height/2, i.pos.z + 0.1);  // Top Right Of The Texture and Quad
			glVertex3f(i.pos.x - i.width/2, i.pos.y + i.height/2, i.pos.z + 0.1);  // Top Left Of The Texture and Quad
		glEnd();
	}
	for(std::vector<Hitbox*>::iterator it = hitboxes.begin(); it != hitboxes.end(); ++it)
	{
		Hitbox i = **it;

		glBegin(GL_QUADS);
		//draw all objects
			glColor3f(1.0, 0.3, 0.0);          // Set The Color
			glVertex3f(i.pos.x - i.width/2, i.pos.y - i.height/2, i.pos.z + 0.1);  // Bottom Left Of The Texture and Quad
			glVertex3f(i.pos.x + i.width/2, i.pos.y - i.height/2, i.pos.z + 0.1);  // Bottom Right Of The Texture and Quad
			glVertex3f(i.pos.x + i.width/2, i.pos.y + i.height/2, i.pos.z + 0.1);  // Top Right Of The Texture and Quad
			glVertex3f(i.pos.x - i.width/2, i.pos.y + i.height/2, i.pos.z + 0.1);  // Top Left Of The Texture and Quad
		glEnd();
	}

	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glEnable(GL_TEXTURE_2D);                        // reEnable Texture Mapping

	door1 = false;
	door2 = false;
	door3 = false;
	door4 = false;
    return true;                                // Keep Going
}


GLvoid KillGLWindow(GLvoid)                         // Properly Kill The Window
{
	if (fullscreen)									// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL,0);                  // If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	if (hRC)                                // Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))                 // Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;                           // Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))                    // Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;                           // Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))                   // Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;                          // Set hWnd To NULL
	}

	 if (!UnregisterClass("OpenGL",hInstance))               // Are We Able To Unregister Class?
    {
        MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
        hInstance=NULL;                     // Set hInstance To NULL
    }
}


bool GraphicsInit(void)
{
	// Create Our OpenGL Window
	if (!CreateGLWindow("Adventures of Weechan", 640, 480, 16, fullscreen))
	{
		return false;                           // Quit If Window Was Not Created
	}
	return true;
}

void GraphicsUpdate(void)
{
	DrawGLScene();              // Draw The Scene
	SwapBuffers(hDC);           // Swap Buffers (Double Buffering)
}
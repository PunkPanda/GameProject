#include "WndProc.h" // WndProc function/windows header
#include <stdio.h>

#include "Graphics.h"
#include "Input.h"
#include "Physics.h"
#include "DebugConsole.h"
#include "Player.h"
#include "AI.h"
#include "Region.h"
#include "Combat.h"

#include <time.h>

#define FRAMERATE 16.6666667

double frametime;
unsigned frames;

float controlFrameRate = 1./60;
float frameTime = 0;
float lastTime = 0;
float elapsedTime;

int WINAPI WinMain( HINSTANCE   hInstance, // Instance
					HINSTANCE   hPrevInstance,       // Previous Instance
					LPSTR       lpCmdLine,           // Command Line Parameters
					int     nCmdShow)                // Window Show State
{
	MSG msg;            // Windows Message Structure
	BOOL    done=FALSE; // Bool Variable To Exit Loop

	CreateConsole();
	if(!GraphicsInit())
		done=true;

	LoadRegion("Region/Start.txt");

	// Player creation
	createPlayer(Point(3.0f, 3.0f, 0.02f) , Point(0.0f, 0.0f), 2.0f, 4.0f, Vector(1.0f, 1.0f, 1.0f), false);

	while(!done) // Loop that runs until done == TRUE
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) // Is there a message waiting?
		{
			if (msg.message==WM_QUIT) // Have we received a quit message?
			{
				done = TRUE;            // If so, done=TRUE
			}
			else                      // If not, deal with window messages
			{
				TranslateMessage(&msg); // Translate the message
				DispatchMessage(&msg);  // Dispatch the message
			}
		}
		else                        // If there are no messages
		{
				// Draw the scene.  Watch for ESC Key and quit messages from DrawGLScene()
			if (active) // Program active?
			{
				if(escapeswitch) // Was ESC pressed?
				{
					done=TRUE;     // ESC signalled a quit
				}
				else             // Not time to quit, update screen
				{
					lastTime = timeGetTime();

					GraphicsUpdate();
					InputUpdate();
					AIUpdate();
					DoodsUpdate();
					CollisionUpdate();
					PhysicsUpdate();

					frameTime = timeGetTime();
					elapsedTime = frameTime - lastTime;

					if (elapsedTime < FRAMERATE)
					{
						Sleep(FRAMERATE - elapsedTime);
					}
				}
			}

			if(fullscreenswitch)
			{
				fullscreenswitch = false;
				keys[VK_F1]=FALSE;              // If so make key FALSE
				KillGLWindow();                 // Kill our current window
				fullscreen=!fullscreen;         // Toggle fullscreen / windowed mode
					// Recreate Our OpenGL Window
				if (!CreateGLWindow("Adventures of Weechan", 640, 480, 16, fullscreen))
				{
					return 0; // Quit if window was not created
				}
			}
		}

		if((timeGetTime() - 1000) >= frametime)
		{
			printf("%i\n", frames);
			frames = 0;
			frametime = timeGetTime();
		}
		++frames;
	}
	RemoveConsole();

	// Shutdown
	KillGLWindow();      // Kill The Window
	return (msg.wParam); // Exit The Program
}

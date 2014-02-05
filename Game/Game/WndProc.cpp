#include "WndProc.h"

#include "Input.h"
#include "Graphics.h"

bool    active = true;

LRESULT CALLBACK WndProc(   HWND    hWnd,                   // Handle For This Window
							UINT    uMsg,                   // Message For This Window
							WPARAM  wParam,                 // Additional Message Information
							LPARAM  lParam)                 // Additional Message Information
{
	switch (uMsg)                               // Check For Windows Messages
	{
		case WM_ACTIVATE:                       // Watch For Window Activate Message
		{
			if (!HIWORD(wParam))                    // Check Minimization State
			{
				active=TRUE;                    // Program Is Active
			}
			else
			{
				active=FALSE;                   // Program Is No Longer Active
			}
 
			return 0;                       // Return To The Message Loop
		}
		case WM_SYSCOMMAND:                     // Intercept System Commands
		{
			switch (wParam)                     // Check System Calls
			{
				case SC_SCREENSAVE:             // Screensaver Trying To Start?
				case SC_MONITORPOWER:               // Monitor Trying To Enter Powersave?
				return 0;                   // Prevent From Happening
			}
			break;                          // Exit
		}
		case WM_CLOSE:                          // Did We Receive A Close Message?
		{
			PostQuitMessage(0);                 // Send A Quit Message
			return 0;                       // Jump Back
		}
		case WM_KEYDOWN:                        // Is A Key Being Held Down?
		{
			if(!keys[wParam])
				keyTriggered[wParam] = true;

			keys[wParam] = TRUE;                    // If So, Mark It As TRUE
			return 0;                       // Jump Back
		}
		case WM_KEYUP:                          // Has A Key Been Released?
		{
			if(keys[wParam])
				keyReleased[wParam] = true;
			keys[wParam] = FALSE;                   // If So, Mark It As FALSE
			return 0;                       // Jump Back
		}
		case WM_LBUTTONDOWN:
		{
			keys[wParam] = TRUE;
			return 0;
		}
		case WM_LBUTTONUP:
		{
			keys[wParam] = FALSE;
			return 0;
		}
		case WM_SIZE:                           // Resize The OpenGL Window
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));       // LoWord=Width, HiWord=Height
			return 0;                       // Jump Back
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
    return DefWindowProc(hWnd,uMsg,wParam,lParam);
}
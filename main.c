#include <windows.h>
#include <stdio.h>


// Globals
BOOL update;
BOOL btn_down;

void greet()
{
  printf("SPACE + ESC to exit..");
}

LRESULT CALLBACK MouseHookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
  if (nCode >= 0)
  {
    if (wParam == WM_RBUTTONDOWN)
    {
      update = TRUE;
      btn_down= TRUE; 
    }
    if (wParam == WM_RBUTTONUP)
    {
      update = TRUE;
      btn_down = FALSE; 
    }
  }
  return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int main()
{
  greet();
  
  HHOOK mouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookCallback, NULL, 0);

  INPUT input = { 0 };
  input.type = INPUT_MOUSE;

  MSG msg;
  while (1)
  {

    BOOL escPressed = FALSE;
    BOOL spacePressed = FALSE;

    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    
    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {  // Check for Escape key
      escPressed = TRUE;
    }
    if (GetAsyncKeyState(VK_SPACE) & 0x8000) {  // Check for Space key
      spacePressed = TRUE;
    }
    
    // Check for exit 
    if (spacePressed && escPressed)
    {
      break;
    }

    // Send input 
    if (update)
    {
      if (btn_down)
      {
        input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN;
      }
      else 
      {
        input.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP;  
      }
      SendInput(1, &input, sizeof(INPUT));
      update = FALSE;
    }
  }

  UnhookWindowsHookEx(mouseHook);

  return 0;
}




#include <windows.h>
#include "resource.h"

const char g_szClassName[] = "Window Calss";

LRESULT CALLBACK WndProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_COMMAND:
        switch(LOWORD(wParam))
        {
            case ID_FILE_EXIT:
                PostMessage(hWindow, WM_CLOSE, 0, 0);
            break;

            case ID_STUFF_GO:
            break;
        }
        break;

        case WM_LBUTTONDOWN:
        {
            char szFileName[MAX_PATH];
            HINSTANCE hInstance = GetModuleHandle(NULL);
            GetModuleFileName(hInstance, szFileName, MAX_PATH);
            MessageBox(NULL, szFileName, "Runinng prog is:", MB_OK | MB_ICONINFORMATION);
            break;
        }
        case WM_CLOSE:
            DestroyWindow(hWindow);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hWindow, msg, wParam, lParam);

    }

    return 0;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    LPSTR LpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    HWND hWindow;
    MSG msg;

    //Register the window class
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;   //Class Styles "CS_xxx"
    wc.lpfnWndProc = WndProc;   //Pointer to window procedure
    wc.cbClsExtra = 0;   //Extra data for class
    wc.cbWndExtra = 0;   //Extra data for window
    wc.hInstance = hInstance;   //Handle to application
    wc.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MYICON));   //icon når man alt-tabber
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);   //Cursor over vinduet
    wc.hbrBackground = (HBRUSH) (COLOR_WINDOW+1);  //Background brush til at sætte farven
    wc.lpszMenuName = MAKEINTRESOURCE(IDR_MYMENU);  //Navn på menu-resource
    wc.lpszClassName =  g_szClassName;   //Classname...
    wc.hIconSm = (HICON) LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MYICON), IMAGE_ICON, 16, 16, 0);  //Icon i taskbar og på vindue

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration failed lol", "ERAWR", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }


    //Create the window
    hWindow = CreateWindowEx(WS_EX_CLIENTEDGE,   //Extended window style
                                g_szClassName,
                                "Title of ze window,",
                                WS_OVERLAPPEDWINDOW,    //Window style
                                CW_USEDEFAULT, //X koordinat for top left
                                CW_USEDEFAULT, //Y koordinat for top left
                                240, 120,   //X/Y til bredde
                                NULL, //PArent window handle
                                NULL, //menu handle
                                hInstance, //App instance handle
                                NULL);  //pointer to window creation data (never used basicaly)
    if(hWindow==NULL)
    {
        MessageBox(NULL, "Window Creation failed lulz", "Erawrs", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }


    ShowWindow(hWindow, nCmdShow); //nCmdShow gives executor ability to use -maximize etc. when calling prog
    UpdateWindow(hWindow);

    while(GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}


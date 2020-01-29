//--------------------------------------------------------------------------
// Bibliotecas
//--------------------------------------------------------------------------
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

//--------------------------------------------------------------------------
// Definições
//--------------------------------------------------------------------------
#define WINDOW_CLASS		"pacSnake"
#define WINDOW_TITLE		"Pac Snake"
#define WINDOW_WIDTH		500
#define WINDOW_HEIGHT	    500

//--------------------------------------------------------------------------
// Variáveis globais
//--------------------------------------------------------------------------
HDC hMemDC = NULL; // DC de memória
HBITMAP hBmp = NULL; // Bitmap compatível

//--------------------------------------------------------------------------
// Protótipo das funções
//--------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

//--------------------------------------------------------------------------
// Recursos e cabeçalhos
//--------------------------------------------------------------------------
#include "resource.h"

//--------------------------------------------------------------------------
// WinMain() -> Função principal
//--------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
    WNDCLASSEX wcl;
    wcl.cbSize = sizeof(WNDCLASSEX);
    wcl.style = CS_HREDRAW | CS_VREDRAW;
    wcl.lpfnWndProc = (WNDPROC)WindowProc;
    wcl.cbClsExtra = 0;
    wcl.cbWndExtra = 0;
    wcl.hInstance = hInstance;
    wcl.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MEUICONE));
    wcl.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcl.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
    wcl.lpszMenuName = MAKEINTRESOURCE(IDM_MEUMENU);
    wcl.lpszClassName = WINDOW_CLASS;
    wcl.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MEUICONE));

    // Registra a classe da janela
    if(RegisterClassEx(&wcl)){
    // Cria a janela principal do programa
        HWND hWnd = NULL;
        hWnd = CreateWindowEx(
            NULL,
            WINDOW_CLASS,
            WINDOW_TITLE,
            WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_EX_CLIENTEDGE,
            (GetSystemMetrics(SM_CXSCREEN) - WINDOW_WIDTH) / 2,
            (GetSystemMetrics(SM_CYSCREEN) - WINDOW_HEIGHT) / 2,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            NULL,
            NULL,
            hInstance,
            NULL
        );

    // Verifica se a janela foi criada
        if(hWnd){

        ShowWindow(hWnd, nCmdShow);
        UpdateWindow(hWnd);
        MSG msg;

        while(GetMessage(&msg, NULL, 0, 0) > 0){
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        return((int)msg.wParam);

        }else{
            MessageBox(NULL, "Não foi possível criar janela.", "Erro!", MB_OK | MB_ICONERROR);
            return(0);
        }
    }else{
        MessageBox(NULL, "Não foi possível registrar a classe da janela.", "Erro!", MB_OK | MB_ICONERROR);
        return(0);
    }
    return(0);
}

//--------------------------------------------------------------------------
// WindowProc() -> Processa as mensagens enviadas para o programa
//--------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){

    HDC hDC = NULL;
    PAINTSTRUCT psPaint;

    static int bInverteBmp = 0;

    switch(uMsg){

        case WM_CREATE:
        {
            hDC = GetDC(hWnd);
            hMemDC = CreateCompatibleDC(hDC);
            hBmp = (HBITMAP)LoadImage(NULL, "background.bmp", IMAGE_BITMAP, WINDOW_WIDTH, WINDOW_HEIGHT, LR_LOADFROMFILE);
            SelectObject(hMemDC, hBmp);
            return 0;
        }
        break;

        case WM_PAINT:
        {
            hDC = BeginPaint(hWnd, &psPaint);
            BitBlt(hDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hMemDC, 0, 0, SRCCOPY);

            EndPaint(hWnd, &psPaint);
            return 0;
        }
        break;

        case WM_COMMAND:
            switch(LOWORD(wParam)){
                case START:
                    {
                        DeleteObject(SelectObject(hMemDC, hBmp));
                        DeleteDC(hMemDC);
                        char texto[255] = "Iniciar Jogo!";
                        TextOut(hDC, 0, 0, texto, lstrlen(texto));
                    }
                break;
                case LOAD:
                    DeleteObject(SelectObject(hMemDC, hBmp));
                    DeleteDC(hMemDC);
                    char texto[255] = "Carregar Jogo!";
                    TextOut(hDC, 0, 0, texto, lstrlen(texto));
                break;
            }
            return 0;
        break;

        case WM_SIZE:
            {
                SendMessage(hWnd, WM_PAINT, (WPARAM)0, (LPARAM)0);
                return 0;
            }
        break;

        case WM_CLOSE:
        {
            DeleteObject(SelectObject(hMemDC, hBmp));
            DeleteDC(hMemDC);
            DestroyWindow(hWnd);
            return 0;
        }
        break;

        case WM_LBUTTONDOWN:
        {
            bInverteBmp = !bInverteBmp;
            InvalidateRect(hWnd, NULL, FALSE);
            return 0;
        }
        break;

        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }
        break;

        default:
        {
            return(DefWindowProc(hWnd, uMsg, wParam, lParam));
        }
        break;

    }
}

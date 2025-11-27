#include <windows.h>
#include <iostream>
#include <string>
#include "api.h"
#include <thread>
#include <atomic>
#include <chrono>





UiVector screenSize = UiVector(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
UiVector windowSize = screenSize;
UiVector mousepos = UiVector();
UiVector windowpos = UiVector();
std::atomic<bool> running = true;

void (*MainLoopFunc)(float);
HDC hdc;


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_SIZE:
            windowSize.x = LOWORD(lParam);
            windowSize.y = HIWORD(lParam);
            break;
        case WM_MOVE:
            windowpos.x = (int)(short)LOWORD(lParam); // posição X da janela
            windowpos.y = (int)(short)HIWORD(lParam); // posição Y da janela
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


auto lastexec = std::chrono::high_resolution_clock::now();

void MainLoop(HWND hwnd) {
    while (running) {
        float duration = std::chrono::duration<float, std::chrono::seconds::period>(std::chrono::high_resolution_clock::now() - lastexec).count();
        if (MainLoopFunc) {
            UpdateMousePos();
            MainLoopFunc(duration);
        }
        lastexec = std::chrono::high_resolution_clock::now();
        //Sleep(16); // ~60 FPS
        std::this_thread::sleep_for(std::chrono::milliseconds(12));
    }
}

void Start(std::string Title, HINSTANCE hInstance, int nCmdShow) {
    const char CLASS_NAME[] = "MinhaJanela";

    WNDCLASSA wc = { };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClassA(&wc);

    HWND hwnd = CreateWindowExA(
        0,
        CLASS_NAME,
        Title.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, screenSize.x/4, screenSize.y/4,
        NULL, NULL, hInstance, NULL
    );

    ShowWindow(hwnd, SW_MAXIMIZE);
    hdc = GetDC(hwnd);

    std::thread renderThread(MainLoop, hwnd);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }


    running = false;
    renderThread.join();
}

void CreateConsole() {
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);  // redireciona std::cout
    freopen_s(&f, "CONOUT$", "w", stderr);  // redireciona std::cerr
    freopen_s(&f, "CONIN$", "r", stdin);    // redireciona std::cin
}

void SetMainFunction(void (*func)(float)) {
    MainLoopFunc = func;
};



void RenderText(std::string text, UiVector pos, Color color) {
    SetTextColor(hdc, RGB(color.r, color.g, color.b));
    SetBkMode(hdc, TRANSPARENT);
    TextOutA(hdc, pos.x, pos.y, text.c_str(), text.length());
}

void RenderRect(UiVector pos, UiVector size, Color color, bool centralized) {
    RECT r;
    if (centralized) {
        r = { pos.x-size.x/2, pos.y-size.y/2, pos.x+size.x/2, pos.y+size.y/2 };
    }
    else {
        r = { pos.x, pos.y, pos.x+size.x, pos.y+size.y };
    }
    HBRUSH brush = CreateSolidBrush(RGB(color.r, color.g, color.b));
    FillRect(hdc, &r, brush);
    DeleteObject(brush);
}

UiVector* GetWindowSize() {
    return &windowSize;
};

UiVector* GetWindowPos() {
    return &windowpos;
}

void UpdateMousePos() {
    POINT p;
    GetCursorPos(&p);
    mousepos.x = p.x;
    mousepos.y = p.y;
}

UiVector* GetMousePos() {
    return &mousepos;
}

void ClearScreen(Color color) {
    RenderRect(UiVector(), screenSize, color);
};

void RenderCircle(UiVector pos, float raio, Color color) {
    HBRUSH hBrush = CreateSolidBrush(RGB(color.r, color.g, color.b));
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
    Ellipse(hdc, pos.x - raio, pos.y-raio, pos.x+raio, pos.y+raio);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hBrush);
};

void RenderCircleOutline(UiVector pos, float raio, Color color) {
    HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
    HPEN hPen = CreatePen(PS_SOLID, 3, RGB(color.r, color.g, color.b));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
    Ellipse(hdc, pos.x - raio, pos.y - raio, pos.x + raio, pos.y + raio);
    SelectObject(hdc, hOldBrush);
    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
};
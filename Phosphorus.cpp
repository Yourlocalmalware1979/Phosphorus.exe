#include <windows.h>
#include <cmath>
#define M_PI 3.14159265358979323846
#pragma comment(lib, "winmm.lib")
#include <cstdlib>
#include <windowsx.h>
using namespace std;
#define MBR_SIZE 512
static ULONGLONG n, r;
int randy() { return n = r, n ^= 0x8ebf635bee3c6d25, n ^= n << 5 | n >> 26, n *= 0xf3e05ca5c43e376b, r = n, n & 0x7fffffff; }
DWORD xs;
VOID SeedXorshift32(DWORD dwSeed) {
    xs = dwSeed;
}
DWORD Xorshift32() {
    xs ^= xs << 13;
    xs ^= xs << 17;
    xs ^= xs << 5;
    return xs;
}
COLORREF COLORHSL(int length) {
    double h = fmod(length, 360.0);
    double s = 1.0;
    double l = 0.5;

    double c = (1.0 - fabs(2.0 * l - 1.0)) * s;
    double x = c * (1.0 - fabs(fmod(h / 60.0, 2.0) - 1.0));
    double m = l - c / 2.0;

    double r1, g1, b1;
    if (h < 60) {
        r1 = c;
        g1 = x;
        b1 = 0;
    }
    else if (h < 120) {
        r1 = x;
        g1 = c;
        b1 = 0;
    }
    else if (h < 180) {
        r1 = 0;
        g1 = c;
        b1 = x;
    }
    else if (h < 240) {
        r1 = 0;
        g1 = x;
        b1 = c;
    }
    else if (h < 300) {
        r1 = x;
        g1 = 0;
        b1 = c;
    }
    else {
        r1 = c;
        g1 = 0;
        b1 = x;
    }

    int red = static_cast<int>((r1 + m) * 255);
    int green = static_cast<int>((g1 + m) * 255);
    int blue = static_cast<int>((b1 + m) * 255);

    return RGB(red, green, blue);
}
typedef union _RGBQUAD {
    COLORREF rgb;
    struct {
        BYTE b;
        BYTE g;
        BYTE r;
        BYTE unused;

    };
} *PRGBQUAD;
int red, green, blue;
bool ifcolorblue = false, ifblue = false;
COLORREF Hue(int length) {
    if (red != length) {
        red < length; red++;
        if (ifblue == true) {
            return RGB(red, 0, length);
        }
        else {
            return RGB(red, 0, 0);
        }
    }
    else {
        if (green != length) {
            green < length; green++;
            return RGB(length, green, 0);
        }
        else {
            if (blue != length) {
                blue < length; blue++;
                return RGB(0, length, blue);
            }
            else {
                red = 0; green = 0; blue = 0;
                ifblue = true;
            }
        }
    }
}
DWORD WINAPI trianglar(LPVOID lpParam) {
    while (1) {
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        HDC hdc = GetDC(0);
        HPEN hPen = CreatePen(PS_SOLID, 1, RGB(1, 0, 0));
        HPEN hOldPen = SelectPen(hdc, hPen);

        HBRUSH hBrush = CreateSolidBrush(RGB(0, 255, 255));
        HBRUSH hOldBrush = SelectBrush(hdc, hBrush);

        POINT vertices[] = { {rand() % w, rand() % h}, {rand() % w, rand() % h}, {rand() % w, rand() % h} };
        Polygon(hdc, vertices, sizeof(vertices) / sizeof(vertices[0]));

        SelectBrush(hdc, hOldBrush);
        DeleteObject(hBrush);

        SelectPen(hdc, hOldPen);
        DeleteObject(hPen);
        ReleaseDC(0, hdc);
    }
}
DWORD WINAPI shader(LPVOID lpParam) {
    HDC hdc = GetDC(0);
    HWND wnd = GetDesktopWindow();
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    BITMAPINFO bmp = { 40, w, h, 1, 24 };
    PRGBTRIPLE rgbtriple;
    while (true) {
        hdc = GetDC(0);
        HDC mdc = CreateCompatibleDC(hdc);
        HBITMAP hbit = CreateDIBSection(hdc, &bmp, 0, (void**)&rgbtriple, 0, 0);
        SelectObject(mdc, hbit);
        BitBlt(mdc, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        for (int i = 0; i < w * h; i++) {
            rgbtriple[i].rgbtRed += 10;
            rgbtriple[i].rgbtGreen += 10;
        }
        BitBlt(hdc, 0, 0, w, h, mdc, 0, 0, SRCCOPY);
        ReleaseDC(wnd, hdc);
        DeleteDC(hdc); DeleteDC(mdc); DeleteObject(hbit); DeleteObject(wnd); DeleteObject(rgbtriple); DeleteObject(&w); DeleteObject(&h); DeleteObject(&bmp);
    }
}
DWORD WINAPI zoom(LPVOID lpParam) {
while (true) {
    HDC hdc = GetDC(NULL);
    HWND desktop = GetDesktopWindow();
    HDC desk = GetDC(NULL);
    RECT rect;
    GetWindowRect(desktop, &rect);
    int w = rect.right - rect.left;
    int h = rect.bottom - rect.top;
    StretchBlt(desk, -12.5, -12.5, w - -25, h - -25, desk, 0, 0, w, h, SRCCOPY);
    Sleep(1);
}
}
DWORD WINAPI sinewave(LPVOID lpParam) {
    HDC desk = GetDC(0); HWND wnd = GetDesktopWindow();
    int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
    double angle = 0;
    while (1) {
        desk = GetDC(0);
        for (float i = 0; i < sw + sh; i += 0.99f) {
            int a = sin(angle) * 20;
            BitBlt(desk, 0, i, sw, 1, desk, a, i, SRCCOPY);
            BitBlt(desk, i, 0, 1, sh, desk, i, a, SRCCOPY);
            angle += M_PI / 80;
            DeleteObject(&i); DeleteObject(&a);
        }
        ReleaseDC(wnd, desk);
        DeleteDC(desk); DeleteObject(&sw); DeleteObject(&sh); DeleteObject(&angle);
    }
}
DWORD WINAPI thing6(LPVOID lpParam) {
    while (1) {
        HDC hdc = GetDC(0);
        HDC hdcMem = CreateCompatibleDC(hdc);
        int sw = GetSystemMetrics(0);
        int sh = GetSystemMetrics(1);
        HBITMAP bm = CreateCompatibleBitmap(hdc, sw, sh);
        SelectObject(hdcMem, bm);
        RECT rect;
        GetWindowRect(GetDesktopWindow(), &rect);
        POINT pt[3];
        int inc3 = rand() % 700;
        int v = rand() % 2;
        if (v == 1) inc3 = -700;
        inc3++;
        pt[0].x = rect.left - inc3;
        pt[0].y = rect.top + inc3;
        pt[1].x = rect.right - inc3;
        pt[1].y = rect.top - inc3;
        pt[2].x = rect.left + inc3;
        pt[2].y = rect.bottom - inc3;
        PlgBlt(hdcMem, pt, hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, 0, 0, 0);
        HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
        SelectObject(hdc, brush);
        BitBlt(hdc, rand() % 20, rand() % 20, sw, sh, hdcMem, rand() % 20, rand() % 20, 0x123456);
        DeleteObject(brush);
        DeleteObject(hdcMem); DeleteObject(bm);
        ReleaseDC(0, hdc);
        Sleep(1);
    }
}

DWORD WINAPI screen(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    _RGBQUAD* data = (_RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(_RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    for (int i = 0;; i++, i %= 3) {
        HDC desk = GetDC(NULL);
        HDC hdcdc = CreateCompatibleDC(desk);
        HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
        SelectObject(hdcdc, hbm);
        BitBlt(hdcdc, 0, 0, w, h, desk, 0, 0, SRCCOPY);
        GetBitmapBits(hbm, w * h * 4, data);
        for (int i = 0; i < w * h; i++) {
            int x = i % w, y = i / h, t = y ^ y | x;
            data[i].rgb = RGB(GetRValue(Hue(239)), GetGValue(Hue(239)), GetBValue(Hue(239)));
        }
        SetBitmapBits(hbm, w * h * 4, data);
        BitBlt(desk, 0, 0, w, h, hdcdc, 0, 0, SRCCOPY);
        DeleteObject(hbm);
        DeleteObject(hdcdc);
        DeleteObject(desk);
    }
    return 0;
}
DWORD WINAPI textout1(LPVOID lpParam) {
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    int signX = 1;
    int signY = 1;
    int signX1 = 1;
    int signY1 = 1;
    int incrementor = 10;
    int x = 10;
    int y = 10;
    LPCSTR lpText = "Phosphorus.exe";
    while (1) {
        HDC hdc = GetDC(0);
        x += incrementor * signX;
        y += incrementor * signY;
        int top_x = 0 + x;
        int top_y = 0 + y;
        SetBkMode(hdc, 0);
        SetTextColor(hdc, Hue(239));
        //SetBkColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
        HFONT font = CreateFontA(43, 32, 0, 0, FW_THIN, 0, 1, 0, ANSI_CHARSET, 0, 0, 0, 0, "TEXTFONT HOLY SHIT");
        SelectObject(hdc, font);
        TextOutA(hdc, top_x, top_y, lpText, strlen(lpText));
        if (y >= GetSystemMetrics(SM_CYSCREEN))
        {
            signY = -1;
        }

        if (x >= GetSystemMetrics(SM_CXSCREEN))
        {
            signX = -1;
        }

        if (y == 0)
        {
            signY = 1;
        }

        if (x == 0)
        {
            signX = 1;
        }
        Sleep(4);
        DeleteObject(font);
        ReleaseDC(0, hdc);
    }
}
DWORD WINAPI shader2(LPVOID lpParam) {
    int ticks = GetTickCount(), w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    RGBQUAD* data = (RGBQUAD*)VirtualAlloc(0, (w * h + w) * sizeof(RGBQUAD), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    for (int i = 0;; i++, i %= 3) {

        HDC hdc = GetDC(0), hdcMem = CreateCompatibleDC(hdc);
        HBITMAP hbm = CreateBitmap(w, h, 1, 32, data);
        SelectObject(hdcMem, hbm);
        BitBlt(hdcMem, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
        GetBitmapBits(hbm, w * h * 4, data);
        int v = 0; BYTE bt = 0;
        if ((GetTickCount() - ticks) > 60000) bt = rand() & 0xffffff;
        for (int i = 0; w * h > i; i++) {
            if (i % h == 0 && rand() % 100 == 0) v = rand() % 50;
            ((BYTE*)(data + i))[v ? 252 : 252] += ((BYTE*)(data + i))[i % 3] ^ bt;
        }
        SetBitmapBits(hbm, w * h * 4, data);
        BitBlt(hdc, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
        DeleteObject(hbm); DeleteObject(hdcMem);
        DeleteObject(hdc);
    }
}
DWORD WINAPI waves(LPVOID lpParam) {
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    double angle = 0.0;
    HDC hdc = GetDC(0);
    HDC mdc = CreateCompatibleDC(hdc);
    HBITMAP bmp = CreateCompatibleBitmap(hdc, x, y);
    SelectObject(mdc, bmp);

    while (true) {
        BitBlt(mdc, 0, 0, x, y, hdc, 0, 0, SRCCOPY);

        for (int i = 0; i < x + y; i++) {
            BitBlt(hdc, 0, i, x, 1, mdc, sin(angle) * 40, i, NOTSRCCOPY);

            angle += rand() % 5 / 4;
        }

        Sleep(10);
    }
}
DWORD WINAPI rgbscreen(LPVOID lpstart)
{
    while (1) {
        HDC hdc = GetDC(0);
        int x = SM_CXSCREEN;
        int y = SM_CYSCREEN;
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        BitBlt(hdc, rand() % 222, rand() % 222, w, h, hdc, rand() % 222, rand() % 222, NOTSRCERASE);
        Sleep(10);
        ReleaseDC(0, hdc);
    }
}
DWORD WINAPI Rotate(LPVOID lpstart) {
    HDC hdc = GetDC(0);
    RECT rekt;
    POINT rotate[3];

    while (true) {
        int rnd = 65 + 60;

        for (int i = 0; i < rnd; i += 50) {
            GetWindowRect(GetDesktopWindow(), &rekt);

            rotate[0].x = rekt.left + i;
            rotate[0].y = rekt.top - i;
            rotate[1].x = rekt.right + i;
            rotate[1].y = rekt.top + i;
            rotate[2].x = rekt.left - i;
            rotate[2].y = rekt.bottom - i;

            PlgBlt(hdc, rotate, hdc, 0, 0, rekt.right - rekt.left, rekt.bottom - rekt.top, 0, 0, 0);
            Sleep(40);
        }
    }
}
int mandelbrot(float real, float imag, float maxiter, float shape) {
    int n = 0;

    float zr = 0.0;
    float zi = 0.0;

    while (n < maxiter && zr * zr + zi * zi <= 4.0) {
        float temp = zr * zr - zi * zi + real;

        zi = shape * zr * zi + imag;
        zr = temp;
        n++;
    }

    return n;
}

DWORD WINAPI rotozoom(LPVOID lpstart) { //by RaduMinecraft
    int x = GetSystemMetrics(SM_CXSCREEN);
    int y = GetSystemMetrics(SM_CYSCREEN);
    int c = 0;
    int p = 3; //set pixelation (best from 1 to 5)
    float shape = 2.0;
    //shape = 1.0 - depends of what you see
    //shape = 2.0 - perfectly normal mandelbrot
    //shape = 3.0 - strinked mandelbrot
    //shape = 4.0 - more strinked mandelbrot
    //shape = ... - keeps strinking
    int maxiter = 50; //50 is right
    float angle = 0.0;

    DWORD* data = new DWORD[4 * x * y];
    HDC hdc = GetDC(0);
    HDC mdc = CreateCompatibleDC(hdc);
    HBITMAP bmp = CreateBitmap(x, y, 1, 32, data);
    SelectObject(mdc, bmp);

    while (true) {
        StretchBlt(mdc, 0, 0, x / p, y / p, hdc, 0, 0, x, y, SRCCOPY);
        GetBitmapBits(bmp, 4 * x * y, data);

        for (int x2 = 0; x2 < x / p; x2++) {
            for (int y2 = 0; y2 < y / p; y2++) {
                int cx = x2 - (x / (2 * p));
                int cy = y2 - (y / (2 * p));

                int zx = (int)((cx * cos(angle) - cy * sin(angle)));
                int zy = (int)((cx * sin(angle) + cy * cos(angle)));

                int x3 = cx + (sin(angle * 100) * 10);
                int y3 = cy + (cos(angle * 100) * 10);

                //0.002 is the size so you can change it
                int n = mandelbrot(zx * (0.002 * p), zy * (0.002 * p), maxiter, shape);

                //here is the main part
                int wave = (c + (c * sin(sqrt((x2 * x2) + (y2 * y2)) / 64))) + (c + (c * (sqrt((x2 * x2) + (cy * cy)) / 64))) + (c + (c * (sqrt((zx * zx) ^ (zy * zy)) / 64))) + ((n == maxiter) ? 0 : n * 255 / maxiter);// +sqrt(x2 * y2);

                data[y2 * x + x2] = COLORHSL((x * y) + wave);
            }
        }

        SetBitmapBits(bmp, 4 * x * y, data);
        StretchBlt(hdc, 0, 0, x, y, mdc, 0, 0, x / p, y / p, SRCCOPY);

        Sleep(10);

        angle += 0.01;
        c++;
    }

    return 0;
}
DWORD WINAPI triangles(LPVOID lpParam) //by fr4ctalz
{
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    int signX = 1;
    int signY = 1;
    int incrementor = 10;
    int x = 10;
    int y = 10;
    float rotationAngle = 0.0f;
    float rotationSpeed = 0.02f;
    while (1)
    {
        HDC hdc = GetDC(0);
        x += incrementor * signX;
        y += incrementor * signY;
        int top_x = 0 + x;
        int top_y = 0 + y;
        int bottom_x = 100 + x;
        int bottom_y = 100 + y;
        HBRUSH brush = CreateSolidBrush(Hue(239));
        SelectObject(hdc, brush);
        POINT vertices[] =
        {
            {
                top_x, bottom_y
            },
            {
                bottom_x, bottom_y
            },
            {
                (top_x + bottom_x) / 2, top_y
            }
        };
        POINT center =
        {
            (top_x + bottom_x) / 2, (top_y + bottom_y) / 2
        };
        Polygon(hdc, vertices, sizeof(vertices) / sizeof(vertices[0]));
        if (y >= GetSystemMetrics(SM_CYSCREEN))
        {
            signY = -1;
        }
        if (x >= GetSystemMetrics(SM_CXSCREEN))
        {
            signX = -1;
        }
        if (y == 0)
        {
            signY = 1;
        }
        if (x == 0)
        {
            signX = 1;
        }
        rotationAngle += rotationSpeed;
        Sleep(1);
        DeleteObject(brush);
        ReleaseDC(0, hdc);
    }
}
DWORD WINAPI patblt(LPVOID lpParam)
{
    while (1) {
        HDC hdc = GetDC(NULL);
        int w = GetSystemMetrics(SM_CXSCREEN),
            h = GetSystemMetrics(SM_CYSCREEN);

        HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
        SelectObject(hdc, brush);
        PatBlt(hdc, 0, 0, w, h, PATINVERT);
        DeleteObject(brush);
        ReleaseDC(NULL, hdc);
    }
}
DWORD WINAPI textout2(LPVOID lpParam)
{
    HDC hdc;
    int sx = 0, sy = 0;
    LPCWSTR lpText = L"Phosphorus.exe";
    while (1)
    {
        hdc = GetWindowDC(GetDesktopWindow());
        sx = GetSystemMetrics(0);
        sy = GetSystemMetrics(1);
        SetTextColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
        SetBkColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
        TextOutW(hdc, rand() % sx, rand() % sy, lpText, wcslen(lpText));
    }
}
VOID WINAPI sound1() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t >> t);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound2() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(10 * (t >> 7 | 3 * t | t >> (t >> 15)) + (t >> 8 & 5));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound3() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t >> 6 | t << 1) + (t >> 5 | t << 3 | t >> 3) | t >> 2 | t << 1;

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound4() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(2 * (1 - (t + 10 >> (t >> 9 & t >> 14) & t >> 4 & -2)) * ((t >> 10 ^ t + (t >> 6 & 127) >> 10) & 1) * 32 + 128);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

VOID WINAPI sound5() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>((t >> 1) * (15 & 0x234568a0 >> (t >> 8 & 28)) | t >> 1 >> (t >> 11) ^ t >> 12) + (t >> 4 & t & 24);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}
VOID WINAPI sound6() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(100 * ((t << 2 | t >> 5 | t ^ 63) & (t << 10 | t >> 11)));

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}
VOID WINAPI sound7() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 22050, 22050, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[22050 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t - (t >> 4 & t >> 8) & t >> 12) - 1;

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}
VOID WINAPI sound8() {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);
    char buffer[8000 * 30] = {};
    for (DWORD t = 0; t < sizeof(buffer); ++t)
        buffer[t] = static_cast<char>(t * ((t & 4096 ? t % 65536 < 59392 ? 7 : t & 7 : 16) + (1 & t >> 14)) >> (3 & t >> (t & 2048 ? 2 : 10)) | t >> (t & 16384 ? t & 4096 ? 10 : 3 : 2) + t & t >> 8);

    WAVEHDR header = { buffer, sizeof(buffer), 0, 0, 0, 0, 0, 0 };
    waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    waveOutClose(hWaveOut);
}

int main() {

    FreeConsole();

    if (MessageBoxW(NULL, L"Run GDI malware?", L"Phosphorus.exe", MB_YESNO | MB_ICONEXCLAMATION) == IDNO)

        ExitProcess(0);

    else
    {
        if (MessageBoxW(NULL, L"You sure?", L"Phosphorus.exe", MB_YESNO | MB_ICONEXCLAMATION) == IDNO)
        {
            ExitProcess(0);
        }

        else {
            HANDLE thread2 = CreateThread(0, 0, shader, 0, 0, 0);
            HANDLE thread2dot = CreateThread(0, 0, trianglar, 0, 0, 0);
            HANDLE thread2dot2 = CreateThread(0, 0, zoom, 0, 0, 0);
            sound1();
            Sleep(30000);
            TerminateThread(thread2, 0);
            CloseHandle(thread2);
            TerminateThread(thread2dot, 0);
            CloseHandle(thread2dot);
            TerminateThread(thread2dot2, 0);
            CloseHandle(thread2dot2);
            InvalidateRect(0, 0, 0);
            Sleep(90);
            HANDLE thread3 = CreateThread(0, 0, sinewave, 0, 0, 0);
            sound2();
            Sleep(30000);
            TerminateThread(thread3, 0);
            CloseHandle(thread3);
            InvalidateRect(0, 0, 0);
            Sleep(90);
            HANDLE thread4 = CreateThread(0, 0, thing6, 0, 0, 0);
            sound3();
            Sleep(30000);
            TerminateThread(thread4, 0);
            CloseHandle(thread4);
            InvalidateRect(0, 0, 0);
            Sleep(90);
            HANDLE thread5 = CreateThread(0, 0, screen, 0, 0, 0);
            HANDLE thread5dot = CreateThread(0, 0, textout1, 0, 0, 0);
            sound4();
            Sleep(30000);
            TerminateThread(thread5, 0);
            CloseHandle(thread5);
            TerminateThread(thread5dot, 0);
            CloseHandle(thread5dot);
            InvalidateRect(0, 0, 0);
            Sleep(90);
            HANDLE thread6 = CreateThread(0, 0, shader2, 0, 0, 0);
            sound5();
            Sleep(30000);
            TerminateThread(thread6, 0);
            CloseHandle(thread6);
            InvalidateRect(0, 0, 0);
            Sleep(90);
            HANDLE thread8 = CreateThread(0, 0, Rotate, 0, 0, 0);
            HANDLE thread8dot = CreateThread(0, 0, rgbscreen, 0, 0, 0);
            HANDLE thread8dot2 = CreateThread(0, 0, waves, 0, 0, 0);
            sound6();
            Sleep(30000);
            TerminateThread(thread8, 0);
            CloseHandle(thread8);
            InvalidateRect(0, 0, 0);
            Sleep(90);
            HANDLE thread9 = CreateThread(0, 0, rotozoom, 0, 0, 0);
            HANDLE thread9dot = CreateThread(0, 0, triangles, 0, 0, 0);
            sound7();
            Sleep(30000);
            TerminateThread(thread9, 0);
            CloseHandle(thread9);
            TerminateThread(thread9dot, 0);
            CloseHandle(thread9dot);
            InvalidateRect(0, 0, 0);
            Sleep(90);
            HANDLE thread10 = CreateThread(0, 0, patblt, 0, 0, 0);
            HANDLE thread10dot = CreateThread(0, 0, textout2, 0, 0, 0);
            sound8();
            Sleep(30000);
            TerminateThread(thread10, 0);
            CloseHandle(thread10);
            TerminateThread(thread10dot, 0);
            CloseHandle(thread10dot);
            InvalidateRect(0, 0, 0);
            return 0;
        }
    }
}


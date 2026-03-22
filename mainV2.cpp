#include"stdio.h"
#include"stdlib.h"
#include"string.h"
#include"windows.h"
#include"windowsx.h"

#define WIDTH 500
#define HEIGHT 500

// Class duh
class SimulationManager{
	private:
		// Which buffer to use
		int turn;
		// Rendering Related Data
		HDC _hdc,_hmemdc;
		HWND _hwnd;
		// Screen Buffer
		unsigned char* _point;
		HBITMAP _hbmp;
		double averageFPS;
		int frameCount;
	public:
		SimulationManager(){}
		SimulationManager(HWND hwnd)
		{

			frameCount=0;
			// Screen buffer prep code here
			_hdc=GetDC(hwnd);
			if(_hdc==0)printf("screen DC error\n");
			_hwnd=hwnd;
			BITMAPINFO bmi={0};
			bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			bmi.bmiHeader.biWidth = WIDTH;
			bmi.bmiHeader.biHeight = -HEIGHT; // top-down
			bmi.bmiHeader.biPlanes = 1;
			bmi.bmiHeader.biBitCount = 32;
			bmi.bmiHeader.biCompression = BI_RGB;
			_hbmp=CreateDIBSection(_hdc,&bmi,DIB_RGB_COLORS,(void**)&_point,0,0);
			if(_hbmp==0)printf("Bitmap error\n");
			memset(_point,RGB(255,255,255),WIDTH*HEIGHT*4);
			_hmemdc=CreateCompatibleDC(_hdc);
			SelectObject(_hmemdc,_hbmp);
			BitBlt(_hdc,0,0,WIDTH,HEIGHT,_hmemdc,0,0,SRCCOPY);
		}
		
		~SimulationManager()
		{
			// Release screen buffer resources
			ReleaseDC(_hwnd,_hdc);
			DeleteObject(_hbmp);
			DeleteDC(_hmemdc);
			//printf("Frames generated: %d\n",turn);
		}	
		
		void Render()
		{
			static unsigned char frameIndex = 0;
			for (int i = 0; i < HEIGHT * WIDTH * 4; i=i+4)
       	 		{
				_point[i + 3] = 255; // alpha
           	 		_point[i + 2] = 255;   // Red
            			_point[i + 1] = frameIndex;   // Green
           	 		_point[i + 0] = frameIndex / 2; // Blue
        		}
        		frameIndex++;
			++frameCount;
			BitBlt(_hdc,0,0,WIDTH,HEIGHT,_hmemdc,0,0,SRCCOPY);
		}
		
		void updateFPS()
		{
			char arr[100];
			snprintf(arr,100,"Thirteen GDI - %d FPS",frameCount);
			SetWindowTextA(_hwnd, arr);
			frameCount=0;
		}
};



#include"conio.h"

SimulationManager *sm;

LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	static bool flag=true;
	static bool drawmode=false;
	switch(Message) {
		case WM_CREATE: {
			SetTimer(hwnd, 1, 1000, 0);
			break;
		}
		case WM_TIMER: {
			if(flag==true)
			{
				sm->updateFPS();
			}
			break;
		}
		case WM_DESTROY: {
			delete sm;
			PostQuitMessage(0);
			break;
		}
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	HWND hwnd;
	MSG Msg;
static constexpr const char* c_windowClassName = "ThirteenWindowClass";
bool ownsClassRegistration = false;

 		WNDCLASSEX wc = {};
                wc.cbSize = sizeof(WNDCLASSEXW);
                wc.style = CS_HREDRAW | CS_VREDRAW;
                wc.lpfnWndProc = WndProc;
                wc.hInstance = GetModuleHandle(nullptr);
                wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
                wc.lpszClassName = c_windowClassName;
                if (RegisterClassEx(&wc))
                {
                    ownsClassRegistration = true;
                }
                else if (GetLastError() == ERROR_CLASS_ALREADY_EXISTS)
                {
                    ownsClassRegistration = false;
                }
                else
                {
                    return false;
                }

                DWORD style = (WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX));
                RECT rect = { 0, 0, (LONG)WIDTH, (LONG)HEIGHT};
                AdjustWindowRect(&rect, style, FALSE);

                hwnd = CreateWindowEx(
                    0,
                    c_windowClassName,
                    "Thirteen",
                    style,
                    CW_USEDEFAULT, CW_USEDEFAULT,
                    rect.right - rect.left,
                    rect.bottom - rect.top,
                    nullptr, nullptr,
                    GetModuleHandle(nullptr),
                    nullptr
                );

                if (!hwnd)
                    return false;
                ShowWindow(hwnd, SW_SHOW);

	sm = new SimulationManager(hwnd);
	while(1)
	{
	while(PeekMessageW(&Msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	sm->Render();
	}
	return Msg.wParam;
}


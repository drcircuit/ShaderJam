#include "WindowContainer.h"

bool RenderWindow::Initialize(WindowContainer *pWindowContainer, HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height) {
	this->hInstance = hInstance;
	this->width = width;
	this->height = height;
	this->window_title = window_title;
	this->window_title_wide = StringConverter::StringToWide(window_title);
	this->window_class = window_class;
	this->window_class_wide = StringConverter::StringToWide(window_class);
	int x = GetSystemMetrics(SM_CXSCREEN)/2 - width /2;
	int y = GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2;
	this->RegisterWindowClass();

	this->handle = CreateWindowEx(0, this->window_class_wide.c_str(), this->window_title_wide.c_str(), WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, x,y, this->width, this->height, NULL, NULL, this->hInstance, pWindowContainer);
	
	if (this->handle == NULL) {
		ErrorLogger::Log(GetLastError(), "Window creation failed!");
		return false;
	}
	ShowWindow(this->handle, SW_SHOW);
	SetForegroundWindow(this->handle);
	SetFocus(this->handle);
	return false;
}

RenderWindow::~RenderWindow()
{
	if (this->handle != NULL) {
		UnregisterClass(this->window_class_wide.c_str(), this->hInstance);
		DestroyWindow(this->handle);
	}
}

LRESULT CALLBACK HandleMsgRedirect(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_CLOSE:
		DestroyWindow(hwnd);
		return 0;
	default:
		WindowContainer* const pWindow = reinterpret_cast<WindowContainer*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
	}
}

LRESULT CALLBACK HandleMessageSetup(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_NCCREATE:
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		WindowContainer* pWindow = reinterpret_cast<WindowContainer*>(pCreate->lpCreateParams);
		if (pWindow == nullptr) {
			ErrorLogger::Log("Critical! Pointer to Window Container is null!");
			exit(-1);
		}
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMsgRedirect));
		return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
	}
	default: 
	{
		return DefWindowProc(hwnd, uMsg, wParam, lParam); 
	}

	}
}

void RenderWindow::RegisterWindowClass() {
	WNDCLASSEX wc;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = HandleMessageSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = this->hInstance;
	wc.hIcon = NULL;
	wc.hIconSm = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = this->window_class_wide.c_str();
	wc.cbSize = sizeof(WNDCLASSEX);
	RegisterClassEx(&wc);
}

bool RenderWindow::ProcessMessage() {
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	if (PeekMessage(&msg, this->handle, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (msg.message == WM_NULL) {
		if (!IsWindow(this->handle)) {
			this->handle = NULL;
			UnregisterClass(this->window_class_wide.c_str(), this->hInstance);
			return false;
		}
	}
	return true;
}
#include <Windows.h>
#include <string>

#include "Calculate.h"

uint64_t
	pointX,
	pointY,
	k = 2;

bool divisorFound = false;

LRESULT CALLBACK WindowProcedure(
	HWND aWindow,
	UINT aMessage,
	WPARAM aWParameter,
	LPARAM aLParameter)
{
	switch (aMessage)
	{
	case WM_CREATE:
	{	
		return 0;
	}	
	case WM_PAINT: 
	{
		PAINTSTRUCT paintStructure;
		HDC deviceContext = BeginPaint(aWindow, &paintStructure);

		RECT clientRect;
		GetClientRect(aWindow, &clientRect);
		FillRect(deviceContext, &clientRect, (HBRUSH)GetStockObject(WHITE_BRUSH));

		HPEN pen = CreatePen(PS_SOLID, 1, RGB(113, 210, 200)); 

		SelectObject(deviceContext, pen);

		for (int x = 0; x < fieldSize + 1; x++) 
		{
			POINT previousPoint;
			MoveToEx(deviceContext, 10 + 3 * x, 10, &previousPoint); 
			LineTo(deviceContext, 10 + 3 * x, fieldSize * 3 + 10);
		}

		for (int y = 0; y < fieldSize + 1; y++) 
		{
			POINT previousPoint;
			MoveToEx(deviceContext, 10, 10 + 3 * y, &previousPoint); 
			LineTo(deviceContext, fieldSize * 3 + 10, 10 + 3 * y);
		}

		DeleteObject(pen);

		bool firstPointFound = false;
		
		HBRUSH 
			redBrush = CreateSolidBrush(RGB(240, 0, 0)),
			blackBrush = CreateSolidBrush(RGB(0, 0, 0));

		for (uint64_t x = 0; x < fieldSize; x++) 
			for(uint64_t y = 0; y < fieldSize; y++) 
			{
				if (onCurve(x, y))
				{
					uint64_t paint_y = fieldSize - 1 - y;
					RECT rectangle{ 10 + 3 * x + 1, 10 + 3 * paint_y + 1, 10 + 3 * x + 3, 10 + 3 * paint_y + 3 }; 

					if (!firstPointFound)
					{
						pointX = x;
						pointY = y;
						firstPointFound = true;

						FillRect(deviceContext, &rectangle, blackBrush);
					}
					else
					{						
						FillRect(deviceContext, &rectangle, redBrush);
					}
				}
			}

		DeleteObject(redBrush);
		DeleteObject(blackBrush);
		
		EndPaint(aWindow, &paintStructure);
	}
	case WM_TIMER: // 
	{
		if (!divisorFound)
		{

			const uint64_t divisor = tryMultiplyPoint(pointX, pointY, k);
			k++;

			if (divisor == 1)
			{
				uint64_t paint_y = fieldSize - 1 - pointY;

				RECT rectangle{ 10 + 3 * pointX, 10 + 3 * paint_y, 10 + 3 * pointX + 4, 10 + 3 * paint_y + 4 };

				for (int i = 0; i < 2 /* 10 */; i++)
				{

					HDC deviceContext = GetDC(aWindow);

					HBRUSH brush = CreateSolidBrush(RGB(240, 240, 240));

					FillRect(deviceContext, &rectangle, brush);

					DeleteObject(brush);

					ReleaseDC(aWindow, deviceContext);

					Sleep(100);

					deviceContext = GetDC(aWindow);

					brush = CreateSolidBrush(RGB(240, 0, 0));

					FillRect(deviceContext, &rectangle, brush);

					DeleteObject(brush);

					ReleaseDC(aWindow, deviceContext);

					Sleep(100);
				}
			}
			else
			{
				divisorFound = true;

				if (divisor == 0)
				{
					std::string message = "Point at infinity reached! Multiplier: " + std::to_string(k);

					MessageBox(aWindow, message.c_str(), "Divisor not found", MB_ICONINFORMATION);
				}
				else
				{
					std::string message = "Divisor is " + std::to_string(divisor) + ", multiplier: " + std::to_string(k);

					MessageBox(aWindow, message.c_str(), "Divisor found", MB_ICONINFORMATION);
				}
			}
		}
		
		return 0;
	}	
	case WM_CLOSE:
	{
		DestroyWindow(aWindow);
		return 0;
	}
	case WM_DESTROY:
	{	
		PostQuitMessage(0);
		break;
	}
	default:
		return DefWindowProc(aWindow, aMessage, aWParameter, aLParameter);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE aInstance, HINSTANCE aPreviousInstance, LPSTR aCommandLine, int aShowCommand)
{
	UNREFERENCED_PARAMETER(aPreviousInstance);
	UNREFERENCED_PARAMETER(aCommandLine);

	WNDCLASSEX vWindowClass;
	vWindowClass.cbSize = sizeof(WNDCLASSEX);
	vWindowClass.style = CS_HREDRAW | CS_VREDRAW;
	vWindowClass.lpfnWndProc = WindowProcedure;
	vWindowClass.cbClsExtra = 0;
	vWindowClass.cbWndExtra = 0;
	vWindowClass.hInstance = aInstance;
	vWindowClass.hIcon = LoadIcon(aInstance, IDI_APPLICATION);
	vWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	vWindowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	vWindowClass.lpszMenuName = NULL;
	vWindowClass.lpszClassName = "MainWindowClass";
	vWindowClass.hIconSm = LoadIcon(aInstance, IDI_APPLICATION);

	RegisterClassEx(&vWindowClass);

	HWND vMainWindow = CreateWindow(
		"MainWindowClass",
		"TEST APPLICATION",
		WS_OVERLAPPEDWINDOW,
		100,
		100,
		fieldSize * 3 + 40,
		fieldSize * 3 + 70,
		NULL,
		NULL,
		aInstance,
		NULL);

	ShowWindow(vMainWindow, aShowCommand);
	UpdateWindow(vMainWindow);

	SetTimer(vMainWindow, NULL, 500, NULL);
	
	MSG vMessage;
		
	while (GetMessage(&vMessage, NULL, 0, 0))
	{
		
		DispatchMessage(&vMessage);
		
	}

	return (int)vMessage.wParam;
}


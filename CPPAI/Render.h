#pragma once
#pragma warning(disable : 4996)
#include <Windows.h>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <cmath>
#include <random>
namespace Renderer
{
	const int __PI = 3.14159265359;
	template <int W, int H, int PixelSize>
	struct Screen
	{
		bool flip = 0;
		const int Width = W;
		const int Height = H;
		const int pixelSize = PixelSize;
		wchar_t s[W * H];
		HANDLE consoleHandle;
		CONSOLE_FONT_INFOEX cfi;
		void ResizeFont(int size)
		{
			consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
			cfi.cbSize = sizeof(cfi);
			cfi.nFont = 0;
			cfi.dwFontSize.X = pixelSize;
			cfi.dwFontSize.Y = pixelSize;
			cfi.FontFamily = FF_DONTCARE;
			cfi.FontWeight = FW_NORMAL;
			std::wcscpy(cfi.FaceName, L"Consolas");
			SetCurrentConsoleFontEx(consoleHandle, FALSE, &cfi);
		}

		Screen()
		{
			//s = new char[W * H];
			flip = 0;


			ResizeFont(pixelSize);
			system(("Mode " + std::to_string(W) + "," + std::to_string(H)).c_str());
			//consoleHandle = CreateConsoleScreenBuffer(GENERIC_ALL, 0 ,0, CONSOLE_TEXTMODE_BUFFER, 0);
			//SetConsoleActiveScreenBuffer(consoleHandle);

			Clear();
		}




		void Flip(COORD& coord)
		{
			coord.X = W - coord.X;
			coord.Y = H - coord.Y;
		}

		void Clear()
		{
			for (int i = 0; i < W * H; i++)
			{
				s[i] = ' ';
			}
		}

		void Put(const COORD& pos, wchar_t c)
		{
			if (pos.X < 0 || pos.X >= Width || pos.Y < 0 || pos.Y > Height) return;
			if (flip)
			{
				s[(W - pos.X) * W + (pos.Y)] = c;
			}
			else
			{
				s[pos.X * W + pos.Y] = c;
			}
		}

		void Fill(wchar_t c)
		{
			for (int i = 0; i < W * H; i++)
			{
				s[i] = c;
			}
		}

		void LineTo(const COORD& c_start, const COORD& c_end, wchar_t c)
		{

			int lenX = -c_start.X + c_end.X;
			int lenY = -c_start.Y + c_end.Y;
			int step = max(abs(lenX), abs(lenY));
			double deltaX = double(lenX) / double(step);
			double deltaY = double(lenY) / double(step);
			double startX = c_start.X;
			double startY = c_start.Y;
			for (int i = 0; i < step; i++)
			{
				//s[int(startX) * W + int(startY)] = c;
				Put({ short(round(startX)),short(round(startY)) }, c);
				startX += deltaX;
				startY += deltaY;
			}
		}


		void DrawRect(const COORD& c_start, const COORD& c_end, bool isHollow, wchar_t c)
		{
			for (int i = c_start.X; i <= c_end.X; i++)
			{
				for (int j = c_start.Y; j <= c_end.Y; j++)
				{
					if (isHollow && (i != c_start.X && j != c_start.Y && i != c_end.X && j != c_end.Y)) continue;
					Put({ short(i),short(j) }, c);
				}
			}
		}

		void WriteAt(const COORD& c_pos, const std::wstring & s)
		{
			for (short i = 0; i < s.length();i++)
			{
				Put({c_pos.X,c_pos.Y + i},s[i]);
			}
		}

		void DrawCircle(const COORD& c_pos, double radius,wchar_t c)
		{
			short step =  round(radius * 2 * __PI);
			for (short i = 0; i <= step * 2; i++)
			{
				double current = i;
				current /= step;
				Put({ c_pos.X + short(round(sin(current * 2.0 * __PI) * radius)),c_pos.Y + short(round(cos(current * 2.0 * __PI) * radius)) },c);
			}

		}

		void Render()
		{
			COORD cursorPosition = { 0,0 };

			DWORD written;
			consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
			WriteConsoleOutputCharacter(consoleHandle, s, (DWORD)(W * H), cursorPosition, &written);
		}
	};
}
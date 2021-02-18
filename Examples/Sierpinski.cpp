/*

The following program renders the Sierpinki triangle, which is a kind of fractal. Fractals are class of mathematical objects with 
repeating self-similar patterns visible on zooming in closer.

This is a naive, single-threaded, recursive implementation of the Sierpinski triangle with a lot of overdraw slowing it down. The user is given 
a choice of whether to use default render resolution (along with a preset triangle) or to use their own values. The performance
is measured and is displayed to the user upon completion of the rendering.

For more information on the Sierpinski triangle, please visit https://en.wikipedia.org/wiki/Sierpi%C5%84ski_triangle

*/

#include <iostream>
#include <cmath>
#include <chrono>
#include "windows.h"
#include "graphics.h"
#include "colors.h"
#include "primitives.h"

void genMidpoint(const Point &a, const Point &b, Point &mp)
{
	mp.x = (a.x + b.x) / 2;
	mp.y = (a.y + b.y) / 2;
}

void drawTriangle(const Triangle &triangle)
{
	line(triangle.a.x, triangle.a.y, triangle.b.x, triangle.b.y); // AB
	line(triangle.b.x, triangle.b.y, triangle.c.x, triangle.c.y); // BC
	line(triangle.c.x, triangle.c.y, triangle.a.x, triangle.a.y); // CA
}

void genSierpinski(const Triangle &triangle, int depth, int maxDepth)
{
	if (depth < maxDepth)
	{
		Triangle inner1, inner2, inner3;
		Point m1, m2, m3;
		genMidpoint(triangle.a, triangle.b, m1);
		genMidpoint(triangle.a, triangle.c, m2);
		genMidpoint(triangle.b, triangle.c, m3);

		inner1.a = triangle.a;
		inner1.b = m1;
		inner1.c = m2;

		inner2.a = m1;
		inner2.b = triangle.b;
		inner2.c = m3;

		inner3.a = m2;
		inner3.b = m3;
		inner3.c = triangle.c;

		drawTriangle(inner1);
		drawTriangle(inner2);
		drawTriangle(inner3);

		genSierpinski(inner1, depth + 1, maxDepth);
		genSierpinski(inner2, depth + 1, maxDepth);
		genSierpinski(inner3, depth + 1, maxDepth);
	}
}

int main()
{
	int maxDepth = 0, ch = 0, vx = 0, vy = 0;
	Triangle root;

	while (true)
	{
		std::cout << "Do you want to run with custom or default values? (1 = custom, 0 = default)" << std::endl;
		std::cin >> ch;

		if (ch)
		{
			std::cout << "Please enter the render resolution [Width x Height] in pixels - x, followed by y." << std::endl;
			std::cin >> vx;
			std::cin >> vy;
			std::cout << "Please enter the coordinates for the vertices of the root triangle." << std::endl;
			std::cout << "Please enter the coordinates(x, y) for vertex A." << std::endl;
			std::cin >> root.a.x;
			std::cin >> root.a.y;
			std::cout << "Please enter the coordinates(x, y) for vertex B." << std::endl;
			std::cin >> root.b.x;
			std::cin >> root.b.y;
			std::cout << "Please enter the coordinates(x, y) for vertex C." << std::endl;
			std::cin >> root.c.x;
			std::cin >> root.c.y;
		}
		else
		{
			vx = 800;
			vy = 800;
			root.a.x = 400;
			root.a.y = 160;
			root.b.x = 160;
			root.b.y = 640;
			root.c.x = 640;
			root.c.y = 640;
		}

		std::cout << "Please enter the max recursion depth." << std::endl;
		std::cin >> maxDepth;
		std::cout << "Generating the Sierpinski triangle..." << std::endl;
		initwindow(vx, vy, "Sierpinski");
		auto start = std::chrono::high_resolution_clock::now();
		genSierpinski(root, 0, maxDepth);
		auto stop = std::chrono::high_resolution_clock::now();
		auto diff = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
		printf("Time taken is %d seconds.\n", diff.count());
		std::cout << "Continue? (1/0)" << std::endl;
		std::cin >> ch;
		if (!ch)
		{
			closegraph();
			break;
		}
		closegraph();
	}
	system("pause"); // windows only feature	
}
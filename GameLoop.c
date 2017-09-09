#include <stdio.h>
#include <cstdlib>
#include <Windows.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

bool clear_screen()
{
#ifdef UNIX
    std::system("clear");
	return false;
#else
    std::system ("cls");
#endif
	return true;
}

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"
#define GRY   "\x1B[1;30m"

typedef struct Point{
	Point(){}
	Point(double X, double Y){ x = X; y = Y;}
	double x;
	double y;
} Point;

typedef struct Vector2D{
	Vector2D(){}
	Vector2D(double I, double J){ i = I; j = J; mag = sqrt(i*i+j*j);}
	Vector2D(Point p1, Point p2){ i = p1.x - p2.x; j = p1.y - p2.y; mag = sqrt(i*i+j*j);}
	double mag;
	double i;
	double j;
} Vector2D;

typedef struct Wall{
	Wall(){}
	Wall(Point P, Vector2D V){p = P; v = V;}
	Point p;
	Vector2D v;
} Wall;


int renderOffsetX = 300;
int renderOffsetY = 100;
int renderSizeX = 600;
int renderSizeY = 200;


Wall MAP[4] = {
		Wall(Point(0.0,0.0),Vector2D(0.0,1.0)),
		Wall(Point(0.0,1.0),Vector2D(1.0,0.0)),
		Wall(Point(1.0,1.0),Vector2D(0.0,-1.0)),
		Wall(Point(1.0,0.0),Vector2D(-1.0,0.0))
	};
	
double Vdot(Vector2D v1, Vector2D v2){
	return v1.i*v2.i + v1.j*v2.j;
}
double VperpDot(Vector2D v1, Vector2D v2){
	return v1.j*v2.i - v1.i*v2.j;
}

double trace(Vector2D ray, Point origin){

	double shortestRay = -1.0;
	
	for (int i = 0; i < sizeof(MAP); i++){
		Vector2D c = Vector2D(origin, MAP[i].p);
		//if(VperpDot(c, ray)/VperpDot(ray, MAP[i].v)) <= MAP[i].v.mag){
			double d = VperpDot(c, MAP[i].v)/VperpDot(ray, MAP[i].v);
			
				if(shortestRay == -1.0 || d<shortestRay){
					shortestRay = abs(d);
				}	
			
		//}
	}
	if(shortestRay == -1.0){return 0.0;}
	return 1.0 / shortestRay;
}

/*height is between 0 and 1*/
void drawLine(HDC dc,int x, double height){
	for(int i = renderOffsetY + (height*renderSizeY/4); i < 100 + renderOffsetY; i++)
    {
		SetPixelV(dc, x + renderOffsetX, i, RGB(255,255,255));
    }
}

int main(void)
{
	if (!clear_screen())
	{
		printf("This game only supports Windows");
		return 1;
	}
	HWND console = GetConsoleWindow();
	HDC dc = GetDC(console);
	
	printf(GRY "||++|<O>o\\O/o|++|o<O>o\\O/|++||\n");
	printf("| " WHT "HP: " RED "10/10" GRY "                  |\n");
	printf("|----------------------------|\n");
	printf("| " WHT "MP: " CYN "23/30" GRY "                  |\n");
	printf("||++|o\\O/o<O>|++|\\O/o<O>o|++||\n" RESET);
	
	for(int i = -renderSizeX/2; i < renderSizeX/2; i++){
		drawLine(dc, i, trace(Vector2D((i*0.02),1.0), Point(0.5,0.5)));
	}
	
	ReleaseDC(console, dc);
    /*std::cin.ignore();*/
	getchar();
	printf("end");
	return 0;
}




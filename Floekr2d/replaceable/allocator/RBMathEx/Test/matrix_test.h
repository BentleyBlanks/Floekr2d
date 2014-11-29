#include <iostream>
using namespace std;
#include <xmmintrin.h>
#include <time.h>
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix.h"
//console光栅化系统
//Buggy code!


//#-1 noprint-0
//80 words perline
//24 lines perscreen
bool screen_buffer[24][80];

void draw_buffer(bool buffer[24][80]);


void draw_circle(RBVector2& o,int tr)
{
	int x,y,d1,d2,dir;
	x = 0;
	y = tr;
	int d = 2*(1-tr);
	while (y>=0)
	{
		screen_buffer[int(y+o.y)][int(x+o.x)] = 1;
		screen_buffer[int(-y+o.y)][int(-x+o.x)] = 1;
		screen_buffer[int(-y+o.y)][int(x+o.x)] = 1;
		screen_buffer[int(y+o.y)][int(-x+o.x)] = 1;

		if(d<0)
		{
			d1 = 2*(d+y)-1;
			if(d1<=0) 
				dir =1;
			else
				dir = 2;
		}
		else if(d>0)
		{
			d2 = 2*(d-x)-1;
			if(d2<=0)
				dir = 2;
			else
				dir = 3;
		}
		else 
			dir =3;
		switch(dir)
		{
		case 1:
			++x;
			d+=2*x+1;
			break;
		case 2:
			++x;
			--y;
			d+=2*(x-y+1);
			break;
		case 3:
			--y;
			d+=-2*y+1;
			break;

		}
	}
}


void draw_line(const RBVector2& a,const RBVector2& b)
{
	
	
	int x = a.x, y = a.y;

	int dx = b.x - a.x, dy = b.y - a.y;

	int sx,sy;
	if(dx>0)
	{
		sx = 1;
	}
	else if(dx<0)
	{
		sx = -1;
		dx = -dx;
	}
	else
	{
		sx = 0;
	}

	if(dy>0)
	{
		sy=1;
	}
	else if(dy<0)
	{
		sy = -1;
		dy = -dy;
	}
	else
	{
		sy = 0;
	}

	int ax = 2*dx,ay = 2*dy;
	if (dy<=dx)
	{
		for(int decy = ay-dx;;x+=sx,decy += ay)
		{
			screen_buffer[y][x] = 1;

			if(x==(int)b.x)
			{
				break;
			}
			if(decy>=0)
			{
				decy-=ax;
				y+=sy;
			}
		}
	
	}
	else
	{
		for(int decx = ax-dy;;y+=sy,decx+=ax)
		{
			screen_buffer[y][x] = 1;

			if(y==(int)b.y)
				break;
			if(decx>=0)
			{
				decx-=ay;
				x+=sx;
			}
			//draw_buffer(screen_buffer);
		}
	}
}

void clear_buffer(bool buffer[24][80])
{
	for(int i=0;i<24;i++)
		for(int j=0;j<80;j++)
			buffer[i][j] = 0;
}

void clear_screen()
{
	for(int i=0;i<1;i++)
		//for(int j=0;j<80;j++)
			cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
}

void draw_buffer(bool buffer[24][80])
{
	for(int i=0;i<24;i++)
		for(int j=0;j<80;j++)
			buffer[i][j] ? cout<<'#' : cout<<' ';
}

void apply_buffer(bool buffer[24][80])
{
	for(int i=0;i<24;i++)
		for(int j=0;j<80;j++)
		{
			buffer[i][j] ? cout<<'#' : cout<<' ';
			buffer[i][j] = 0;
		}
}



void draw_polygon(RBVector2 **list,int length = 3)
{
	for(int i=0;i<length;i++)
	{
		
		if(list[i]->x*list[i]->y<0)
			continue;
			
		
		if(length==i+1) 
			draw_line(*list[i],*list[0]);
		else
			draw_line(*list[i],*list[i+1]);
		
		screen_buffer[(int)list[i]->y][(int)list[i]->x] = 1;
	}
	
}

RBVector2** set_matrix(RBVector2 **list,int length,RBMatrix& m)
{
	RBVector2** tl =  new RBVector2*[length];
	for(int i=0;i<4;i++)
	{
		tl[i] = new RBVector2;
	}
	RBVector4 vec4(0,0,1,1);
	for(int i=0;i<length;i++)
	{
		vec4.set(list[i]->x,list[i]->y,1,1);
		vec4 = vec4*m;
		tl[i]->x = vec4.x;
		tl[i]->y = vec4.y;		
	}
	return tl;
	
}

enum ARR_LEN
{
	trigle = 3,
	rectangle,
} g_len;

void set_rotation_degree(RBMatrix& r,f32 degree)
{
	r.set_identity();
	r.m[0][0] = RBMath::is_nearly_zero(RBMath::cos(DEG2RAD(degree))) ? 0 : RBMath::cos(DEG2RAD(degree));
	r.m[0][1] = RBMath::sin(DEG2RAD(degree));
	r.m[1][0] = -RBMath::sin(DEG2RAD(degree));
	r.m[1][1] = RBMath::is_nearly_zero(RBMath::cos(DEG2RAD(degree))) ? 0 : RBMath::cos(DEG2RAD(degree));
}

void set_translate(RBMatrix& t,f32 lx,f32 ly)
{
	t.set_identity();
	t.m[2][0] = lx;
	t.m[2][1] = ly;
}

void set_scale(RBMatrix& s,f32 factor)
{
	s.set_identity();
	s.m[0][0] = factor;
	s.m[1][1] = factor;
}

void main()
{
	RBVector2* rect[trigle];
	rect[0] = new RBVector2(10.f,10.f);
	rect[1] = new RBVector2(10.f,20.f);
	rect[2] = new RBVector2(16.f,20.f);


	draw_polygon(rect);

	RBMatrix t,r,s;
	t.set_identity();
	t.m[2][0] = 9;
	t.m[2][1] = 8;

	//x->y
	f32 degree = -90;
	r.set_identity();
	set_rotation_degree(r,-90);
	
	s.set_identity();
	s.m[0][0] = 2;
	s.m[1][1] = 2;

	//draw_circle(RBVector2(18,9),8);
	draw_buffer(screen_buffer);

	RBVector2::zero_vector*RBVector2::unit_vector;

	f32 d = 0.f;
	cin.get();
	
	
	//buggy code whit draw_line()
	while(1)//cin.get())
	{
		set_rotation_degree(r,d);
		draw_polygon(set_matrix(rect,3,r));
		//set_translate(t,d,0);
		//draw_polygon(set_matrix(rect,3,t));
		//set_scale(s,d);
		//draw_polygon(set_matrix(rect,3,s));
		d-=1.f;
		
		clear_screen();
		draw_buffer(screen_buffer);
		//clear_buffer(screen_buffer);
	}
	

}
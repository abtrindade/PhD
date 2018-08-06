#include<graphics.h>
#include<stdio.h>

int main(){
int gd=DETECT,gm;
int i,x,y;
initgraph(&gd,&gm,NULL);
    circle(50, 50, 30);
line(0,0,640,0);
line(0,0,0,480);
line(639,0,639,480);
line(639,479,0,479);
   // location of ellipse
    x = 250; y = 200;
 
    // here is the starting angle
    // and end angle
    int start_angle = 0;
    int end_angle = 360;
 
    // radius from x axis and y axis
    int x_rad = 100;
    int y_rad = 50;
 
    // ellipse fuction
    ellipse(x, y, start_angle,
     end_angle, x_rad, y_rad);

//random points of plotting
for(i=0;i<=1000;i++){
   x=rand()%639;
   y=rand()%480;
   putpixel(x,y,15);
}
  getch();
  closegraph();
}

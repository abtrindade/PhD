#include<graphics.h>
#include<stdio.h> 
#include <string.h>

#define AMPLIX 10
#define AMPLIY 30
#define Voc 45.5
#define Isc 9.34

int convertX(int);
int convertY(int);
void itoa(int, char *);
void reverse(char *);


int main(){
	int gd=DETECT,gm;
	int i,x,y;
	char buffer[5];
	initgraph(&gd,&gm,NULL);

	outtextxy(100, 20, "PV modeling validation - Alessandro Trindade");

	//drawing a rectangle around the windows borders
	line(0,0,639,0); //top border line
	line(0,479,639,479); //bottom border line
	line(0,0,0,479); //left side line
	line(639,0,639,479); //rigth side line

	//drawwing the X axys and the Y axys
	setcolor(1);
	line (convertX(0),convertY(0),convertX((int)Voc),convertY(0)); //line(from xo, from yo, to x1, to y1)
	line (convertX(0),convertY(0),convertX(0),convertY((int)Isc+1));

	//dots to aid identify X and Y axys
	for (i=2;i<=(int)Voc;i=i+2) putpixel(convertX(i),convertY(0),4); //X axys dots
 	for (i=2;i<=(int)Isc;i=i+2) putpixel(convertX(0),convertY(i),4); //Y axys dots



	// X axys graphic caption
	outtextxy((convertX(0)-10), (convertY(0)+10),"0,0"); //print 0,0 at the graphic
	itoa((int)Voc,buffer);
	outtextxy((convertX((int)Voc)-5), (convertY(0)+10),buffer); //print 0,maxX at the graphic	
	itoa((int)Voc/2,buffer);
	outtextxy((convertX((int)Voc/2)-5), (convertY(0)+10),buffer); //print 0,1/2 maxX at the graphic
	itoa((int)Voc/4,buffer);
	outtextxy((convertX((int)Voc/4)-5), (convertY(0)+10),buffer); //print 0,1/4 maxX at the graphic
	itoa((int)Voc*3/4,buffer);
	outtextxy((convertX((int)Voc*3/4)-5), (convertY(0)+10),buffer); //print 0,3/4 maxX at the graphic

	// Y axys graphic caption
	itoa((int)Isc,buffer);
	outtextxy((convertX(0)-10),convertY((int)Isc), buffer); //print 0,maxY at the graphic	
	itoa((int)Isc/2,buffer);
	outtextxy((convertX(0)-10),convertY((int)Isc/2), buffer); //print 0,1/2 maxY at the graphic
	itoa((int)Isc/4,buffer);
	outtextxy((convertX(0)-10),convertY((int)Isc/4), buffer); //print 0,1/4 maxY at the graphic
	itoa((int)Isc*3/4,buffer);
	outtextxy((convertX(0)-10),convertY((int)Isc*3/4), buffer); //print 0,3/4 maxY at the graphic

	//plotting the data

  	getch();
  	closegraph();
}


int convertX(int value){
	return ((100+value*AMPLIX));
}

int convertY(int value){
	return ((400-value*AMPLIY));
}

/* itoa:  convert n to characters in s */
void itoa(int n, char s[]){
     int i, sign;
     if ((sign = n) < 0)  /* record sign */
         n = -n;          /* make n positive */
     i = 0;
     do {       /* generate digits in reverse order */
         s[i++] = n % 10 + '0';   /* get next digit */
     } while ((n /= 10) > 0);     /* delete it */
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
}  


/* reverse:  reverse string s in place */
 void reverse(char s[]){
     int i, j;
     char c;

     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
}  

/*
Color	Value
BLACK		0
BLUE		1
GREEN		2
CYAN		3
RED		4
MAGENTA		5
BROWN		6
LIGHTGRAY	7
DARKGRAY	8
LIGHTBLUE	9
LIGHTGREEN	10
LIGHTCYAN	11
LIGHTRED	12
LIGHTMAGENTA	13
YELLOW		14
WHITE		15   */

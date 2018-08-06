/* mathmatical model of PV system with the 1-diode model */

#include<graphics.h>
#include<stdio.h> 
#include<string.h>
#include<math.h>

//amplify factor of axis X and Y
#define AMPLIX 10
#define AMPLIY 30

//data sheet information from the PV model Canadian 325W CS6U-325P
#define N 72    //N=number of series-connected cells
#define NOCT 45 //Nominal Operating Cell Temperature=45°C +-2 at G=800 W/m2, light spectrum AM 1.5G, windspeed=1m/s, and air temp at 20°C
#define Gref 1000 //reference irradiance is 1000 W/m²
#define Tref 298.15 //reference temperature is 25oC or 298.15K
#define mii 0.053 //short-circuit current temperature coefficient (A/K)
#define miv -0.31 //open-circuit voltage temperature coefficient (V/K)
#define Iscref 9.34
#define Vocref 45.5
#define Imref 8.78
#define Vmref 37

int convertX(float);
int convertY(float);
void itoa(int, char *);
void reverse(char *);


int main(){
	int gd=DETECT,gm;
	int i,x,y;
	double Iphref, Tair, G, kb, q, T, Tk, VT, a, Io, Iph, I, V, Vmax, Imax;
	char buffer[5];
	initgraph(&gd,&gm,NULL);

	outtextxy(100, 20, "PV modeling validation - by Alessandro Trindade May2018");


	//drawing a rectangle around the windows borders
	line(0,0,639,0); //top border line
	line(0,479,639,479); //bottom border line
	line(0,0,0,479); //left side line
	line(639,0,639,479); //rigth side line

	//drawwing the X axis and the Y axis
	setcolor(1);
	line (convertX(0),convertY(0),convertX((int)Vocref),convertY(0)); //line(from xo, yo, to x1, to y1)
	line (convertX(0),convertY(0),convertX(0),convertY((int)Iscref+1));

	//dots to aid identify X and Y axis
	for (i=1;i<=(int)Vocref;i++) putpixel(convertX(i),convertY(0),4); //X axis dots
 	for (i=1;i<=(int)Iscref;i++) putpixel(convertX(0),convertY(i),4); //Y axis dots



	// X axis graphic caption
	outtextxy((convertX(0)-10), (convertY(0)+10),"0,0"); //print 0,0 at the graphic
	itoa((int)Vocref,buffer);
	outtextxy((convertX((int)Vocref)-5), (convertY(0)+10),buffer); //print 0,maxX at the graphic	
	itoa((int)Vocref/2,buffer);
	outtextxy((convertX((int)Vocref/2)-5), (convertY(0)+10),buffer); //print 0,1/2 maxX at the graphic
	itoa((int)Vocref/4,buffer);
	outtextxy((convertX((int)Vocref/4)-5), (convertY(0)+10),buffer); //print 0,1/4 maxX at the graphic
	itoa((int)Vocref*3/4,buffer);
	outtextxy((convertX((int)Vocref*3/4)-5), (convertY(0)+10),buffer); //print 0,3/4 maxX at the graphic

	// Y axis graphic caption
	itoa((int)Iscref,buffer);
	outtextxy((convertX(0)-10),convertY((int)Iscref), buffer); //print 0,maxY at the graphic	
	itoa((int)Iscref/2,buffer);
	outtextxy((convertX(0)-10),convertY((int)Iscref/2), buffer); //print 0,1/2 maxY at the graphic
	itoa((int)Iscref/4,buffer);
	outtextxy((convertX(0)-10),convertY((int)Iscref/4), buffer); //print 0,1/4 maxY at the graphic
	itoa((int)Iscref*3/4,buffer);
	outtextxy((convertX(0)-10),convertY((int)Iscref*3/4), buffer); //print 0,3/4 maxY at the graphic

	//model and plotting the data ////////////////////
	Iphref = Iscref; //mathematical approximation from the model

	//non-deterministic variables (ESBMC will varificate it)
	Tair = 20;  //ambient temperature in degree Celsius. Depends on the day and the time
	G = 800; //irradiance of site in W/m2. Depends on the day and the time

	outtextxy(100,30, "G=");
	itoa(G,buffer);
	outtextxy(120,30,buffer);
	outtextxy(100,40, "Tair=");
	itoa(Tair,buffer);
	outtextxy(150,40,buffer);

	//set up of parameters
	kb = 1.3806503*pow(10,-23);  //Boltzmann constant in Joule
	q = 1.60217646*pow(10,-19);  //absolute value of the electron's charge
	T= Tair + G*(NOCT-20)/800; //temperature of the p-n junction or cell temperature

	outtextxy(100,50, "Tcell=");
	itoa(T,buffer);
	outtextxy(150,50,buffer);

	Tk = T + 273.15; //is the temperaute T in Kelvin
	VT = kb*Tk/q;

	//a ideality/quality factor
	a = q*(Vmref-Vocref)/(N*kb*Tk*log(1-Imref/Iscref));


	//Io = reverse saturation current of the diode
	Io = (Iscref+mii*(Tk-Tref))/(exp (q*(Vocref+miv*(Tk-Tref))/(a*N*kb*Tk)) - 1);

	//Iph = photocurrent delivered from the model
	Iph = G * (Iphref+ mii*(Tk-Tref))/Gref;

	//(voltage shifting to plot)
	Imax=0;
	for (V=0; V<= (Vocref+1); V=V+0.1){
		//PV panel model: 1-diode model
		I = Iph - Io*(exp(V/(N*a*VT)) - 1);
		if (I>Iscref) I=Iscref; //calculated current cannot be greater then Iscref, that is a limite
		if (I>Imax){ 
			Imax=I;
			Vmax=V;
		}
		if (I < 0) { //the currnet can not be negative, it is impossible
			I=0;
			putpixel(convertX(V),convertY(I),15);
			V=Vocref+10;
		}
		else putpixel(convertX(V),convertY(I),15);
	}

	/* correcting to an array configuration */
	//Iarray = Ncellsparallel*I
	//Varray = Ncellsseries*V


  	getch();
  	closegraph();
}


int convertX(float value){
	return ((100+value*AMPLIX));
}

int convertY(float value){
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

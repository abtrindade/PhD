/* mathmatical model of PV system with the 1-diode model */

#include <stdio.h>
#include <math.h>
#include<graphics.h> 

//data sheet information from the PV model Canadian 325W CS6U-325P
#define N 72    //N=number of series-connected cells
#define NOCT 45 //Nominal Operating Cell Temperature=45°C +-2 at G=800 W/m2, light spectrum AM 1.5G, windspeed=1m/s, and air temp at 20°C
#define Gref 1000 //reference irradiance is 1000 W/m²
#define Tref 298.15 //reference temperature is 25oC or 298.15K
#define mii 0.053 //short-circuit current temperature coefficient (A/K)
#define miv -0.31 //open-circuit voltage temperature coefficient (V/K)
#define Iscref 7.57
#define Vocref 41.8
#define Voc 45.5
#define Imref 6.89
#define Vmref 33.7



int main(){
//	int gd=DETECT,gm;
//	int i,x,y;
//	int N, NOCT;
//	double Gref, Tref, mii, miv, Iscref, Iphref, Vocref, Imref, Vmref, Tair, G, kb, q, T, Tk, VT, a, Io, Iph, I, V, Voc;
double Iphref, Tair, G, kb, q, T, Tk, VT, a, Io, Iph, I, V;

	//graphical initiallization
//	initgraph(&gd,&gm,NULL);

	

	//data from the site where the PV will be deployed
//	Gref = 1000; //reference irradiance is 1000 W/m²
//	Tref = 298.15; //reference temperature is 25oC or 298.15K
//	mii = 0.053; //short-circuit current temperature coefficient (A/K)***
//	miv = -0.31; //open-circuit voltage temperature coefficient (V/K)***
//	Iscref = 7.57;
	Iphref = Iscref; //mathematical approximation
//	Vocref = 41.8;
//	Voc = 45.5;
//	Imref = 6.89;
//	Vmref = 33.7;

	//non-deterministic variables
	Tair = 25;  //ambient temperature in degree Celsius. Depends on the day and the time
	G = 1000; //irradiance of site in W/m2. Depends on the day and the time

	//set up of parameters
	kb = 1.3806503*0.001;  //Boltzmann constant in Joule
	q = -1.60217646*pow(10,-19);  //absolute value of the electron's charge
	T= Tair + G*(NOCT-20)/800; //temperature of the p-n junction or cell temperature
	Tk = T + 273.15; //is the temperaute T in Kelvin
	VT = kb*Tk/q;

	//a ideality/quality factor
	a = q*(Vmref-Vocref)/(N*kb*Tk*log(1-Imref/Iscref));

	//Io = reverse saturation current of the diode
	Io = (Iscref+mii*(Tk-Tref))/(exp (q*(Vocref+miv*(Tk-Tref))/(a*N*kb*Tk)) - 1);

	//Iph = photocurrent delivered from the model
	Iph = G * (Iphref+ mii*(Tk-Tref))/Gref;

	printf ("----------PV mathematical model validation-----------\n");	
	printf ("G= %f a= %f;  Io= %f; Iph= %f \n", G, a, Io, Iph);

	//(voltage shifting to plot)
	for (V=0; V<= Voc; V++){
		//PV panel model: 1-diode model
		I = Iph - Io*(exp(V/(N*a*VT)) - 1);
		if (I < 0) {
			I=0;
			printf ("Voltage V %f and current I %f \n",V,I);
			V=Voc+1;
		}
		else printf ("Voltage V %f and current I %f \n",V,I);
	}
//  	getch();
//  	closegraph();

//correcting to an array configuration
//Iarray = Ncellsparallel*I
//Varray = Ncellsseries*V
}

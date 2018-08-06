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
#define Isc 9.34
#define Imref 6.89
#define Vmref 33.7



int main(){

	double Iphref, Tair, G, kb, q, T, Tk, VT, a, Io, Iph, I, V, Vm, Im;

	Iphref = Iscref; //mathematical approximation

	//non-deterministic variables
	Tair = 25;  //ambient temperature in degree Celsius. Depends on the day and the time
	G = 1000; //irradiance of site in W/m2. Depends on the day and the time

	//set up of parameters
	kb = 1.3806503*pow(10,-23);  //Boltzmann constant in Joule
	q = 1.60217646*pow(10,-19);  //absolute value of the electron's charge
	T= Tair + G*(NOCT-20)/800; //temperature of the p-n junction or cell temperature
T=25;
	Tk = T + 273.15; //is the temperaute T in Kelvin
	VT = kb*Tk/q;
printf("VT = %f\n",VT);

	//a ideality/quality factor
	a = q*(Vmref-Vocref)/(N*kb*Tk*log(1-Imref/Iscref));
//a=1;

	//Io = reverse saturation current of the diode
	Io = (Iscref+mii*(Tk-Tref))/(exp (q*(Vocref+miv*(Tk-Tref))/(a*N*kb*Tk)) - 1);

	//Iph = photocurrent delivered from the model
	Iph = G * (Iphref+ mii*(Tk-Tref))/Gref;

//mppt point
	//Im= Iph + Io-((a*N*kb*Tk*Isc)/(q*Voc));
	Im= Iph + Io-((a*N*VT*Isc)/Voc);
	Vm= (a*N*kb*Tk*log((a*N*kb*Tk*Isc)/(q*Io*Voc)))/q;

	printf ("----------PV mathematical model validation-----------\n");	
	printf ("G= %f a= %f;  Io= %f; Iph= %f; Im= %f; Vm= %f; Pm= %f \n", G, a, Io, Iph, Im, Vm, (Im*Vm));

	//(voltage shifting to plot)
	for (V=0; V<= Voc; V++){
		//PV panel model: 1-diode model
		I = Iph - Io*(exp(V/(N*a*VT)) - 1);
		if (I < 0) {
			I=0;
			printf ("Voltage V %f and current I %f  and Power %f\n",V,I, V*I);
			V=Voc+1;
		}
		else printf ("Voltage V %f and current I %f  and Power %f\n",V,I, V*I);
	}

//correcting to an array configuration
//Iarray = Ncellsparallel*I
//Varray = Ncellsseries*V
}

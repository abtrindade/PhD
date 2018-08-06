/* mathmatical model of PV system with the 1-diode model */
/* here the output is the MPPT (max power), with the highest values of voltage, current and power from the array of solar panels */

#include<stdio.h> 
#include<math.h>

//definition of array arrangement of solar panels (star energy projet has 3x325W in serie)
#define Ncellsparallel 1
#define Ncellsseries 3

//data sheet information from the PV model Canadian 325W CS6U-325P
#define N 72    //N=number of series-connected cells
#define NOCT 45 //Nominal Operating Cell Temperature=45°C +-2 at G=800 W/m2, light spectrum AM 1.5G, windspeed=1m/s, air temp at 20°C
#define Gref 1000 //reference irradiance is 1000 W/m²
#define Tref 298.15 //reference temperature is 25oC or 298.15K
#define mii 0.053 //short-circuit current temperature coefficient (A/K)
#define miv -0.31 //open-circuit voltage temperature coefficient (V/K)
#define Iscref 9.34
#define Vocref 45.5
#define Imref 8.78
#define Vmref 37
#define Pmref 325


int main(){
	int i,x,y;
	double Iphref, AMBtemperature, G, kb, q, T, Tk, VT, a, Io, Iph, I, V, Vmax, Imax, Pmax, Parray, Iarray, Varray;

	//model 
	Iphref = Iscref; //mathematical approximation from the model

	//non-deterministic variables (ESBMC will generate)
	AMBtemperature = 25;  //ambient temperature in degree Celsius. Depends on the day and the time
	G = 800; //irradiance of site in W/m2. Depends on the day and the time

	//set up of parameters
	kb = 1.3806503*pow(10,-23);  //Boltzmann constant in Joule
	q = 1.60217646*pow(10,-19);  //absolute value of the electron's charge
	T= AMBtemperature + G*(NOCT-20)/800; //temperature of the p-n junction or cell temperature

	Tk = T + 273.15; //is the temperaute T in Kelvin
	VT = kb*Tk/q;

	//a ideality/quality factor
	a = q*(Vmref-Vocref)/(N*kb*Tk*log(1-Imref/Iscref));


	//Io = reverse saturation current of the diode
	Io = (Iscref+mii*(Tk-Tref))/(exp (q*(Vocref+miv*(Tk-Tref))/(a*N*kb*Tk)) - 1);

	//Iph = photocurrent delivered from the model
	Iph = G * (Iphref+ mii*(Tk-Tref))/Gref;

	printf ("----------PV mathematical model validation-----------\n");	
	printf ("G= %f a= %f;  Io= %f; Iph= %f ; Tcell= %f\n", G, a, Io, Iph, T);

	Pmax=0;
	for (V=0; V<= (Vocref+1); V=V+0.1){
		//PV panel model: 1-diode model
		I = Iph - Io*(exp(V/(N*a*VT)) - 1);
		if (I>Iscref) I=Iscref; //calculated current cannot be greater then Iscref, that is a limite
		if ((V*I) > Pmax){ 
			Imax=I;
			Vmax=V;
			Pmax = Imax*Vmax;
		}
		if (I < 0) { //the currnet can not be negative, it is impossible
			I=0;
			V=Vocref+10; //break the "for" loop
		}
	}

	/* correcting to an array configuration */
	Iarray = Ncellsparallel*Imax; //current going out from the solar panels (DC current)
	Varray = Ncellsseries*Vmax; //voltage going out from the solar penels (DC voltage)
	Parray = Iarray*Varray; //output power 
	printf ("Voltage Vmax= %f, current Imax= %f , and Pmax= %f \n",Varray,Iarray, Parray);

}



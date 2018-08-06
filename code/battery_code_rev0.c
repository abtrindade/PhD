//code for the battery by Alessandro, July 2018

//#include <stdlib.h>

#include<graphics.h>
#include<stdio.h> 
#include<string.h>
#include<math.h>

#define SOC 0.90
#define AMBtemperature 32
#define C10 195

int main(){
	float I, Vd, Vc;
	int DeltaT;
	

	I = 23; //23 A has to be generated to feed the load demand (700W and 127V)
	DeltaT = (AMBtemperature - 25);
	Vd = (2.085-0.12*(1-SOC))-(1-0.007*DeltaT)*I*(0.02+4/(1+pow(I,1.3))+0.27/(pow(SOC,1.5))/C10);
	printf ("----------Battery model validation-----------\n");	
	printf ("Discharge voltage related to DeltaT= %i is Vd= %f\n", DeltaT, Vd);
	Vc = (2 + 0.16*SOC)-(1-0.025*DeltaT)*I*(0.036+6/(1+pow(I,0.86))+0.48/(pow((1-SOC),1.2))/C10);
	printf ("Charge voltage related to DeltaT= %i is Vd= %f\n", DeltaT, Vc);
	getch();

} 



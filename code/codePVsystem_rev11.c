/* --------------------------------------------------------------------------------------------------------
  Here is the code that performs the automated verification of a stand-alone solar photovoltaic system
---------------------------------------------------------------------by Alessandro Trindade, DEZ 2018 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//IMPORTANT: to declare precision here to LOG of natural base and EXP
#define precisionLN 0.001 //0.0000000001
#define precisionEXP 0.001 //0.00000001 

//case studies concerning the houses at Nova Esperanca (Star Energy Project): 510, 253, 263, and 283W. But with the same PV system (700W)
//case study at urban area of 915W of demand and 4.88 kWh of energy

//Francisco's house 501W and 3.9kWh
#ifdef HOUSE1 
  #define Phouse 501
  #define Psurge 501
  #define Econsumption 3900
  #define PACref 700
  #define MAXACref 1600
  #define loadcurve {118,118,118,46,46,46,95,95,170,170,296,242,242,95,95,95,95,95,342,288,288,288,288,118}
  #define NBS 2
  #define NBP 2
  #define capacity 220.0
  #define auton 48
  #define NPP 1  
  #define NPS 3
  #define VC 24.0  //controller voltage
  #define Vsystem 24.0  //DCbus
#elif HOUSE2
  //Aldovar's house 253W and 3.6kWh
  #define Phouse 253
  #define Psurge 722
  #define Econsumption 3600
  #define PACref 700
  #define MAXACref 1600
  #define loadcurve {136, 136, 136, 136, 136, 136, 67, 67, 184, 184, 184, 184, 184, 67, 67, 67, 67, 67, 253, 253, 253, 253, 253, 136}
  #define NBS 2
  #define NBP 2
  #define capacity 220.0
  #define auton 48
  #define NPP 1  
  #define NPS 3
  #define VC 24.0  //controller voltage
  #define Vsystem 24.0  //DCbus
#elif HOUSE3
  //Sandriane's house 263W and 2.5kWh
  #define Phouse 263
  #define Psurge 732
  #define Econsumption 2500
  #define PACref 700
  #define MAXACref 1600
  #define loadcurve {113,113,113,113,113,113,67,67,217,97,97,97,97,97,97,97,97,97,263,113,113,113,113,113}
  #define NBS 2
  #define NBP 2
  #define capacity 220.0
  #define auton 48
  #define NPP 1  
  #define NPS 3
  #define VC 24.0  //controller voltage
  #define Vsystem 24.0  //DCbus
#elif HOUSE4
  //Marilene's house 322W and 4.3 kWh
  #define Phouse 322
  #define Psurge 896
  #define Econsumption 4300
  #define PACref 700
  #define MAXACref 1600
  #define loadcurve {207,207,207,135,135,135,66,66,161,161,233,253,248,66,66,66,66,66,302,317,322,302,302,207}
  #define NBS 2
  #define NBP 2
  #define capacity 220.0
  #define auton 48
  #define NPP 1  
  #define NPS 3
  #define VC 24.0  //controller voltage
  #define Vsystem 24.0  //DCbus
#elif HOUSE5
  //Alessandro's house 915W and 4.88 kWh
  #define Phouse 915
  #define Psurge 980
  #define Econsumption 4880
  #define PACref 1200
  #define MAXACref 2400
  #define loadcurve {45,16,16,16,16,16,0,0,0,72,72,222,150,150,0,0,72,72,814,814,814,742,742,16}
  #define NBS 4
  #define NBP 1
  #define capacity 120.0
  #define auton 6
  #define NPP 2  
  #define NPS 2
  #define VC 48.0  //controller voltage
  #define Vsystem 48.0  //DCbus
#endif

//requirements/restrictions

#define bankcapacity (capacity*NBP)
#define SOClimit 75
#define SOCabsorption 95
#define SOCfull 100
#define Vbat 12.0
#define Vbulk (14.40*NBS)
#define Vfloat (13.20*NBS)
#define nb 1

//inverter data (outlet voltage, efficiency)
#define VoutAC 110.0
#define ni 0.93 
#define VAC 127.0
#ifdef HOUSE1
   #define VinDC 24.0
#elif HOUSE2
   #define VinDC 24.0
#elif HOUSE3
   #define VinDC 24.0
#elif HOUSE4
   #define VinDC 24.0
#elif HOUSE5
   #define VinDC 48.0
#endif

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
#define Vmref 37.0
#define Pmref 325.0
#define np 0.1672
#define Insol 4.92 //(kWh/m2/day CRESESB 2016)
#define Ap 1.94432 //(panel area in m2, 1.96x0.992)
#define Tcell 45.0  //NOCT

//controller
#define nc 0.98
#define IC 35.0

//global variables
int Parray, Iarray, Varray;
unsigned int nondet_uint();


//the functions to calculate the log in ESBMC, in substitution of log from language C
//code log1 if number is greater than 1, source https://stackoverflow.com/questions/3343395/value-of-natural-logarithm
double logESBMC1 (double x){
	double logx, tty, ty;
	int i;
	i=1; 
	logx = 0 ;
	ty = (x-1)/(x+1) ;
	do{
	   	logx = logx + ty / i;
    		tty = ty ;
    		ty = (ty * ((x-1)/(x+1)) * ((x-1)/(x+1)));
    		i = i + 2 ;
	} while((tty - ty) > precisionLN);
	logx = 2*logx ;
	return logx;

}

//code log2 if the number is between 0 (except) and 1, source //from: https://www.youtube.com/watch?v=zaTdPTzw5Pc if 0<x<1
double logESBMC2 (double d){ 
	double result = 0.0, term = d-1;
	int power_of_one=-1;
	int denominator = 2;
	double temp = term;
	while (temp > precisionLN || -temp > precisionLN){
		if (temp > precisionLN) result -= temp;
		else result += temp;
		term = term * (d-1);
		temp = term * power_of_one;
		temp /= denominator;
		power_of_one *= -1;
		denominator += 1;	
	}
	result += term;
	return result;
}

//the main funcion of logarithm of natural base. It calls logESBMC1 or ESBMC2.
//The natural logarithm function ln(x) is defined only for x>0. So the natural logarithm of a negative number is undefined
double logESBMC (double x){
	double answer;
	if (x > 1.0) answer = logESBMC1(x);
	else if (x>0.0) answer=logESBMC2(x);
	else answer = NAN;
	return answer;
}

//function that calculates the exponential ESBMC in substitution of EXP from language C
//from https://codereview.stackexchange.com/questions/123970/implementation-of-exp-function-in-c-using-taylor-series-expansion 
//but with improvement in the while, to solve negative numbers
double expESBMC (double num){  
    double x1, sum=0.0;
    int n=0;
    // Stops when the next term to add becomes smaller than precision.
    x1=1;
    do {
        sum += x1;
        x1  *= (num/++n);
    } while ((x1 > precisionEXP) || ((-1*x1) > precisionEXP));
    return sum;
}

void pvmodel (int, int);
//function that generates the MPPT values (Varray and Iarray) from the PV panel, 
// considering values of solar irradiance(G) and ambient temperature
void pvmodel (int Gf, int AMBtemperaturef){
	float Iphref, kb, q, T, Tk, VT, a, Io, Iph, I, V, Vmax, Imax, Pmax;
	Iphref = Iscref; //mathematical approximation from the model
	//set up of parameters
	kb = 1.3806503*pow(10,-23);  //Boltzmann constant in Joule
	q = 1.60217646*pow(10,-19);  //absolute value of the electron's charge
	T= AMBtemperaturef + Gf*(NOCT-20)/800; //temperature of the p-n junction or cell temperature

	Tk = T + 273.15; //is the temperaute T in Kelvin
	VT = kb*Tk/q;

	//a ideality/quality factor
	a = q*(Vmref-Vocref)/(N*kb*Tk*logESBMC(1-Imref/Iscref));


	//Io = reverse saturation current of the diode
	Io = (Iscref+mii*(Tk-Tref))/(expESBMC (q*(Vocref+miv*(Tk-Tref))/(a*N*kb*Tk)) - 1);

	//Iph = photocurrent delivered from the model
	Iph = Gf * (Iphref+ mii*(Tk-Tref))/Gref;

	Pmax=0;
	//sweep the V-I graph to find the MPPT
	for (V=0; V<= (Vocref+1); V=V+0.2){
		//PV panel model: 1-diode model
		I = Iph - Io*(expESBMC(V/(N*a*VT)) - 1);
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
	Iarray = NPP*Imax; //current going out from the solar panels (DC current)
	Varray = NPS*Vmax; //voltage going out from the solar penels (DC voltage)
	Parray = Iarray*Varray; //output power 
}

int sizing_check(void){
	char NTP;
	float Ecorrected, Ep, NTPmin, NPSmin, NPPmin, Cbank, NBtotal, ICmin, Iscamb, NBSmin, NBPmin; 

	//Econsumption = energy consumed in one day from the house
	Ecorrected = Econsumption / (nb*nc*ni);

	//estimation of the energy of each panel (Wh)
	Ep = Insol * Ap * np * 1000;

	//PV panel check
	NTPmin = Ecorrected/Ep;
	NTP = NPS * NPP;
	assert (NTP >= NTPmin);

	NPSmin = Vsystem / Vmref;
	//NPPmin = NTPmin/NPSmin;

	assert (NPS >= NPSmin);
	//assert (NPP >= NPPmin);

	//battery check
	Cbank = Ecorrected * auton / (Vsystem*(float)((100-SOClimit)/100));
	NBSmin = Vsystem/Vbat;
	assert (NBS >= NBSmin);
	NBPmin = Cbank / capacity;
	assert (NBP >= NBPmin);
	NBtotal = NBSmin * NBPmin;
	assert ((NBS * NBP) >= NBtotal);

	//controller check, considering just one controller
	assert (VC == Vsystem); 

	Iscamb = Iscref*(1+mii*(Tcell-25)); //from each PV panel
	ICmin = Iscamb * NPP;
	assert (IC >= ICmin);

	//inverter check
	assert (VinDC == Vsystem);
	assert ((VoutAC >= (VAC-VAC*15/100)) && (VoutAC <= (VAC+VAC*15/100)));

	assert ((Phouse <= PACref) && (Psurge <= MAXACref));
	return 1;
}


//main code
int main(){
	unsigned char SOC, checkstatus;
     	int count, countlimit, AMBtemperature, G, demand_temp[]=loadcurve, demand;
	float tempf, drainedCurrent, PinDCmin, Idcmin, Idc, Vdc;

//	checkstatus = sizing_check();
//	assert (checkstatus == 1);   //if 1, there is NO sizing error of the PV system

	//premise: start verification considering Batteries not empty
	SOC = 100; 

	//****1 - the first 8h, with solar PV generation
	// START HERE AUTOMATED VERIFICATION from 8h to 16h ******* //
	for (count=8; count <=15; count++){

		//defining minimum values to met the house requirements, considering demand, output voltage of VoutAC from inverter
		//at the inverter:
		demand = demand_temp[count];
		PinDCmin = demand/ ni; //(PoutAC = efficiency*PinputDC)
		//at the charge controller output (Vdc is 12*NBS and Idc must be greater than PinDC/Vdc)
		Idcmin = PinDCmin / (Vbat*NBS);

		drainedCurrent = demand / VoutAC;  

		//Non-deterministic variable AMBtemperature (ambient temperature: in Manaus from 23 to 32 degree Celsius)
		AMBtemperature = nondet_uint(); //ambient temperature
		__VERIFIER_assume(AMBtemperature>=23 && AMBtemperature<=32);
	
		//Non-deterministic variable: from 0h to 7:58h is 0 ; 
		//from 8h to 15:59h varies from 274 to 852 W/m2 ; 
		// from  16h to 23:59 is 0
		G = nondet_uint(); //solar irradiance from SUN in W/m2
		__VERIFIER_assume(G>=274 && G<=852);
	
		// HERE THE CODE FROM PV PANELS MODEL
		pvmodel (G, AMBtemperature);
		// PV panels output are: Varray, Iarray, and Parray. All global variables
	
		//
		//Algorithm of charge controller: decides if the load can be feed by the batteries, by the PV panels or is disconnected
		if (SOC <= SOClimit) {//battery empty (<=75% of SOC)
			assert (Varray >= Vbulk); //if not, the battery is empty and the PV panels are not generating power ***FAIL***
			//controller output is enough to charge the battery and to feed the inverter
			Vdc=Vbulk;
			Idc=Idcmin;
			//charge of battery for 1h
			SOC = SOC + 100*1*Parray/(Vbulk*1.15*bankcapacity);
			__VERIFIER_assume(SOC>=0 && SOC<=100);
		}
		else if (SOC <= SOCabsorption) { //battery is in absortion (<95) and needs bulk stage of charging, with 14.40V
			if (Varray >= Vbulk){ //PV panel has enough power to charge the battery AND to supply energy to the load
				Vdc=Vbulk;
				Idc=Idcmin; //?Iarray or Idcmin?
				//charge of battery for 1h
				SOC = SOC + 100*1*Parray/(Vbulk*1.15*bankcapacity);
				__VERIFIER_assume(SOC>=0 && SOC<=100);
			}
			else { //PV panel has NOT enough power but the battery can supply the energy to the load
				//here battery is not charging and feed the load
				Vdc=Vbat*NBS;
				Idc=Idcmin;
				//discharge battery in 1 hour
				tempf = 100*drainedCurrent*1/bankcapacity;
        			__VERIFIER_assume(tempf>=0 && tempf<=100);
				SOC = SOC - (int)tempf; 
 				__VERIFIER_assume(SOC>=0 && SOC<=100);
				assert(SOC>=SOClimit && SOC<=100); 
			}
		}
		else { //battery enter in float stage (SOC > 95%) and needs to continue the charge with 13.20V OR Battery is full
			
			if ((Varray >= Vfloat) && (SOC < SOCfull)){ 	//PV panel has enough power to charge the battery AND 
									//to supply energy to the load
				Vdc=Vfloat;
				Idc=Idcmin;  //?Iarray or Idcmin?
				//charge of battery for 1h
				SOC = SOC + 100*1*Parray/(Vfloat*1.15*bankcapacity);
				__VERIFIER_assume(SOC>=0 && SOC<=100);
			}
			else { //PV panel has NOT enough power but the battery can supply the energy to the load
				//here battery is not charging and feed the load
				Vdc=Vbat*NBS;
				Idc=Idcmin;
				//discharge battery in 1 hour
				tempf = 100*drainedCurrent*1/bankcapacity;
        			__VERIFIER_assume(tempf>=0 && tempf<=100);
				SOC = SOC - (int)tempf;
 				__VERIFIER_assume(SOC>=0 && SOC<=100); 
				assert(SOC>=SOClimit && SOC<=100); 
			}
	
		}
		//SOC=SOC;
		//with Idc and Vdc defined here, is just calculus to check if the community requeriment will be met by the system
		assert ((Vdc*Idc) >= PinDCmin);
	
	}

	//****2- here the next hours without PV generation (not VERIFICATION, just CHECK  by code), from 16:00h to 7:00h
	//always decreasing the SOC 
	assert(SOC>=SOClimit && SOC<=100);
	//step 2.1: from 16h to 24h
	if (auton <=8) countlimit = auton+15;  //means 16:00h + battery autonomy
	else countlimit = 23;   //means go until 24:00h
	for (count=16; count <= countlimit; count++){
		demand = demand_temp[count];
		drainedCurrent = demand / VoutAC; 

		tempf = 100*drainedCurrent*6/bankcapacity;
        	__VERIFIER_assume(tempf>=0 && tempf<=100);
		SOC = SOC - (int)tempf; 
		__VERIFIER_assume(SOC>=0 && SOC<=100);
		assert(SOC>=SOClimit && SOC<=100); 
	}
	//step 2.2: from 24:00h to beyond (limited to 7:00h)
	if (auton >= 9) {
		if (auton <=16) countlimit = auton-8-1;  //means 16h + battery autonomy ?????????????
		else countlimit = 7;   //means go until 7:00h
		for (count=0; count <= countlimit; count++){
			demand = demand_temp[count];
			drainedCurrent = demand / VoutAC; 

			tempf = 100*drainedCurrent*6/bankcapacity;
        		__VERIFIER_assume(tempf>=0 && tempf<=100);
			SOC = SOC - (int)tempf; 
			__VERIFIER_assume(SOC>=0 && SOC<=100);
			assert(SOC>=SOClimit && SOC<=100); 
		}
	}
	//step 3: From 8:00h to 16:00h, now with PV generation
	if (auton >= 17) {
////////////////////////////////

	for (count=8; count <=15; count++){

		//defining minimum values to met the house requirements, considering demand, output voltage of VoutAC from inverter
		//at the inverter:
		demand = demand_temp[count];
		PinDCmin = demand / ni; //(PoutAC = efficiency*PinputDC)
		//at the charge controller output (Vdc is 12*NBS and Idc must be greater than PinDC/Vdc)
		Idcmin = PinDCmin / (Vbat*NBS);

		drainedCurrent = demand / VoutAC; 

		//Non-deterministic variable AMBtemperature (ambient temperature: in Manaus from 23 to 32 degree Celsius)
		AMBtemperature = nondet_uint(); //ambient temperature
		__VERIFIER_assume(AMBtemperature>=23 && AMBtemperature<=32);
	
		//Non-deterministic variable: from 0h to 7:58h is 0 ; 
		//from 8h to 15:59h varies from 274 to 852 W/m2 ; 
		// from  16h to 23:59 is 0
		G = nondet_uint(); //solar irradiance from SUN in W/m2
		__VERIFIER_assume(G>=274 && G<=852);
	
		// HERE THE CODE FROM PV PANELS MODEL
		pvmodel (G, AMBtemperature);
		// PV panels output are: Varray, Iarray, and Parray. All global variables
	
		//
		//Algorithm of charge controller: decides if the load can be feed by the batteries, by the PV panels or is disconnected
		if (SOC <= SOClimit) {//battery empty (<=75% of SOC)
			assert (Varray >= Vbulk); //if not, the battery is empty and the PV panels are not generating power ***FAIL***
			//controller output is enough to charge the battery and to feed the inverter
			Vdc=Vbulk;
			Idc=Idcmin;
			//charge of battery for 1h
			SOC = SOC + 100*1*Parray/(Vbulk*1.15*bankcapacity);
		}
		else if (SOC <= SOCabsorption) { //battery is in absortion (<95) and needs bulk stage of charging, with 14.40V
			if (Varray >= Vbulk){ //PV panel has enough power to charge the battery AND to supply energy to the load
				Vdc=Vbulk;
				Idc=Idcmin; //?Iarray or Idcmin?
				//charge of battery for 1h
				SOC = SOC + 100*1*Parray/(Vbulk*1.15*bankcapacity);
	 			__VERIFIER_assume(SOC>=0 && SOC<=100);
			}
			else { //PV panel has NOT enough power but the battery can supply the energy to the load
				//here battery is not charging and feed the load
				Vdc=Vbat*NBS;
				Idc=Idcmin;
				//discharge battery in 1 hour
				tempf = 100*drainedCurrent*1/bankcapacity;
        			__VERIFIER_assume(tempf>=0 && tempf<=100);
				SOC = SOC - (int)tempf; 
 				__VERIFIER_assume(SOC>=0 && SOC<=100);
				assert(SOC>=SOClimit && SOC<=100); 
			}
		}
		else { //battery enter in float stage (SOC > 95%) and needs to continue the charge with 13.20V OR Battery is full
			
			if ((Varray >= Vfloat) && (SOC < SOCfull)){ 	//PV panel has enough power to charge the battery AND 
									//to supply energy to the load
				Vdc=Vfloat;
				Idc=Idcmin;  //?Iarray or Idcmin?
				//charge of battery for 1h
				SOC = SOC + 100*1*Parray/(Vfloat*1.15*bankcapacity);
 				__VERIFIER_assume(SOC>=0 && SOC<=100);
			}
			else { //PV panel has NOT enough power but the battery can supply the energy to the load
				//here battery is not charging and feed the load
				Vdc=Vbat*NBS;
				Idc=Idcmin;
				//discharge battery in 1 hour
				tempf = 100*drainedCurrent*1/bankcapacity;
        			__VERIFIER_assume(tempf>=0 && tempf<=100);
				SOC = SOC - (int)tempf; 
 				__VERIFIER_assume(SOC>=0 && SOC<=100);
				assert(SOC>=SOClimit && SOC<=100); 
			}
	
		}
		//SOC=SOC;
		//with Idc and Vdc defined here, is just calculus to check if the community requeriment will be met by the system
		assert ((Vdc*Idc) >= PinDCmin);
	
	}

///////////////////////////////
	}
	assert(SOC>=SOClimit && SOC<=100);
	//from 16:00h to 7:00h
	if (auton >= 25) {
		for (count=16; count <= 23; count++){
			demand = demand_temp[count];
			drainedCurrent = demand / VoutAC; 

			tempf = 100*drainedCurrent*6/bankcapacity;
        		__VERIFIER_assume(tempf>=0 && tempf<=100);
			SOC = SOC - (int)tempf;
			__VERIFIER_assume(SOC>=0 && SOC<=100); 
			assert(SOC>=SOClimit && SOC<=100); 
		}
		for (count=0; count <= 7; count++){
			demand = demand_temp[count];
			drainedCurrent = demand / VoutAC; 

			tempf = 100*drainedCurrent*6/bankcapacity;
        		__VERIFIER_assume(tempf>=0 && tempf<=100);
			SOC = SOC - (int)tempf;
			__VERIFIER_assume(SOC>=0 && SOC<=100); 
			assert(SOC>=SOClimit && SOC<=100); 
		}
	}
//**** HERE END THE AUTOMATED VERIFICATION ********
}



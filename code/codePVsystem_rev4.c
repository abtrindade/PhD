/* --------------------------------------------------------------------------------------------------------
  Here is the code that describes the whole PV system, focused at the charge controller and decides if the 
  load can be met by the battery, PV panel or if is desconnected (system-off)
------------------------------------------------------------------------by Alessandro Trindade, July 2018 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//IMPORTANT: to declare precision here to LOG of natural base and EXP
#define precisionLN 0.0001 //0.0000000001
#define precisionEXP 0.0001 //0.00000001 

//requirements/restrictions
//battery of 220 Ah capacity, and minimum SOC (before desconnect load) 80% (i.e., 20% of DOD) as usual to lead-acid batteries
#define NBS 2
#define NBP 2
#define capacity 220*NBP
#define SOClimit 75
#define SOCabsorption 95
#define SOCfull 100
#define Vbat 12
#define Vbulk 14.40*NBS
#define Vfloat 13.20*NBS
//PV system autonomy concerning batteries: 48h
#define auton 48
//power demand in Watt from the house served by the PV system (maximum is 700W considering Star Energy project): 510 W
//case studies concerning the houses at Nova Esperanca (Star Energy Project): 510, 253, 263, and 283W. But with the same PV system (700W)
#ifdef HOUSE1 
#define Phouse 510
#elif HOUSE2
#define Phouse 253
#elif HOUSE3
#define Phouse 263
#elif HOUSE4
#define Phouse 283
#endif
//inverter data (outlet voltage, efficiency)
#define VoutAC 110
#define ni 0.93 

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
	Iarray = Ncellsparallel*Imax; //current going out from the solar panels (DC current)
	Varray = Ncellsseries*Vmax; //voltage going out from the solar penels (DC voltage)
	Parray = Iarray*Varray; //output power 
}


//main code
int main(){
     	int count, SOC, AMBtemperature, G, RemCap, drainedCurrent;
	float PinDCmin, Idcmin, Idc, Vdc;

	//defining minimum values to met the house requirements, considering demand of Phouse, output voltage of VoutAC from inverter
	//at the inverter:
	PinDCmin = Phouse / ni; //(PoutAC = efficiency*PinputDC)

	//at the charge controller output (Vdc is 12*NBS and Idc must be greater than PinDC/Vdc)
	Idcmin = PinDCmin / (Vbat*NBS);

	//
	//HERE START THE PROCESS OF SWEEP A 48h-PERIOD TO CHECK VIOLATION RELATED THE AUTONOMY OF BATTERIES
	//
	//Considering the start at 16h and going ahead for 48h, i.e., finishing at 15:59h of day D+2
	//
	//(WORST CASE SCENARIO)from 16h to 7:59h of D and D+1 there is no charging of batteries, just discharging 

	//****1- Defining the initial value of SOC and doing the first check for the next 16h
	//SOC for the next 16h, when the G is low and there is no photovoltaic generation
	// The power demand is Phouse define at the beginning in a flat demand

	// random variable SOC (SOC is a number from 0 to 1: 0 means battery is empty: 1 means battery full)
	// At this code, as a percentage from 0 to 100 (100 means fully charged)
	//SOC = nondet_uint()%100; 
	SOC = 95;

	assert (SOC >= SOClimit);

	//calculus of remaining capacity from the battery, corrected in terms of charge controller setup of SOC limit
	RemCap = (SOC - SOClimit)*capacity/100; 

	// considering that the Power demand is predefined (Phouse) and the current demanded from the controller can be calculated
	drainedCurrent = Phouse / VoutAC; 

	//Therefore the amount of hours without discharge the battery is RemCap/drainedCurrent and must be greater than 16h 
	assert (RemCap/drainedCurrent >= 16);   //if is not >= 16h, then the remaining power supply from battery won't enough (FAIL)

	SOC = SOC - drainedCurrent*16; //decreasing the SOC in 16 hours 


	//****2 - the next 8h, with solar PV generation
	// START HERE AUTOMATED VERIFICATION from 8h to 16h ******* //
	for (count=1; count <=8; count++){
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
			SOC = SOC + 1*Parray/(Vbulk*1.15);
		}
		else if (SOC <= SOCabsorption) { //battery is in absortion (<95) and needs bulk stage of charging, with 14.40V
			if (Varray >= Vbulk){ //PV panel has enough power to charge the battery AND to supply energy to the load
				Vdc=Vbulk;
				Idc=Idcmin; //?Iarray or Idcmin?
				//charge of battery for 1h
				SOC = SOC + 1*Parray/(Vbulk*1.15);
			}
			else { //PV panel hs NOT enough power but the battery can supply the energy to the load
				//here battery is not charging and feed the load
				Vdc=Vbat*NBS;
				Idc=Idcmin;
				//discharge battery in 1 hour
				SOC = SOC - drainedCurrent*1;
			}
		}
		else { //battery enter in float stage (SOC > 95%) and needs to continue the charge with 13.20V OR Battery is full
			
			if ((Varray >= Vfloat) && (SOC < SOCfull)){ 	//PV panel has enough power to charge the battery AND 
									//to supply energy to the load
				Vdc=Vfloat;
				Idc=Idcmin;  //?Iarray or Idcmin?
				//charge of battery for 1h
				SOC = SOC + 1*Parray/(Vbulk*1.15);
			}
			else { //PV panel has NOT enough power but the battery can supply the energy to the load
				//here battery is not charging and feed the load
				Vdc=Vbat*NBS;
				Idc=Idcmin;
				//discharge battery in 1 hour
				SOC = SOC - drainedCurrent*1;
			}
	
		}
		SOC=SOC;
		//with Idc and Vdc defined here, is just calculus to check if the community requeriment will be met by the system
		assert ((Vdc*Idc) >= PinDCmin);
	
	}

	//****3- here the next 16h without PV generation (not VERIFICATION, just CHECK  by code)
	//calculus of remaining capacity from the battery, corrected in terms of charge controller setup of SOC limit
	RemCap = (SOC - SOClimit)*capacity/100; 

	// considering that the demand is predefined and the current demanded can be calculated I=power/127V
	drainedCurrent = Phouse/VoutAC;

	//Therefore the amount of hours without discharge the battery is RemCap/drainedCurrent and must be greater than 16h 
	assert (RemCap/drainedCurrent >= 16);   //if is not >= 16h, then the remaining power supply from battery won't enough (FAIL)
	
	//SOC decreasing
	SOC = SOC - drainedCurrent*16; //decreasing the SOC in 16 hours 

	//****4 - here the last 8h with PV generation, doing automated verification
	// START HERE AUTOMATED VERIFICATION from 8h to 16h ******* //
	for (count=1; count <=8; count++){
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
			SOC = SOC + 1*Parray/(Vbulk*1.15);
		}
		else if (SOC <= SOCabsorption) { //battery is in absortion (<95) and needs bulk stage of charging, with 14.40V
			if (Varray >= Vbulk){ //PV panel has enough power to charge the battery AND to supply energy to the load
				Vdc=Vbulk;
				Idc=Idcmin; //?Iarray or Idcmin?
				//charge of battery for 1h
				SOC = SOC + 1*Parray/(Vbulk*1.15);
			}
			else { //PV panel hs NOT enough power but the battery can supply the energy to the load
				//here battery is not charging and feed the load
				Vdc=Vbat*NBS;
				Idc=Idcmin;
				//discharge battery in 1 hour
				SOC = SOC - drainedCurrent*1;
			}
		}
		else { //battery enter in float stage (SOC > 95%) and needs to continue the charge with 13.20V OR Battery is full
			
			if ((Varray >= Vfloat) && (SOC < SOCfull)){ 	//PV panel has enough power to charge the battery AND 
									//to supply energy to the load
				Vdc=Vfloat;
				Idc=Idcmin;  //?Iarray or Idcmin?
				//charge of battery for 1h
				SOC = SOC + 1*Parray/(Vbulk*1.15);
			}
			else { //PV panel has NOT enough power but the battery can supply the energy to the load
				//here battery is not charging and feed the load
				Vdc=Vbat*NBS;
				Idc=Idcmin;
				//discharge battery in 1 hour
				SOC = SOC - drainedCurrent*1;
			}
	
		}
		SOC=SOC;
		//with Idc and Vdc defined here, is just calculus to check if the community requeriment will be met by the system
		assert ((Vdc*Idc) >= PinDCmin);
	
	}

//**** HERE END THE 4-PHASES OF AUTOMATED VERIFICATION ********
}






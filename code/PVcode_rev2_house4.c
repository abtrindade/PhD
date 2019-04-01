/*
 ============================================================================
 Name        : PVcode.c
 Author      : Alessandro
 Version     : rev2 - April 2019
 Description : New PV code (Universal: CBMC, ESBMC, UAutomizer and CPChecker)
 //assume (exp);  MUST BE REPLACE BY:   if (!expr) { __VERIFIER_error(); }
 ============================================================================*/

//case studies
//HOUSE1 
// int loadcurve[24] = {118,118,118,46,46,46,95,95,170,170,296,242,242,95,95,95,95,95,342,288,288,288,288,118};
// int Phouse = 501, Psurge = 501, Econsumption = 3900;
// int PACref = 700, MAXACref = 1600, NBS = 2, NBP = 2, capacity = 220, autonomy = 48, NPP = 1, NPS = 3, VC = 24, Vsystem = 24;
// int bankcapacity = 440; //(capacity*NBP);
// int SOClimit = 75, SOCabsorption = 95, SOCfull = 100, Vbat = 12;
// float Vbulk = 28.80; //(14.40*NBS);
// float Vfloat = 26.40; //(13.20*NBS);
// int nb = 1;
// float ni = 0.93;
// int VoutAC = 110, VinDC = 24, VAC = 127;
//HOUSE2 
// int loadcurve[24] = {136, 136, 136, 136, 136, 136, 67, 67, 184, 184, 184, 184, 184, 67, 67, 67, 67, 67, 253, 253, 253, 253, 253, 136};
// int Phouse = 253, Psurge = 722, Econsumption = 3600;
// int PACref = 700, MAXACref = 1600, NBS = 2, NBP = 2, capacity = 220, autonomy = 48, NPP = 1, NPS = 3, VC = 24, Vsystem = 24;
// int bankcapacity = 440; //(capacity*NBP);
// int SOClimit = 75, SOCabsorption = 95, SOCfull = 100, Vbat = 12;
// float Vbulk = 28.80; //(14.40*NBS);
// float Vfloat = 26.40; //(13.20*NBS);
// int nb = 1;
// float ni = 0.93;
// int VoutAC = 110, VinDC = 24, VAC = 127;
//HOUSE3 
// int loadcurve[24] = {113,113,113,113,113,113,67,67,217,97,97,97,97,97,97,97,97,97,263,113,113,113,113,113};
// int Phouse = 263, Psurge = 732, Econsumption = 2500;
// int PACref = 700, MAXACref = 1600, NBS = 2, NBP = 2, capacity = 220, autonomy = 48, NPP = 1, NPS = 3, VC = 24, Vsystem = 24;
// int bankcapacity = 440; //(capacity*NBP);
// int SOClimit = 75, SOCabsorption = 95, SOCfull = 100, Vbat = 12;
// float Vbulk = 28.80; //(14.40*NBS);
// float Vfloat = 26.40; //(13.20*NBS);
// int nb = 1;
// float ni = 0.93;
// int VoutAC = 110, VinDC = 24, VAC = 127;
//HOUSE4 
// int loadcurve[24] = {207,207,207,135,135,135,66,66,161,161,233,253,248,66,66,66,66,66,302,317,322,302,302,207};
// int Phouse = 322, Psurge = 896, Econsumption = 4300;
// int PACref = 700, MAXACref = 1600, NBS = 2, NBP = 2, capacity = 220, autonomy = 48, NPP = 1, NPS = 3, VC = 24, Vsystem = 24;
// int bankcapacity = 440; //(capacity*NBP);
// int SOClimit = 75, SOCabsorption = 95, SOCfull = 100, Vbat = 12;
// float Vbulk = 28.80; //(14.40*NBS);
// float Vfloat = 26.40; //(13.20*NBS);
// int nb = 1;
// float ni = 0.93;
// int VoutAC = 110, VinDC = 24, VAC = 127;
//HOUSE5 
// int loadcurve[24] = {45,16,16,16,16,16,0,0,0,72,72,222,150,150,0,0,72,72,814,814,814,742,742,16};
// int Phouse = 915, Psurge = 980, Econsumption = 4880;
// int PACref = 1200, MAXACref = 2400, NBS = 4, NBP = 1, capacity = 120, autonomy = 6, NPP = 2, NPS = 2, VC = 48, Vsystem = 48;
// int bankcapacity = 120; //(capacity*NBP);
// int SOClimit = 75, SOCabsorption = 95, SOCfull = 100, Vbat = 12;
// float Vbulk = 57.60; //(14.40*NBS);
// float Vfloat = 52.80; //(13.20*NBS);
// int nb = 1;
// float ni = 0.93;
// int VoutAC = 110, VinDC = 48, VAC = 127;

//data from Manaus
int minSolarIrrad [24] = {0,0,0,0,0,0,25,135,274,422,509,537,503,505,430,281,80,10,0,0,0,0,0,0};
int maxSolarIrrad [24] = {0,0,0,0,0,4,87,295,487,648,751,852,817,742,610,418,128,51,0,0,0,0,0,0};
int minTemp [12] = {23,23,23,23,23,23,23,23,23,24,24,23};
int maxTemp [12] = {30,30,30,30,30,30,30,31,32,32,31,30};


// CASE DESCRIPTION BELOW
int loadcurve[24] = {207,207,207,135,135,135,66,66,161,161,233,253,248,66,66,66,66,66,302,317,322,302,302,207};
int Phouse = 322, Psurge = 896, Econsumption = 4300;
int PACref = 700, MAXACref = 1600, NBS = 2, NBP = 2, capacity = 220, autonomy = 48, NPP = 1, NPS = 3, VC = 24, Vsystem = 24;
int bankcapacity = 440; //(capacity*NBP);
int SOClimit = 75, SOCabsorption = 95, SOCfull = 100, Vbat = 12;
float Vbulk = 28.80; //(14.40*NBS);
float Vfloat = 26.40; //(13.20*NBS);
int nb = 1;
float ni = 0.93;
int VoutAC = 110, VinDC = 24, VAC = 127;

//data sheet information from the PV model Canadian 325W CS6U-325P
int N = 72;    //N=number of series-connected cells
int NOCT = 45; //Nominal Operating Cell Temperature=45°C +-2 at G=800 W/m2, light spectrum AM 1.5G, windspeed=1m/s, air temp at 20°C
int Gref = 1000; //reference irradiance is 1000 W/m²
float Tref = 298.15; //reference temperature is 25oC or 298.15K
float mii = 0.053; //short-circuit current temperature coefficient (A/K)
float miv = -0.31; //open-circuit voltage temperature coefficient (V/K)
float Iscref = 9.34;
float Vocref = 45.5;
float Imref = 8.78;
float Vmref = 37.0;
float Pmref = 325.0;
float Vmpptmin = 33.7;
float Imp = 6.98;
float np = 0.1672;
float Insol = 4.92; //(kWh/m2/day CRESESB 2016)
float Ap = 1.94432; //(panel area in m2, 1.96x0.992)
float Tcell = 45.0;  //NOCT

//controller
float nc = 0.98;
int IC = 35;
float VCmax = 145.0;

//global variables
float Parray, Iarray, Varray;
double precisionLN = 0.0001;
double precisionEXP = 0.0001;
float Vdc,Idc, Idcmin, PinDCmin;
int SOC = 100; //battery is charged when starts the verification


/* start: C compiler */
//unsigned int nondet_uint() {
//	unsigned int x;
//	return x;
//}

//void __VERIFIER_error() {
//	printf("reached");
//}

//void __VERIFIER_assume(int i){
//	if(i){}else{printf("wrong");exit(1);};
//}
 /* start Automated Verification version */

extern void __VERIFIER_error() __attribute__ ((__noreturn__));
unsigned int nondet_uint();

//function that analyze data from solar irradiance and discovers which hour starts PV generation
int findStartPVGeneration(void){
	int cont;
	int hour=0;
	for (cont=0; cont <= 23; cont=cont+1){
		if ((minSolarIrrad [cont] == 0) && (maxSolarIrrad [cont] == 0)) hour = cont;
		if ((minSolarIrrad [cont] > 0) && (maxSolarIrrad [cont] > 0)) cont=24;
	}
	return (hour+1);
}

//function that analyze data from solar irradiance and discovers which hour ends PV generation
int findEndPVGeneration(void){
	int cont;
	int hour=23;
	for (cont=23; cont >= 0; cont=cont-1){
		if ((minSolarIrrad [cont] == 0) && (maxSolarIrrad [cont] == 0)) hour = cont;
		if ((minSolarIrrad [cont] > 0) && (maxSolarIrrad [cont] > 0)) cont=0;
	}
	return (hour-1);
}

void dischargeBattery(int hday){
	int demand, drainedCurrent;
	float tempf;
	//defining minimum values to met the house requirements, considering demand, output voltage of VoutAC from inverter
	//at the inverter:
	demand = loadcurve[hday];
	PinDCmin = demand/ ni; //(PoutAC = efficiency*PinputDC)
	//at the charge controller output (Vdc is 12*NBS and Idc must be greater than PinDC/Vdc)
	Idcmin = PinDCmin / (Vbat*NBS);

	drainedCurrent = demand / VoutAC;

	Vdc=Vbat*NBS;
	Idc=Idcmin;
	//discharge battery in 1 hour
	tempf = 100*drainedCurrent*1/bankcapacity;
		__VERIFIER_assume(tempf>=0 && tempf<=100);
		__VERIFIER_assume(SOC>=0 && SOC<=100);
	SOC = SOC - (int)tempf;
		if ( !(SOC>=SOClimit && SOC<=100)) { __VERIFIER_error(); }

}

void chargeBattery (void){
	//charge of battery for 1h
	SOC = SOC + 100*1*Parray/(Vbulk*1.15*bankcapacity);

}

//the functions to calculate the log (in substitution of log from language C)
//code log1 if number is greater than 1, source https://stackoverflow.com/questions/3343395/value-of-natural-logarithm
double logFUNC1 (double x){
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
double logFUNC2 (double d){
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

//the main funcion of logarithm of natural base. It calls logFUNC1 or FUNC2.
//The natural logarithm function ln(x) is defined only for x>0. So the natural logarithm of a negative number is undefined
double logFUNC (double x){
	double answer;
	if (x > 1.0) answer = logFUNC1(x);
	else if (x>0.0) answer=logFUNC2(x);
	else answer = 1;
	return answer;
}

//function that calculates the exponential (in substitution of EXP from language C)
//from https://codereview.stackexchange.com/questions/123970/implementation-of-exp-function-in-c-using-taylor-series-expansion
//but with improvement in the while, to solve negative numbers
double expFUNC (double num){
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

void PVgeneration (int, int);


//function that generates the MPPT values (Varray and Iarray) from the PV panel,
// considering values of solar irradiance(G) and ambient temperature
void PVgeneration (int Gf, int AMBtemperaturef){
	float Iphref, T, Tk, VT, a, Io, Iph, I, V, Vmax, Imax, Pmax;
	float kbq, qkb;
	Iphref = Iscref; //mathematical approximation from the model
	//set up of parameters
//	kb = 1.3806503*pow(10,-23);  //Boltzmann constant in Joule
//	q = 1.60217646*pow(10,-19);  //absolute value of the electron's charge
	kbq = 0.00008617342311;
	qkb = 11604.5059346;
	T= AMBtemperaturef + Gf*(NOCT-20)/800; //temperature of the p-n junction or cell temperature

	Tk = T + 273.15; //is the temperaute T in Kelvin
//	VT = kb*Tk/q;
	VT = kbq*Tk;

	//a ideality/quality factor
//	a = q*(Vmref-Vocref)/(N*kb*Tk*logFUNC(1-Imref/Iscref));
	a = qkb*(Vmref-Vocref)/(N*Tk*logFUNC(1-Imref/Iscref));

	//Io = reverse saturation current of the diode
//	Io = (Iscref+mii*(Tk-Tref))/(expFUNC (q*(Vocref+miv*(Tk-Tref))/(a*N*kb*Tk)) - 1);
	Io = (Iscref+mii*(Tk-Tref))/(expFUNC (qkb*(Vocref+miv*(Tk-Tref))/(a*N*Tk)) - 1);

	//Iph = photocurrent delivered from the model
	Iph = Gf * (Iphref+ mii*(Tk-Tref))/Gref;

	Pmax=0;
	//sweep the V-I graph to find the MPPT
	for (V=0; V<= Vocref; V=V+0.1){
		//PV panel model: 1-diode model
		I = Iph - Io*(expFUNC(V/(N*a*VT)) - 1);
		__VERIFIER_assume ( (I >=0) && (I<=Iscref) );	////calculated current cannot be greater than Iscref or negative
		if ((V*I) > Pmax){
			Imax=I;
			Vmax=V;
			Pmax = Imax*Vmax;
		}
	}

	/* correcting to an array configuration */
	Iarray = NPP*Imax; //current going out from the solar panels (DC current)
	Varray = NPS*Vmax; //voltage going out from the solar penels (DC voltage)
	Parray = Iarray*Varray; //output power
}

void sizing_check(void){
	char NTP;
	float Ecorrected, Ep, NTPmin, NPSmin1, NPSmin2, NPPmin, Cbank, NBtotal, ICmin, Iscamb, NBSmin, NBPmin;

	//Econsumption = energy consumed in one day from the house
	Ecorrected = Econsumption / (nb*nc*ni);

	//estimation of the energy of each panel (Wh)
	Ep = Insol * Ap * np * 1000;

	//PV panel check
	NTPmin = Ecorrected/Ep;
	NTP = NPS * NPP;
	if( ! ( NTP >= NTPmin ) ){ __VERIFIER_error();}

	NPSmin1 = (Vsystem + 1) / (float)Vmref;
	NPSmin2 = VCmax / Vmpptmin;

	NPPmin = Phouse / (NPS * Pmref);

	if( ! (( NPS >= NPSmin1 ) && (NPS <= NPSmin2)) ) { __VERIFIER_error();}
	if( ! ( NPP >= NPPmin ) ){ __VERIFIER_error();}

	//battery check
	Cbank = Ecorrected * autonomy / (Vsystem*(float)(100-SOClimit));
	NBSmin = Vsystem/Vbat;
	if( ! ( NBS >= NBSmin ) ){ __VERIFIER_error(); }
	NBPmin = Cbank / capacity;
	if( ! ( NBP >= NBPmin ) ){ __VERIFIER_error(); }
	NBtotal = NBSmin * NBPmin;
	if( ! ( (NBS * NBP) >= NBtotal ) ){ __VERIFIER_error(); }

	//controller check, considering just one controller
	if( ! ( VC == Vsystem ) ){ __VERIFIER_error(); }

	Iscamb = Iscref*(1+mii*(Tcell-25)); //from each PV panel
	ICmin = Iscamb * NPP;
	if( ! ( IC >= ICmin ) ){ __VERIFIER_error(); }

	//inverter check
	if( ! ( VinDC == Vsystem ) ){ __VERIFIER_error(); }
	if( ! ( (VoutAC >= (VAC-VAC*15/100)) && (VoutAC <= (VAC+VAC*15/100)) ) ){ __VERIFIER_error(); }

	if( ! ( (Phouse <= PACref) && (Psurge <= MAXACref) ) ){ __VERIFIER_error(); }
}

/*--------------------------------
            main function
----------------------------------*/
int main (void){
	int h, autonomyCount=0, hcount, AMBtemperature, G,loop24h, loopcontrol;
	sizing_check();
	int StartPVGeneration = findStartPVGeneration();
	int EndPVGeneration = findEndPVGeneration();

	//calculating the number of times that the first 24h for-loop will be performed
	if (autonomy <= 24) loopcontrol = 1;
	if ( (autonomy > 24) && (autonomy <=48) ) loopcontrol =2;
	if ( (autonomy > 48) && (autonomy <=72) ) loopcontrol =3;
	if ( (autonomy > 72) && (autonomy <=96) ) loopcontrol =4;


	//start of 24h loop
	for (loop24h=1;loop24h <= loopcontrol;loop24h=loop24h+1) {
		//start with the time just after there is not PV generation
		h=EndPVGeneration+1; //adjust hour to the 1st hour without PV generation
		for (hcount=1; hcount<=(StartPVGeneration + 24-EndPVGeneration-1); hcount=hcount+1){
				if (h==24) h=0;
				dischargeBattery(h); //discharge battery according demand ins a specific time of the day
				autonomyCount = autonomyCount+1;
				if (autonomyCount == autonomy) return 0; //finished without violate any property
				if ( !(SOC >= 75)) { __VERIFIER_error(); }
				h=h+1;
			}

			//PV generation (a.k.a., solar irradiaton) starts below

			if (autonomy >= 24 ) { //autonomy autonomy is higher than 24h, so the PV generation time is complete
				for (hcount=1;hcount <= (EndPVGeneration-StartPVGeneration+1);hcount=hcount+1){
						//Non-deterministic variable AMBtemperature (ambient temperature according average min and max in degree Celsius)
						AMBtemperature = nondet_uint(); //ambient temperature
						__VERIFIER_assume( (AMBtemperature >= minTemp[h]) && (AMBtemperature<=maxTemp[h]) );
						G = nondet_uint(); //solar irradiance from SUN in W/m2
						__VERIFIER_assume( (G >= (minSolarIrrad[h])) && (G<=(maxSolarIrrad[h])) );
						PVgeneration(G,AMBtemperature);
						if (SOC <= SOClimit) {//battery empty (<=75% of SOC)
							if( ! ( Varray >= Vbulk ) ){ __VERIFIER_error(); }
							//controller output is enough to charge the battery and to feed the inverter
							Vdc=Vbulk;
							Idc=Idcmin;
							chargeBattery();
						}
						else if (SOC <= SOCabsorption) { //battery is in absortion (<95) and needs bulk stage of charging, with 14.40V
							if (Varray >= Vbulk){ //PV panel has enough power to charge the battery AND to supply energy to the load
								Vdc=Vbulk;
								Idc=Idcmin;
								chargeBattery();
							}
							else { //PV panel has NOT enough power but the battery can supply the energy to the load
								//here battery is not charging and feed the load
								Vdc=Vbat*NBS;
								Idc=Idcmin;
								dischargeBattery(h);
							}
						}
						else { //battery enter in float stage (SOC > 95%) and needs to continue the charge with 13.20V OR Battery is full

							if ((Varray >= Vfloat) && (SOC < SOCfull)){ 	//PV panel has enough power to charge the battery AND
													//to supply energy to the load
								Vdc=Vfloat;
								Idc=Idcmin;
								chargeBattery();
							}
							else { //PV panel has NOT enough power but the battery can supply the energy to the load
								//here battery is not charging and feed the load
								Vdc=Vbat*NBS;
								Idc=Idcmin;
								dischargeBattery(h);
							}
						}
						//with Idc and Vdc defined here, is just calculus to check if the community requeriment will be met by the system
						if( ! ( (Vdc*Idc) >= PinDCmin ) ){ __VERIFIER_error(); }
						autonomyCount = autonomyCount+1;
						if (autonomyCount == autonomy) return 0; //finished without violate any property
						if ( !(SOC >= 75) ) { __VERIFIER_error(); }
						h=h+1;
					}
			}
			else { //loop if autonomy is lower than 24h
				if (autonomy >= autonomyCount){ //at this case, the system has some PV generation yet
					for (hcount=1;hcount <= (autonomy - autonomyCount);hcount = hcount+1){
						//Non-deterministic variable AMBtemperature (ambient temperature according average min and max in degree Celsius)
								AMBtemperature = nondet_uint(); //ambient temperature
								__VERIFIER_assume( (AMBtemperature >= minTemp[h]) && (AMBtemperature<=maxTemp[h]) );

								G = nondet_uint(); //solar irradiance from SUN in W/m2
								__VERIFIER_assume ( (G>=(minSolarIrrad[h])) && (G<=(maxSolarIrrad[h])) );
								PVgeneration(G,AMBtemperature);

								if (SOC <= SOClimit) {//battery empty (<=75% of SOC)
									if( ! ( Varray >= Vbulk ) ){ __VERIFIER_error(); }
									//controller output is enough to charge the battery and to feed the inverter
									Vdc=Vbulk;
									Idc=Idcmin;
									chargeBattery();
								}
								else if (SOC <= SOCabsorption) { //battery is in absortion (<95) and needs bulk stage of charging, with 14.40V
									if (Varray >= Vbulk){ //PV panel has enough power to charge the battery AND to supply energy to the load
										Vdc=Vbulk;
										Idc=Idcmin;
										chargeBattery();
									}
									else { //PV panel has NOT enough power but the battery can supply the energy to the load
										//here battery is not charging and feed the load
										Vdc=Vbat*NBS;
										Idc=Idcmin;
										dischargeBattery(h);
									}
								}
								else { //battery enter in float stage (SOC > 95%) and needs to continue the charge with 13.20V OR Battery is full
									if ((Varray >= Vfloat) && (SOC < SOCfull)){ 	//PV panel has enough power to charge the battery AND
															//to supply energy to the load
										Vdc=Vfloat;
										Idc=Idcmin;
										chargeBattery();
									}
									else { //PV panel has NOT enough power but the battery can supply the energy to the load
										//here battery is not charging and feed the load
										Vdc=Vbat*NBS;
										Idc=Idcmin;
										dischargeBattery(h);
									}
								}
								//with Idc and Vdc defined here, is just calculus to check if the community requeriment will be met by the system
								if( ! ( (Vdc*Idc) >= PinDCmin ) ){ __VERIFIER_error(); }
								autonomyCount = autonomyCount+1;
								if ( !(SOC >= 75) ) { __VERIFIER_error(); }
								h=h+1;
					}
				}
				else return 0; //at this case, autonomy is lower than 24h and did not reach PV time

			}
			//24h max autonomy verification completed
	}



}


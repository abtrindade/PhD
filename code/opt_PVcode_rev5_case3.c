/*
 ============================================================================
 Name        : opt_PVcode_rev5_improved database.c CASE 3
 Author      : Alessandro Trindade
 Version     : rev5 - May 2019 (better cost tuning)
 Description : stand-alone PV sizing optimization (Universal: CBMC, ESBMC, UAutomizer and CPChecker)
 //assume (exp);  MUST BE REPLACE BY:   if (!expr) { __VERIFIER_error(); }
 ============================================================================*/

//case studies
//HOUSE1 
// int loadcurve[24] = {118,118,118,46,46,46,95,95,170,170,296,242,242,95,95,95,95,95,342,288,288,288,288,118};
// int Phouse = 501, Psurge = 501, Econsumption = 3900;
// int SOClimit = 75, SOCabsorption = 95, SOCfull = 100, Vbat = 12, VAC = 127;
//HOUSE2 
// int loadcurve[24] = {136, 136, 136, 136, 136, 136, 67, 67, 184, 184, 184, 184, 184, 67, 67, 67, 67, 67, 253, 253, 253, 253, 253, 136};
// int Phouse = 253, Psurge = 722, Econsumption = 3600;
// int SOClimit = 75, SOCabsorption = 95, SOCfull = 100, Vbat = 12, VAC = 127;
//HOUSE3 
// int loadcurve[24] = {113,113,113,113,113,113,67,67,217,97,97,97,97,97,97,97,97,97,263,113,113,113,113,113};
// int Phouse = 263, Psurge = 732, Econsumption = 2500;
// int SOClimit = 75, SOCabsorption = 95, SOCfull = 100, Vbat = 12, VAC = 127;
//HOUSE4 
// int loadcurve[24] = {207,207,207,135,135,135,66,66,161,161,233,253,248,66,66,66,66,66,302,317,322,302,302,207};
// int Phouse = 322, Psurge = 896, Econsumption = 4300;
// int SOClimit = 75, SOCabsorption = 95, SOCfull = 100, Vbat = 12, VAC = 127;
//HOUSE5 
// int loadcurve[24] = {45,16,16,16,16,16,0,0,0,72,72,222,150,150,0,0,72,72,814,814,814,742,742,16};
// int Phouse = 915, Psurge = 980, Econsumption = 4880;
// int SOClimit = 75, SOCabsorption = 95, SOCfull = 100, Vbat = 12, VAC = 127;

//data from Manaus
int minSolarIrrad [24] = {0,0,0,0,0,0,25,135,274,422,509,537,503,505,430,281,80,10,0,0,0,0,0,0};
int maxSolarIrrad [24] = {0,0,0,0,0,4,87,295,487,648,751,852,817,742,610,418,128,51,0,0,0,0,0,0};
int minTemp [12] = {23,23,23,23,23,23,23,23,23,24,24,23};
int maxTemp [12] = {30,30,30,30,30,30,30,31,32,32,31,30};
float Insol = 4.92; //(kWh/m2/day CRESESB 2016)
int Gref = 1000; //reference irradiance is 1000 W/m²
float Tref = 298.15; //reference temperature is 25oC or 298.15K

//data from the house
int Phouse = 915, Psurge = 980, Econsumption = 4880;

//must define
int MaxCost=100000; //100000 maximum cost just to control the loop of main function
int Vsystem = 24;  //12V, 24V, 36V or 48V: DC Bus voltage definition
int autonomy = 12; //autonomy in hours
int SOClimit = 75, SOCabsorption = 95, SOCfull = 100, Vbat = 12, VAC = 127;

//Quantities of each equipment at the database below
unsigned char PanelQuant = 10, BatteryQuant=10, ControllerQuant=10,InverterQuant=10;

// panel data        {area, efficiency, num cells, NOCT, mii, miv, Iscref, Vocref, Pmref, Imref, Vmref, VmpNOCT, cost} 
// Canadian (CS6U-315, 320, 325, 330P), Kyocera (KU340-8BCA, KC200GT), Sinosola (SA275-60P, SA360-72M), Risen (RSM36-6-150P), Yingli (YL150P-17b)
float PanelData[10][13] = {{1.94432, 0.1620, 72, 45, 0.053, -0.31, 9.18, 45.1, 315, 8.16, 36.6, 33.4, 268.40},
                          {1.94432, 0.1646, 72, 45, 0.053, -0.31, 9.26, 45.3, 320, 8.69, 36.8, 33.6, 190.00},
                          {1.94432, 0.1672, 72, 45, 0.053, -0.31, 9.34, 45.5, 325, 8.78, 37.0, 33.7, 216.67},
			  {1.94432, 0.1697, 72, 45, 0.053, -0.31, 9.45, 45.6, 330, 8.88, 37.2, 33.9, 170.30},
			  {2.19384, 0.1515, 80, 47, 0.020, -0.48, 8.86, 50.8, 340, 8.26, 41.2, 37.0, 214.20},
			  {1.41075, 0.1600, 54, 47, 0.00318, -0.123, 8.21, 32.9, 200, 7.61, 32.9, 23.2, 300.00},
			  {1.62688, 0.1690, 60, 47, 0.039, -0.307, 9.44, 38.84, 275, 8.81, 31.22, 26.72, 150.00},
			  {1.94035, 0.1855, 72, 45, 0.039, -0.307, 9.73, 47.60, 360, 9.33, 38.59, 34.96, 237.24},
			  {1.00640, 0.1515, 36, 45, 0.033, -0.39, 8.81, 22.30, 150, 8.20, 18.30, 14.40, 94.75},
			  {0.99960, 0.1500, 36, 46, 0.060, -0.37, 8.61, 22.90, 150, 8.12, 18.50, 14.61, 108.50}};
//
// battery data  	{efficiency, voltage, capacityC20, Vbulk, Vfloat, cost} 
// moura (12MF80, 12MF105, 12MF150, 12MF175, 12MF220) , heliar (DF1000, DF1500, DF2500, DF3000, DF4001)
float BatteryData [10][6] = {{0.85, 12, 80, 14.4, 13.8, 131.00},
			    {0.85, 12, 105, 14.4, 13.8, 150.00},
			    {0.85, 12, 150, 14.4, 13.8, 324.75},
			    {0.85, 12, 175, 14.4, 13.8, 299.75},
                            {0.85, 12, 220, 14.4, 13.8, 374.75},
			    {0.85, 12, 60, 14.4, 13.8, 114.75},
			    {0.85, 12, 80, 14.4, 13.2, 138.00},
			    {0.85, 12, 150, 14.4, 13.2, 275.00},
			    {0.85, 12, 170, 14.4, 13.2, 299.01},
			    {0.85, 12, 220, 14.4, 13.2, 330.73}};


// controller data  	{efficiency, nominal current, voltage  output, Vmpptmin=vbat+1, Vcmax, cost}
// victron MPPT (35-145, 15-75, 15-100, 50-100), Epever (TRIRON 2210N 20A 12/24V, TRIRON 3210N 30A 12/24V, TRIRON 4210N 40A 12/24V), Epsolar (Tracer-2210RN 20A 12/24V, New Tracer-3215BN 30A 12/24V), Schneider Electric (XW-MPPT 60/150)
float ControllerData [10][6] = {{0.98, 35, 24, 13, 145, 294.95},
			       {0.98, 15, 24, 13, 75, 88.40},
			       {0.98, 15, 24, 13, 100, 137.70},
                               {0.98, 50, 24, 13, 100, 294.95},
			       {0.98, 20, 24, 13, 100, 132.25},
			       {0.98, 30, 24, 13, 100, 161.00},
			       {0.98, 40, 24, 13, 100, 184.75},
			       {0.97, 20, 24, 13, 100, 217.25},
			       {0.97, 30, 24, 13, 100, 297.25},
			       {0.96, 60, 60, 13, 140, 1072.50}};

// inverter data  	{efficiency, VinDC, VoutAC, PACref, MAXACref, cost}   //???VinDC pode variar tem range
// victron (Phoenix 24-800,Phoenix 48-1200), Epever (IP1500-11,  IP350-11, IP500-11), Epsolar (SHI600-12, STI1000-24-120), Xantrex (SW 1000, HFS 1055 1000W, )
float InverterData [10][6] =   {{0.93, 48, 110, 700, 1600, 400.00},
			       {0.93, 48, 110, 1200, 2400, 750.00},
			       {0.93, 24, 120, 1200, 2400, 450.00},
			       {0.91, 24, 120, 280, 750, 149.75},
			       {0.91, 24, 120, 400, 1000, 187.25},
			       {0.93, 12, 220, 600, 1350, 342.25},
			       {0.93, 24, 120, 800, 1200, 500.00},
			       {0.90, 12, 120, 900, 2000, 649.75},
			       {0.82, 12, 120, 1000, 2000, 1122.25},
			       {0.90, 24, 125, 1800, 2900, 1669.75}};


//global variables
float SOC = 100; //battery is charged when starts the verification

extern void __VERIFIER_error() __attribute__ ((__noreturn__));
extern void __VERIFIER_assume(int expression);
unsigned int nondet_uint();
unsigned char nondet_uchar();


int FHintCost(void){
	int lowervalue, cost=0, cont;
	lowervalue=(int)PanelData[0][12];
	for (cont=1; cont <= 9; cont++) {
		if ((int)PanelData[cont][12] < lowervalue) lowervalue=(int)PanelData[cont][12];
	}
	cost = lowervalue;
	lowervalue=(int)BatteryData[0][5];
	for (cont=1; cont <= 9; cont++) {
		if ((int)BatteryData[cont][5] < lowervalue) lowervalue=(int)BatteryData[cont][5];
	}
	cost = cost+lowervalue;
	lowervalue=(int)ControllerData[0][5];
	for (cont=1; cont <= 9; cont++) {
		if ((int)ControllerData[cont][5] < lowervalue) lowervalue=(int)ControllerData[cont][5];
	}
	cost = cost+lowervalue;
	lowervalue=(int)InverterData[0][5];
	for (cont=1; cont <= 9; cont++) {
		if ((int)InverterData[cont][5] < lowervalue) lowervalue=(int)InverterData[cont][5];
	}
	cost = cost+lowervalue;
	return(cost);
}

/*--------------------------------
            Auxiliary function
----------------------------------*/
int Faux (int cost){
	int NTP, NPP, NPS, NBtotal, NBS, NBP, NPPmin, ICmin;
	float Ecorrected, Ep, NTPmin, NPSmin1, NPSmin2, Cbank, Iscamb, NBSmin, NBPmin, Fobj;
	unsigned char PanelChoice, BatteryChoice, ControllerChoice, InverterChoice;

	PanelChoice = nondet_uchar();
        __VERIFIER_assume(PanelChoice <= (PanelQuant-1));
//	__VERIFIER_assume((PanelChoice >= 0) && (PanelChoice <= PanelQuant));

	BatteryChoice = nondet_uchar();
        __VERIFIER_assume(BatteryChoice <= (BatteryQuant-1));

	ControllerChoice = nondet_uchar();
	__VERIFIER_assume(ControllerChoice <= (ControllerQuant-1));

	InverterChoice = nondet_uchar();
	__VERIFIER_assume(InverterChoice <= (InverterQuant-1));

	// information from the PV Panel
	float Ap = PanelData[PanelChoice][0];
	float np = PanelData[PanelChoice][1];
	int N = (int)PanelData[PanelChoice][2];    //N=number of series-connected cells
	int NOCT = (int)PanelData[PanelChoice][3]; //Nominal Operating Cell Temperature=45°C +-2 at G=800 W/m2, light spectrum AM 1.5G, windspeed=1m/s, air temp at 20°C
	float mii = PanelData[PanelChoice][4]; //short-circuit current temperature coefficient (A/K)
	float miv = PanelData[PanelChoice][5]; //open-circuit voltage temperature coefficient (V/K)
	float Iscref = PanelData[PanelChoice][6];
	float Vocref = PanelData[PanelChoice][7];
	float Pmref = PanelData[PanelChoice][8];
	float Imref = PanelData[PanelChoice][9];
	float Vmref = PanelData[PanelChoice][10];
	float VmpNOCT = PanelData[PanelChoice][11];
	float PanelCost = PanelData[PanelChoice][12];

	//battery information
	float nb = BatteryData [BatteryChoice][0];
	int Vbat = (int)BatteryData [BatteryChoice][1];
	int capacity = (int)BatteryData [BatteryChoice][2];
	float Vbulk = BatteryData [BatteryChoice][3]; //(14.40*NBS);
	float Vfloat = BatteryData [BatteryChoice][4]; //(13.20*NBS);
	float BatteryCost = BatteryData [BatteryChoice][5];

	// charge controller
	float nc = ControllerData[ControllerChoice][0];
	int IC = (int)ControllerData[ControllerChoice][1];
	int VC = (int)ControllerData[ControllerChoice][2];
	int Vmpptmin = (int)ControllerData[ControllerChoice][3];
	float VCmax = ControllerData[ControllerChoice][4];
	float ControllerCost = ControllerData[ControllerChoice][5];

	//inverter
	float ni = InverterData[InverterChoice][0];
	int VinDC = (int)InverterData[InverterChoice][1];
	int VoutAC = (int)InverterData[InverterChoice][2];
	int PACref = (int)InverterData[InverterChoice][3];
	int MAXACref = (int)InverterData[InverterChoice][4];
	float InverterCost = InverterData[InverterChoice][5];

	//Econsumption = energy consumed in one day from the house
	Ecorrected = Econsumption / (nb*nc*ni);

	//estimation of the energy of each panel (Wh)
	Ep = Insol * Ap * np * 1000;

	//PV panel check
	NTPmin = Ecorrected/Ep;
	NTP = ((Ecorrected -1)/Ep) + 1; //up next integer of division
	//if( ! ( NTP >= NTPmin ) ){ __VERIFIER_error();}
	NPSmin1 = (Vsystem + 1) / Vmpptmin;
	NPSmin2 = VCmax / (float)Vmref;
	NPS = VCmax / Vmref;
	NPPmin = (int)Phouse / (int)(NPS * Pmref);
	if (NPPmin == 0) NPP =1;
	else NPP = (int)NPPmin;
			//if( ! (( NPS >= NPSmin1 ) && (NPS <= NPSmin2)) ) { __VERIFIER_error();}
			//if( ! ( NPP >= NPPmin ) ){ __VERIFIER_error();}
	NTP = NPS * NPP;

	//battery check
	Cbank = Ecorrected * autonomy / (Vsystem*(float)(100-SOClimit));
	NBSmin = Vsystem/Vbat;
	NBS = ((Vsystem -1)/Vbat) + 1;
			//if( ! ( NBS >= NBSmin ) ){ __VERIFIER_error(); }
			//bankcapacity = capacity * NBP
	NBPmin = Cbank / capacity;
	NBP = ((Cbank -1)/capacity)+1;
			//if( ! ( NBP >= NBPmin ) ){ __VERIFIER_error(); }
	NBtotal = NBS * NBP;
			//if( ! ( (NBS * NBP) >= NBtotal ) ){ __VERIFIER_error(); }

	//controller sizing and check, considering just one controller
			//if( ! ( VC == Vsystem ) ){ __VERIFIER_error(); }
	__VERIFIER_assume( (VC == Vsystem) );
	Iscamb = Iscref*(1+mii*(NOCT-25)); //from each PV panel
	ICmin = ((Iscamb - 1) * NPP)+1;
	__VERIFIER_assume( (IC >= ICmin) );

	//inverter sizing and check
	__VERIFIER_assume ( ( VinDC >= Vsystem ) ); //if inverter supports 48V, then it supports 12 and 24 as well

	__VERIFIER_assume ( (VoutAC >= (VAC-VAC*15/100)) && (VoutAC <= (VAC+VAC*15/100)) ); //Inverter output mus be +- 15% of the expected house's VAC

	__VERIFIER_assume ( (Phouse <= PACref) && (Psurge <= MAXACref) ); //Power supported by the inverter must be greater than House's demand
																	  // and the Peak power from inverter must support the Reak from the house

	// Function objective: minimum acquisition cost
	Fobj= NTP*PanelCost + NBtotal*BatteryCost + ControllerCost + InverterCost;

	//minimize cost of the solution, considering all the equipment that can be used and was declared at the code
	if (!(Fobj > cost)) { __VERIFIER_error(); }
	return 0;
}


int main() {
	int HintCost;
	HintCost = FHintCost();
	for (; HintCost <= MaxCost; HintCost=HintCost+500){
		Faux(HintCost);
	}
	return 0;
}

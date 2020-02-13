int MaxCost=10000; //10 million maximum cost just to control the loop of main function
int Vsystem = 24;  //12V, 24V, 36V or 48V: DC Bus voltage definition
int autonomy = 48; //autonomy in hours
int SOClimit = 75, SOCabsorption = 95, SOCfull = 100, Vbat = 12, VAC = 127;
int Econsumption = 3900;
float nb=0.85, nc=0.93, ni=0.98;
float Insol = 3.80;

//global variables
float SOC = 100; //battery is charged when starts the verification
extern void __VERIFIER_error() __attribute__ ((__noreturn__));
extern void __VERIFIER_assume(int expression);
unsigned int nondet_uint();
unsigned char nondet_uchar();

int main (){
	float Ecorrected, Pminpanels, ItotalPVpanels, VtotalPVpanels, Eb, DODdaycalc;
	int NTP, NPmin, NPP, NPS, NBC, NBP, NPPmin, NBtotal,DODmax;
	float Pmref = 330, Imref=8.88, Vmref=37, IC=40, VCmax=150;

	//Econsumption = energy consumed in one day from the house
	Ecorrected = Econsumption / (nb*nc*ni);

	Pminpanels = 1.25 * Ecorrected / Insol;

NPmin = ((Pminpanels -1)/ Pmref)+1;

		NPP = 2;
		if ((NPmin % 2) == 0) NPS = NPmin/2; //even number
		else NPS = (NPmin + 1)/2; //odd number

NTP = NPS * NPP;

//checking power from PV panels
/////////////////////////////////////__VERIFIER_assume  (Pminpanels <= (NPS*NPP*Pmref));

//checking voltage and current from the PV panel and the controller
ItotalPVpanels = NPP * Imref;
__VERIFIER_assume ( (IC >= ItotalPVpanels) );

VtotalPVpanels = NPS * Vmref;
__VERIFIER_assume ( (VCmax >= VtotalPVpanels) );

DODmax = (100-SOClimit)*2;
//checking batteries
Eb = (autonomy/24) * Ecorrected*100/DODmax;


if (!(Eb > 15000)) { __VERIFIER_error(); }
	return 0;
}



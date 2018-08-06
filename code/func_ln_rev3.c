// 
// Function that calculates the natural logarithmic of base e for a number x - Alessandro Trindade July 2018
// 
// log(m) base e = 2[ (m-1/m+1) + (1/3) * (m-1/m+1)^3 + (1/5) * (m-1/m+1)^5 + ... ]
//
// To get an approximation of log (all depends on the precision demanded by the code)
// OBS: The natural logarithm function ln(x) is defined only for x>0. So the natural logarithm of a negative number is undefined.

// Function logESBMC of natural base e
#include <stdio.h>
#include <math.h>

//IMPORTANT: to declare precision here
#define precisionLN 0.00000000001

double logESBMC (double d){ //from: https://www.youtube.com/watch?v=zaTdPTzw5Pc if 0<x<1
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

//main code just for test
int main(void) {
	double cont;
//	long double cont=0.01;
//    		printf("num=%Lf, sum=%Lf, log=%lf\n",cont, logESBMC(cont),log(cont));
	for (cont=0.05; cont <= 1; cont=cont+0.05){ //test positive numbers
    		printf("num=%lf, sum=%lf, log=%lf\n",cont, logESBMC(cont),log(cont));
		printf ("----\n");
	}
    	return 0;
}



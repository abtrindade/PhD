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
#define precisionLN 0.00001


//code here from https://stackoverflow.com/questions/3343395/value-of-natural-logarithm
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

double logESBMC2 (double d){ //from: https://www.youtube.com/watch?v=zaTdPTzw5Pc if 0<x<1
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

double logESBMC (double x){
	double answer;
	if (x > 1.0) answer = logESBMC1(x);
	else if (x>0.0) answer=logESBMC2(x);
	else answer = NAN;
	return answer;
}


//main code just for test
int main(void) {
	double cont;
	for (cont=0.1; cont <= 50; cont=cont+0.1){ //test positive numbers
    		printf("num=%lf, sum=%lf, log=%lf\n",cont, logESBMC(cont),log(cont));
		printf ("----\n");
	}
    	return 0;
}



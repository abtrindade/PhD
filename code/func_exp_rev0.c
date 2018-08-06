// 
// Function that calculates the exponential of a number x (e^x) - Alessandro Trindade July 2018
// 
// Uses Tayler Series 
// The exponential function has the following Maclaurin series
// Sums: 1 + x + x^2/2! + x^3/3! + x^4/4! + x^5/5! + ...
// and it converges for all x
//
// To get an approximation of e^x (all depends on the precision demanded by the code)
//
#include <stdio.h>
#include <math.h>

//IMPORTANT: to declare precision here
#define precisionEXP 0.0001

//from https://codereview.stackexchange.com/questions/123970/implementation-of-exp-function-in-c-using-taylor-series-expansion but with improvement in the while, to solve negative numbers
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


//main code just for test
int main(void) {
	double cont;
	//for (cont=0; cont >= -10; cont=cont-0.2){ //test negative numbers
	for (cont=0; cont <= 10; cont=cont+0.5){ //test positive numbers
    		printf("num=%lf, sum=%lf, exp=%lf\n",cont, expESBMC(cont),exp(cont));
		printf ("----\n");
	}
    	return 0;
}



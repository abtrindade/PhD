// 
// Function that calculates the exponential of a number x (e^x)
// 
// Uses Tayler Series 
// The exponential function has the following Maclaurin series
// Sums: 1 + x + x^2/2! + x^3/3! + x^4/4! + x^5/5! + ...
// and it converges for all x
//
// To get an approximation of e^x.
//
#include <stdio.h>
#include <math.h>

#define precisionEXP 0.0000001

long long int factorial (int);
double expESBMC (double);

int main(void) {
	double cont;
	//for (cont=0; cont >= -10; cont--){
	for (cont=0; cont <= 10; cont=cont+0.5){
    		printf("num=%lf, sum=%lf, exp=%lf\n",cont, expESBMC(cont),exp(cont));
		printf ("----\n");
	}
    	return 0;
}

double expESBMC (double num){
    double x1, sum=0.0;
    int n=1;
    // Stops when the next term to add becomes smaller than precision.
    x1=1;
    do {
        sum = sum + x1;
        x1  = pow(num,n)/factorial(n);
	n=n+1;
	//printf("x1= %lf, ",x1);
    } while (x1 > precisionEXP);
    return sum;
}


long long int factorial(int num)
{
    int i;
    long int result = 1;

    // show error if the user enters a negative integer
    if (num < 0)
        printf("Error! Factorial of a negative number doesn't exist.");

    else{
        for(i=1; i<=num; ++i){
            result *= i;              // factorial = factorial*i;
        }
    }
//printf("fatorial = %lli \n", result);
    return result;
}

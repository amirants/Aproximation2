#include <stdlib.h>
#include <iostream>
#include <cmath>

#include "akim.cpp"

using namespace std;

double f(double x){
    return pow(x,2);
}

int main( void ){
    double a, b, dlt;
    int n;
    printf( " Введите a, b, n \n ");
    std::cin >> a >> b >> n;

    dlt = (b - a) / (n - 1);
    double x = a;

    FILE *out = fopen( "input.txt", "w" );

    int i;

    for (i = 0; i < n; i++ ){
        fprintf( out, "%.2e ", x );
        fprintf( out, "%.2e\n", f(x) );
        x += dlt;
    }

    fprintf( out, "%.2e ", first_deriv( a ) );
    fprintf( out, "%.2e ", first_deriv( b ) );

    fprintf( out, "%.2e ", second_deriv( a ) );
    fprintf( out, "%.2e ", second_deriv( b ) );
}
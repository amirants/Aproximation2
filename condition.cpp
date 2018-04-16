// #include <stdlib.h>
// #include <stdio.h>
#include <iostream>
#include <cmath>

using namespace std; 
double f( double x );

double f( double x ){
    return exp(pow(x,2));
}

void by_fun ( int n, double a, double b, double* f_arr ){
    double dlt, cur_x;
    int i;
    
    dlt = ( b - a ) / n;

    cur_x = a;

    for ( i = 0; i < n + 1; i++ ){
        f_arr[i] = f(cur_x);
        cur_x += dlt;
    }
}

void from_file( int n, double* x_arr, double* f_arr, double *derivatives ){
    int i;

    FILE* dat = fopen( "input.txt", "r" );
    for ( i = 0; i < n; i++ ){
        fscanf( dat, "%lg", &x_arr[i] );
        fscanf( dat, "%lg", &f_arr[i] );
    }
    for ( i = 0; i < 4; i++ ){
        fscanf( dat, "%le", &derivatives[i] );
    }
}
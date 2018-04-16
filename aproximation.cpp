#include <stdlib.h>
#include <iostream>

using namespace std; 

void Newton_polynomial( int n, double* x_arr, double* f_arr ){
    int i, j;

    for ( i = 1; i < n; i++ )
        for ( j = n - 1; j > i - 1; j-- )
            f_arr[j] = (f_arr[j] - f_arr[j - 1]) / (x_arr[j] - x_arr[j - i]);

    // printf("\n(((((((((");
    // for ( i = 0; i < n + 1; i++ )
    //     printf("%g ", f_arr[i]);
    // printf(")))))))))\n");
}

double f_val_1( double t, int n, double *coeff, double *x ){
    int i;
    double fx;

    fx = coeff[n - 1];

    for ( i = n - 1; i >= 0; i-- ){
        fx *= (t - x[i]);
        fx += coeff[i];
    }
    return fx;
}

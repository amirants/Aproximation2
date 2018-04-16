// #include "condition.cpp"
#include <stdlib.h>
#include <iostream>

static double* f_arr;
static double* f_arr2;
static double* coeffs2;
static double* d2;
static double* x_arr;

double f( double x );

void splitting_x( int n, double* x_arr, double a, double b ){
    int i;
    double dlt = (b - a)/(n - 1);
    for (i = 0; i < n; i++ )
        x_arr[i] = a + i*dlt;
}

void splitting_f( int n, double* x_arr, double *f_arr ){
    int i;
    for ( i = 0; i < n; i++ )
        f_arr[i] = f(x_arr[i]);
}

void free_all( double *arr1, double *arr2, double *arr3, double *arr4){
    free(arr1);
    free(arr2);
    free(arr3);
    free(arr4);
}

void memory_allocation( int n ){
    
    if ( !( f_arr = new double[n + 1]) || !( f_arr2 = new double[n + 1]) 
    || !( coeffs2 = new double[4 * n]) || !( d2 = new double[n + 1]) 
    || !( x_arr = new double[n + 1]))
        std::cout << "Не достаточно памяти\n";
    else   
        std::cout << "Память выделена успешно\n";
}

int i_max( int n, double *f_arr ){
    int i, imax = 0;
    int max = f_arr[0];
    
    for (i = 0; i < n; i++){
        if ( f_arr[i] > max ){
            max = f_arr[i];
            imax = i;
        }
    }
    return imax;
}

int i_min( int n, double *f_arr ){
    int i, imin = 0;
    int min = f_arr[0];
    
    for (i = 0; i < n; i++){
        if ( f_arr[i] < min ){
            min = f_arr[i];
            imin = i;
        }
    }
    return imin;
}
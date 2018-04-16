#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>

#include <string> 
#include <sstream>
double f1( double x );
double f2( double x );
void render( double(*f)(double x), int n, double a, double b, cv::Mat field, cv::Scalar color_2, double con );

template <typename T>
std::string toString(T val)
{
    std::ostringstream oss;
    oss<< val;
    return oss.str();
}

using namespace cv;
using namespace std;

void render_function( double (*f)(double x), int n, double a, double b, 
double x_max, double x_min, Mat field, Scalar color_2, double con ){
    int i;
    int scale_x = 1000/n + 1;
    int d;
    
    double delta = (b - a)/n;
    double y_min = f(x_min);
    double y_max = f(x_max);

    double scale_y =  (600/(y_max - y_min));

    double x1 = a, x2 = x1, y1 = f(con*x1), y2 = y1;
   
    int Ax = 100, Ay, Bx = 100, By;
    int max_B = 0;

    Ay = 800 - ((int)(scale_y * (y1 - y_min)) + 100);
    By = Ay;

    for ( i = 0; i < n; i++ ){

        if ( i == (1000 % n)/2 )
            scale_x--;
        if ( i == n - (1000 % n)/2 )
            scale_x++;

        x2 += delta;
        y2 = f(con * x2);
        d = (int)((y2 - y1) * scale_y);
        
        Bx += scale_x;
        By -= d;
        if (By > max_B) max_B = By;
        Point B( Bx, By );
        Point A( Ax, Ay );
        line( field, A, B, color_2 );
        Ax = Bx;
        Ay = By;
        x1 = x2;
        y1 = y2;
    }
}


void render_approximation( double (*f)(double x), double (*approx)(double t, int n, double *coeff, double *x), int n, double a, double b, 
double x_max, double x_min, Mat field, Scalar color_2, double con, double *f_arr, double *x_arr, int m ){
    double delta = (b - a)/n;
    double y_min = f(x_min);
    double y_max = f(x_max);

    double scale_y =  (600/(y_max - y_min));

    int i;
    int scale_x = 1000/n + 1;
    int d;
   
    double x1 = a, x2 = x1, y1 = approx(con*x1, m, f_arr, x_arr ), y2 = y1;
   
    int Ax = 100, Ay, Bx = 100, By;
    int max_B = 0;

    Ay = 800 - ((int)(scale_y * (y1 - y_min)) + 100);// % 600;
    By = Ay;

    for ( i = 0; i < n; i++ ){
        
        if ( i == (1000 % n)/2 )
            scale_x--;
        if ( i == n - (1000 % n)/2 )
            scale_x++;

        x2 += delta;
        y2 = approx(con * x2, m, f_arr, x_arr);

        if (y2 > y_max) 
            y2 = y_max;
        if (y2 < y_min)
            y2 = y_min;

            
        d = (int)((y2 - y1) * scale_y);
        
        Bx += scale_x;
        By -= d;
        if (By > max_B) max_B = By;
        Point B( Bx, By );
        Point A( Ax, Ay );
        line( field, A, B, color_2 );
        Ax = Bx;
        Ay = By;
        x1 = x2;
        y1 = y2;
    }
}

double maxf( double(*f)(double x), double a, double b, int n){
    int i;

    double delta = (b - a)/n;
    double t = a;
    double x_max = a;

    for ( i = 0; i < n; i++ ){
        if ( f(t) > f(x_max) )
            x_max = t;
        t += delta;
    }
    return x_max;
}

double minf( double(*f)(double x), double a, double b, int n){
    int i;

    double delta = (b - a)/n;
    double t = a;
    double x_min = a;

    for ( i = 0; i < n; i++ ){
        if ( f(t) < f( x_min ))
            x_min = t;
        t += delta;
    }
    return x_min;
}

void render_coords( double (*f)(double x), double a, double b, double x_max, double x_min, Mat field, int n ){
    Point A1 (100, 400), A2 (600, 700), B1 (1100, 400), B2(600, 100);

    Point y_1 ( 600, 10 ), y_2 ( 600, 890 ),
    x_1 ( 10, 400 ), x_2 ( 1190, 400 );
    Scalar color_1 ( 255, 255, 255 );

    line( field, y_1, y_2, color_1 );
    line( field, x_1, x_2, color_1 );

    string aa = toString(a);
    string bb = toString(b);

    Point A ( 90, 420 ), B ( 1085, 420 );
    putText ( field, aa, A, 2, 0.5, color_1 );
    putText ( field, bb, B, 2, 0.5, color_1 );
    circle( field, A1, 1, color_1, 3 );
    circle( field, B1, 1, color_1, 3 );
    circle( field, A2, 1, color_1, 3 );
    circle( field, B2, 1, color_1, 3 );
    aa = toString( f(x_max) );
    bb = toString( f(x_min) );
    string nn = toString( n );
    nn = "n = " + nn;
    Point A3 (600, 100);
    Point B3 (600, 700);
    Point N ( 1100, 40 );
    putText ( field, aa, A3, 2, 0.5, color_1 );
    putText ( field, bb, B3, 2, 0.5, color_1 );
    putText ( field, nn, N, 2, 0.6, color_1);
    /////////////////////
}

void render_differ( double (*f)(double x), double (*approx)(double t, int n, double *coeff, double *x), int n, double a, double b,
double x_max, double x_min, Mat field, Scalar color_2, double con, double *f_arr, double *x_arr, int m){
    double delta = (b - a)/n;
    double y_min = f(x_min);
    double y_max = f(x_max);

    double scale_y =  (600/(y_max - y_min));

    int i;
    int scale_x = 1000/n + 1;
    int d;

    double x1 = a, x2 = x1, y1 = f(x1) - approx( x1, m, f_arr, x_arr ), y2 = y1;

    int Ax = 100, Ay, Bx = 100, By;
    int max_B = 0;

    Ay = 800 - ((int)(scale_y * (y1 - y_min)) + 100);// % 600;
    By = Ay;

    for ( i = 0; i < n; i++ ){

        if ( i == (1000 % n)/2 )
            scale_x--;
        if ( i == n - (1000 % n)/2 )
            scale_x++;

        x2 += delta;
        y2 = f(x2) - approx(con * x2, m, f_arr, x_arr);

        if (y2 > y_max)
            y2 = y_max;
        if (y2 < y_min)
            y2 = y_min;


        d = (int)((y2 - y1) * scale_y);

        Bx += scale_x;
        By -= d;
        if (By > max_B) max_B = By;
        Point B( Bx, By );
        Point A( Ax, Ay );
        line( field, A, B, color_2 );
        Ax = Bx;
        Ay = By;
        x1 = x2;
        y1 = y2;
    }
}

void render_fromFile( int n, double *f_arr, double *x_arr, 
int imin, int imax, Mat field, Scalar color ){
    int i;
    int scale_x = 1000 / (n - 1) ;
    int d;
    int a = x_arr[0], b = x_arr[n - 1];
    double dlt;
    
    printf("imin %d imax %d\n", imin, imax);

    double y_min = f_arr[imin];
    double y_max = f_arr[imax];

    double scale_y =  (600/(y_max - y_min));

    double x1 = a, x2 = x1, y1 = f_arr[0], y2 = y1;
    // printf("----Ymin %g\n", y_min);


//    printf("\n -===");
//     for ( int i = 0; i < n; i++ )
//         printf("%e ", f_arr[i]);
//     printf("===-\n");

   
    int Ax = 100, Ay, Bx = 100, By;
    int max_B = 0;

    Ay = 800 - ((int)(scale_y * (y1 - y_min)) + 100);
    By = Ay;

    // printf("Ay %d\n", Ay);
    

    for ( i = 1; i < n; i++ ){

        dlt = (x_arr[i] - x_arr[i - 1])/(x_arr[n - 1] - x_arr[0]);

        printf("i = %d dlt = %e \n", i, dlt);
        scale_x = dlt*1000;
        printf("i = %d s_x = %d \n", i, scale_x);
        // printf("----y %le\n", y_max);
        // printf("----y %le\n", y_min);

        x2 = x_arr[i];
        y2 = f_arr[i];
        d = (int)((y2 - y1) * scale_y);
        
        Bx += scale_x;
        By -= d;
        if (By > max_B) max_B = By;
        Point B( Bx, By );
        Point A( Ax, Ay );
        line( field, A, B, color );
        Ax = Bx;
        Ay = By;
        x1 = x2;
        y1 = y2;
        // printf("Ay %d\n", Ay);
    }
}

void render_fileCoords( int n, int imax, int imin, double *f_arr, double *x_arr, Mat field ){
    int a = x_arr[0];
    int b = x_arr[n - 1];

    Point A1 (100, 400), A2 (600, 700), B1 (1100, 400), B2(600, 100);

    Point y_1 ( 600, 10 ), y_2 ( 600, 890 ),
    x_1 ( 10, 400 ), x_2 ( 1190, 400 );
    Scalar color_1 ( 255, 255, 255 );

    line( field, y_1, y_2, color_1 );
    line( field, x_1, x_2, color_1 );

    string aa = toString(a);
    string bb = toString(b);

    Point A ( 90, 420 ), B ( 1085, 420 );
    putText ( field, aa, A, 2, 0.5, color_1 );
    putText ( field, bb, B, 2, 0.5, color_1 );
    circle( field, A1, 1, color_1, 3 );
    circle( field, B1, 1, color_1, 3 );
    circle( field, A2, 1, color_1, 3 );
    circle( field, B2, 1, color_1, 3 );
    aa = toString( f_arr[imax] );
    bb = toString( f_arr[imin] );
    string nn = toString( n );
    nn = "n = " + nn;
    Point A3 (600, 100);
    Point B3 (600, 700);
    Point N ( 1100, 40 );
    putText ( field, aa, A3, 2, 0.5, color_1 );
    putText ( field, bb, B3, 2, 0.5, color_1 );
    putText ( field, nn, N, 2, 0.6, color_1);
    /////////////////////
}

void render_approximation_f( double (*approx)(double t, int n, double *coeff, double *x), int n, double a, double b, 
double max, double min, Mat field, Scalar color_2, double *f_arr, double *x_arr, int m ){
    double delta = (b - a)/n;
    double y_min = min;
    double y_max = max;

    printf("}}}}}}}}}}}}%e %e{{{{{{{{{{{{\n", y_min, y_max);
    
    printf("???????????????????????\n");
    double scale_y =  (600/(y_max - y_min));

    int i;
    int scale_x = 1000/n + 1;
    int d;
   
    double x1 = a, x2 = x1, y1 = approx(x1, m, f_arr, x_arr ), y2 = y1;
   
    int Ax = 100, Ay, Bx = 100, By;
    int max_B = 0;

    Ay = 800 - ((int)(scale_y * (y1 - y_min)) + 100);// % 600;
    By = Ay;

    for ( i = 0; i < n; i++ ){
        
        if ( i == (1000 % n)/2 )
            scale_x--;
        if ( i == n - (1000 % n)/2 )
            scale_x++;

        x2 += delta;
        y2 = approx(x2, m, f_arr, x_arr);

        // if (y2 > y_max) 
        //     y2 = y_max;
        // if (y2 < y_min)
        //     y2 = y_min;

            
        d = (int)((y2 - y1) * scale_y);
        
        Bx += scale_x;
        By -= d;
        if (By > max_B) max_B = By;
        Point B( Bx, By );
        Point A( Ax, Ay );
        line( field, A, B, color_2 );
        Ax = Bx;
        Ay = By;
        x1 = x2;
        y1 = y2;
    }
}

void render_differ_f( double (*approx)(double t, int n, double *coeff, double *x), int n, double a, double b, 
double max, double min, Mat field, Scalar color_2, double *f_arr, double *x_arr, int m ){
    double delta = (b - a)/n;
    double y_min = min;
    double y_max = max;

    printf("}}}}}}}}}}}}%e %e{{{{{{{{{{{{\n", y_min, y_max);
    
    printf("???????????????????????\n");
    double scale_y =  (600/(y_max - y_min));

    int i;
    int scale_x = 1000/n + 1;
    int d;
   
    double x1 = a, x2 = x1, y1 = approx(x1, m, f_arr, x_arr ), y2 = y1;
   
    int Ax = 100, Ay, Bx = 100, By;
    int max_B = 0;

    Ay = 800 - ((int)(scale_y * (y1 - y_min)) + 100);// % 600;
    By = Ay;

    for ( i = 0; i < n; i++ ){
        
        if ( i == (1000 % n)/2 )
            scale_x--;
        if ( i == n - (1000 % n)/2 )
            scale_x++;

        x2 += delta;
        y2 = approx(x2, m, f_arr, x_arr);

        // if (y2 > y_max) 
        //     y2 = y_max;
        // if (y2 < y_min)
        //     y2 = y_min;

            
        d = (int)((y2 - y1) * scale_y);
        
        Bx += scale_x;
        By -= d;
        if (By > max_B) max_B = By;
        Point B( Bx, By );
        Point A( Ax, Ay );
        line( field, A, B, color_2 );
        Ax = Bx;
        Ay = By;
        x1 = x2;
        y1 = y2;
    }
}
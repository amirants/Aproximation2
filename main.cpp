#include <stdlib.h>
#include <iostream>
#include "condition.cpp"
#include "help.cpp"

#include "aproximation.cpp"
#include "akim.cpp"

#include "graphics.cpp"

using namespace std; 

double* by_fun ( int n, double a, double b );
double f( double x );

double minf( double(*f)(double x), double a, double b, int n);
double maxf( double(*f)(double x), double a, double b, int n);

void render_function( double (*f)(double x), int n, double a, double b, 
double x_max, double x_min, Mat field, Scalar color_2, double con );

void render_approximation( double (*f)(double x), double (*approx)(double t, int n, double *coeff, double *x), int n, double a, double b, 
double x_max, double x_min, Mat field, Scalar color_2, double con, double *f_arr, double *x_arr, int m );

void render_coords( double (*f)(double x), double a, double b, double x_max, double x_min, Mat field, int n );


int main( void ){

    int n, fun, i, imax, imin;
    double a, b, x_min, x_max, max, min;
    double x;

    char c;


    Scalar color_1(251, 121, 29);
    Scalar color_2(10, 221, 29);
    Scalar color_3(10, 21, 229);
    Mat field( 800, 1200, CV_8UC3 );
    Mat field0 = field.clone();
    
    
    std::cout << "Введите a, b и n \n";
    std::cin >> a >> b >> n;
    std::cout << "Способ ввода функции: \n1. Из файла\n2. По формуле\n";
    std::cin >> fun;

    
    memory_allocation( n );

    if ( (fun != 1)  && (fun != 2 ) )
        std::cout << "Не корректный выбор ввода";
    else   
        {
            if (fun == 2){
                splitting_x( n, x_arr, a, b );
                splitting_f( n, x_arr, f_arr );
                splitting_f( n, x_arr, f_arr2 );

                x_min = minf( f, a, b, 1000 );
                x_max = maxf( f, a, b, 1000 );

                render_coords( f, a, b, x_max, x_min, field, n );

                // printf( "\n***********");
                // for (i = 0; i < n + 1;  i++)
                //     printf("%g ", f_arr[i]);
                // printf("***********\n");
            }


            if ( fun == 1){
                double* derivates = new double[4];
                from_file( n, x_arr, f_arr, derivates );

                for (i = 0; i < n; i++)
                    f_arr2[i] = f_arr[i];

                imax = i_max( n, f_arr );
                imin = i_min( n, f_arr );
                max = f_arr[imax];
                min = f_arr[imin];

                render_fileCoords( n, imax, imin, f_arr, x_arr, field );

                printf( "\n***********");
                for (i = 0; i < n;  i++)
                    printf("%g ", f_arr[i]);
                printf("***********\n");
            }

        }
    Mat blankField = field.clone();   
    // printf("\n");
    // for ( int i = 0; i < n; i++ )
    //     printf("%e ", f_arr[i]);
    // printf("\n");
    Newton_polynomial( n, x_arr, f_arr );
    Akim( n, x_arr, f_arr2, coeffs2, d2);
    
    
    
    x = f_val_2( 3, n, coeffs2, x_arr );

    double y = f_val_1( 5, n, f_arr, x_arr );

    printf( "\n***** x = %.2le  y = %.2le ******\n", x, y );

    imshow( "Graphic", field );
    double scl;

    while (true){
        c = waitKey(0);
        if ( c == 'q' ){
            field = blankField.clone();
            if ( fun == 2 )
                render_function( f, 200, a, b, x_max, x_min, field, color_1, 1 );
            if ( fun == 1 ){
                render_fromFile( n, f_arr2, x_arr, imin, imax, field, color_1 );
            }
            imshow( "Graphic", field );
        }
        if ( c == 'w' ){
            field = blankField.clone();
            
            if ( fun == 2){
                render_function( f, 200, a, b, x_max, x_min, field, color_1, 1 );
                render_approximation( f, f_val_1, 200, a, b, x_max, x_min, field, color_2, 1, f_arr, x_arr, n );
            }
            if ( fun == 1 ){
                render_fromFile( n, f_arr2, x_arr, imin, imax, field, color_1 );
                render_approximation_f( f_val_1, 200, a, b, max, min, field, color_2, f_arr, x_arr, n );
            }

            imshow( "Graphic", field );
        }
        if ( c == 'e'){
                field = blankField.clone();
                if ( fun == 2){
                render_function( f, 200, a, b, x_max, x_min, field, color_1, 1 );
                render_approximation( f, f_val_2, 200, a, b, x_max, x_min, field, color_3, 1, coeffs2, x_arr, n );
                }
                if ( fun == 1 ){
                    render_fromFile( n, f_arr2, x_arr, imin, imax, field, color_1 );
                    render_approximation_f( f_val_2, 200, a, b, max, min, field, color_3, coeffs2, x_arr, n );
                }
                imshow( "Graphic", field );
        }
        if ( c == 'r' ){
            field = blankField.clone();
            render_function( f, 200, a, b, x_max, x_min, field, color_1, 1 );
            render_approximation( f, f_val_1, 200, a, b, x_max, x_min, field, color_2, 1, f_arr, x_arr, n );
            render_approximation( f, f_val_2, 200, a, b, x_max, x_min, field, color_3, 1, coeffs2, x_arr, n );
            imshow( "Graphic", field );
        }
        if ( c == 'z' ){
            scl = (b - a)/4;
            a += scl;
            b -= scl;
            
            x_min = minf( f, a, b, 1000 );
            x_max = maxf( f, a, b, 1000 );
            splitting_x( n, x_arr, a, b );
            splitting_f( n, x_arr, f_arr);
            splitting_f( n, x_arr, f_arr2);
            
            Newton_polynomial( n, x_arr, f_arr );
            Akim( n, x_arr, f_arr2, coeffs2, d2);
    
            field = field0.clone();

            render_coords( f, a, b, x_max, x_min, field, n );
            blankField = field.clone();
            imshow( "Graphic", field );
        }
        if ( c == 'x' ){
            scl = (b - a)/2;
            a -= scl;
            b += scl;
            x_min = minf( f, a, b, 1000 );
            x_max = maxf( f, a, b, 1000 );
            
            splitting_x( n, x_arr, a, b );
            splitting_f( n, x_arr, f_arr);
            splitting_f( n, x_arr, f_arr2);
            
            Newton_polynomial( n, x_arr, f_arr );
            Akim( n, x_arr, f_arr2, coeffs2, d2);
    
            field = field0.clone();

            render_coords( f, a, b, x_max, x_min, field, n );
            blankField = field.clone();
            imshow( "Graphic", field );
        }
        if (c == 'c'){
            
            free_all( f_arr, f_arr2, x_arr, coeffs2 );      
            n += 3;
            memory_allocation( n );

            splitting_x( n, x_arr, a, b );
            splitting_f( n, x_arr, f_arr );
            splitting_f( n, x_arr, f_arr2 );
            
            
            field = field0.clone();
            render_coords( f, a, b, x_max, x_min, field, n );
            blankField = field.clone();
            imshow( "Graphic", field );

            Newton_polynomial( n, x_arr, f_arr );
            double s = f_val_1( M_PI/2, n, f_arr, x_arr );

            Akim( n, x_arr, f_arr2, coeffs2, d2);
        }
        if ( c == 'v' ){
           free_all( f_arr, f_arr2, x_arr, coeffs2 );      
            n -= 3;
            memory_allocation( n );

            splitting_x( n, x_arr, a, b );
            splitting_f( n, x_arr, f_arr );
            splitting_f( n, x_arr, f_arr2 );
            
            
            field = field0.clone();
            render_coords( f, a, b, x_max, x_min, field, n );
            blankField = field.clone();
            imshow( "Graphic", field );

            Newton_polynomial( n, x_arr, f_arr );
            double s = f_val_1( M_PI/2, n, f_arr, x_arr );
            
            Akim( n, x_arr, f_arr2, coeffs2, d2);

        }

        if ( c == 't' ){
            field = blankField.clone();
            render_differ( f, f_val_1, 200, a, b, x_max, x_min, field, color_2, 1, f_arr, x_arr, n );
            imshow( "Graphic", field );
        }

        if ( c == 'y' ){
            field = blankField.clone();
            render_differ( f, f_val_2, 200, a, b, x_max, x_min, field, color_3, 1, coeffs2, x_arr, n );
            imshow( "Graphic", field );
        }

        if ( c == 's' ){
            f_arr2[n / 2] += 0.001;
            
            for ( i = 0; i < n; i++ ){
                f_arr[i] = f_arr2[i];
            }

            Newton_polynomial( n, x_arr, f_arr );
            Akim( n, x_arr, f_arr2, coeffs2, d2 );
        }

        if (c == 'a')
            break;        
    }
    
    destroyWindow( "Graphic" );
    free(f_arr);
    free(f_arr2);
    free(x_arr);
    free(coeffs2);

    return 0;
}

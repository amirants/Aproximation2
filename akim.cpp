#include <math.h>
#include <stdlib.h>
#define delt 01e-8

double f( double x );


double first_deriv( double x ){
    return ( 0.5 * ( f(x + delt) - f(x - delt) ) ) / delt;
}

double second_deriv( double x ){
    return ( 0.5 * ( first_deriv( (x + delt) ) - first_deriv(x - delt ) ) ) / delt;
}


void Akim(int n, double *x_arr, double *f_arr2, double *coeffs2, double *d)
{
	int i, j;
	double tmp1, tmp2;

	for (i = 1; i < n - 1; i++)
	{
		tmp1 = (f_arr2[i + 1] - f_arr2[i]) / (x_arr[i + 1] - x_arr[i]);
		tmp2 = (f_arr2[i] - f_arr2[i - 1]) / (x_arr[i] - x_arr[i - 1]);
		coeffs2[i] = fabs(tmp1 - tmp2);
	}

	for (i = 2; i < n - 2; i++)
	{
		tmp1 = (f_arr2[i] - f_arr2[i - 1]) / (x_arr[i] - x_arr[i - 1]);
		tmp2 = (f_arr2[i + 1] - f_arr2[i]) / (x_arr[i + 1] - x_arr[i]);
		if (coeffs2[i - 1] * coeffs2[i - 1] + coeffs2[i + 1] * coeffs2[i + 1] < 1e-50)
		{
			d[i] = (x_arr[i + 1] - x_arr[i]) * tmp1 + (x_arr[i] - x_arr[i - 1]) * tmp2;
			d[i] /= x_arr[i + 1] - x_arr[i - 1];
		}
		else
		{
			d[i] = coeffs2[i + 1] * tmp1 + coeffs2[i - 1] * tmp2;
			d[i] /= coeffs2[i + 1] + coeffs2[i - 1];
		}
	}

	d[0] = first_deriv(x_arr[0]);
	d[n - 1] = first_deriv(x_arr[n - 1]);

	tmp1 = (f_arr2[1] - f_arr2[0]) / (x_arr[1] - x_arr[0]);
	tmp2 = (f_arr2[n - 1] - f_arr2[n - 2]) / (x_arr[n - 1] - x_arr[n - 2]);

	d[1] = 3.0 * tmp1 - 0.5 * second_deriv(x_arr[0]) * (x_arr[1] - x_arr[0]) - 2.0 * d[0];
	d[n - 2] = 3.0 * tmp2 + 0.5 * second_deriv(x_arr[n - 1]) * (x_arr[n - 1] - x_arr[n - 2]) - 2.0 * d[n - 1];

	j = 0;

	for (i = 0; i < n - 1; i ++)
	{
		coeffs2[j] = f_arr2[i];
		coeffs2[j + 1] = d[i];
		tmp2 = x_arr[i + 1] - x_arr[i];
		tmp1 = (f_arr2[i + 1] - f_arr2[i]) / tmp2;
		coeffs2[j + 2] = (3 * tmp1 - 2 * d[i] - d[i + 1]) / tmp2;
		tmp2 *= tmp2;
		coeffs2[j + 3] = (d[i] + d[i + 1] - 2 * tmp1) / tmp2;
		j += 4;
	}
}

double f_val_2(double t, int n, double *coeffs2, double *x_arr)
{
	int i;

	for (i = 0; i < n - 2; i++) if (t <= x_arr[i + 1]) break;

	return coeffs2[4 * i] + coeffs2[4 * i + 1] * (t - x_arr[i]) +
		coeffs2[4 * i + 2] * (t - x_arr[i]) * (t - x_arr[i]) +
		coeffs2[4 * i + 3] * (t - x_arr[i]) * (t - x_arr[i]) * (t - x_arr[i]);
}

#include "transient.h"
#include <math.h>

#define PI (3.141592653589793)

double calc_trans_exp(double i1,double i2,double td1,double tc1,double td2,double tc2,double t){

	if( t >= 0 && t <= td1){
		return i1;
	}
	else if (t >= td1 && t <= td2){
		double temp = -(t - td1)/tc1;

		return (i1 + (i2-i1)*(1-exp(temp)) );
	}
	else{
		double temp1 = -(t - td2)/tc2;
		double temp2 = -(t - td1)/tc1;
		
		return (i1 + (i2 - i1)*(exp(temp1)- exp(temp2)) );
	}
}


double calc_trans_sin(double i1,double ia,double fr,double td,double df,double ph,double t){

	if (t >= 0 && t <= td){
		double temp = 2*PI*ph/360;

		return (i1 +ia*sin(temp) );
	}
	else{
		double temp1 = 2*PI*fr*(t - td) + 2*PI*ph/360;
		double temp2 = -(t - td)*df;

		return (i1 + i1*sin(temp1)*exp(temp2) );
	}

}


double calc_trans_pulse(double i1,double i2,double td,double tr,double tf,double pw,double per,double t){

	int k = (int)floor( t / per );

	double lim1 = td;
	double lim2 = td + k*per;
	double lim3 = td + tr + k*per;
	double lim4 = td + tr + pw + k*per;
	double lim5 = td + tr + pw + tf + k*per;

	double slope,b;

	if( t >= 0 && t <= lim1){
		return i1;
	}
	else if(t >=  lim2 && t <= lim3){
		slope = (i2 - i1) / (lim3 - lim2);
		b = i1 - slope * lim2;
		return (slope * t + b);
	}
	else if(lim3 <= t && t <= lim4){
		return i2;
	}
	else if(lim4 <= t && t <= lim5){
		slope = (i2 - i1) / (lim4 - lim5);
		b = i1 - slope * lim5;
		return (slope * t + b);
	}
	else{
		return i1;
	}
}

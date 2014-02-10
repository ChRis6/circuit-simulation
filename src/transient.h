#ifndef TRANSIENT_H
#define TRANSIENT_H


double calc_trans_exp(double i1,double i2,double td1,double tc1,double td2,double tc2,double t);

double calc_trans_sin(double i1,double ia,double fr,double td,double df,double ph,double t);

double calc_trans_pulse(double i1,double i2,double td,double tr,double tf,double pw,double per,double t);

#endif
#include "stdafx.h"
#include "PID_Controller.h"

PID_Controller::PID_Controller(double lowBoundary, double highBoundary){
	lB = lowBoundary;
	hB = highBoundary;
	eOld = 0;
	E = 0;
	overshoot = false;
}

double PID_Controller::step(double setPoint, double currentValue, double Kp, double Tn, double Tv, double dt){
	//error
	e = setPoint - currentValue;

	//accumulate error (trapezoidal) if NOT overshoot
	if (!overshoot){
		E = E + (((e + eOld) / 2) * dt); 
	}

	// derivative error
	de = (e - eOld) / dt;	

	//output value (additive)
	u = Kp * e + (E / Tn) + (de * Tv);
			
	//checking for overshoot
	if (u>hB){	
		u = hB;
		overshoot = true;
	}
	else if (u<lB){
		u = lB;
		overshoot = true;
	}
	else
		overshoot = false;

	//prepare next step
	eOld = e;

	return u;
}
	
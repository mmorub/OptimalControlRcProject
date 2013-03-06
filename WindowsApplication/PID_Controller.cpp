#include "stdafx.h"
#include "PID_Controller.h"

PID_Controller::PID_Controller(double lowBoundary, double highBoundary){
	lB = lowBoundary;
	hB = highBoundary;
	error = 0;
	oldError = 0;
	E = 0;
	de = 0;
	overshoot = false;
}

double PID_Controller::step(double setPoint, double outputValue, double Kp, double Tn, double Tv, double dt){
	//error
	error = setPoint - outputValue;

	//accumulate error (trapezoidal) if NOT overshoot
	if (!overshoot){
		E = E + (((error + oldError) / 2) * dt); 
	}

	// derivative error
	de = (error - oldError) / dt;	

	//output value
	u = Kp * error + (E / Tn) + (de * Tv);
			
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
	oldError = error;

	return u;
}
	
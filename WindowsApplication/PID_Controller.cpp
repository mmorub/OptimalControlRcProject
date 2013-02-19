#include "stdafx.h"
#include "PID_Controller.h"

PID_Controller::PID_Controller(double lowBoundary, double highBoundary){
	lB = lowBoundary;
	hB = highBoundary;
	QueryPerformanceCounter(&oldTime);	//get current time
	QueryPerformanceFrequency(&freq);	//get ticks per sec
	error = 0;
	oldError = 0;
	E = 0;
	de = 0;
	overshoot = false;
}

double PID_Controller::step(double setPoint, double outputValue, double Kp, double Tn, double Tv){
	//get current time
	QueryPerformanceCounter(&time);	

	// elapsed time since last frame
	dt = (time.QuadPart - oldTime.QuadPart) *1000 / freq.QuadPart; 

	//error
	error = setPoint - outputValue;

	//accumulate error (trapezoidal) if NOT overshoot
	if (!overshoot){
		E = E + (((error + oldError) / 2) * dt/1000); 
	}

	// derivative error
	de = (error - oldError) / dt/1000;	

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
	oldTime = time;

	return u;
}
	
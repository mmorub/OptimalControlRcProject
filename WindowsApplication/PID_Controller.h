#include "stdafx.h"
#include <Windows.h>

class PID_Controller
{
public:
	double error;
	double oldError;	//previous step error
	double E;			//accumulated error
	double de;			//derivative error
	double dt;			//elapsed time since last frame
	double lB, hB;		//low/high boundary
	double Kp, Tn, Tv;	//proportional gain, integral time, derivative time
	double u;			//output
	bool overshoot;		//true if output exceedes boundary
public:
	PID_Controller(double lowBoundary, double highBoundary);
	double step(double setPoint, double outputValue, double Kp, double Tn, double Tv, double dt);
};
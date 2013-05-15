#include "stdafx.h"
#include <Windows.h>

///A PID-Controller
/**This class provides a simple PID-Controller with anti-windup.  */

class PID_Controller
{
private:
	double lB;		///<low boundary of controller output
	double hB;		///<high boundary of controller output
	double e;		///<error
	double eOld;	///<previous step error
	double E;		///<accumulated error
	double de;		///<derivative error
	double u;		///<controller output
	bool overshoot;	///<true if output exceedes boundaries
public:
	///Constructor
	/**After initialisation of an objekt from this class the vaules for low and high boundary (lB, hB)
	are set to the submitted parameters (lowBoundary, highBoundary). The variables eOld, E and overshoot
	are set to zero/false.*/
	PID_Controller(double lowBoundary, double highBoundary);

	///returns the controller output value
	/**This function calculates and returns the output value of the controller. When called the parameters
	setPoint, currentValue, Kp (proportional gain), Tn (integral time), Tv (derivative time) and dt
	(elapsed time since last frame) need to be passed, bacause they might have changed since the
	last call of this funktion. First the error is calculated by the difference of currentValue and setPoint.
	In the case of not exceeding the output boundaries (overschoot is false) the accumulated error is
	increased/decreased, using the trapezoidal rule \f$\Delta E=\frac{e+eOld}{2}dt\f$. The derivative error
	is calculated by \f$\frac{e-eOld}{dt}\f$. The controller output value u ist computed in the additive way
	\f$u=Kp*e+\frac{E}{Tn}+de*Tv\f$. Bevore the output value is returned the function checks for the case of
	an overshoot and prepairs the next call of the funktion by setting eOld to e.*/
	double step(double setPoint, double currentValue, double Kp, double Tn, double Tv, double dt);
};
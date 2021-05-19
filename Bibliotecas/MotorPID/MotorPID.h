#ifndef PID_h
#define PID_h

#include <Arduino.h>

class PID {

    public:
        PID();
        double compute(double PV);

		double Kp;
		double Ki;
		double Kd;
		double Setpoint;
		double DeadBand;
		double MaxMV;
		double MinMV;
    
    private:
        double error;
	    double lastSample;
	    double P, I, D;
		double MV;
	    long lastCompute;
};

#endif
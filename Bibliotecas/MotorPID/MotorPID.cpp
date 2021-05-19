#include "PID.h"

PID::PID() {
}

double PID::compute(double PV) {

	error = Setpoint - PV;

	float deltaTime = (millis() - lastCompute) / 1000.0;
	lastCompute = millis();

	if (abs(error) > DeadBand) {
		P = error * Kp;
		I = I + (error * Ki) * deltaTime;
		D = (lastSample - PV) * Kd / deltaTime;
	}

	lastSample = PV;

	return P + I + D + 50;
	//return map((P + I + D), (Setpoint - 100), Setpoint, 0, 100);
}
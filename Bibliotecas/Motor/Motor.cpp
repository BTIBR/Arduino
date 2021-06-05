#include "Motor.h"

Motor::Motor(int pFault, int pDir, int pSleep, int pReset, int pMode2, int pMode1, int pEnable, int pMode0, int pStep) {
    PinFault  = pFault;
    PinDir    = pStep;
    PinSleep  = pSleep;
    PinReset  = pReset;
    PinMode2  = pMode2;
    PinMode1  = pMode1;
    PinEnable = pEnable;
    PinMode0  = pMode0;
    PinStep   = pStep;
}

void Motor::setMode(int Mode) {                                   // Seleção do modo de operação do Motor
    if (Mode == "FULL") {                                         // Full Step
        digitalWrite(PinMode0, LOW);
        digitalWrite(PinMode1, LOW);
        digitalWrite(PinMode2, LOW);
    }
    else if (Mode == "HALF") {                                    // Half Step
        digitalWrite(PinMode0, HIGH);
        digitalWrite(PinMode1, LOW);
        digitalWrite(PinMode2, LOW);
    }
    else if (Mode == "1/4") {                                     // 1/4 Microstepping
        digitalWrite(PinMode0, LOW);
        digitalWrite(PinMode1, HIGH);
        digitalWrite(PinMode2, LOW);
    }
    else if (Mode == "1/8") {                                     // 1/8 Microstepping
        digitalWrite(PinMode0, HIGH);
        digitalWrite(PinMode1, HIGH);
        digitalWrite(PinMode2, LOW);
    }
    else if (Mode == "1/16") {                                    // 1/16 Microstepping
        digitalWrite(PinMode0, LOW);
        digitalWrite(PinMode1, LOW);
        digitalWrite(PinMode2, HIGH);
    }
    else if (Mode == "1/32") {                                    // 1/32 Microstepping
        digitalWrite(PinMode0, HIGH);
        digitalWrite(PinMode1, HIGH);
        digitalWrite(PinMode2, HIGH);
    }
}

void Motor::setPins() {
    pinMode(PinEnable, OUTPUT);
    pinMode(PinMode0 , OUTPUT);
    pinMode(PinMode1 , OUTPUT);
    pinMode(PinMode2 , OUTPUT);
    pinMode(PinReset , OUTPUT);
    pinMode(PinSleep , OUTPUT);
    pinMode(PinStep  , OUTPUT);
    pinMode(PinDir   , OUTPUT);
    pinMode(PinFault , INPUT);
}


void Motor::jog(byte _dir) {
    digitalWrite(PinEnable, LOW);
    digitalWrite(PinReset, HIGH);
    digitalWrite(PinSleep, HIGH);
    digitalWrite(PinStep, HIGH);
    digitalWrite(PinDir, _dir);
}











/*
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
*/
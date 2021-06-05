#ifndef Motor_h
#define Motor_h

#include <Arduino.h>

class Motor {                                                           // Classe de motor

    public:
        Motor(int pFault, int pDir, int pSleep, int pReset, int pMode2, int pMode1, int pEnable, int pMode0, int pStep);        // Método construtor
        void setMode(int Mode);                                         // Seleção do modo de funcionamento do motor
        void setPins();                                                 // Define automaticamente os pinos do Driver como IN/OUT
        void jog(byte _dir);
        float Vel;                                                      // Velocidade de movimento [°/s]
        int Dir;                                                        // Sentido de Giro: 0+ / 1-

    private:
        // Pinos do Driver DRV8825
        int PinFault;                                                   // Diagnóstico de Falha
        int PinDir;                                                     // Sentido de Giro
        int PinSleep;                                                   // Sleep Driver
        int PinReset;                                                   // Reset do Driver
        int PinMode2;                                                   // Seleção de modo de operação 2
        int PinMode1;                                                   // Seleção de modo de operação 1
        int PinEnable;                                                  // Enable Driver
        int PinMode0;                                                   // Seleção de modo de operação 0
        int PinStep;                                                    // Steps para o Motor
};

#endif






















/*
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



PID	    KEYWORD1

compute	KEYWORD2

Kp   LITERAL1
Ki   LITERAL1
Kd   LITERAL1
Setpoint   LITERAL1
DeadBand   LITERAL1
MaxMV   LITERAL1
MinMV   LITERAL1

*/
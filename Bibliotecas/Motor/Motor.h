#ifndef Motor_h
#define Motor_h

#include <Arduino.h>

class Motor {                                                           // Classe de motor

    public:
        Motor(int pEnable, int pMode0, int pMode1, int pMode2, int pReset, int pSleep, int pStep, int pDir, int pFault);        // Método construtor
        void setMode(int Mode);                                         // Seleção do modo de funcionamento do motor
        void setPins();                                                 // Define automaticamente os pinos do Driver como IN/OUT
        float Vel;                                                      // Velocidade de movimento [°/s]
        int Dir;                                                        // Sentido de Giro: 0+ / 1-

    private:
        // Pinos do Driver DRV8825
        int PinEnable;                                                  // Enable Driver
        int PinMode0;                                                   // Seleção de modo de operação 0
        int PinMode1;                                                   // Seleção de modo de operação 1
        int PinMode2;                                                   // Seleção de modo de operação 2
        int PinReset;                                                   // Reset do Driver
        int PinSleep;                                                   // Sleep Driver
        int PinStep;                                                    // Steps para o Motor
        int PinDir;                                                     // Sentido de Giro
        int PinFault;                                                   // Diagnóstico de Falha
};

#endif
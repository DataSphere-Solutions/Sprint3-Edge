// Wokwi Custom Chip - For docs and examples see:
// https://docs.wokwi.com/chips-api/getting-started
//
// SPDX-License-Identifier: MIT
// Copyright 2023 Ian Monteiro

#include "wokwi-api.h"
#include <stdio.h>
#include <stdlib.h>

uint8_t motorPin1, motorPin2; // Pinos para direção do motor

void setup() {
  // Configuração inicial do chip
  motorPin1 = pin_init("motorPin1", INPUT);  // Pino para sentido horário
  motorPin2 = pin_init("motorPin2", INPUT);  // Pino para sentido anti-horário
}

void loop() {
  // Ler o estado dos pinos (valores de PWM)
  int motorSpeed1 = pin_read(motorPin1);
  int motorSpeed2 = pin_read(motorPin2);

  if (motorSpeed1 > 0) {
    // Motor gira para frente
    printf("Motor girando para frente com velocidade %d\n", motorSpeed1);
  } else if (motorSpeed2 > 0) {
    // Motor gira para trás
    printf("Motor girando para trás com velocidade %d\n", motorSpeed2);
  } else {
    // Motor parado
    printf("Motor parado.\n");
  }
}


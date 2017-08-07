# Go-to-Goal

Algorithm of Go to Goal task for differential drive robot, easiest and control friendly of mobile robot kind, is developed and implemented on actual robot.


## Hardware

STM Nucleo board serves as a central controller having 32 bit ARM microcontroller STM32f446RE. The reason behind its selection among numbers of the available efficient microcontroller is its higher 168 MHz clock speed and commonly used peripheral's on chip availability.


## Code pre requisites 

* As Entire code had been developed in Keil environment, it is inevitable to get prior knowledge regarding it.
* Basic concept of Object Orientated Programming is inevitable as it is used as individual coordinate frame handler.

## How to use Code

**Kindly follow these steps**
1. Generate Keil uvision project as per STM 32-bit microcontroller and add main.cpp and user libraries.
2. Replace stm32f4xx_it.cpp and stm32f4xx_it.h with those provided here.
3. Other required changes in HAL libraries as per your STM controller are compulsory for code functionality.

## Friendly advice

* First, Learn the theory of unicycle approach of differential drive for go to goal task.
* Then, get familiar with programming of STM microcontroller using HAL drivers.
* Modulize your project in the subdivision as per your own convenience. That increases progression rate and ensures the better outcome.

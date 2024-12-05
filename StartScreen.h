#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include <cmath>
#include <iostream>
#include "Button.h"
#include <GL/freeglut.h>


struct StartScreen{

Button titleButton;
Button startButton;
Button AiButton;
Button humanButton;
Button threeButton;
Button fourButton;
Button fiveButton;

StartScreen(){
titleButton = Button(0.50f, 0.90f, 0.40f, 0.15f, "TIC-TAC-TOE!", false, false);
startButton = Button(0.75f, 0.90f, 0.40f, 0.15f, "START", false, false);
AiButton = Button(0.75f, 0.90f, 0.40f, 0.15f, "AI", false, false);
humanButton = Button(0.75f, 0.90f, 0.40f, 0.15f, "HUMAN", false, false);
threeButton = Button(0.75f, 0.90f, 0.40f, 0.15f, "3 x 3", false, false);
fourButton = Button(0.75f, 0.90f, 0.40f, 0.15f, "4 x 4", false, false);
fiveButton = Button(0.75f, 0.90f, 0.40f, 0.15f, "5 x 5", false, false);


}
};








#endif

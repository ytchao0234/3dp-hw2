//
// 3D Game Programming
// National Yang Ming Chiao Tung University (NYCU), Taiwan
// Instructor: Sai-Keung Wong
// Date: 2022/10/05
//
#include <iostream>
#include "SystemParameter.h"

using namespace std;

// Initialize the static member variables.
double SystemParameter::radius = 600;
double SystemParameter::maxSpeed = 1.5;
double SystemParameter::acceleration = 0.15;
int SystemParameter::maxNumParticleSystems = 60;
double SystemParameter::nearClipDistance = 5;

SystemParameter::SystemParameter() 
{
}

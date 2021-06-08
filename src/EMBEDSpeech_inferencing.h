/*
Author: Francis Kiptengwer Chemorion
Email: kchemorion@gmail.com
*/

#ifndef _INFERENCE_H
#define _INFERENCE_H

// Undefine min/max macros as these conflict with C++ std min/max functions
// these are often included by Arduino cores
#include <Arduino.h>
#ifdef min
#undef min
#endif // min
#ifdef max
#undef max
#endif // max
// Similar the ESP32 seems to define this, which is also used as an enum value in TFLite
#ifdef DEFAULT
#undef DEFAULT
#endif // DEFAULT

/* Includes ---------------------------------------------------------------- */
#include "edge-impulse-sdk/classifier/ei_run_classifier.h"
#include "edge-impulse-sdk/dsp/numpy.hpp"
#include "model-parameters/model_metadata.h"

extern void ei_printf(const char *format, ...);

#endif // _INFERENCE_H

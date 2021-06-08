/*
Author: Francis Kiptengwer Chemorion
Email: kchemorion@gmail.com
*/

#ifndef _EI_CLASSIFIER_CONFIG_H_
#define _EI_CLASSIFIER_CONFIG_H_

// clang-format off
#ifndef EI_CLASSIFIER_TFLITE_ENABLE_CMSIS_NN
#if defined(__MBED__)
    #include "mbed.h"
    #if (MBED_VERSION < MBED_ENCODE_VERSION(5, 7, 0))
        #define EI_CLASSIFIER_TFLITE_ENABLE_CMSIS_NN      0
    #else
        #define EI_CLASSIFIER_TFLITE_ENABLE_CMSIS_NN      1
    #endif // Mbed OS 5.7 version check
#elif defined(__TARGET_CPU_CORTEX_M0) || defined(__TARGET_CPU_CORTEX_M0PLUS) || defined(__TARGET_CPU_CORTEX_M3) || defined(__TARGET_CPU_CORTEX_M4) || defined(__TARGET_CPU_CORTEX_M7) || defined(ARDUINO_NRF52_ADAFRUIT)
    #define EI_CLASSIFIER_TFLITE_ENABLE_CMSIS_NN      1
#else
    #define EI_CLASSIFIER_TFLITE_ENABLE_CMSIS_NN      0
#endif
#endif // EI_CLASSIFIER_TFLITE_ENABLE_CMSIS_NN

// CMSIS-NN falls back to reference kernels when __ARM_FEATURE_DSP and __ARM_FEATURE_MVE are not defined
// we should never use those... So disable CMSIS-NN in that case and throw a warning
#if EI_CLASSIFIER_TFLITE_ENABLE_CMSIS_NN == 1
    #if !defined(__ARM_FEATURE_DSP) && !defined(__ARM_FEATURE_MVE)
        #pragma message( \
            "CMSIS-NN enabled, but neither __ARM_FEATURE_DSP nor __ARM_FEATURE_MVE defined. Falling back.")
        #undef EI_CLASSIFIER_TFLITE_ENABLE_CMSIS_NN
        #define EI_CLASSIFIER_TFLITE_ENABLE_CMSIS_NN 0
    #endif
#endif // EI_CLASSIFIER_TFLITE_ENABLE_CMSIS_NN == 1

#if EI_CLASSIFIER_TFLITE_ENABLE_CMSIS_NN == 1
#define CMSIS_NN                    1
#endif

#ifndef EI_CLASSIFIER_TFLITE_ENABLE_ARC
#ifdef CPU_ARC
#define EI_CLASSIFIER_TFLITE_ENABLE_ARC             1
#else
#define EI_CLASSIFIER_TFLITE_ENABLE_ARC             0
#endif // CPU_ARC
#endif // EI_CLASSIFIER_TFLITE_ENABLE_ARC

// clang-format on
#endif // _EI_CLASSIFIER_CONFIG_H_

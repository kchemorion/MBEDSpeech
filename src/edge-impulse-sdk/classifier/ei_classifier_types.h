/*
Author: Francis Kiptengwer Chemorion
Email: kchemorion@gmail.com
*/
#ifndef _EDGE_IMPULSE_RUN_CLASSIFIER_TYPES_H_
#define _EDGE_IMPULSE_RUN_CLASSIFIER_TYPES_H_

#include <stdint.h>
#include "model-parameters/model_metadata.h"

typedef struct {
    const char *label;
    float value;
} ei_impulse_result_classification_t;

typedef struct {
    const char *label;
    uint32_t x;
    uint32_t y;
    uint32_t width;
    uint32_t height;
    float value;
} ei_impulse_result_bounding_box_t;

typedef struct {
    int sampling;
    int dsp;
    int classification;
    int anomaly;
} ei_impulse_result_timing_t;

typedef struct {
#if EI_CLASSIFIER_OBJECT_DETECTION == 1
    ei_impulse_result_bounding_box_t bounding_boxes[EI_CLASSIFIER_OBJECT_DETECTION_COUNT];
#else
    ei_impulse_result_classification_t classification[EI_CLASSIFIER_LABEL_COUNT];
#endif
    float anomaly;
    ei_impulse_result_timing_t timing;
} ei_impulse_result_t;

typedef struct {
    uint32_t buf_idx;
    float running_sum;
#if (EI_CLASSIFIER_SLICES_PER_MODEL_WINDOW > 1)
    float maf_buffer[(EI_CLASSIFIER_SLICES_PER_MODEL_WINDOW >> 1)];
#else
    float maf_buffer[1];
#endif
}ei_impulse_maf;

#endif // _EDGE_IMPULSE_RUN_CLASSIFIER_TYPES_H_

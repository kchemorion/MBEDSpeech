/*
Author: Francis Kiptengwer Chemorion
Email: kchemorion@gmail.com
*/

#ifndef _EDGE_IMPULSE_MODEL_TYPES_H_
#define _EDGE_IMPULSE_MODEL_TYPES_H_

#include <stdint.h>
#include "edge-impulse-sdk/dsp/numpy.hpp"

typedef struct {
    size_t n_output_features;
    int (*extract_fn)(ei::signal_t *signal, ei::matrix_t *output_matrix, void *config, const float frequency);
    void *config;
    uint8_t *axes;
    size_t axes_size;
} ei_model_dsp_t;

typedef struct {
    size_t n_output_features;
    int (*extract_fn)(ei::signal_i16_t *signal, ei::matrix_i32_t *output_matrix, void *config, const float frequency);
    void *config;
    uint8_t *axes;
    size_t axes_size;
} ei_model_dsp_i16_t;

#endif // _EDGE_IMPULSE_MODEL_TYPES_H_

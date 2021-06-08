/*
Author: Francis Kiptengwer Chemorion
Email: kchemorion@gmail.com
*/

#ifndef _EI_CLASSIFIER_DSP_BLOCKS_H_
#define _EI_CLASSIFIER_DSP_BLOCKS_H_

#include "model-parameters/model_metadata.h"
#include "edge-impulse-sdk/classifier/ei_run_dsp.h"
#include "edge-impulse-sdk/classifier/ei_model_types.h"

const size_t ei_dsp_blocks_size = 1;
ei_model_dsp_t ei_dsp_blocks[ei_dsp_blocks_size] = {
    { // DSP block 3
        650,
        &extract_mfcc_features,
        (void*)&ei_dsp_config_3,
        ei_dsp_config_3_axes,
        ei_dsp_config_3_axes_size
    }
};

const size_t ei_dsp_blocks_i16_size = 0;
ei_model_dsp_i16_t ei_dsp_blocks_i16[ei_dsp_blocks_i16_size] = {

};

#endif // _EI_CLASSIFIER_DSP_BLOCKS_H_
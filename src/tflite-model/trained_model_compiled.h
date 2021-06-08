/*
Author: Francis Kiptengwer Chemorion
Email: kchemorion@gmail.com
*/

#ifndef trained_model_GEN_H
#define trained_model_GEN_H

#include "edge-impulse-sdk/tensorflow/lite/c/common.h"

// Sets up the model with init and prepare steps.
TfLiteStatus trained_model_init( void*(*alloc_fnc)(size_t,size_t) );
// Returns the input tensor with the given index.
TfLiteTensor *trained_model_input(int index);
// Returns the output tensor with the given index.
TfLiteTensor *trained_model_output(int index);
// Runs inference for the model.
TfLiteStatus trained_model_invoke();
//Frees memory allocated
TfLiteStatus trained_model_reset( void (*free)(void* ptr) );


// Returns the number of input tensors.
inline size_t trained_model_inputs() {
  return 1;
}
// Returns the number of output tensors.
inline size_t trained_model_outputs() {
  return 1;
}

inline void *trained_model_input_ptr(int index) {
  return trained_model_input(index)->data.data;
}
inline size_t trained_model_input_size(int index) {
  return trained_model_input(index)->bytes;
}
inline int trained_model_input_dims_len(int index) {
  return trained_model_input(index)->dims->data[0];
}
inline int *trained_model_input_dims(int index) {
  return &trained_model_input(index)->dims->data[1];
}

inline void *trained_model_output_ptr(int index) {
  return trained_model_output(index)->data.data;
}
inline size_t trained_model_output_size(int index) {
  return trained_model_output(index)->bytes;
}
inline int trained_model_output_dims_len(int index) {
  return trained_model_output(index)->dims->data[0];
}
inline int *trained_model_output_dims(int index) {
  return &trained_model_output(index)->dims->data[1];
}

#endif

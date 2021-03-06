/*
Author: Francis Kiptengwer Chemorion
Email: kchemorion@gmail.com
*/

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "edge-impulse-sdk/tensorflow/lite/c/builtin_op_data.h"
#include "edge-impulse-sdk/tensorflow/lite/c/common.h"
#include "edge-impulse-sdk/tensorflow/lite/micro/kernels/micro_ops.h"

#if EI_CLASSIFIER_PRINT_STATE
#if defined(__cplusplus) && EI_C_LINKAGE == 1
extern "C" {
    extern void ei_printf(const char *format, ...);
}
#else
extern void ei_printf(const char *format, ...);
#endif
#endif

#if defined __GNUC__
#define ALIGN(X) __attribute__((aligned(X)))
#elif defined _MSC_VER
#define ALIGN(X) __declspec(align(X))
#elif defined __TASKING__
#define ALIGN(X) __align(X)
#endif

namespace {

constexpr int kTensorArenaSize = 1616;

#if defined(EI_CLASSIFIER_ALLOCATION_STATIC)
uint8_t tensor_arena[kTensorArenaSize] ALIGN(16);
#elif defined(EI_CLASSIFIER_ALLOCATION_STATIC_HIMAX)
#pragma Bss(".tensor_arena")
uint8_t tensor_arena[kTensorArenaSize] ALIGN(16);
#pragma Bss()
#elif defined(EI_CLASSIFIER_ALLOCATION_STATIC_HIMAX_GNU)
uint8_t tensor_arena[kTensorArenaSize] ALIGN(16) __attribute__((section(".tensor_arena")));
#else
#define EI_CLASSIFIER_ALLOCATION_HEAP 1
uint8_t* tensor_arena = NULL;
#endif

static uint8_t* tensor_boundary;
static uint8_t* current_location;

template <int SZ, class T> struct TfArray {
  int sz; T elem[SZ];
};
enum used_operators_e {
  OP_RESHAPE, OP_CONV_2D, OP_MAX_POOL_2D, OP_FULLY_CONNECTED, OP_SOFTMAX,  OP_LAST
};
struct TensorInfo_t { // subset of TfLiteTensor used for initialization from constant memory
  TfLiteAllocationType allocation_type;
  TfLiteType type;
  void* data;
  TfLiteIntArray* dims;
  size_t bytes;
  TfLiteQuantization quantization;
};
struct NodeInfo_t { // subset of TfLiteNode used for initialization from constant memory
  struct TfLiteIntArray* inputs;
  struct TfLiteIntArray* outputs;
  void* builtin_data;
  used_operators_e used_op_index;
};

TfLiteContext ctx{};
TfLiteTensor tflTensors[23];
TfLiteRegistration registrations[OP_LAST];
TfLiteNode tflNodes[11];

const TfArray<2, int> tensor_dimension0 = { 2, { 1,650 } };
const TfArray<1, float> quant0_scale = { 1, { 0.049485228955745697, } };
const TfArray<1, int> quant0_zero = { 1, { -3 } };
const TfLiteAffineQuantization quant0 = { (TfLiteFloatArray*)&quant0_scale, (TfLiteIntArray*)&quant0_zero, 0 };
const ALIGN(8) int32_t tensor_data1[2] = { -1, 208, };
const TfArray<1, int> tensor_dimension1 = { 1, { 2 } };
const ALIGN(8) int32_t tensor_data2[8] = { 690, -4254, -7842, -6531, -1294, -11405, 476, -4073, };
const TfArray<1, int> tensor_dimension2 = { 1, { 8 } };
const TfArray<8, float> quant2_scale = { 8, { 0.0010420500766485929, 0.00017410548753105104, 0.00045044888975098729, 0.00020763227075804025, 0.00035473730531521142, 0.00027495878748595715, 0.001453903503715992, 0.00023340033658314496, } };
const TfArray<8, int> quant2_zero = { 8, { 0,0,0,0,0,0,0,0 } };
const TfLiteAffineQuantization quant2 = { (TfLiteFloatArray*)&quant2_scale, (TfLiteIntArray*)&quant2_zero, 0 };
const ALIGN(8) int32_t tensor_data3[16] = { -1671, -512, -545, 86, 14, 206, 16, 723, -55, -128, 106, -422, -2118, 8, 23, 584, };
const TfArray<1, int> tensor_dimension3 = { 1, { 16 } };
const TfArray<16, float> quant3_scale = { 16, { 0.00097612477838993073, 0.0014863319229334593, 0.001454194774851203, 0.0016583396354690194, 0.00039336446207016706, 0.0026251389645040035, 0.0040546278469264507, 0.00096584530547261238, 0.00086562760407105088, 0.0024669028352946043, 0.0017316316952928901, 0.0022658410016447306, 0.00041167644667439163, 0.0027242940850555897, 0.0024788565933704376, 0.00037892287946306169, } };
const TfArray<16, int> quant3_zero = { 16, { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 } };
const TfLiteAffineQuantization quant3 = { (TfLiteFloatArray*)&quant3_scale, (TfLiteIntArray*)&quant3_zero, 0 };
const ALIGN(8) int32_t tensor_data4[12] = { -3154, -257, 389, -2077, 5282, 767, -923, -931, 13, -1591, 2923, -1236, };
const TfArray<1, int> tensor_dimension4 = { 1, { 12 } };
const TfArray<1, float> quant4_scale = { 1, { 0.00070917553966864944, } };
const TfArray<1, int> quant4_zero = { 1, { 0 } };
const TfLiteAffineQuantization quant4 = { (TfLiteFloatArray*)&quant4_scale, (TfLiteIntArray*)&quant4_zero, 0 };
const ALIGN(8) int8_t tensor_data5[12*208] = { 
  8, -15, -74, 20, -5, -12, 0, -4, 1, -6, 2, -41, -2, -11, -79, 4, 8, -6, -10, 19, -3, 13, -17, -7, -37, -3, -1, -38, -7, -5, 6, 1, 7, -4, -26, 23, -2, 9, 3, 0, -21, 0, 13, -46, 2, -12, 12, 8, 5, -9, -29, 20, 0, 19, -9, -13, -8, -4, 18, -32, 7, 8, 14, 3, 3, -4, -33, 19, 0, 22, -4, -20, 4, -1, 5, -25, 7, 13, 10, 10, 1, -7, -26, 19, 1, 21, -3, -18, 2, -2, 7, -29, 11, 7, 12, 13, 6, -8, -27, 21, 4, 21, -6, -16, -1, -5, 8, -31, 6, 11, 13, 19, 2, -9, -36, 15, 0, 19, -6, -11, 3, -15, 7, -24, 8, 16, 13, 13, -2, -5, -24, 22, -8, 18, -8, -19, 6, -8, 4, -18, 10, 10, 7, 21, -13, -12, -40, 14, -16, 20, 3, -40, 2, -14, -4, -33, 0, 11, 11, 13, -12, -7, -32, 15, -24, 22, 4, -36, 9, -8, -3, -19, 8, 6, 3, 11, -22, -20, -18, -7, -8, 18, 1, -14, 0, -9, -5, -28, 5, 5, 7, 12, -32, -24, -18, -29, -10, -24, -58, -25, -31, -21, -23, -22, -118, -3, 10, 2, 
  -30, -26, -22, 1, -5, -7, -7, -5, -1, -33, 30, -51, 4, -11, -31, 3, -16, -6, -13, 11, -3, -6, 2, -13, -16, -7, -15, -19, 12, -29, 8, -5, -24, -5, -17, 13, -6, -11, 11, -25, -10, -4, -9, -12, 5, 10, 9, -1, -9, 1, -8, 5, -5, -16, 10, -38, -18, -5, -13, -19, 8, 10, 10, -1, -1, -3, -5, 11, -6, -12, 15, -29, -31, -15, -10, -16, 14, 15, 10, -3, -10, -4, -14, 17, -8, -8, 17, -33, -25, -10, -11, -20, 15, 13, 9, -4, -12, 0, -15, 16, -2, 1, 16, -44, -16, -16, -12, -20, 11, 13, 7, -1, -5, 0, -8, 15, -8, -1, 17, -51, -15, -8, -6, -19, 12, 15, 12, -3, -4, 6, -19, 11, -15, -4, 8, -44, -21, -8, -7, -14, 7, 11, 11, -9, -12, 7, -37, 9, -19, -6, 13, -37, -16, -14, -9, -22, 13, 17, 14, -11, 2, 9, -64, 7, -39, -2, 15, -41, -16, -30, -13, -17, 6, 14, 15, -18, -4, -11, -33, -7, -15, -11, 1, -23, -23, -24, -18, -24, -5, 17, 5, -17, -40, -25, -14, -31, -16, -21, -18, -11, -24, -26, -40, -20, -11, 1, -48, -13, 
  -34, 11, -38, -18, -12, -14, -3, -9, 4, 13, -10, 0, -15, -2, -20, -11, -9, 8, -20, -14, -7, -13, -22, -26, 6, 12, -1, 11, -19, -17, -23, -11, -5, 12, -22, -5, -18, -22, -35, -9, 3, 12, 14, 9, -10, -24, -45, -7, -7, 10, -21, 9, -4, -16, -33, 5, 2, 12, 21, 8, -6, -32, -39, -9, -22, 13, -16, 7, -8, -22, -25, 12, 5, 16, 13, 8, -20, -27, -35, -10, -17, 9, -14, 2, -8, -19, -32, 7, -1, 16, 26, 8, -25, -38, -34, -3, -17, 9, -19, -2, 5, -16, -27, 9, 4, 14, 18, 14, -24, -44, -31, -13, -27, 12, -27, 3, 5, -22, -34, 6, 5, 21, 18, 11, -19, -43, -35, -13, -56, 11, -26, -6, 5, -19, -39, 8, -1, 12, 20, 4, -20, -47, -48, -27, -26, 10, -17, -14, 2, -32, -41, 7, 1, 19, 13, 4, -17, -59, -47, -21, -58, 2, -23, -3, 15, -33, -48, -4, 3, 15, 15, 4, -18, -60, -35, -13, -61, -7, -29, -12, 10, -23, -25, 0, 0, -6, 11, -16, -35, -46, -45, -8, -17, -36, -7, -1, 1, -20, 3, -4, -15, -18, 2, -11, -7, -23, -71, 1, 
  8, 7, -55, -46, -9, -3, -6, 1, -2, 13, 20, -21, -24, -27, -53, 6, 8, 17, -23, -2, -8, -19, -29, 5, -10, 15, -29, -33, 6, 4, -2, 18, 11, 6, -29, 0, -17, -12, -1, -9, -17, 11, -6, -33, 5, -5, 15, 15, 13, 5, -31, -1, -24, 1, 4, -20, -21, 14, 7, -21, 7, 9, 12, 22, 9, 11, -46, 0, -17, -6, 5, -17, -16, 18, 2, -21, 10, 7, 9, 19, 12, 3, -36, 1, -26, -8, 6, -17, -20, 13, 1, -26, 9, 11, 13, 16, 11, 5, -58, 4, -22, -5, 6, -27, -19, 10, -6, -20, 7, 6, 13, 18, 6, 7, -35, 3, -35, -4, 10, -23, -22, 16, -2, -28, 10, 8, 12, 14, 3, 6, -30, 10, -19, 5, 11, -29, -24, 12, 0, -15, 6, 9, 11, 13, -10, 4, -55, 7, -23, -3, 11, -27, -17, 13, -6, -23, 9, 10, 8, 7, 1, 3, -42, -1, -34, -3, 12, -41, -19, 9, -5, -19, 6, 11, 12, -4, 7, -15, -25, -29, -14, -4, 2, -23, -28, 11, -9, -26, 4, 11, 12, -16, -13, -25, -1, -20, 0, -13, -101, -25, -89, -15, -7, -11, 12, 5, -9, -11, 
  -17, -53, -33, -13, 11, -12, -22, -3, 6, -60, 6, -25, -18, -12, -41, -10, 3, -47, -16, -28, 5, -14, -26, -2, 7, -41, -5, -4, -21, -14, -46, -15, -16, -50, -19, -28, 9, -35, -41, -3, -7, -29, -4, -2, -23, -21, -76, -27, -36, -44, -11, -30, 8, -30, -33, -3, -6, -36, -15, -10, -25, -28, -82, -12, -20, -45, -4, -27, 3, -31, -64, -5, -5, -34, -17, -21, -20, -31, -85, -17, -19, -52, -3, -43, 0, -42, -60, -10, 0, -43, -13, -18, -18, -22, -65, -13, -40, -49, -13, -38, 6, -34, -44, -10, -8, -51, -14, -9, -15, -41, -79, -16, -33, -43, -9, -34, 4, -47, -37, -15, -7, -44, -10, -16, -38, -35, -72, -19, -5, -40, -11, -36, 3, -50, -37, -8, -4, -32, -34, -14, -30, -31, -71, -28, -12, -24, -8, -34, 6, -33, -46, -8, -9, -29, -5, -13, -23, -43, -79, -21, -15, -22, -10, -28, 8, -13, -35, -12, -4, -26, -16, -10, -14, -28, -61, -15, -19, -36, -11, -14, 8, -21, -42, -6, 3, -42, -8, -31, -28, -21, -39, -5, 1, -4, -6, -2, 1, -16, 2, 8, -9, -12, -13, 25, -41, -15, -20, -11, 
  -12, -23, 18, -74, -11, -9, 3, -7, 0, -59, -44, 2, -31, -4, 8, -11, -8, -20, 0, -56, -16, 22, 3, -35, -21, -30, -19, 4, -24, 12, 10, -13, -62, -15, 16, -78, -28, 12, -7, -30, -36, -22, -15, 10, -24, 4, 8, -18, -28, -4, 15, -41, -9, 8, -4, -9, -26, -21, -19, 3, -16, 5, 5, -32, -1, -10, 19, -30, -14, 2, -7, 6, -39, -32, -13, 5, -15, 1, 13, -35, -22, -13, 17, -39, -11, 5, -4, 14, -37, -33, -9, 4, -23, -3, 7, -27, -9, -11, 20, -37, -9, 1, 3, 18, -37, -42, -12, 4, -18, 0, 4, -39, -6, -4, 16, -41, -10, -2, -5, 20, -30, -25, -3, 8, -24, 0, 12, -43, -19, -16, 12, -44, -8, -4, -11, 17, -34, -39, -7, 5, -22, -4, 11, -31, -12, -15, 2, -33, -1, -5, -10, 17, -30, -30, -3, 2, -17, -3, 11, -20, -5, -19, 0, -23, -3, -7, -15, 22, -32, -38, 3, -5, -18, -7, 13, -23, -13, -12, -11, -34, -12, -11, -32, 15, -10, -39, 3, -8, -35, -5, 8, -10, -38, -28, -30, -22, -1, -33, -27, -1, -45, -10, 3, -27, -7, 2, -13, 3, 
  -22, -29, 14, -49, -8, 5, -23, -2, 6, -58, -13, 2, -32, 0, 9, -9, -12, -23, 4, -54, -8, 15, -1, -2, -26, -37, -14, 7, -18, 3, 4, -8, -13, -23, 12, -39, -9, 24, 11, -20, 11, -21, -24, 8, -27, -1, 6, -22, -31, -21, 18, -48, -24, 17, 3, -13, 18, -21, -28, 4, -23, -5, 5, -9, 2, -18, 19, -49, -24, 19, 9, -15, 20, -24, -30, 7, -38, -6, 6, -7, -17, -21, 13, -62, -25, 17, 9, -28, 16, -24, -25, 7, -30, -2, 2, 8, -12, -15, 20, -46, -24, 22, 2, -22, 13, -19, -23, 5, -15, -8, 4, 11, -5, -11, 15, -52, -15, 20, 3, -14, 15, -18, -23, 5, -29, -3, 4, 10, 2, -12, 15, -42, -24, 17, 2, -23, 15, -14, -26, 5, -24, -12, -3, 13, -4, -9, 10, -31, -17, 18, 10, -22, 13, -5, -17, 1, -24, -10, -3, 10, -1, -3, -4, -56, -35, 18, -1, -19, 16, -12, -26, 1, -62, -13, -9, 10, -11, -22, -7, -41, -17, 9, 1, -14, 8, -8, -52, -5, -68, -23, -5, 20, -38, -14, -15, -15, -13, -20, -2, -24, -2, -21, -21, -39, 30, -28, -47, -2, 
  -123, -10, -16, -17, -18, -8, -21, 0, -8, 13, 1, 7, -5, -5, -13, -10, -66, -1, -6, -38, 1, -17, 23, -3, 23, 14, -11, 15, -19, -29, -31, -19, -11, -5, -6, -24, -2, -1, 23, 0, 22, 11, -18, 15, -14, -21, -24, -22, 4, -12, -11, -15, 2, -11, 21, -5, 15, 12, -10, 8, -23, -21, -43, -21, 0, -11, 0, -24, 2, 0, 22, -19, 21, 13, -18, 11, -14, -14, -36, -17, 8, -10, -11, -27, -3, -4, 23, -9, 18, 11, -16, 7, -14, -33, -35, -19, 17, -15, -1, -32, 9, -11, 23, -5, 19, 12, -29, 13, -7, -27, -36, -18, 17, -13, 2, -32, 4, -6, 22, 0, 19, 19, -31, 11, -15, -27, -37, -17, 16, -15, -4, -39, 8, -11, 16, -1, 21, 11, -25, 7, -18, -29, -40, -22, 7, -15, 1, -30, 8, -9, 21, 3, 18, 13, -35, 4, -7, -19, -39, -19, 4, -16, -9, -30, 2, -21, 17, 6, 11, -1, -33, 3, -23, -29, -50, -13, -1, -17, -2, -10, 2, -24, 11, 8, 13, 3, -28, -18, -14, -41, -93, -13, 9, -5, -1, -13, -13, -48, -5, 7, 15, -20, -19, -37, -6, -41, -82, -11, 
  -86, -48, -37, -9, 11, -7, -23, 8, -10, -28, -49, -41, -3, -20, -6, -14, -75, -32, -16, -7, 16, -59, -8, 19, 2, -23, 1, -39, 7, -31, -1, -20, -63, -33, -15, -7, 13, -35, -49, 21, -11, -40, -14, -20, 7, -18, -1, -7, -79, -26, -13, -3, 17, -17, -44, 19, -4, -28, 1, -18, 10, -10, 11, -14, -58, -34, -5, 4, 16, -20, -35, 14, -23, -33, 3, -21, 6, -1, 9, -9, -53, -19, -13, -2, 19, -12, -26, 14, -10, -17, 2, -17, 7, 3, 5, -7, -63, -18, -13, 0, 15, -11, -32, 7, -19, -27, 2, -13, 7, 2, 9, -15, -51, -20, -12, -2, 7, -13, -28, 3, -23, -35, 4, -16, 9, 2, 10, -17, -55, -18, -15, 6, 2, -6, -32, -3, -26, -10, 10, -18, 7, 0, 11, -8, -41, -13, -11, 11, -1, -12, -26, -22, -32, -38, 8, -15, 7, -5, 10, -14, -15, -17, -3, 16, -23, -15, -34, -20, -22, -30, 11, -15, -3, -7, 7, -14, -34, -20, -9, -4, -21, -20, -55, -37, -31, -45, 9, -28, 2, -5, 7, -8, -8, -19, -30, -67, -14, -7, -55, -30, -55, -4, -2, -44, -127, -11, -2, -8, 
  -21, 7, -3, -14, -10, -6, -6, 7, -3, -1, 3, -12, -16, -36, -30, -14, -25, -6, 3, -8, -1, -7, -5, 16, 12, 0, -33, -15, -2, -47, -11, -12, -8, 4, 3, -5, 5, 1, 11, 10, 8, -9, -22, -8, 1, -11, 8, -17, 4, 3, 1, -9, 5, -6, 7, 9, 12, -5, -17, -5, 2, -7, 8, 1, -3, -1, -3, -7, 10, 0, 8, 8, 10, -1, -18, -6, 4, 0, 1, 0, 7, 1, -13, -8, 9, 7, 6, 3, 10, -2, -11, -9, 4, -11, 0, 4, 5, 2, 1, -5, 6, 4, 4, 0, 14, -4, 2, 5, 4, -4, 0, -2, -5, 5, 1, -2, 7, 4, -1, -1, 14, -3, -11, 1, 3, -6, -3, -1, -1, 5, -5, -5, 8, 5, 2, -1, 15, 3, -9, -7, -5, -6, -6, -2, 3, 11, -8, -8, -1, -1, -3, -4, 11, -8, -5, -3, -5, -1, -9, 0, -5, 5, 7, -8, -4, 0, 8, 0, 10, -18, -12, -2, -1, 1, -5, -7, -1, -5, -34, -15, -10, -2, 15, -5, 1, -10, -6, -23, 8, -6, -1, -5, -3, -11, -11, -16, -22, -7, -21, -13, -37, -10, -37, -12, -63, -11, -39, -3, 
  -25, -25, 8, -33, -4, -6, -19, 3, 0, -25, -25, -7, 2, -12, 5, -3, -18, -39, 4, -33, -20, -27, -5, -10, 0, -17, -4, -2, 1, -13, 4, -22, -47, -39, 12, -27, -22, -14, -20, -11, -8, -23, 9, 0, 7, -16, 7, -23, -78, -24, 14, -44, -21, -25, -12, -28, -20, -29, 0, -5, 8, -13, 5, -28, -12, -36, 18, -24, -14, -26, -11, -31, -17, -30, 14, -2, 10, 0, 1, -20, -12, -37, 15, -35, -13, -25, -12, -18, -22, -34, 4, -3, 10, -14, 5, -14, -8, -32, 16, -27, -23, -25, -7, -15, -10, -30, -3, 0, 9, -3, 3, -16, -31, -36, 12, -27, -20, -38, -14, -15, -21, -23, 4, -7, 11, -12, 4, -15, -15, -24, 13, -33, -20, -21, -11, -11, -10, -23, 10, -4, -1, -12, 5, -13, -2, -30, 10, -24, -19, -27, -29, -14, -17, -28, 11, -8, 2, -15, 1, -9, -4, -56, 4, -36, -14, -30, -19, -5, -13, -24, 3, -11, 7, -23, -6, -9, -12, -20, -3, -48, -18, -30, -42, -8, -12, -28, 5, -42, -15, -11, -7, 5, -33, -12, -22, -23, -17, -17, -74, -6, -30, -18, -2, -44, -38, -13, -22, -4, 
  1, 11, -60, -5, -7, 0, -6, -16, -9, 11, -3, -28, -2, -20, -27, -1, 23, 17, -38, 24, 1, -21, -22, -15, -22, 7, -21, -27, -22, -8, -39, 2, 22, 14, -93, 15, -1, -7, -16, -4, -23, 6, -6, -23, -16, -4, -54, 4, 24, 12, -95, 21, 2, -11, -19, 9, -21, 11, -29, -30, -3, -11, -58, 7, 21, 18, -52, 18, 4, -9, -33, 17, -15, 11, -33, -33, -12, -26, -65, -3, 23, 13, -25, 19, 1, -10, -31, 21, -23, 5, -17, -27, -11, -23, -79, 6, 25, 14, -78, 15, 8, -4, -44, 20, -23, 7, -10, -36, -29, -33, -62, -1, 22, 14, -51, 10, 11, 1, -42, 24, -28, 9, -20, -28, -18, -38, -68, -4, 23, 19, -41, 8, -1, 4, -22, 27, -22, 2, -11, -21, -17, -41, -37, -2, 12, 9, -29, 14, 7, -7, -10, 29, -23, 10, -14, -46, -20, -42, -47, -9, -4, 9, -19, 9, 4, -18, -25, 31, -22, 2, -18, -38, -26, -50, -89, -20, 21, 11, -88, 11, 1, -10, -26, 26, -2, -5, -9, -31, -30, -45, -68, -20, -16, -22, 1, -7, -30, -13, 8, 22, -6, -30, -32, -25, -54, -63, -48, -1, 
};
const TfArray<2, int> tensor_dimension5 = { 2, { 12,208 } };
const TfArray<1, float> quant5_scale = { 1, { 0.027251444756984711, } };
const TfArray<1, int> quant5_zero = { 1, { 0 } };
const TfLiteAffineQuantization quant5 = { (TfLiteFloatArray*)&quant5_scale, (TfLiteIntArray*)&quant5_zero, 0 };
const ALIGN(8) int8_t tensor_data6[8*1*3*13] = { 
  /* [0][0][][] */ -127,-10,25,2,-6,-9,9,3,-9,-2,0,0,-2, -55,-22,-34,0,9,4,-1,-7,1,-2,-3,-9,1, 70,12,2,0,11,7,2,5,1,3,1,5,-4, 
  /* [1][0][][] */ 9,31,127,-14,-42,-49,-54,-14,20,9,34,34,-11, 61,54,120,-12,-70,-20,-94,-28,12,20,14,36,-20, 2,64,102,-9,-112,27,-74,-3,-9,9,33,30,-30, 
  /* [2][0][][] */ -10,32,-53,-10,58,-48,-14,41,-13,-33,15,24,-39, 41,-7,-35,22,-19,-17,33,3,-17,-3,14,-14,10, 127,-12,29,34,-87,19,31,-15,-16,8,-8,-15,17, 
  /* [3][0][][] */ -84,83,6,-85,34,24,-46,-23,16,20,20,2,-3, 8,82,-55,-29,26,5,7,-34,9,12,20,-11,-26, 127,89,15,-83,50,-24,-32,1,-3,16,1,10,16, 
  /* [4][0][][] */ -127,66,17,64,13,2,-26,7,-8,-7,0,-25,-3, 5,74,39,58,11,-8,-31,3,0,6,17,-2,1, -48,55,81,64,2,-3,-24,11,-15,3,-1,5,-10, 
  /* [5][0][][] */ 127,-60,5,46,-13,-19,34,16,-57,0,23,-25,3, 55,6,-4,5,23,-27,6,57,-15,-40,12,21,-15, 91,76,-60,-69,64,-6,-61,38,54,-39,-50,10,20, 
  /* [6][0][][] */ -26,3,5,1,5,0,-2,0,0,3,5,1,0, 102,7,4,1,-1,0,-1,2,-4,-3,1,-1,5, -127,-18,-9,-7,3,-1,0,-4,5,0,-4,-2,-1, 
  /* [7][0][][] */ -83,-81,56,18,79,-42,27,-4,15,-3,4,6,-19, 104,-127,85,12,-14,-54,-1,-44,19,-7,20,-1,15, 112,65,94,-36,-67,-54,-22,14,34,16,11,-10,3, 
};
const TfArray<4, int> tensor_dimension6 = { 4, { 8,1,3,13 } };
const TfArray<8, float> quant6_scale = { 8, { 0.021057801321148872, 0.0035183324944227934, 0.0091026937589049339, 0.0041958433575928211, 0.0071685495786368847, 0.0055563813075423241, 0.029380554333329201, 0.0047165658324956894, } };
const TfArray<8, int> quant6_zero = { 8, { 0,0,0,0,0,0,0,0 } };
const TfLiteAffineQuantization quant6 = { (TfLiteFloatArray*)&quant6_scale, (TfLiteIntArray*)&quant6_zero, 0 };
const ALIGN(8) int8_t tensor_data7[16*1*3*8] = { 
  /* [0][0][][] */ -1,61,-37,-23,-2,-127,-11,-6, -9,-17,-33,-41,-10,-40,0,3, 1,2,-13,-59,-13,17,1,-22, 
  /* [1][0][][] */ -4,22,-4,8,8,-31,-19,3, -9,-2,-6,1,6,-9,-127,7, -40,11,6,3,-13,8,-35,0, 
  /* [2][0][][] */ 16,-46,-6,8,-28,2,-8,-17, -4,-36,3,3,-52,0,-127,-12, -86,-6,0,5,-68,8,-57,-7, 
  /* [3][0][][] */ 1,10,5,-11,-6,-12,-16,18, -32,-2,1,-12,-30,-5,-65,-11, -127,-13,-1,-14,-57,4,-50,-26, 
  /* [4][0][][] */ 11,9,20,-70,-21,-127,-27,-2, -20,-4,-95,-65,-19,-14,-51,88, -78,15,-68,-25,-58,-5,30,-10, 
  /* [5][0][][] */ -127,-6,-5,4,-10,-3,-29,-12, -36,-5,-5,-1,-6,-4,-14,4, -17,-3,2,-1,16,4,-10,0, 
  /* [6][0][][] */ -77,-4,1,-1,-21,0,-24,-2, -127,-4,-1,-2,-9,-2,-11,-3, -45,15,2,2,-3,0,-3,-3, 
  /* [7][0][][] */ -4,-20,-19,-14,-14,-30,-13,6, -15,-18,-83,-59,-64,-50,-4,2, -10,-22,-72,-127,-47,-45,-4,30, 
  /* [8][0][][] */ -55,-9,45,0,-5,4,-34,-9, -127,1,22,-26,-6,-26,-10,4, -30,22,24,-67,28,-68,3,3, 
  /* [9][0][][] */ -3,2,4,4,17,-1,-37,-2, -11,0,3,-1,-7,0,-80,-4, -39,-6,7,-2,-10,-1,-127,-6, 
  /* [10][0][][] */ -14,-7,11,-3,-127,18,-6,-25, -2,-63,-6,-2,-39,-10,-2,-42, -5,-80,-15,0,-10,-11,2,-107, 
  /* [11][0][][] */ 4,2,-127,17,1,-12,1,-8, 0,-2,-1,-1,2,-5,-6,-9, -2,-9,14,19,-4,3,-120,-16, 
  /* [12][0][][] */ 12,-12,15,-83,-27,3,-13,66, -1,-11,0,-37,-14,-16,-36,9, 11,4,44,14,6,127,-87,31, 
  /* [13][0][][] */ -36,2,-9,7,-5,2,-9,-9, -15,1,-25,-2,-1,-9,-2,-11, -3,2,-80,8,2,-127,0,-9, 
  /* [14][0][][] */ 2,-4,-4,-1,-20,7,-23,-2, -127,-1,-4,0,-16,0,-22,-4, -28,0,-4,10,0,4,-3,-7, 
  /* [15][0][][] */ -33,30,-13,-52,8,-127,-45,-21, -42,-9,-72,-111,75,-54,-46,-27, -49,-10,-102,-108,-22,102,-21,-16, 
};
const TfArray<4, int> tensor_dimension7 = { 4, { 16,1,3,8 } };
const TfArray<16, float> quant7_scale = { 16, { 0.011358603835105896, 0.017295591533184052, 0.016921630129218102, 0.019297147169709206, 0.0045773563906550407, 0.030547235161066055, 0.047181375324726105, 0.011238987557590008, 0.010072811506688595, 0.028705934062600136, 0.020150003954768181, 0.02636629156768322, 0.0047904425300657749, 0.031701046973466873, 0.028845030814409256, 0.0044093080796301365, } };
const TfArray<16, int> quant7_zero = { 16, { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 } };
const TfLiteAffineQuantization quant7 = { (TfLiteFloatArray*)&quant7_scale, (TfLiteIntArray*)&quant7_zero, 0 };
const ALIGN(8) int32_t tensor_data8[4] = { 1, 1, 50, 13, };
const TfArray<1, int> tensor_dimension8 = { 1, { 4 } };
const ALIGN(8) int32_t tensor_data9[4] = { 1, 50, 1, 8, };
const TfArray<1, int> tensor_dimension9 = { 1, { 4 } };
const ALIGN(8) int32_t tensor_data10[4] = { 1, 1, 25, 8, };
const TfArray<1, int> tensor_dimension10 = { 1, { 4 } };
const ALIGN(8) int32_t tensor_data11[4] = { 1, 25, 1, 16, };
const TfArray<1, int> tensor_dimension11 = { 1, { 4 } };
const TfArray<4, int> tensor_dimension12 = { 4, { 1,1,50,13 } };
const TfArray<1, float> quant12_scale = { 1, { 0.049485228955745697, } };
const TfArray<1, int> quant12_zero = { 1, { -3 } };
const TfLiteAffineQuantization quant12 = { (TfLiteFloatArray*)&quant12_scale, (TfLiteIntArray*)&quant12_zero, 0 };
const TfArray<4, int> tensor_dimension13 = { 4, { 1,1,50,8 } };
const TfArray<1, float> quant13_scale = { 1, { 0.085937038064002991, } };
const TfArray<1, int> quant13_zero = { 1, { -128 } };
const TfLiteAffineQuantization quant13 = { (TfLiteFloatArray*)&quant13_scale, (TfLiteIntArray*)&quant13_zero, 0 };
const TfArray<4, int> tensor_dimension14 = { 4, { 1,50,1,8 } };
const TfArray<1, float> quant14_scale = { 1, { 0.085937038064002991, } };
const TfArray<1, int> quant14_zero = { 1, { -128 } };
const TfLiteAffineQuantization quant14 = { (TfLiteFloatArray*)&quant14_scale, (TfLiteIntArray*)&quant14_zero, 0 };
const TfArray<4, int> tensor_dimension15 = { 4, { 1,25,1,8 } };
const TfArray<1, float> quant15_scale = { 1, { 0.085937038064002991, } };
const TfArray<1, int> quant15_zero = { 1, { -128 } };
const TfLiteAffineQuantization quant15 = { (TfLiteFloatArray*)&quant15_scale, (TfLiteIntArray*)&quant15_zero, 0 };
const TfArray<4, int> tensor_dimension16 = { 4, { 1,1,25,8 } };
const TfArray<1, float> quant16_scale = { 1, { 0.085937038064002991, } };
const TfArray<1, int> quant16_zero = { 1, { -128 } };
const TfLiteAffineQuantization quant16 = { (TfLiteFloatArray*)&quant16_scale, (TfLiteIntArray*)&quant16_zero, 0 };
const TfArray<4, int> tensor_dimension17 = { 4, { 1,1,25,16 } };
const TfArray<1, float> quant17_scale = { 1, { 0.026023410260677338, } };
const TfArray<1, int> quant17_zero = { 1, { -128 } };
const TfLiteAffineQuantization quant17 = { (TfLiteFloatArray*)&quant17_scale, (TfLiteIntArray*)&quant17_zero, 0 };
const TfArray<4, int> tensor_dimension18 = { 4, { 1,25,1,16 } };
const TfArray<1, float> quant18_scale = { 1, { 0.026023410260677338, } };
const TfArray<1, int> quant18_zero = { 1, { -128 } };
const TfLiteAffineQuantization quant18 = { (TfLiteFloatArray*)&quant18_scale, (TfLiteIntArray*)&quant18_zero, 0 };
const TfArray<4, int> tensor_dimension19 = { 4, { 1,13,1,16 } };
const TfArray<1, float> quant19_scale = { 1, { 0.026023410260677338, } };
const TfArray<1, int> quant19_zero = { 1, { -128 } };
const TfLiteAffineQuantization quant19 = { (TfLiteFloatArray*)&quant19_scale, (TfLiteIntArray*)&quant19_zero, 0 };
const TfArray<2, int> tensor_dimension20 = { 2, { 1,208 } };
const TfArray<1, float> quant20_scale = { 1, { 0.026023410260677338, } };
const TfArray<1, int> quant20_zero = { 1, { -128 } };
const TfLiteAffineQuantization quant20 = { (TfLiteFloatArray*)&quant20_scale, (TfLiteIntArray*)&quant20_zero, 0 };
const TfArray<2, int> tensor_dimension21 = { 2, { 1,12 } };
const TfArray<1, float> quant21_scale = { 1, { 0.13342574238777161, } };
const TfArray<1, int> quant21_zero = { 1, { 58 } };
const TfLiteAffineQuantization quant21 = { (TfLiteFloatArray*)&quant21_scale, (TfLiteIntArray*)&quant21_zero, 0 };
const TfArray<2, int> tensor_dimension22 = { 2, { 1,12 } };
const TfArray<1, float> quant22_scale = { 1, { 0.00390625, } };
const TfArray<1, int> quant22_zero = { 1, { -128 } };
const TfLiteAffineQuantization quant22 = { (TfLiteFloatArray*)&quant22_scale, (TfLiteIntArray*)&quant22_zero, 0 };
const TfLiteReshapeParams opdata0 = { { 0, 0, 0, 0, 0, 0, 0, 0, }, 0 };
const TfArray<2, int> inputs0 = { 2, { 0,8 } };
const TfArray<1, int> outputs0 = { 1, { 12 } };
const TfLiteConvParams opdata1 = { kTfLitePaddingSame, 1,1, kTfLiteActRelu, 1,1 };
const TfArray<3, int> inputs1 = { 3, { 12,6,2 } };
const TfArray<1, int> outputs1 = { 1, { 13 } };
const TfLiteReshapeParams opdata2 = { { 0, 0, 0, 0, 0, 0, 0, 0, }, 0 };
const TfArray<2, int> inputs2 = { 2, { 13,9 } };
const TfArray<1, int> outputs2 = { 1, { 14 } };
const TfLitePoolParams opdata3 = { kTfLitePaddingSame, 1,2, 1,2, kTfLiteActNone, { { 0,0, 0, 0 } } };
const TfArray<1, int> inputs3 = { 1, { 14 } };
const TfArray<1, int> outputs3 = { 1, { 15 } };
const TfLiteReshapeParams opdata4 = { { 0, 0, 0, 0, 0, 0, 0, 0, }, 0 };
const TfArray<2, int> inputs4 = { 2, { 15,10 } };
const TfArray<1, int> outputs4 = { 1, { 16 } };
const TfLiteConvParams opdata5 = { kTfLitePaddingSame, 1,1, kTfLiteActRelu, 1,1 };
const TfArray<3, int> inputs5 = { 3, { 16,7,3 } };
const TfArray<1, int> outputs5 = { 1, { 17 } };
const TfLiteReshapeParams opdata6 = { { 0, 0, 0, 0, 0, 0, 0, 0, }, 0 };
const TfArray<2, int> inputs6 = { 2, { 17,11 } };
const TfArray<1, int> outputs6 = { 1, { 18 } };
const TfLitePoolParams opdata7 = { kTfLitePaddingSame, 1,2, 1,2, kTfLiteActNone, { { 0,0, 0, 0 } } };
const TfArray<1, int> inputs7 = { 1, { 18 } };
const TfArray<1, int> outputs7 = { 1, { 19 } };
const TfLiteReshapeParams opdata8 = { { 0, 0, 0, 0, 0, 0, 0, 0, }, 0 };
const TfArray<2, int> inputs8 = { 2, { 19,1 } };
const TfArray<1, int> outputs8 = { 1, { 20 } };
const TfLiteFullyConnectedParams opdata9 = { kTfLiteActNone, kTfLiteFullyConnectedWeightsFormatDefault, false, false };
const TfArray<3, int> inputs9 = { 3, { 20,5,4 } };
const TfArray<1, int> outputs9 = { 1, { 21 } };
const TfLiteSoftmaxParams opdata10 = { 1 };
const TfArray<1, int> inputs10 = { 1, { 21 } };
const TfArray<1, int> outputs10 = { 1, { 22 } };
const TensorInfo_t tensorData[] = {
  { kTfLiteArenaRw, kTfLiteInt8, tensor_arena + 656, (TfLiteIntArray*)&tensor_dimension0, 650, {kTfLiteAffineQuantization, const_cast<void*>(static_cast<const void*>(&quant0))}, },
  { kTfLiteMmapRo, kTfLiteInt32, (void*)tensor_data1, (TfLiteIntArray*)&tensor_dimension1, 8, {kTfLiteNoQuantization, nullptr}, },
  { kTfLiteMmapRo, kTfLiteInt32, (void*)tensor_data2, (TfLiteIntArray*)&tensor_dimension2, 32, {kTfLiteAffineQuantization, const_cast<void*>(static_cast<const void*>(&quant2))}, },
  { kTfLiteMmapRo, kTfLiteInt32, (void*)tensor_data3, (TfLiteIntArray*)&tensor_dimension3, 64, {kTfLiteAffineQuantization, const_cast<void*>(static_cast<const void*>(&quant3))}, },
  { kTfLiteMmapRo, kTfLiteInt32, (void*)tensor_data4, (TfLiteIntArray*)&tensor_dimension4, 48, {kTfLiteAffineQuantization, const_cast<void*>(static_cast<const void*>(&quant4))}, },
  { kTfLiteMmapRo, kTfLiteInt8, (void*)tensor_data5, (TfLiteIntArray*)&tensor_dimension5, 2496, {kTfLiteAffineQuantization, const_cast<void*>(static_cast<const void*>(&quant5))}, },
  { kTfLiteMmapRo, kTfLiteInt8, (void*)tensor_data6, (TfLiteIntArray*)&tensor_dimension6, 312, {kTfLiteAffineQuantization, const_cast<void*>(static_cast<const void*>(&quant6))}, },
  { kTfLiteMmapRo, kTfLiteInt8, (void*)tensor_data7, (TfLiteIntArray*)&tensor_dimension7, 384, {kTfLiteAffineQuantization, const_cast<void*>(static_cast<const void*>(&quant7))}, },
  { kTfLiteMmapRo, kTfLiteInt32, (void*)tensor_data8, (TfLiteIntArray*)&tensor_dimension8, 16, {kTfLiteNoQuantization, nullptr}, },
  { kTfLiteMmapRo, kTfLiteInt32, (void*)tensor_data9, (TfLiteIntArray*)&tensor_dimension9, 16, {kTfLiteNoQuantization, nullptr}, },
  { kTfLiteMmapRo, kTfLiteInt32, (void*)tensor_data10, (TfLiteIntArray*)&tensor_dimension10, 16, {kTfLiteNoQuantization, nullptr}, },
  { kTfLiteMmapRo, kTfLiteInt32, (void*)tensor_data11, (TfLiteIntArray*)&tensor_dimension11, 16, {kTfLiteNoQuantization, nullptr}, },
  { kTfLiteArenaRw, kTfLiteInt8, tensor_arena + 0, (TfLiteIntArray*)&tensor_dimension12, 650, {kTfLiteAffineQuantization, const_cast<void*>(static_cast<const void*>(&quant12))}, },
  { kTfLiteArenaRw, kTfLiteInt8, tensor_arena + 656, (TfLiteIntArray*)&tensor_dimension13, 400, {kTfLiteAffineQuantization, const_cast<void*>(static_cast<const void*>(&quant13))}, },
  { kTfLiteArenaRw, kTfLiteInt8, tensor_arena + 0, (TfLiteIntArray*)&tensor_dimension14, 400, {kTfLiteAffineQuantization, const_cast<void*>(static_cast<const void*>(&quant14))}, },
  { kTfLiteArenaRw, kTfLiteInt8, tensor_arena + 400, (TfLiteIntArray*)&tensor_dimension15, 200, {kTfLiteAffineQuantization, const_cast<void*>(static_cast<const void*>(&quant15))}, },
  { kTfLiteArenaRw, kTfLiteInt8, tensor_arena + 0, (TfLiteIntArray*)&tensor_dimension16, 200, {kTfLiteAffineQuantization, const_cast<void*>(static_cast<const void*>(&quant16))}, },
  { kTfLiteArenaRw, kTfLiteInt8, tensor_arena + 400, (TfLiteIntArray*)&tensor_dimension17, 400, {kTfLiteAffineQuantization, const_cast<void*>(static_cast<const void*>(&quant17))}, },
  { kTfLiteArenaRw, kTfLiteInt8, tensor_arena + 0, (TfLiteIntArray*)&tensor_dimension18, 400, {kTfLiteAffineQuantization, const_cast<void*>(static_cast<const void*>(&quant18))}, },
  { kTfLiteArenaRw, kTfLiteInt8, tensor_arena + 400, (TfLiteIntArray*)&tensor_dimension19, 208, {kTfLiteAffineQuantization, const_cast<void*>(static_cast<const void*>(&quant19))}, },
  { kTfLiteArenaRw, kTfLiteInt8, tensor_arena + 0, (TfLiteIntArray*)&tensor_dimension20, 208, {kTfLiteAffineQuantization, const_cast<void*>(static_cast<const void*>(&quant20))}, },
  { kTfLiteArenaRw, kTfLiteInt8, tensor_arena + 208, (TfLiteIntArray*)&tensor_dimension21, 12, {kTfLiteAffineQuantization, const_cast<void*>(static_cast<const void*>(&quant21))}, },
  { kTfLiteArenaRw, kTfLiteInt8, tensor_arena + 0, (TfLiteIntArray*)&tensor_dimension22, 12, {kTfLiteAffineQuantization, const_cast<void*>(static_cast<const void*>(&quant22))}, },
};const NodeInfo_t nodeData[] = {
  { (TfLiteIntArray*)&inputs0, (TfLiteIntArray*)&outputs0, const_cast<void*>(static_cast<const void*>(&opdata0)), OP_RESHAPE, },
  { (TfLiteIntArray*)&inputs1, (TfLiteIntArray*)&outputs1, const_cast<void*>(static_cast<const void*>(&opdata1)), OP_CONV_2D, },
  { (TfLiteIntArray*)&inputs2, (TfLiteIntArray*)&outputs2, const_cast<void*>(static_cast<const void*>(&opdata2)), OP_RESHAPE, },
  { (TfLiteIntArray*)&inputs3, (TfLiteIntArray*)&outputs3, const_cast<void*>(static_cast<const void*>(&opdata3)), OP_MAX_POOL_2D, },
  { (TfLiteIntArray*)&inputs4, (TfLiteIntArray*)&outputs4, const_cast<void*>(static_cast<const void*>(&opdata4)), OP_RESHAPE, },
  { (TfLiteIntArray*)&inputs5, (TfLiteIntArray*)&outputs5, const_cast<void*>(static_cast<const void*>(&opdata5)), OP_CONV_2D, },
  { (TfLiteIntArray*)&inputs6, (TfLiteIntArray*)&outputs6, const_cast<void*>(static_cast<const void*>(&opdata6)), OP_RESHAPE, },
  { (TfLiteIntArray*)&inputs7, (TfLiteIntArray*)&outputs7, const_cast<void*>(static_cast<const void*>(&opdata7)), OP_MAX_POOL_2D, },
  { (TfLiteIntArray*)&inputs8, (TfLiteIntArray*)&outputs8, const_cast<void*>(static_cast<const void*>(&opdata8)), OP_RESHAPE, },
  { (TfLiteIntArray*)&inputs9, (TfLiteIntArray*)&outputs9, const_cast<void*>(static_cast<const void*>(&opdata9)), OP_FULLY_CONNECTED, },
  { (TfLiteIntArray*)&inputs10, (TfLiteIntArray*)&outputs10, const_cast<void*>(static_cast<const void*>(&opdata10)), OP_SOFTMAX, },
};
static std::vector<void*> overflow_buffers;
static TfLiteStatus AllocatePersistentBuffer(struct TfLiteContext* ctx,
                                                 size_t bytes, void** ptr) {
  if (current_location - bytes < tensor_boundary) {
    // OK, this will look super weird, but.... we have CMSIS-NN buffers which
    // we cannot calculate beforehand easily.
    *ptr = malloc(bytes);
    if (*ptr == NULL) {
      printf("ERR: Failed to allocate persistent buffer of size %d\n", (int)bytes);
      return kTfLiteError;
    }
    overflow_buffers.push_back(*ptr);
    return kTfLiteOk;
  }

  current_location -= bytes;

  *ptr = current_location;
  return kTfLiteOk;
}
typedef struct {
  size_t bytes;
  void *ptr;
} scratch_buffer_t;
static std::vector<scratch_buffer_t> scratch_buffers;

static TfLiteStatus RequestScratchBufferInArena(struct TfLiteContext* ctx, size_t bytes,
                                                int* buffer_idx) {
  scratch_buffer_t b;
  b.bytes = bytes;

  TfLiteStatus s = AllocatePersistentBuffer(ctx, b.bytes, &b.ptr);
  if (s != kTfLiteOk) {
    return s;
  }

  scratch_buffers.push_back(b);

  *buffer_idx = scratch_buffers.size() - 1;

  return kTfLiteOk;
}

static void* GetScratchBuffer(struct TfLiteContext* ctx, int buffer_idx) {
  if (buffer_idx > static_cast<int>(scratch_buffers.size()) - 1) {
    return NULL;
  }
  return scratch_buffers[buffer_idx].ptr;
}
} // namespace

  TfLiteStatus trained_model_init( void*(*alloc_fnc)(size_t,size_t) ) {
#ifdef EI_CLASSIFIER_ALLOCATION_HEAP
  tensor_arena = (uint8_t*) alloc_fnc(16, kTensorArenaSize);
  if (!tensor_arena) {
    printf("ERR: failed to allocate tensor arena\n");
    return kTfLiteError;
  }
#endif
  tensor_boundary = tensor_arena;
  current_location = tensor_arena + kTensorArenaSize;
  ctx.AllocatePersistentBuffer = &AllocatePersistentBuffer;
  ctx.RequestScratchBufferInArena = &RequestScratchBufferInArena;
  ctx.GetScratchBuffer = &GetScratchBuffer;
  ctx.tensors = tflTensors;
  ctx.tensors_size = 23;
  for(size_t i = 0; i < 23; ++i) {
    tflTensors[i].type = tensorData[i].type;
    tflTensors[i].is_variable = 0;

#if defined(EI_CLASSIFIER_ALLOCATION_HEAP)
    tflTensors[i].allocation_type = tensorData[i].allocation_type;
#else
    tflTensors[i].allocation_type = (tensor_arena <= tensorData[i].data && tensorData[i].data < tensor_arena + kTensorArenaSize) ? kTfLiteArenaRw : kTfLiteMmapRo;
#endif
    tflTensors[i].bytes = tensorData[i].bytes;
    tflTensors[i].dims = tensorData[i].dims;

#if defined(EI_CLASSIFIER_ALLOCATION_HEAP)
    if(tflTensors[i].allocation_type == kTfLiteArenaRw){
      uint8_t* start = (uint8_t*) ((uintptr_t)tensorData[i].data + (uintptr_t) tensor_arena);

     tflTensors[i].data.data =  start;
    }
    else{
       tflTensors[i].data.data = tensorData[i].data;
    }
#else
    tflTensors[i].data.data = tensorData[i].data;
#endif // EI_CLASSIFIER_ALLOCATION_HEAP
    tflTensors[i].quantization = tensorData[i].quantization;
    if (tflTensors[i].quantization.type == kTfLiteAffineQuantization) {
      TfLiteAffineQuantization const* quant = ((TfLiteAffineQuantization const*)(tensorData[i].quantization.params));
      tflTensors[i].params.scale = quant->scale->data[0];
      tflTensors[i].params.zero_point = quant->zero_point->data[0];
    }
    if (tflTensors[i].allocation_type == kTfLiteArenaRw) {
      auto data_end_ptr = (uint8_t*)tflTensors[i].data.data + tensorData[i].bytes;
      if (data_end_ptr > tensor_boundary) {
        tensor_boundary = data_end_ptr;
      }
    }
  }
  if (tensor_boundary > current_location /* end of arena size */) {
    printf("ERR: tensor arena is too small, does not fit model - even without scratch buffers\n");
    return kTfLiteError;
  }
  registrations[OP_RESHAPE] = *tflite::ops::micro::Register_RESHAPE();
  registrations[OP_CONV_2D] = *tflite::ops::micro::Register_CONV_2D();
  registrations[OP_MAX_POOL_2D] = *tflite::ops::micro::Register_MAX_POOL_2D();
  registrations[OP_FULLY_CONNECTED] = *tflite::ops::micro::Register_FULLY_CONNECTED();
  registrations[OP_SOFTMAX] = *tflite::ops::micro::Register_SOFTMAX();

  for(size_t i = 0; i < 11; ++i) {
    tflNodes[i].inputs = nodeData[i].inputs;
    tflNodes[i].outputs = nodeData[i].outputs;
    tflNodes[i].builtin_data = nodeData[i].builtin_data;
    tflNodes[i].custom_initial_data = nullptr;
    tflNodes[i].custom_initial_data_size = 0;
    if (registrations[nodeData[i].used_op_index].init) {
      tflNodes[i].user_data = registrations[nodeData[i].used_op_index].init(&ctx, (const char*)tflNodes[i].builtin_data, 0);
    }
  }
  for(size_t i = 0; i < 11; ++i) {
    if (registrations[nodeData[i].used_op_index].prepare) {
      TfLiteStatus status = registrations[nodeData[i].used_op_index].prepare(&ctx, &tflNodes[i]);
      if (status != kTfLiteOk) {
        return status;
      }
    }
  }
  return kTfLiteOk;
}

static const int inTensorIndices[] = {
  0, 
};
TfLiteTensor* trained_model_input(int index) {
  return &ctx.tensors[inTensorIndices[index]];
}

static const int outTensorIndices[] = {
  22, 
};
TfLiteTensor* trained_model_output(int index) {
  return &ctx.tensors[outTensorIndices[index]];
}

TfLiteStatus trained_model_invoke() {
  for(size_t i = 0; i < 11; ++i) {
    TfLiteStatus status = registrations[nodeData[i].used_op_index].invoke(&ctx, &tflNodes[i]);

#if EI_CLASSIFIER_PRINT_STATE
    ei_printf("layer %lu\n", i);
    ei_printf("    inputs:\n");
    for (size_t ix = 0; ix < tflNodes[i].inputs->size; ix++) {
      auto d = tensorData[tflNodes[i].inputs->data[ix]];

      size_t data_ptr = (size_t)d.data;

      if (d.allocation_type == kTfLiteArenaRw) {
        data_ptr = (size_t)tensor_arena + data_ptr;
      }

      if (d.type == TfLiteType::kTfLiteInt8) {
        int8_t* data = (int8_t*)data_ptr;
        ei_printf("        %lu (%zu bytes, ptr=%p, alloc_type=%d, type=%d): ", ix, d.bytes, data, (int)d.allocation_type, (int)d.type);
        for (size_t jx = 0; jx < d.bytes; jx++) {
          ei_printf("%d ", data[jx]);
        }
      }
      else {
        float* data = (float*)data_ptr;
        ei_printf("        %lu (%zu bytes, ptr=%p, alloc_type=%d, type=%d): ", ix, d.bytes, data, (int)d.allocation_type, (int)d.type);
        for (size_t jx = 0; jx < d.bytes / 4; jx++) {
          ei_printf("%f ", data[jx]);
        }
      }
      ei_printf("\n");
    }
    ei_printf("\n");

    ei_printf("    outputs:\n");
    for (size_t ix = 0; ix < tflNodes[i].outputs->size; ix++) {
      auto d = tensorData[tflNodes[i].outputs->data[ix]];

      size_t data_ptr = (size_t)d.data;

      if (d.allocation_type == kTfLiteArenaRw) {
        data_ptr = (size_t)tensor_arena + data_ptr;
      }

      if (d.type == TfLiteType::kTfLiteInt8) {
        int8_t* data = (int8_t*)data_ptr;
        ei_printf("        %lu (%zu bytes, ptr=%p, alloc_type=%d, type=%d): ", ix, d.bytes, data, (int)d.allocation_type, (int)d.type);
        for (size_t jx = 0; jx < d.bytes; jx++) {
          ei_printf("%d ", data[jx]);
        }
      }
      else {
        float* data = (float*)data_ptr;
        ei_printf("        %lu (%zu bytes, ptr=%p, alloc_type=%d, type=%d): ", ix, d.bytes, data, (int)d.allocation_type, (int)d.type);
        for (size_t jx = 0; jx < d.bytes / 4; jx++) {
          ei_printf("%f ", data[jx]);
        }
      }
      ei_printf("\n");
    }
    ei_printf("\n");
#endif // EI_CLASSIFIER_PRINT_STATE

    if (status != kTfLiteOk) {
      return status;
    }
  }
  return kTfLiteOk;
}

TfLiteStatus trained_model_reset( void (*free_fnc)(void* ptr) ) {
#ifdef EI_CLASSIFIER_ALLOCATION_HEAP
  free_fnc(tensor_arena);
#endif
  scratch_buffers.clear();
  for (size_t ix = 0; ix < overflow_buffers.size(); ix++) {
    free(overflow_buffers[ix]);
  }
  overflow_buffers.clear();
  return kTfLiteOk;
}

//Code generated automatically by TMVA for Inference of Model file [model.pt] at [Fri Oct  4 17:06:59 2024] 

#ifndef ROOT_TMVA_SOFIE_MODEL
#define ROOT_TMVA_SOFIE_MODEL

#include <algorithm>
#include <cmath>
#include <vector>
#include "TMVA/SOFIE_common.hxx"
#include <fstream>

namespace TMVA_SOFIE_model{
namespace BLAS{
	extern "C" void sgemv_(const char * trans, const int * m, const int * n, const float * alpha, const float * A,
	                       const int * lda, const float * X, const int * incx, const float * beta, const float * Y, const int * incy);
	extern "C" void sgemm_(const char * transa, const char * transb, const int * m, const int * n, const int * k,
	                       const float * alpha, const float * A, const int * lda, const float * B, const int * ldb,
	                       const float * beta, float * C, const int * ldc);
}//BLAS
struct Session {
std::vector<float> fTensor_layers4weight = std::vector<float>(4096);
float * tensor_layers4weight = fTensor_layers4weight.data();
std::vector<float> fTensor_layers4bias = std::vector<float>(64);
float * tensor_layers4bias = fTensor_layers4bias.data();
std::vector<float> fTensor_layers2weight = std::vector<float>(4096);
float * tensor_layers2weight = fTensor_layers2weight.data();
std::vector<float> fTensor_layers2bias = std::vector<float>(64);
float * tensor_layers2bias = fTensor_layers2bias.data();
std::vector<float> fTensor_layers6weight = std::vector<float>(64);
float * tensor_layers6weight = fTensor_layers6weight.data();
std::vector<float> fTensor_layers6bias = std::vector<float>(1);
float * tensor_layers6bias = fTensor_layers6bias.data();
std::vector<float> fTensor_layers0weight = std::vector<float>(448);
float * tensor_layers0weight = fTensor_layers0weight.data();
std::vector<float> fTensor_layers0bias = std::vector<float>(64);
float * tensor_layers0bias = fTensor_layers0bias.data();

//--- declare and allocate the intermediate tensors
std::vector<float> fTensor_16 = std::vector<float>(1);
float * tensor_16 = fTensor_16.data();
std::vector<float> fTensor_input4 = std::vector<float>(64);
float * tensor_input4 = fTensor_input4.data();
std::vector<float> fTensor_result3 = std::vector<float>(64);
float * tensor_result3 = fTensor_result3.data();
std::vector<float> fTensor_result7 = std::vector<float>(64);
float * tensor_result7 = fTensor_result7.data();
std::vector<float> fTensor_layers2biasbcast = std::vector<float>(64);
float * tensor_layers2biasbcast = fTensor_layers2biasbcast.data();
std::vector<float> fTensor_result = std::vector<float>(64);
float * tensor_result = fTensor_result.data();
std::vector<float> fTensor_input6 = std::vector<float>(1);
float * tensor_input6 = fTensor_input6.data();
std::vector<float> fTensor_input2 = std::vector<float>(64);
float * tensor_input2 = fTensor_input2.data();
std::vector<float> fTensor_input0 = std::vector<float>(64);
float * tensor_input0 = fTensor_input0.data();
std::vector<float> fTensor_layers6biasbcast = std::vector<float>(1);
float * tensor_layers6biasbcast = fTensor_layers6biasbcast.data();
std::vector<float> fTensor_layers4biasbcast = std::vector<float>(64);
float * tensor_layers4biasbcast = fTensor_layers4biasbcast.data();
std::vector<float> fTensor_layers0biasbcast = std::vector<float>(64);
float * tensor_layers0biasbcast = fTensor_layers0biasbcast.data();


Session(std::string filename ="model.dat") {

//--- reading weights from file
   std::ifstream f;
   f.open(filename);
   if (!f.is_open()) {
      throw std::runtime_error("tmva-sofie failed to open file " + filename + " for input weights");
   }
   std::string tensor_name;
   size_t length;
   f >> tensor_name >> length;
   if (tensor_name != "tensor_layers4weight" ) {
      std::string err_msg = "TMVA-SOFIE failed to read the correct tensor name; expected name is tensor_layers4weight , read " + tensor_name;
      throw std::runtime_error(err_msg);
    }
   if (length != 4096) {
      std::string err_msg = "TMVA-SOFIE failed to read the correct tensor size; expected size is 4096 , read " + std::to_string(length) ;
      throw std::runtime_error(err_msg);
    }
   for (size_t i = 0; i < length; ++i)
      f >> tensor_layers4weight[i];
   f >> tensor_name >> length;
   if (tensor_name != "tensor_layers4bias" ) {
      std::string err_msg = "TMVA-SOFIE failed to read the correct tensor name; expected name is tensor_layers4bias , read " + tensor_name;
      throw std::runtime_error(err_msg);
    }
   if (length != 64) {
      std::string err_msg = "TMVA-SOFIE failed to read the correct tensor size; expected size is 64 , read " + std::to_string(length) ;
      throw std::runtime_error(err_msg);
    }
   for (size_t i = 0; i < length; ++i)
      f >> tensor_layers4bias[i];
   f >> tensor_name >> length;
   if (tensor_name != "tensor_layers2weight" ) {
      std::string err_msg = "TMVA-SOFIE failed to read the correct tensor name; expected name is tensor_layers2weight , read " + tensor_name;
      throw std::runtime_error(err_msg);
    }
   if (length != 4096) {
      std::string err_msg = "TMVA-SOFIE failed to read the correct tensor size; expected size is 4096 , read " + std::to_string(length) ;
      throw std::runtime_error(err_msg);
    }
   for (size_t i = 0; i < length; ++i)
      f >> tensor_layers2weight[i];
   f >> tensor_name >> length;
   if (tensor_name != "tensor_layers2bias" ) {
      std::string err_msg = "TMVA-SOFIE failed to read the correct tensor name; expected name is tensor_layers2bias , read " + tensor_name;
      throw std::runtime_error(err_msg);
    }
   if (length != 64) {
      std::string err_msg = "TMVA-SOFIE failed to read the correct tensor size; expected size is 64 , read " + std::to_string(length) ;
      throw std::runtime_error(err_msg);
    }
   for (size_t i = 0; i < length; ++i)
      f >> tensor_layers2bias[i];
   f >> tensor_name >> length;
   if (tensor_name != "tensor_layers6weight" ) {
      std::string err_msg = "TMVA-SOFIE failed to read the correct tensor name; expected name is tensor_layers6weight , read " + tensor_name;
      throw std::runtime_error(err_msg);
    }
   if (length != 64) {
      std::string err_msg = "TMVA-SOFIE failed to read the correct tensor size; expected size is 64 , read " + std::to_string(length) ;
      throw std::runtime_error(err_msg);
    }
   for (size_t i = 0; i < length; ++i)
      f >> tensor_layers6weight[i];
   f >> tensor_name >> length;
   if (tensor_name != "tensor_layers6bias" ) {
      std::string err_msg = "TMVA-SOFIE failed to read the correct tensor name; expected name is tensor_layers6bias , read " + tensor_name;
      throw std::runtime_error(err_msg);
    }
   if (length != 1) {
      std::string err_msg = "TMVA-SOFIE failed to read the correct tensor size; expected size is 1 , read " + std::to_string(length) ;
      throw std::runtime_error(err_msg);
    }
   for (size_t i = 0; i < length; ++i)
      f >> tensor_layers6bias[i];
   f >> tensor_name >> length;
   if (tensor_name != "tensor_layers0weight" ) {
      std::string err_msg = "TMVA-SOFIE failed to read the correct tensor name; expected name is tensor_layers0weight , read " + tensor_name;
      throw std::runtime_error(err_msg);
    }
   if (length != 448) {
      std::string err_msg = "TMVA-SOFIE failed to read the correct tensor size; expected size is 448 , read " + std::to_string(length) ;
      throw std::runtime_error(err_msg);
    }
   for (size_t i = 0; i < length; ++i)
      f >> tensor_layers0weight[i];
   f >> tensor_name >> length;
   if (tensor_name != "tensor_layers0bias" ) {
      std::string err_msg = "TMVA-SOFIE failed to read the correct tensor name; expected name is tensor_layers0bias , read " + tensor_name;
      throw std::runtime_error(err_msg);
    }
   if (length != 64) {
      std::string err_msg = "TMVA-SOFIE failed to read the correct tensor size; expected size is 64 , read " + std::to_string(length) ;
      throw std::runtime_error(err_msg);
    }
   for (size_t i = 0; i < length; ++i)
      f >> tensor_layers0bias[i];
   f.close();

//---- allocate the intermediate dynamic tensors
//--- broadcast bias tensor layers0biasfor Gemm op
   {
      float * data = TMVA::Experimental::SOFIE::UTILITY::UnidirectionalBroadcast<float>(tensor_layers0bias,{ 64 }, { 1 , 64 });
      std::copy(data, data + 64, tensor_layers0biasbcast);
      delete [] data;
   }
//--- broadcast bias tensor layers2biasfor Gemm op
   {
      float * data = TMVA::Experimental::SOFIE::UTILITY::UnidirectionalBroadcast<float>(tensor_layers2bias,{ 64 }, { 1 , 64 });
      std::copy(data, data + 64, tensor_layers2biasbcast);
      delete [] data;
   }
//--- broadcast bias tensor layers4biasfor Gemm op
   {
      float * data = TMVA::Experimental::SOFIE::UTILITY::UnidirectionalBroadcast<float>(tensor_layers4bias,{ 64 }, { 1 , 64 });
      std::copy(data, data + 64, tensor_layers4biasbcast);
      delete [] data;
   }
//--- broadcast bias tensor layers6biasfor Gemm op
   {
      float * data = TMVA::Experimental::SOFIE::UTILITY::UnidirectionalBroadcast<float>(tensor_layers6bias,{ 1 }, { 1 , 1 });
      std::copy(data, data + 1, tensor_layers6biasbcast);
      delete [] data;
   }
}

std::vector<float> infer(float* tensor_x1){

//--------- Gemm
   char op_0_transA = 'n';
   char op_0_transB = 't';
   int op_0_m = 1;
   int op_0_n = 64;
   int op_0_k = 7;
   float op_0_alpha = 1;
   float op_0_beta = 1;
   int op_0_lda = 7;
   int op_0_ldb = 7;
   std::copy(tensor_layers0biasbcast, tensor_layers0biasbcast + 64, tensor_input0);
   BLAS::sgemm_(&op_0_transB, &op_0_transA, &op_0_n, &op_0_m, &op_0_k, &op_0_alpha, tensor_layers0weight, &op_0_ldb, tensor_x1, &op_0_lda, &op_0_beta, tensor_input0, &op_0_n);

//------ RELU
   for (int id = 0; id < 64 ; id++){
      tensor_result[id] = ((tensor_input0[id] > 0 )? tensor_input0[id] : 0);
   }

//--------- Gemm
   char op_2_transA = 'n';
   char op_2_transB = 't';
   int op_2_m = 1;
   int op_2_n = 64;
   int op_2_k = 64;
   float op_2_alpha = 1;
   float op_2_beta = 1;
   int op_2_lda = 64;
   int op_2_ldb = 64;
   std::copy(tensor_layers2biasbcast, tensor_layers2biasbcast + 64, tensor_input2);
   BLAS::sgemm_(&op_2_transB, &op_2_transA, &op_2_n, &op_2_m, &op_2_k, &op_2_alpha, tensor_layers2weight, &op_2_ldb, tensor_result, &op_2_lda, &op_2_beta, tensor_input2, &op_2_n);

//------ RELU
   for (int id = 0; id < 64 ; id++){
      tensor_result3[id] = ((tensor_input2[id] > 0 )? tensor_input2[id] : 0);
   }

//--------- Gemm
   char op_4_transA = 'n';
   char op_4_transB = 't';
   int op_4_m = 1;
   int op_4_n = 64;
   int op_4_k = 64;
   float op_4_alpha = 1;
   float op_4_beta = 1;
   int op_4_lda = 64;
   int op_4_ldb = 64;
   std::copy(tensor_layers4biasbcast, tensor_layers4biasbcast + 64, tensor_input4);
   BLAS::sgemm_(&op_4_transB, &op_4_transA, &op_4_n, &op_4_m, &op_4_k, &op_4_alpha, tensor_layers4weight, &op_4_ldb, tensor_result3, &op_4_lda, &op_4_beta, tensor_input4, &op_4_n);

//------ RELU
   for (int id = 0; id < 64 ; id++){
      tensor_result7[id] = ((tensor_input4[id] > 0 )? tensor_input4[id] : 0);
   }

//--------- Gemm
   char op_6_transA = 'n';
   char op_6_transB = 't';
   int op_6_m = 1;
   int op_6_n = 1;
   int op_6_k = 64;
   float op_6_alpha = 1;
   float op_6_beta = 1;
   int op_6_lda = 64;
   int op_6_ldb = 64;
   std::copy(tensor_layers6biasbcast, tensor_layers6biasbcast + 1, tensor_input6);
   BLAS::sgemm_(&op_6_transB, &op_6_transA, &op_6_n, &op_6_m, &op_6_k, &op_6_alpha, tensor_layers6weight, &op_6_ldb, tensor_result7, &op_6_lda, &op_6_beta, tensor_input6, &op_6_n);
	for (int id = 0; id < 1 ; id++){
		tensor_16[id] = 1 / (1 + std::exp( - tensor_input6[id]));
	}
   return fTensor_16;
}
};
} //TMVA_SOFIE_model

#endif  // ROOT_TMVA_SOFIE_MODEL

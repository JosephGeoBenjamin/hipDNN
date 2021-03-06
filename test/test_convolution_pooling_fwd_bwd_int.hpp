#ifndef TEST_CONVOLUTION_POOLING_HPP
#define TEST_CONVOLUTION_POOLING_HPP

#include "hipdnn.h"
#include "hipdnn_test_common.h"
#include "gtest/gtest.h"
#include "common.hpp"

template <typename dataType>
void compute_conv_fwd(convulution_Size &c, dataType *src, dataType *weights,
                      dataType *bias, dataType *dst, float *avg_time) {

  hipdnnHandle_t hipdnn;
  checkHIPDNN(hipdnnCreate(&hipdnn));

  hipdnnTensorDescriptor_t in_desc;
  checkHIPDNN(hipdnnCreateTensorDescriptor(&in_desc));
  checkHIPDNN(hipdnnSetTensor4dDescriptor(
      in_desc, HIPDNN_TENSOR_NCHW, HIPDNN_DATA_FLOAT, c.mb, c.ic, c.ih, c.iw));

  hipdnnFilterDescriptor_t filt_desc;
  checkHIPDNN(hipdnnCreateFilterDescriptor(&filt_desc));
  int filterDimA[] = {c.oc, c.ic, c.kh, c.kw};
  checkHIPDNN(hipdnnSetFilterNdDescriptor(filt_desc, HIPDNN_DATA_FLOAT,
                                          HIPDNN_TENSOR_NCHW, 4, filterDimA));

  hipdnnConvolutionDescriptor_t conv_desc;
  checkHIPDNN(hipdnnCreateConvolutionDescriptor(&conv_desc));
  checkHIPDNN(hipdnnSetConvolution2dDescriptor(
      conv_desc, c.padh, c.padw, c.strh, c.strw, c.dilh, c.dilw,
      HIPDNN_CROSS_CORRELATION, HIPDNN_DATA_FLOAT));

  checkHIPDNN(hipdnnGetConvolution2dForwardOutputDim(
      conv_desc, in_desc, filt_desc, &c.mb, &c.oc, &c.oh, &c.ow));

  hipdnnTensorDescriptor_t out_desc;
  checkHIPDNN(hipdnnCreateTensorDescriptor(&out_desc));
  checkHIPDNN(hipdnnSetTensor4dDescriptor(
      out_desc, HIPDNN_TENSOR_NCHW, HIPDNN_DATA_FLOAT, c.mb, c.oc, c.oh, c.ow));

  hipdnnConvolutionFwdAlgo_t algo;
  int MaxAlgoCount = 1;
  size_t ws_size{0};
  float *ws_data{nullptr};
  int calgo;
  hipdnnConvolutionFwdAlgoPerf_t algoPerf[MaxAlgoCount];

  checkHIPDNN(hipdnnGetConvolutionForwardWorkspaceSize(
      hipdnn, in_desc, filt_desc, conv_desc, out_desc, algo, &ws_size));

  hipMalloc(&ws_data, ws_size);

  std::cout<<"\nWS:"<<ws_size;

  hipdnnFindConvolutionForwardAlgorithmEx(
      hipdnn, in_desc, src, filt_desc, weights, conv_desc, out_desc, dst,
      MaxAlgoCount, &calgo, algoPerf, ws_data, ws_size);

  algo = (hipdnnConvolutionFwdAlgo_t)algoPerf[0].algo;

  float alpha = 1.f;
  float beta = 0.f;

  high_resolution_timer_t timer;
  std::vector<double> time_vector(benchmark_iterations, 0);

  for (int i = 0; i < benchmark_iterations; i++) {

        timer.restart();
        checkHIPDNN(hipdnnConvolutionForward(hipdnn, &alpha, in_desc, src,
                                       filt_desc, weights, conv_desc, algo,
                                       ws_data, ws_size, &beta, out_desc, dst));

        hipDeviceSynchronize();

        std::uint64_t time_elapsed = timer.elapsed_nanoseconds();
        time_vector[i] = (double)time_elapsed / 1000;
    }

  *avg_time = (float)std::accumulate(time_vector.begin() + 10, time_vector.end(),
                                    0) / (benchmark_iterations - 10);

  hipFree(ws_data);
  hipdnnDestroyTensorDescriptor(out_desc);
  hipdnnDestroyConvolutionDescriptor(conv_desc);
  hipdnnDestroyFilterDescriptor(filt_desc);
  hipdnnDestroyTensorDescriptor(in_desc);
  hipdnnDestroy(hipdnn);

}

template <typename dataType>
void compute_mpool_fwd(test_pooling_descriptor &c, dataType *src,
                                dataType *dst, float *avg_time) {

  hipdnnHandle_t handle;
  checkHIPDNN(hipdnnCreate(&handle));

  hipdnnTensorDescriptor_t in_desc, out_desc;

  checkHIPDNN(hipdnnCreateTensorDescriptor(&in_desc));
  checkHIPDNN(hipdnnSetTensor4dDescriptor(
      in_desc, HIPDNN_TENSOR_NCHW, HIPDNN_DATA_FLOAT, c.mb, c.c, c.ih, c.iw));

  hipdnnPoolingDescriptor_t pool_desc;
  checkHIPDNN(hipdnnCreatePoolingDescriptor(&pool_desc));
  checkHIPDNN(hipdnnSetPooling2dDescriptor(pool_desc, HIPDNN_POOLING_MAX,
                                           HIPDNN_NOT_PROPAGATE_NAN, c.kw, c.kh,
                                           c.padt, c.padl, c.strh, c.strw));
  checkHIPDNN(hipdnnGetPooling2dForwardOutputDim(pool_desc, in_desc, &c.mb,
                                                 &c.c, &c.oh, &c.ow));

  checkHIPDNN(hipdnnCreateTensorDescriptor(&out_desc));
  checkHIPDNN(hipdnnSetTensor4dDescriptor(
      out_desc, HIPDNN_TENSOR_NCHW, HIPDNN_DATA_FLOAT, c.mb, c.c, c.oh, c.ow));

  float alpha = 1.f;
  float beta = 0.f;

  high_resolution_timer_t timer;
  std::vector<double> time_vector(benchmark_iterations, 0);

  for (int i = 0; i < benchmark_iterations; i++) {

        timer.restart();
        checkHIPDNN(hipdnnPoolingForward(handle, pool_desc, &alpha, in_desc, src,
                                   &beta, out_desc, dst, true));

        hipDeviceSynchronize();

        std::uint64_t time_elapsed = timer.elapsed_nanoseconds();
        time_vector[i] = (double)time_elapsed / 1000;
    }

  *avg_time = (float)std::accumulate(time_vector.begin() + 10, time_vector.end(),
                                    0) / (benchmark_iterations - 10);

  checkHIPDNN(hipdnnDestroyTensorDescriptor(in_desc));
  checkHIPDNN(hipdnnDestroyTensorDescriptor(out_desc));
  checkHIPDNN(hipdnnDestroyPoolingDescriptor(pool_desc));
  checkHIPDNN(hipdnnDestroy(handle));
}

template <typename dataType>
void compute_mpool_bwd(pool_bwd &test_case, dataType *src,
                               dataType *grad, dataType *dst, float *avg_time) {

  hipdnnHandle_t hipdnn;
  checkHIPDNN(hipdnnCreate(&hipdnn));

  hipdnnTensorDescriptor_t in_desc;
  checkHIPDNN(hipdnnCreateTensorDescriptor(&in_desc));
  checkHIPDNN(hipdnnSetTensor4dDescriptor(
      in_desc, HIPDNN_TENSOR_NCHW, HIPDNN_DATA_FLOAT, test_case.in,
      test_case.ichannel, test_case.iheight, test_case.oheight));

  hipdnnPoolingDescriptor_t pool_desc;
  checkHIPDNN(hipdnnCreatePoolingDescriptor(&pool_desc));

  hipdnnPoolingMode_t poolmode = HIPDNN_POOLING_MAX;
  hipdnnNanPropagation_t maxpoolingNanOpt = HIPDNN_NOT_PROPAGATE_NAN;

  checkHIPDNN(hipdnnSetPooling2dDescriptor(
      pool_desc, poolmode, maxpoolingNanOpt, test_case.wheight,
      test_case.wwidth, test_case.vpadding, test_case.hpadding,
      test_case.vstride, test_case.hstride));

  checkHIPDNN(hipdnnGetPooling2dForwardOutputDim(
      pool_desc, in_desc, &test_case.on, &test_case.ochannel,
      &test_case.oheight, &test_case.owidth)) hipdnnTensorDescriptor_t out_desc;
  checkHIPDNN(hipdnnCreateTensorDescriptor(&out_desc));
  checkHIPDNN(hipdnnSetTensor4dDescriptor(
      out_desc, HIPDNN_TENSOR_NCHW, HIPDNN_DATA_FLOAT, test_case.on,
      test_case.ochannel, test_case.oheight, test_case.owidth));

  float alpha = 1.f;
  float beta = 0.f;

  hipdnnPoolingForward(hipdnn, pool_desc, &alpha, in_desc, src, &beta, out_desc,
                       dst, true);

  high_resolution_timer_t timer;
  std::vector<double> time_vector(benchmark_iterations, 0);

  for (int i = 0; i < benchmark_iterations; i++) {

        timer.restart();
        hipdnnPoolingBackward(hipdnn, pool_desc, &alpha, out_desc, dst, out_desc,
                        dst, in_desc, src, &beta, in_desc, grad);

        hipDeviceSynchronize();

        std::uint64_t time_elapsed = timer.elapsed_nanoseconds();
        time_vector[i] = (double)time_elapsed / 1000;
    }

  *avg_time = (float)std::accumulate(time_vector.begin() + 10, time_vector.end(),
                                    0) / (benchmark_iterations - 10);

  hipdnnDestroyTensorDescriptor(out_desc);
  hipdnnDestroyPoolingDescriptor(pool_desc);
  hipdnnDestroyTensorDescriptor(in_desc);
  hipdnnDestroy(hipdnn);

}

template <typename dataType>
void compute_conv_bwd_kernel(convulution_Size &c, dataType *src,
                             dataType *weights, dataType *grad, dataType *bias,
                             dataType *dst, float *avg_time) {

  hipdnnHandle_t hipdnn;
  checkHIPDNN(hipdnnCreate(&hipdnn));

  hipdnnTensorDescriptor_t in_desc;
  checkHIPDNN(hipdnnCreateTensorDescriptor(&in_desc));
  checkHIPDNN(hipdnnSetTensor4dDescriptor(in_desc, HIPDNN_TENSOR_NCHW,
                                          HIPDNN_DATA_FLOAT, c.mb, c.ic, c.ih,
                                          c.iw));

  hipdnnFilterDescriptor_t filt_desc;
  checkHIPDNN(hipdnnCreateFilterDescriptor(&filt_desc));

  int filterDimA[] = {c.oc, c.ic, c.kh, c.kw};

  checkHIPDNN(hipdnnSetFilterNdDescriptor(filt_desc, HIPDNN_DATA_FLOAT,
                                            HIPDNN_TENSOR_NCHW, 4, filterDimA));

  hipdnnConvolutionDescriptor_t conv_desc;
  checkHIPDNN(hipdnnCreateConvolutionDescriptor(&conv_desc));
  checkHIPDNN(hipdnnSetConvolution2dDescriptor(
        conv_desc, c.padh, c.padw, c.strh, c.strw, c.dilh, c.dilw,
        HIPDNN_CROSS_CORRELATION, HIPDNN_DATA_FLOAT));

  checkHIPDNN(hipdnnGetConvolution2dForwardOutputDim(
        conv_desc, in_desc, filt_desc, &c.mb, &c.oc, &c.oh, &c.ow));

  hipdnnTensorDescriptor_t out_desc;
  checkHIPDNN(hipdnnCreateTensorDescriptor(&out_desc));
  checkHIPDNN(hipdnnSetTensor4dDescriptor(out_desc, HIPDNN_TENSOR_NCHW,
                                            HIPDNN_DATA_FLOAT, c.mb, c.oc, c.oh,
                                            c.ow));

  hipdnnConvolutionFwdAlgo_t algo;
  int MaxAlgoCount = 2;
  size_t ws_size{0};
  float *ws_data{nullptr};
  int calgo;
  hipdnnConvolutionFwdAlgoPerf_t algoPerf[MaxAlgoCount];

  checkHIPDNN(hipdnnGetConvolutionForwardWorkspaceSize(
        hipdnn, in_desc, filt_desc, conv_desc, out_desc, algo, &ws_size));

  hipMalloc(&ws_data, ws_size);

  hipdnnFindConvolutionForwardAlgorithmEx(
        hipdnn, in_desc, src, filt_desc, weights, conv_desc, out_desc, dst,
        MaxAlgoCount, &calgo, algoPerf, ws_data, ws_size);

  algo = (hipdnnConvolutionFwdAlgo_t)algoPerf[0].algo;

  hipLaunchKernel(dev_const, c.mb * c.oc, c.oh * c.ow, 0, 0, dst, 0.0f);

  float alpha = 1.f;
  float beta = 0.f;

  checkHIPDNN(hipdnnConvolutionForward(
        hipdnn, &alpha, in_desc, src, filt_desc, weights, conv_desc, algo,
        ws_data, ws_size, &beta, out_desc, dst));

  hipdnnConvolutionBwdFilterAlgo_t b_algo = HIPDNN_CONVOLUTION_BWD_FILTER_ALGO_1;

  ws_size = 0;

  hipdnnConvolutionBwdFilterAlgoPerf_t b_algoPerf[MaxAlgoCount];

  checkHIPDNN(hipdnnGetConvolutionBackwardFilterWorkspaceSize(
        hipdnn, in_desc, out_desc, conv_desc, filt_desc, b_algo, &ws_size));

  hipMalloc(&ws_data, ws_size);

  hipLaunchKernel(dev_const, c.oc * c.ic, c.kh * c.kw, 0, 0, grad, 0.0f);

  hipdnnFindConvolutionBackwardFilterAlgorithmEx(
        hipdnn, in_desc, src, out_desc, dst, conv_desc, filt_desc, weights,
        MaxAlgoCount, &calgo, b_algoPerf, ws_data, ws_size);

  b_algo = (hipdnnConvolutionBwdFilterAlgo_t)b_algoPerf[0].algo;

  high_resolution_timer_t timer;
  std::vector<double> time_vector(benchmark_iterations, 0);

  for (int i = 0; i < benchmark_iterations; i++) {

        timer.restart();
        checkHIPDNN(hipdnnConvolutionBackwardFilter(
            hipdnn, &alpha, in_desc, src, out_desc, dst, conv_desc, b_algo,
            ws_data, ws_size, &beta, filt_desc, grad));

        hipDeviceSynchronize();

        std::uint64_t time_elapsed = timer.elapsed_nanoseconds();
        time_vector[i] = (double)time_elapsed / 1000;
    }

  *avg_time = (float)std::accumulate(time_vector.begin() + 10, time_vector.end(),
                                    0) / (benchmark_iterations - 10);

  hipFree(ws_data);
  hipdnnDestroyTensorDescriptor(out_desc);
  hipdnnDestroyConvolutionDescriptor(conv_desc);
  hipdnnDestroyFilterDescriptor(filt_desc);
  hipdnnDestroyTensorDescriptor(in_desc);
  hipdnnDestroy(hipdnn);

}

#endif //TEST_CONVOLUTION_POOLING_HPP

#include "test_pooling_common.hpp"

hipdnnPoolingMode_t pool_mode;

TEST(pooling_fwd, func_check_zero_padding) {

  pool_mode = HIPDNN_POOLING_MAX;
  test_pooling_descriptor pool(1, 1, 4, 4, 2, 2, 2, 2, 0, 0, 2, 2);

  Memory<float> srcData(pool.mb * pool.c * pool.ih * pool.iw);
  Memory<float> dstDataGPU(pool.mb * pool.c * pool.oh * pool.ow);

  float avg_time = 0;

  populateMemoryRandom<float>(srcData);

  int ip_size[4] = {pool.mb, pool.c, pool.ih, pool.iw};
  int k_size[4] = {pool.mb, pool.c, pool.kh, pool.kw};
  int op_size[4] =  {pool.mb, pool.c, pool.oh, pool.ow};

  std::string str_ip_size  = convert_to_string((int*)ip_size,4);
  std::string str_k_size  = convert_to_string((int*)k_size,4);
  std::string str_op_size  = convert_to_string((int*)op_size,4);

  hipdnn_pooling_forward<float>(pool, srcData.gpu(), dstDataGPU.gpu(), pool_mode,
                                &avg_time);

  std::cout << "\nAverage Time is: " << avg_time << "micro seconds"<<std::endl;

  float* temp2 = srcData.getDataFromGPU();
  std::string ip = convert_to_string((float*)temp2,(int)srcData.get_num_elements());

  float* temp = dstDataGPU.getDataFromGPU();

  std::string strt = "./result_unittest.csv";
  std::string testname = "pooling_fwd:func_check_zero_padding";
  std::string filename="pooling_forward.csv";

  std::string str  = convert_to_string((float*)temp,(int)dstDataGPU.get_num_elements());

  write_to_csv(strt, str, testname, avg_time, str_ip_size, str_k_size, str_op_size);
  dump_result_csv(filename, testname, temp, (int)dstDataGPU.get_num_elements());
}

TEST(pooling_fwd, func_check_zero_padding_DETERMINISTIC) {

  pool_mode = HIPDNN_POOLING_MAX_DETERMINISTIC;
  test_pooling_descriptor pool(1, 1, 4, 4, 2, 2, 2, 2, 0, 0, 2, 2);

  Memory<float> srcData(pool.mb * pool.c * pool.ih * pool.iw);
  Memory<float> dstDataGPU(pool.mb * pool.c * pool.oh * pool.ow);

  float avg_time = 0;

  populateMemoryRandom<float>(srcData);

  int ip_size[4] = {pool.mb, pool.c, pool.ih, pool.iw};
  int k_size[4] = {pool.mb, pool.c, pool.kh, pool.kw};
  int op_size[4] =  {pool.mb, pool.c, pool.oh, pool.ow};

  std::string str_ip_size  = convert_to_string((int*)ip_size,4);
  std::string str_k_size  = convert_to_string((int*)k_size,4);
  std::string str_op_size  = convert_to_string((int*)op_size,4);

  hipdnn_pooling_forward<float>(pool, srcData.gpu(), dstDataGPU.gpu(), pool_mode,
                                &avg_time);

  std::cout << "\nAverage Time is: " << avg_time << "micro seconds"<<std::endl;

  float* temp2 = srcData.getDataFromGPU();
  std::string ip = convert_to_string((float*)temp2,(int)srcData.get_num_elements());

  float* temp = dstDataGPU.getDataFromGPU();

  std::string strt = "./result_unittest.csv";
  std::string testname = "pooling_fwd:func_check_zero_padding_DETERMINISTIC";
  std::string filename="pooling_forward.csv";

  std::string str  = convert_to_string((float*)temp,(int)dstDataGPU.get_num_elements());

  write_to_csv(strt, str, testname, avg_time, str_ip_size, str_k_size, str_op_size);
  dump_result_csv(filename, testname, temp, (int)dstDataGPU.get_num_elements());
}

TEST(pooling_fwd, func_check_AVERAGE_COUNT_INCLUDE_PADDING) {

  pool_mode = HIPDNN_POOLING_AVERAGE_COUNT_INCLUDE_PADDING;
  test_pooling_descriptor pool(1, 1, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2);

  Memory<float> srcData(pool.mb * pool.c * pool.ih * pool.iw);
  Memory<float> dstDataGPU(pool.mb * pool.c * pool.oh * pool.ow);

  float avg_time = 0;

  populateMemoryRandom<float>(srcData);

  int ip_size[4] = {pool.mb, pool.c, pool.ih, pool.iw};
  int k_size[4] = {pool.mb, pool.c, pool.kh, pool.kw};
  int op_size[4] =  {pool.mb, pool.c, pool.oh, pool.ow};

  std::string str_ip_size  = convert_to_string((int*)ip_size,4);
  std::string str_k_size  = convert_to_string((int*)k_size,4);
  std::string str_op_size  = convert_to_string((int*)op_size,4);

  hipdnn_pooling_forward<float>(pool, srcData.gpu(), dstDataGPU.gpu(), pool_mode,
                                &avg_time);

  std::cout << "\nAverage Time is: " << avg_time << "micro seconds"<<std::endl;

  float* temp2 = srcData.getDataFromGPU();
  std::string ip = convert_to_string((float*)temp2,(int)srcData.get_num_elements());

  float* temp = dstDataGPU.getDataFromGPU();

  std::string strt = "./result_unittest.csv";
  std::string testname = "pooling_fwd:func_check_AVERAGE_COUNT_INCLUDE_PADDING";
  std::string filename="pooling_forward.csv";

  std::string str  = convert_to_string((float*)temp,(int)dstDataGPU.get_num_elements());

  write_to_csv(strt, str, testname, avg_time, str_ip_size, str_k_size, str_op_size);
  dump_result_csv(filename, testname, temp, (int)dstDataGPU.get_num_elements());
}

TEST(pooling_fwd, func_check_AVERAGE_COUNT_EXCLUDE_PADDING) {

  pool_mode = HIPDNN_POOLING_AVERAGE_COUNT_EXCLUDE_PADDING;
  test_pooling_descriptor pool(1, 1, 4, 4, 2, 2, 2, 2, 2, 2, 2, 2);

  Memory<float> srcData(pool.mb * pool.c * pool.ih * pool.iw);
  Memory<float> dstDataGPU(pool.mb * pool.c * pool.oh * pool.ow);

  float avg_time = 0;

  populateMemoryRandom<float>(srcData);

  int ip_size[4] = {pool.mb, pool.c, pool.ih, pool.iw};
  int k_size[4] = {pool.mb, pool.c, pool.kh, pool.kw};
  int op_size[4] =  {pool.mb, pool.c, pool.oh, pool.ow};

  std::string str_ip_size  = convert_to_string((int*)ip_size,4);
  std::string str_k_size  = convert_to_string((int*)k_size,4);
  std::string str_op_size  = convert_to_string((int*)op_size,4);

  hipdnn_pooling_forward<float>(pool, srcData.gpu(), dstDataGPU.gpu(), pool_mode,
                                &avg_time);

  std::cout << "\nAverage Time is: " << avg_time << "micro seconds"<<std::endl;

  float* temp2 = srcData.getDataFromGPU();
  std::string ip = convert_to_string((float*)temp2,(int)srcData.get_num_elements());

  float* temp = dstDataGPU.getDataFromGPU();

  std::string strt = "./result_unittest.csv";
  std::string testname = "pooling_fwd:func_check_AVERAGE_COUNT_EXCLUDE_PADDING";
  std::string filename="pooling_forward.csv";

  std::string str  = convert_to_string((float*)temp,(int)dstDataGPU.get_num_elements());

  write_to_csv(strt, str, testname, avg_time, str_ip_size, str_k_size, str_op_size);
  dump_result_csv(filename, testname, temp, (int)dstDataGPU.get_num_elements());
}

TEST(pooling_fwd, func_check_batch32) {

  pool_mode = HIPDNN_POOLING_MAX;
  test_pooling_descriptor pool(32, 1, 224, 224, 224 / 2, 224 / 2, 2, 2, 0, 0, 2, 2);

  Memory<float> srcData(pool.mb * pool.c * pool.ih * pool.iw);
  Memory<float> dstDataGPU(pool.mb * pool.c * pool.oh * pool.ow);

  float avg_time = 0;

  populateMemoryRandom<float>(srcData);

  int ip_size[4] = {pool.mb, pool.c, pool.ih, pool.iw};
  int k_size[4] = {pool.mb, pool.c, pool.kh, pool.kw};
  int op_size[4] =  {pool.mb, pool.c, pool.oh, pool.ow};

  std::string str_ip_size  = convert_to_string((int*)ip_size,4);
  std::string str_k_size  = convert_to_string((int*)k_size,4);
  std::string str_op_size  = convert_to_string((int*)op_size,4);

  hipdnn_pooling_forward<float>(pool, srcData.gpu(), dstDataGPU.gpu(), pool_mode,
                                &avg_time);

  std::cout << "\nAverage Time is: " << avg_time << "micro seconds"<<std::endl;

  float* temp = dstDataGPU.getDataFromGPU();

  std::string strt = "./result_unittest.csv";
  std::string testname = "pooling_fwd:func_check_batch32";
  std::string filename="pooling_forward.csv";

  std::string str  = convert_to_string((float*)temp,(int)dstDataGPU.get_num_elements());

  write_to_csv(strt, str, testname, avg_time, str_ip_size, str_k_size, str_op_size);
  dump_result_csv(filename, testname, temp, (int)dstDataGPU.get_num_elements());
}
/*
TEST(pooling_fwd, func_check_batch64) {

  test_pooling_descriptor pool(64, 1, 224, 224, 224 / 2, 224 / 2, 2, 2, 0, 0, 2, 2);

  Memory<float> srcData(pool.mb * pool.c * pool.ih * pool.iw);
  Memory<float> dstDataGPU(pool.mb * pool.c * pool.oh * pool.ow);

  float avg_time = 0;

  populateMemoryRandom<float>(srcData);

  int ip_size[4] = {pool.mb, pool.c, pool.ih, pool.iw};
  int k_size[4] = {pool.mb, pool.c, pool.kh, pool.kw};
  int op_size[4] =  {pool.mb, pool.c, pool.oh, pool.ow};

  std::string str_ip_size  = convert_to_string((int*)ip_size,4);
  std::string str_k_size  = convert_to_string((int*)k_size,4);
  std::string str_op_size  = convert_to_string((int*)op_size,4);

  hipdnn_pooling_forward<float>(pool, srcData.gpu(), dstDataGPU.gpu(), &avg_time);
  std::cout << "\nAverage Time is: " << avg_time << "micro seconds"<<std::endl;

  float* temp = dstDataGPU.getDataFromGPU();

  std::string strt = "./result_unittest.csv";
  std::string testname = "pooling_fwd:func_check_batch64";
  std::string filename="pooling_forward.csv";

  std::string str  = convert_to_string((float*)temp,(int)dstDataGPU.get_num_elements());
  write_to_csv(strt, str, testname, avg_time, str_ip_size, str_k_size, str_op_size);
  dump_result_csv(filename, testname, temp, (int)dstDataGPU.get_num_elements());
}

TEST(pooling_fwd, func_check_batch128) {

  test_pooling_descriptor pool(128, 1, 4, 4, 2, 2, 2, 2, 0, 0, 2, 2);

  Memory<float> srcData(pool.mb * pool.c * pool.ih * pool.iw);
  Memory<float> dstDataGPU(pool.mb * pool.c * pool.oh * pool.ow);

  float avg_time = 0;

  populateMemoryRandom<float>(srcData);

  int ip_size[4] = {pool.mb, pool.c, pool.ih, pool.iw};
  int k_size[4] = {pool.mb, pool.c, pool.kh, pool.kw};
  int op_size[4] =  {pool.mb, pool.c, pool.oh, pool.ow};

  std::string str_ip_size  = convert_to_string((int*)ip_size,4);
  std::string str_k_size  = convert_to_string((int*)k_size,4);
  std::string str_op_size  = convert_to_string((int*)op_size,4);

  hipdnn_pooling_forward<float>(pool, srcData.gpu(), dstDataGPU.gpu(), &avg_time);
  std::cout << "\nAverage Time is: " << avg_time << "micro seconds"<<std::endl;

  float* temp = dstDataGPU.getDataFromGPU();

  std::string strt = "./result_unittest.csv";
  std::string testname = "pooling_fwd:func_check_batch128";
  std::string filename="pooling_forward.csv";

  std::string str  = convert_to_string((float*)temp,(int)dstDataGPU.get_num_elements());
  write_to_csv(strt, str, testname, avg_time, str_ip_size, str_k_size, str_op_size);
  dump_result_csv(filename, testname, temp, (int)dstDataGPU.get_num_elements());
}
*/
//This test currently fails in NVidia, so it has been commented
/*
//half2

TEST(pooling_fwd, func_check_half) {

  test_pooling_descriptor pool(1, 1, 4, 4, 2, 2, 2, 2, 0, 0, 2, 2);

  Memory<half2> srcData(pool.mb * pool.c * pool.ih * pool.iw);
  Memory<half2> dstDataGPU(pool.mb * pool.c * pool.oh * pool.ow);

  float avg_time = 0;

  populateMemoryRandom<half2>(srcData);

  int ip_size[4] = {pool.mb, pool.c, pool.ih, pool.iw};
  int k_size[4] = {pool.mb, pool.c, pool.kh, pool.kw};
  int op_size[4] =  {pool.mb, pool.c, pool.oh, pool.ow};

  std::string str_ip_size  = convert_to_string((int*)ip_size,4);
  std::string str_k_size  = convert_to_string((int*)k_size,4);
  std::string str_op_size  = convert_to_string((int*)op_size,4);

  hipdnn_pooling_forward(pool, srcData.gpu(), dstDataGPU.gpu(), &avg_time);
  std::cout << "\nAverage Time is: " << avg_time << "micro seconds"<<std::endl;

  half2* temp = dstDataGPU.getDataFromGPU();

  std::string strt = "./result_unittest.csv";
  std::string testname = "pooling_fwd:func_half_datatype";
  std::string filename="pooling_forward.csv";

  std::string str  = convert_to_string((float*)temp,(int)dstDataGPU.get_num_elements());

  half2* temp2 = srcData.getDataFromGPU();
  std::string ip = convert_to_string((float*)temp2,(int)srcData.get_num_elements());

  write_to_csv(strt, str, testname, avg_time, str_ip_size, str_k_size, str_op_size);
  dump_result_csv(filename, testname, temp, (int)dstDataGPU.get_num_elements());
}
*/

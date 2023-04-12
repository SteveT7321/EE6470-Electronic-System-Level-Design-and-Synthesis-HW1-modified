#ifndef MM_FILTER_H_
#define MM_FILTER_H_
#include <systemc>
using namespace sc_core;

#include "filter_def.h"

class mmfilter : public sc_module {
public:
  sc_in_clk i_clk;
  sc_in<bool> i_rst;
  sc_fifo_in<unsigned char> i_r;
  sc_fifo_in<unsigned char> i_g;
  sc_fifo_in<unsigned char> i_b;

  //for the data buffer
  sc_fifo_in<bool> i_buffer;

  //for the added rgb channel
  sc_fifo_out<int> o_result_r;
  sc_fifo_out<int> o_result_g;
  sc_fifo_out<int> o_result_b;

  SC_HAS_PROCESS(mmfilter);
  mmfilter(sc_module_name n);
  ~mmfilter() = default;

private:
  void do_med_filter();
  void do_mean_filter();
  int val[MASK_N];
};
#endif

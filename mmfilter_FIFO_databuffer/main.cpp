#include <iostream>
#include <string>
using namespace std;

// Wall Clock Time Measurement
#include <sys/time.h>

#include "mmfilter.h"
#include "Testbench.h"

// TIMEVAL STRUCT IS Defined ctime
// use start_time and end_time variables to capture
// start of simulation and end of simulation
struct timeval start_time, end_time;

// int main(int argc, char *argv[])
int sc_main(int argc, char **argv) {
  if ((argc < 3) || (argc > 4)) {
    cout << "No arguments for the executable : " << argv[0] << endl;
    cout << "Usage : >" << argv[0] << " in_image_file_name out_image_file_name"
         << endl;
    return 0;
  }

  //Create modules and signals
  Testbench tb("tb");
  mmfilter mm_filter("mm_filter");
  sc_clock clk("clk", CLOCK_PERIOD, SC_NS);
  sc_signal<bool> rst("rst");

  //Create FIFO channels
  sc_fifo<unsigned char> r;
  sc_fifo<unsigned char> g;
  sc_fifo<unsigned char> b;
  sc_fifo<int> x;
  sc_fifo<int> y;
  sc_fifo<int> result_r;
  sc_fifo<int> result_g;
  sc_fifo<int> result_b;
  sc_fifo<bool> buffer;

  //Connect FIFO channels with modules
  tb.i_clk(clk);
  tb.o_rst(rst);
  mm_filter.i_clk(clk);
  mm_filter.i_rst(rst);

  tb.o_r(r);
  tb.o_g(g);
  tb.o_b(b);
  tb.o_buffer(buffer);
  tb.i_result_r(result_r);
  tb.i_result_g(result_g);
  tb.i_result_b(result_b);
  mm_filter.i_r(r);
  mm_filter.i_g(g);
  mm_filter.i_b(b);
  mm_filter.i_buffer(buffer);
  mm_filter.o_result_r(result_r);
  mm_filter.o_result_g(result_g);
  mm_filter.o_result_b(result_b);

  tb.read_bmp(argv[1]);
  sc_start();
  std::cout << "Simulated time == " << sc_core::sc_time_stamp() << std::endl;
  tb.write_bmp(argv[2]);

  return 0;
}

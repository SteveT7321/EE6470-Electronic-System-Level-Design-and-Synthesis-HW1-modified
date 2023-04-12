#include <cmath>

#include "mmfilter.h"

mmfilter::mmfilter(sc_module_name n) : sc_module(n) {
  SC_THREAD(do_med_filter);
  sensitive << i_clk.pos();
  SC_THREAD(do_mean_filter);
  sensitive << i_clk.pos();
  dont_initialize();
  reset_signal_is(i_rst, false);
}

// mean_mask
const int mask[MASK_X][MASK_Y] = {{1, 1, 1}, {1, 2, 1}, {1, 1, 1}}; // sum = 10
int pixel_r[3][3]={0};
int pixel_g[3][3]={0};
int pixel_b[3][3]={0};
int r_buffer[2][3]={0};
int g_buffer[2][3]={0};
int b_buffer[2][3]={0};

void mmfilter::do_med_filter() {
  while (true) {
    if(i_buffer.read()){
      // Read in a 3x3 sliding window of pixels centered around the current pixel
      for (unsigned int v = 0; v < MASK_Y; ++v) {
        for (unsigned int u = 0; u < MASK_X; ++u) {
          pixel_r[u][v] = i_r.read();
          pixel_g[u][v] = i_g.read();
          pixel_b[u][v] = i_b.read();
        }
      }
      // To write the buffer
      for (unsigned int v = 0; v < MASK_Y; ++v){
        for (unsigned int u = 0; u < MASK_X-1; ++u){ 
        r_buffer[u][v] = pixel_r[u+1][v];
        g_buffer[u][v] = pixel_g[u+1][v]; 
        b_buffer[u][v] = pixel_b[u+1][v];
        }
      }
    }
    else{
      for (unsigned int v = 0; v < MASK_Y; ++v){
        for (unsigned int u = 0; u < MASK_X; ++u){
          if(u!=2){
          pixel_r[u][v] = r_buffer[u][v];
          pixel_g[u][v] = g_buffer[u][v];
          pixel_b[u][v] = b_buffer[u][v];
          }
          else{
          pixel_r[u][v] = i_r.read();
          pixel_g[u][v] = i_g.read();
          pixel_b[u][v] = i_b.read();
          }
        }
      }
      // To write the buffer
      for (unsigned int v = 0; v < MASK_Y; ++v){
        for (unsigned int u = 0; u < MASK_X-1; ++u){ 
        r_buffer[u][v] = pixel_r[u+1][v];
        g_buffer[u][v] = pixel_g[u+1][v]; 
        b_buffer[u][v] = pixel_b[u+1][v];
        }
      }
    }

    // 1. Applying "median filter" to each color channel
    // Flatten the 2D array into a 1D array
    int flattened_r[9],flattened_g[9],flattened_b[9];
    int k = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            flattened_r[k++] = pixel_r[i][j];
            flattened_g[k++] = pixel_g[i][j];
            flattened_b[k++] = pixel_b[i][j];

        }
    }
    std::sort(flattened_r, flattened_r + 9);
    std::sort(flattened_g, flattened_g + 9);
    std::sort(flattened_b, flattened_b + 9);

    unsigned char filtered_r = flattened_r[4];
    unsigned char filtered_g = flattened_g[4];
    unsigned char filtered_b = flattened_b[4];

    // Write out the filtered pixel
    o_result_r.write(filtered_r);
    o_result_g.write(filtered_g);
    o_result_b.write(filtered_b);

    wait(5); //emulate module delay
    
  }
}

void mmfilter::do_mean_filter() {
  while (true) {
    if(i_buffer.read()){
      // Read in a 3x3 sliding window of pixels centered around the current pixel
      for (unsigned int v = 0; v < MASK_Y; ++v) {
        for (unsigned int u = 0; u < MASK_X; ++u) {
          pixel_r[u][v] = i_r.read();
          pixel_g[u][v] = i_g.read();
          pixel_b[u][v] = i_b.read();
        }
      }
      // To write the buffer
      for (unsigned int v = 0; v < MASK_Y; ++v){
        for (unsigned int u = 0; u < MASK_X-1; ++u){ 
        r_buffer[u][v] = pixel_r[u+1][v];
        g_buffer[u][v] = pixel_g[u+1][v]; 
        b_buffer[u][v] = pixel_b[u+1][v];
        }
      }
    }
    else{
      for (unsigned int v = 0; v < MASK_Y; ++v){
        for (unsigned int u = 0; u < MASK_X; ++u){
          if(u!=2){
          pixel_r[u][v] = r_buffer[u][v];
          pixel_g[u][v] = g_buffer[u][v];
          pixel_b[u][v] = b_buffer[u][v];
          }
          else{
          pixel_r[u][v] = i_r.read();
          pixel_g[u][v] = i_g.read();
          pixel_b[u][v] = i_b.read();
          }
        }
      }
      // To write the buffer
      for (unsigned int v = 0; v < MASK_Y; ++v){
        for (unsigned int u = 0; u < MASK_X-1; ++u){ 
        r_buffer[u][v] = pixel_r[u+1][v];
        g_buffer[u][v] = pixel_g[u+1][v]; 
        b_buffer[u][v] = pixel_b[u+1][v];
        }
      }
    }

    // 2. Applying "mean filter" to each color channel
    int sum_r = 0, sum_g = 0, sum_b = 0;

    for (unsigned int v = 0; v < MASK_Y; ++v) {
      for (unsigned int u = 0; u < MASK_X; ++u) {
        sum_r += pixel_r[u][v] * mask[u][v];
        sum_g += pixel_g[u][v] * mask[u][v];
        sum_b += pixel_b[u][v] * mask[u][v];
      }
    }


    unsigned char filtered_r = sum_r / 10;
    unsigned char filtered_g = sum_g / 10;
    unsigned char filtered_b = sum_b / 10;

    // Write out the filtered pixel
    o_result_r.write(filtered_r);
    o_result_g.write(filtered_g);
    o_result_b.write(filtered_b);

    wait(5); //emulate module delay
    
  }
}


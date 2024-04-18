Part1 median and mean filter with FIFO

Part2 median and mean filter with FIFO (using a 18-pixels data buffer)

Introduction: 本作業分為兩個部分。第一部分(mmfilter_FIFO)使用 FIFO channels 實現 median and mean filter ，一次只在filter中處理一個pixel的計算；而第二部分 (mmfilter_FIFO_databuffer) 基於第一部分的功能，加入了18-pixels的data buffer，表示一次傳送 18-pixels的資料進入filter，並在之後的計算直接使用buffer儲存的資料來加快運算。

Conclusion: 兩個部份的 simulation time 分別為 2359296ns 及 1312256 ns，可以發現使用 buffer 可以減少資料重複讀取，並加速不少simulation的時間，但實際上buffer的增加還應該需要檢查 power, speed, area 等等其他影響因素，以做出比較好的設計。

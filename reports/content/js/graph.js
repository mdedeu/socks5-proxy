/*
   Licensed to the Apache Software Foundation (ASF) under one or more
   contributor license agreements.  See the NOTICE file distributed with
   this work for additional information regarding copyright ownership.
   The ASF licenses this file to You under the Apache License, Version 2.0
   (the "License"); you may not use this file except in compliance with
   the License.  You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
$(document).ready(function() {

    $(".click-title").mouseenter( function(    e){
        e.preventDefault();
        this.style.cursor="pointer";
    });
    $(".click-title").mousedown( function(event){
        event.preventDefault();
    });

    // Ugly code while this script is shared among several pages
    try{
        refreshHitsPerSecond(true);
    } catch(e){}
    try{
        refreshResponseTimeOverTime(true);
    } catch(e){}
    try{
        refreshResponseTimePercentiles();
    } catch(e){}
});


var responseTimePercentilesInfos = {
        data: {"result": {"minY": 474.0, "minX": 0.0, "maxY": 3687.0, "series": [{"data": [[0.0, 474.0], [0.1, 474.0], [0.2, 480.0], [0.3, 480.0], [0.4, 480.0], [0.5, 480.0], [0.6, 480.0], [0.7, 480.0], [0.8, 482.0], [0.9, 482.0], [1.0, 483.0], [1.1, 483.0], [1.2, 483.0], [1.3, 483.0], [1.4, 485.0], [1.5, 485.0], [1.6, 485.0], [1.7, 485.0], [1.8, 485.0], [1.9, 485.0], [2.0, 485.0], [2.1, 485.0], [2.2, 486.0], [2.3, 486.0], [2.4, 486.0], [2.5, 486.0], [2.6, 487.0], [2.7, 487.0], [2.8, 488.0], [2.9, 488.0], [3.0, 488.0], [3.1, 488.0], [3.2, 489.0], [3.3, 489.0], [3.4, 489.0], [3.5, 489.0], [3.6, 490.0], [3.7, 490.0], [3.8, 490.0], [3.9, 490.0], [4.0, 490.0], [4.1, 490.0], [4.2, 490.0], [4.3, 490.0], [4.4, 490.0], [4.5, 490.0], [4.6, 490.0], [4.7, 490.0], [4.8, 490.0], [4.9, 490.0], [5.0, 491.0], [5.1, 491.0], [5.2, 491.0], [5.3, 491.0], [5.4, 491.0], [5.5, 491.0], [5.6, 492.0], [5.7, 492.0], [5.8, 492.0], [5.9, 492.0], [6.0, 492.0], [6.1, 492.0], [6.2, 492.0], [6.3, 492.0], [6.4, 492.0], [6.5, 492.0], [6.6, 492.0], [6.7, 493.0], [6.8, 493.0], [6.9, 493.0], [7.0, 493.0], [7.1, 494.0], [7.2, 494.0], [7.3, 494.0], [7.4, 494.0], [7.5, 494.0], [7.6, 494.0], [7.7, 494.0], [7.8, 494.0], [7.9, 494.0], [8.0, 494.0], [8.1, 494.0], [8.2, 494.0], [8.3, 495.0], [8.4, 495.0], [8.5, 496.0], [8.6, 496.0], [8.7, 496.0], [8.8, 496.0], [8.9, 496.0], [9.0, 496.0], [9.1, 496.0], [9.2, 496.0], [9.3, 496.0], [9.4, 496.0], [9.5, 496.0], [9.6, 496.0], [9.7, 496.0], [9.8, 496.0], [9.9, 497.0], [10.0, 497.0], [10.1, 497.0], [10.2, 497.0], [10.3, 497.0], [10.4, 497.0], [10.5, 497.0], [10.6, 497.0], [10.7, 497.0], [10.8, 497.0], [10.9, 497.0], [11.0, 497.0], [11.1, 497.0], [11.2, 497.0], [11.3, 497.0], [11.4, 498.0], [11.5, 498.0], [11.6, 498.0], [11.7, 498.0], [11.8, 498.0], [11.9, 498.0], [12.0, 498.0], [12.1, 498.0], [12.2, 498.0], [12.3, 498.0], [12.4, 498.0], [12.5, 498.0], [12.6, 498.0], [12.7, 498.0], [12.8, 498.0], [12.9, 498.0], [13.0, 498.0], [13.1, 498.0], [13.2, 498.0], [13.3, 498.0], [13.4, 498.0], [13.5, 498.0], [13.6, 499.0], [13.7, 499.0], [13.8, 499.0], [13.9, 499.0], [14.0, 499.0], [14.1, 499.0], [14.2, 499.0], [14.3, 499.0], [14.4, 499.0], [14.5, 499.0], [14.6, 499.0], [14.7, 499.0], [14.8, 499.0], [14.9, 499.0], [15.0, 499.0], [15.1, 499.0], [15.2, 500.0], [15.3, 500.0], [15.4, 500.0], [15.5, 500.0], [15.6, 500.0], [15.7, 500.0], [15.8, 500.0], [15.9, 500.0], [16.0, 500.0], [16.1, 500.0], [16.2, 500.0], [16.3, 500.0], [16.4, 500.0], [16.5, 500.0], [16.6, 500.0], [16.7, 501.0], [16.8, 501.0], [16.9, 501.0], [17.0, 501.0], [17.1, 501.0], [17.2, 501.0], [17.3, 501.0], [17.4, 501.0], [17.5, 501.0], [17.6, 501.0], [17.7, 501.0], [17.8, 501.0], [17.9, 501.0], [18.0, 501.0], [18.1, 501.0], [18.2, 501.0], [18.3, 501.0], [18.4, 501.0], [18.5, 502.0], [18.6, 502.0], [18.7, 502.0], [18.8, 502.0], [18.9, 502.0], [19.0, 502.0], [19.1, 502.0], [19.2, 502.0], [19.3, 502.0], [19.4, 502.0], [19.5, 502.0], [19.6, 502.0], [19.7, 502.0], [19.8, 502.0], [19.9, 503.0], [20.0, 503.0], [20.1, 503.0], [20.2, 503.0], [20.3, 503.0], [20.4, 503.0], [20.5, 503.0], [20.6, 503.0], [20.7, 503.0], [20.8, 503.0], [20.9, 503.0], [21.0, 503.0], [21.1, 503.0], [21.2, 503.0], [21.3, 503.0], [21.4, 503.0], [21.5, 503.0], [21.6, 503.0], [21.7, 503.0], [21.8, 503.0], [21.9, 503.0], [22.0, 503.0], [22.1, 503.0], [22.2, 503.0], [22.3, 504.0], [22.4, 504.0], [22.5, 504.0], [22.6, 504.0], [22.7, 504.0], [22.8, 504.0], [22.9, 504.0], [23.0, 504.0], [23.1, 504.0], [23.2, 504.0], [23.3, 504.0], [23.4, 504.0], [23.5, 504.0], [23.6, 505.0], [23.7, 505.0], [23.8, 505.0], [23.9, 505.0], [24.0, 505.0], [24.1, 505.0], [24.2, 505.0], [24.3, 505.0], [24.4, 505.0], [24.5, 505.0], [24.6, 505.0], [24.7, 505.0], [24.8, 505.0], [24.9, 505.0], [25.0, 506.0], [25.1, 506.0], [25.2, 506.0], [25.3, 506.0], [25.4, 506.0], [25.5, 506.0], [25.6, 506.0], [25.7, 506.0], [25.8, 506.0], [25.9, 506.0], [26.0, 506.0], [26.1, 506.0], [26.2, 506.0], [26.3, 506.0], [26.4, 506.0], [26.5, 506.0], [26.6, 506.0], [26.7, 506.0], [26.8, 507.0], [26.9, 507.0], [27.0, 507.0], [27.1, 507.0], [27.2, 507.0], [27.3, 507.0], [27.4, 507.0], [27.5, 507.0], [27.6, 507.0], [27.7, 507.0], [27.8, 507.0], [27.9, 507.0], [28.0, 507.0], [28.1, 507.0], [28.2, 507.0], [28.3, 507.0], [28.4, 507.0], [28.5, 507.0], [28.6, 507.0], [28.7, 507.0], [28.8, 507.0], [28.9, 508.0], [29.0, 508.0], [29.1, 508.0], [29.2, 508.0], [29.3, 508.0], [29.4, 508.0], [29.5, 508.0], [29.6, 508.0], [29.7, 508.0], [29.8, 508.0], [29.9, 508.0], [30.0, 508.0], [30.1, 509.0], [30.2, 509.0], [30.3, 509.0], [30.4, 509.0], [30.5, 509.0], [30.6, 509.0], [30.7, 509.0], [30.8, 509.0], [30.9, 509.0], [31.0, 509.0], [31.1, 509.0], [31.2, 509.0], [31.3, 509.0], [31.4, 509.0], [31.5, 510.0], [31.6, 510.0], [31.7, 510.0], [31.8, 510.0], [31.9, 510.0], [32.0, 510.0], [32.1, 510.0], [32.2, 510.0], [32.3, 510.0], [32.4, 510.0], [32.5, 510.0], [32.6, 510.0], [32.7, 510.0], [32.8, 510.0], [32.9, 510.0], [33.0, 510.0], [33.1, 510.0], [33.2, 510.0], [33.3, 510.0], [33.4, 510.0], [33.5, 510.0], [33.6, 510.0], [33.7, 510.0], [33.8, 510.0], [33.9, 510.0], [34.0, 510.0], [34.1, 510.0], [34.2, 511.0], [34.3, 511.0], [34.4, 511.0], [34.5, 511.0], [34.6, 511.0], [34.7, 511.0], [34.8, 511.0], [34.9, 511.0], [35.0, 511.0], [35.1, 511.0], [35.2, 511.0], [35.3, 511.0], [35.4, 511.0], [35.5, 511.0], [35.6, 511.0], [35.7, 511.0], [35.8, 511.0], [35.9, 511.0], [36.0, 511.0], [36.1, 511.0], [36.2, 511.0], [36.3, 511.0], [36.4, 511.0], [36.5, 511.0], [36.6, 511.0], [36.7, 511.0], [36.8, 511.0], [36.9, 511.0], [37.0, 512.0], [37.1, 512.0], [37.2, 512.0], [37.3, 512.0], [37.4, 512.0], [37.5, 512.0], [37.6, 512.0], [37.7, 512.0], [37.8, 512.0], [37.9, 512.0], [38.0, 512.0], [38.1, 512.0], [38.2, 512.0], [38.3, 512.0], [38.4, 512.0], [38.5, 512.0], [38.6, 512.0], [38.7, 512.0], [38.8, 512.0], [38.9, 512.0], [39.0, 512.0], [39.1, 512.0], [39.2, 512.0], [39.3, 512.0], [39.4, 512.0], [39.5, 513.0], [39.6, 513.0], [39.7, 513.0], [39.8, 513.0], [39.9, 513.0], [40.0, 513.0], [40.1, 513.0], [40.2, 513.0], [40.3, 513.0], [40.4, 513.0], [40.5, 513.0], [40.6, 513.0], [40.7, 513.0], [40.8, 513.0], [40.9, 513.0], [41.0, 513.0], [41.1, 514.0], [41.2, 514.0], [41.3, 514.0], [41.4, 514.0], [41.5, 514.0], [41.6, 514.0], [41.7, 514.0], [41.8, 514.0], [41.9, 515.0], [42.0, 515.0], [42.1, 515.0], [42.2, 515.0], [42.3, 515.0], [42.4, 515.0], [42.5, 515.0], [42.6, 515.0], [42.7, 515.0], [42.8, 515.0], [42.9, 515.0], [43.0, 515.0], [43.1, 515.0], [43.2, 515.0], [43.3, 515.0], [43.4, 515.0], [43.5, 515.0], [43.6, 515.0], [43.7, 516.0], [43.8, 516.0], [43.9, 516.0], [44.0, 516.0], [44.1, 516.0], [44.2, 516.0], [44.3, 516.0], [44.4, 516.0], [44.5, 516.0], [44.6, 516.0], [44.7, 516.0], [44.8, 516.0], [44.9, 516.0], [45.0, 516.0], [45.1, 516.0], [45.2, 516.0], [45.3, 516.0], [45.4, 516.0], [45.5, 516.0], [45.6, 516.0], [45.7, 516.0], [45.8, 516.0], [45.9, 516.0], [46.0, 516.0], [46.1, 516.0], [46.2, 516.0], [46.3, 516.0], [46.4, 516.0], [46.5, 516.0], [46.6, 516.0], [46.7, 516.0], [46.8, 516.0], [46.9, 516.0], [47.0, 517.0], [47.1, 517.0], [47.2, 517.0], [47.3, 517.0], [47.4, 517.0], [47.5, 517.0], [47.6, 517.0], [47.7, 517.0], [47.8, 517.0], [47.9, 517.0], [48.0, 517.0], [48.1, 517.0], [48.2, 517.0], [48.3, 517.0], [48.4, 517.0], [48.5, 517.0], [48.6, 517.0], [48.7, 517.0], [48.8, 517.0], [48.9, 517.0], [49.0, 517.0], [49.1, 517.0], [49.2, 518.0], [49.3, 518.0], [49.4, 518.0], [49.5, 518.0], [49.6, 518.0], [49.7, 518.0], [49.8, 519.0], [49.9, 519.0], [50.0, 519.0], [50.1, 519.0], [50.2, 519.0], [50.3, 519.0], [50.4, 519.0], [50.5, 519.0], [50.6, 519.0], [50.7, 519.0], [50.8, 519.0], [50.9, 520.0], [51.0, 520.0], [51.1, 520.0], [51.2, 520.0], [51.3, 520.0], [51.4, 520.0], [51.5, 520.0], [51.6, 520.0], [51.7, 520.0], [51.8, 520.0], [51.9, 520.0], [52.0, 520.0], [52.1, 520.0], [52.2, 520.0], [52.3, 521.0], [52.4, 521.0], [52.5, 521.0], [52.6, 521.0], [52.7, 521.0], [52.8, 521.0], [52.9, 521.0], [53.0, 521.0], [53.1, 521.0], [53.2, 521.0], [53.3, 521.0], [53.4, 521.0], [53.5, 521.0], [53.6, 521.0], [53.7, 521.0], [53.8, 521.0], [53.9, 521.0], [54.0, 521.0], [54.1, 522.0], [54.2, 522.0], [54.3, 522.0], [54.4, 522.0], [54.5, 522.0], [54.6, 522.0], [54.7, 522.0], [54.8, 522.0], [54.9, 523.0], [55.0, 523.0], [55.1, 523.0], [55.2, 523.0], [55.3, 523.0], [55.4, 523.0], [55.5, 523.0], [55.6, 523.0], [55.7, 523.0], [55.8, 523.0], [55.9, 523.0], [56.0, 523.0], [56.1, 523.0], [56.2, 523.0], [56.3, 523.0], [56.4, 523.0], [56.5, 523.0], [56.6, 523.0], [56.7, 523.0], [56.8, 523.0], [56.9, 523.0], [57.0, 523.0], [57.1, 523.0], [57.2, 523.0], [57.3, 523.0], [57.4, 523.0], [57.5, 523.0], [57.6, 524.0], [57.7, 524.0], [57.8, 524.0], [57.9, 524.0], [58.0, 524.0], [58.1, 524.0], [58.2, 524.0], [58.3, 524.0], [58.4, 524.0], [58.5, 524.0], [58.6, 524.0], [58.7, 524.0], [58.8, 525.0], [58.9, 525.0], [59.0, 525.0], [59.1, 525.0], [59.2, 525.0], [59.3, 525.0], [59.4, 526.0], [59.5, 526.0], [59.6, 526.0], [59.7, 526.0], [59.8, 526.0], [59.9, 526.0], [60.0, 526.0], [60.1, 526.0], [60.2, 526.0], [60.3, 526.0], [60.4, 526.0], [60.5, 526.0], [60.6, 527.0], [60.7, 527.0], [60.8, 527.0], [60.9, 527.0], [61.0, 528.0], [61.1, 528.0], [61.2, 528.0], [61.3, 528.0], [61.4, 528.0], [61.5, 529.0], [61.6, 529.0], [61.7, 529.0], [61.8, 529.0], [61.9, 530.0], [62.0, 530.0], [62.1, 530.0], [62.2, 530.0], [62.3, 530.0], [62.4, 530.0], [62.5, 531.0], [62.6, 531.0], [62.7, 531.0], [62.8, 531.0], [62.9, 531.0], [63.0, 531.0], [63.1, 531.0], [63.2, 531.0], [63.3, 531.0], [63.4, 531.0], [63.5, 532.0], [63.6, 532.0], [63.7, 532.0], [63.8, 532.0], [63.9, 532.0], [64.0, 532.0], [64.1, 533.0], [64.2, 533.0], [64.3, 533.0], [64.4, 533.0], [64.5, 533.0], [64.6, 533.0], [64.7, 534.0], [64.8, 534.0], [64.9, 534.0], [65.0, 534.0], [65.1, 535.0], [65.2, 535.0], [65.3, 535.0], [65.4, 535.0], [65.5, 536.0], [65.6, 536.0], [65.7, 536.0], [65.8, 536.0], [65.9, 536.0], [66.0, 536.0], [66.1, 536.0], [66.2, 536.0], [66.3, 537.0], [66.4, 537.0], [66.5, 537.0], [66.6, 537.0], [66.7, 537.0], [66.8, 537.0], [66.9, 537.0], [67.0, 537.0], [67.1, 537.0], [67.2, 538.0], [67.3, 538.0], [67.4, 539.0], [67.5, 539.0], [67.6, 539.0], [67.7, 539.0], [67.8, 539.0], [67.9, 539.0], [68.0, 539.0], [68.1, 539.0], [68.2, 539.0], [68.3, 539.0], [68.4, 541.0], [68.5, 541.0], [68.6, 541.0], [68.7, 541.0], [68.8, 541.0], [68.9, 541.0], [69.0, 541.0], [69.1, 541.0], [69.2, 542.0], [69.3, 542.0], [69.4, 542.0], [69.5, 542.0], [69.6, 542.0], [69.7, 542.0], [69.8, 542.0], [69.9, 542.0], [70.0, 544.0], [70.1, 544.0], [70.2, 544.0], [70.3, 544.0], [70.4, 544.0], [70.5, 544.0], [70.6, 544.0], [70.7, 544.0], [70.8, 545.0], [70.9, 545.0], [71.0, 546.0], [71.1, 546.0], [71.2, 546.0], [71.3, 546.0], [71.4, 546.0], [71.5, 546.0], [71.6, 546.0], [71.7, 546.0], [71.8, 548.0], [71.9, 548.0], [72.0, 548.0], [72.1, 548.0], [72.2, 548.0], [72.3, 548.0], [72.4, 548.0], [72.5, 548.0], [72.6, 548.0], [72.7, 549.0], [72.8, 549.0], [72.9, 549.0], [73.0, 549.0], [73.1, 550.0], [73.2, 550.0], [73.3, 550.0], [73.4, 550.0], [73.5, 550.0], [73.6, 550.0], [73.7, 551.0], [73.8, 551.0], [73.9, 551.0], [74.0, 551.0], [74.1, 552.0], [74.2, 552.0], [74.3, 553.0], [74.4, 553.0], [74.5, 553.0], [74.6, 553.0], [74.7, 554.0], [74.8, 554.0], [74.9, 555.0], [75.0, 555.0], [75.1, 555.0], [75.2, 555.0], [75.3, 555.0], [75.4, 555.0], [75.5, 556.0], [75.6, 556.0], [75.7, 557.0], [75.8, 557.0], [75.9, 557.0], [76.0, 557.0], [76.1, 557.0], [76.2, 557.0], [76.3, 557.0], [76.4, 557.0], [76.5, 557.0], [76.6, 557.0], [76.7, 557.0], [76.8, 557.0], [76.9, 559.0], [77.0, 559.0], [77.1, 560.0], [77.2, 560.0], [77.3, 560.0], [77.4, 560.0], [77.5, 560.0], [77.6, 560.0], [77.7, 560.0], [77.8, 561.0], [77.9, 561.0], [78.0, 561.0], [78.1, 561.0], [78.2, 562.0], [78.3, 562.0], [78.4, 562.0], [78.5, 562.0], [78.6, 563.0], [78.7, 563.0], [78.8, 563.0], [78.9, 563.0], [79.0, 564.0], [79.1, 564.0], [79.2, 565.0], [79.3, 565.0], [79.4, 565.0], [79.5, 565.0], [79.6, 565.0], [79.7, 565.0], [79.8, 565.0], [79.9, 565.0], [80.0, 565.0], [80.1, 565.0], [80.2, 566.0], [80.3, 566.0], [80.4, 566.0], [80.5, 566.0], [80.6, 567.0], [80.7, 567.0], [80.8, 568.0], [80.9, 568.0], [81.0, 568.0], [81.1, 568.0], [81.2, 568.0], [81.3, 568.0], [81.4, 569.0], [81.5, 569.0], [81.6, 569.0], [81.7, 569.0], [81.8, 569.0], [81.9, 569.0], [82.0, 570.0], [82.1, 570.0], [82.2, 570.0], [82.3, 570.0], [82.4, 571.0], [82.5, 571.0], [82.6, 572.0], [82.7, 572.0], [82.8, 573.0], [82.9, 573.0], [83.0, 576.0], [83.1, 576.0], [83.2, 578.0], [83.3, 578.0], [83.4, 579.0], [83.5, 579.0], [83.6, 579.0], [83.7, 580.0], [83.8, 580.0], [83.9, 582.0], [84.0, 582.0], [84.1, 589.0], [84.2, 589.0], [84.3, 589.0], [84.4, 589.0], [84.5, 590.0], [84.6, 590.0], [84.7, 591.0], [84.8, 591.0], [84.9, 595.0], [85.0, 595.0], [85.1, 599.0], [85.2, 599.0], [85.3, 1505.0], [85.4, 1505.0], [85.5, 1528.0], [85.6, 1528.0], [85.7, 1532.0], [85.8, 1532.0], [85.9, 1538.0], [86.0, 1538.0], [86.1, 1538.0], [86.2, 1538.0], [86.3, 1538.0], [86.4, 1538.0], [86.5, 1540.0], [86.6, 1540.0], [86.7, 1541.0], [86.8, 1541.0], [86.9, 1541.0], [87.0, 1541.0], [87.1, 1542.0], [87.2, 1542.0], [87.3, 1543.0], [87.4, 1543.0], [87.5, 1543.0], [87.6, 1543.0], [87.7, 1545.0], [87.8, 1545.0], [87.9, 1546.0], [88.0, 1546.0], [88.1, 1546.0], [88.2, 1546.0], [88.3, 1546.0], [88.4, 1546.0], [88.5, 1547.0], [88.6, 1547.0], [88.7, 1547.0], [88.8, 1547.0], [88.9, 1548.0], [89.0, 1548.0], [89.1, 1548.0], [89.2, 1549.0], [89.3, 1549.0], [89.4, 1550.0], [89.5, 1550.0], [89.6, 1551.0], [89.7, 1551.0], [89.8, 1552.0], [89.9, 1552.0], [90.0, 1553.0], [90.1, 1553.0], [90.2, 1554.0], [90.3, 1554.0], [90.4, 1554.0], [90.5, 1554.0], [90.6, 1554.0], [90.7, 1554.0], [90.8, 1554.0], [90.9, 1554.0], [91.0, 1555.0], [91.1, 1555.0], [91.2, 1555.0], [91.3, 1555.0], [91.4, 1556.0], [91.5, 1556.0], [91.6, 1556.0], [91.7, 1556.0], [91.8, 1558.0], [91.9, 1558.0], [92.0, 1560.0], [92.1, 1560.0], [92.2, 1560.0], [92.3, 1560.0], [92.4, 1560.0], [92.5, 1560.0], [92.6, 1560.0], [92.7, 1560.0], [92.8, 1561.0], [92.9, 1561.0], [93.0, 1561.0], [93.1, 1561.0], [93.2, 1562.0], [93.3, 1562.0], [93.4, 1563.0], [93.5, 1563.0], [93.6, 1563.0], [93.7, 1563.0], [93.8, 1564.0], [93.9, 1564.0], [94.0, 1565.0], [94.1, 1565.0], [94.2, 1565.0], [94.3, 1565.0], [94.4, 1565.0], [94.5, 1565.0], [94.6, 1565.0], [94.7, 1565.0], [94.8, 1565.0], [94.9, 1568.0], [95.0, 1568.0], [95.1, 1569.0], [95.2, 1569.0], [95.3, 1576.0], [95.4, 1576.0], [95.5, 1605.0], [95.6, 1605.0], [95.7, 1612.0], [95.8, 1612.0], [95.9, 1798.0], [96.0, 1798.0], [96.1, 1822.0], [96.2, 1822.0], [96.3, 1823.0], [96.4, 1823.0], [96.5, 1823.0], [96.6, 1823.0], [96.7, 1825.0], [96.8, 1825.0], [96.9, 1826.0], [97.0, 1826.0], [97.1, 1827.0], [97.2, 1827.0], [97.3, 1827.0], [97.4, 1827.0], [97.5, 1829.0], [97.6, 1829.0], [97.7, 1833.0], [97.8, 1833.0], [97.9, 3671.0], [98.0, 3671.0], [98.1, 3672.0], [98.2, 3672.0], [98.3, 3673.0], [98.4, 3673.0], [98.5, 3673.0], [98.6, 3673.0], [98.7, 3675.0], [98.8, 3675.0], [98.9, 3675.0], [99.0, 3675.0], [99.1, 3676.0], [99.2, 3676.0], [99.3, 3676.0], [99.4, 3676.0], [99.5, 3676.0], [99.6, 3676.0], [99.7, 3685.0], [99.8, 3685.0], [99.9, 3687.0]], "isOverall": false, "label": "HTTP Request", "isController": false}], "supportsControllersDiscrimination": true, "maxX": 100.0, "title": "Response Time Percentiles"}},
        getOptions: function() {
            return {
                series: {
                    points: { show: false }
                },
                legend: {
                    noColumns: 2,
                    show: true,
                    container: '#legendResponseTimePercentiles'
                },
                xaxis: {
                    tickDecimals: 1,
                    axisLabel: "Percentiles",
                    axisLabelUseCanvas: true,
                    axisLabelFontSizePixels: 12,
                    axisLabelFontFamily: 'Verdana, Arial',
                    axisLabelPadding: 20,
                },
                yaxis: {
                    axisLabel: "Percentile value in ms",
                    axisLabelUseCanvas: true,
                    axisLabelFontSizePixels: 12,
                    axisLabelFontFamily: 'Verdana, Arial',
                    axisLabelPadding: 20
                },
                grid: {
                    hoverable: true // IMPORTANT! this is needed for tooltip to
                                    // work
                },
                tooltip: true,
                tooltipOpts: {
                    content: "%s : %x.2 percentile was %y ms"
                },
                selection: { mode: "xy" },
            };
        },
        createGraph: function() {
            var data = this.data;
            var dataset = prepareData(data.result.series, $("#choicesResponseTimePercentiles"));
            var options = this.getOptions();
            prepareOptions(options, data);
            $.plot($("#flotResponseTimesPercentiles"), dataset, options);
            // setup overview
            $.plot($("#overviewResponseTimesPercentiles"), dataset, prepareOverviewOptions(options));
        }
};

/**
 * @param elementId Id of element where we display message
 */
function setEmptyGraph(elementId) {
    $(function() {
        $(elementId).text("No graph series with filter="+seriesFilter);
    });
}

// Response times percentiles
function refreshResponseTimePercentiles() {
    var infos = responseTimePercentilesInfos;
    prepareSeries(infos.data);
    if(infos.data.result.series.length == 0) {
        setEmptyGraph("#bodyResponseTimePercentiles");
        return;
    }
    if (isGraph($("#flotResponseTimesPercentiles"))){
        infos.createGraph();
    } else {
        var choiceContainer = $("#choicesResponseTimePercentiles");
        createLegend(choiceContainer, infos);
        infos.createGraph();
        setGraphZoomable("#flotResponseTimesPercentiles", "#overviewResponseTimesPercentiles");
        $('#bodyResponseTimePercentiles .legendColorBox > div').each(function(i){
            $(this).clone().prependTo(choiceContainer.find("li").eq(i));
        });
    }
}

var responseTimeDistributionInfos = {
        data: {"result": {"minY": 1.0, "minX": 400.0, "maxY": 357.0, "series": [{"data": [[1500.0, 52.0], [400.0, 77.0], [1600.0, 2.0], [1700.0, 1.0], [1800.0, 9.0], [3600.0, 11.0], [500.0, 357.0]], "isOverall": false, "label": "HTTP Request", "isController": false}], "supportsControllersDiscrimination": true, "granularity": 100, "maxX": 3600.0, "title": "Response Time Distribution"}},
        getOptions: function() {
            var granularity = this.data.result.granularity;
            return {
                legend: {
                    noColumns: 2,
                    show: true,
                    container: '#legendResponseTimeDistribution'
                },
                xaxis:{
                    axisLabel: "Response times in ms",
                    axisLabelUseCanvas: true,
                    axisLabelFontSizePixels: 12,
                    axisLabelFontFamily: 'Verdana, Arial',
                    axisLabelPadding: 20,
                },
                yaxis: {
                    axisLabel: "Number of responses",
                    axisLabelUseCanvas: true,
                    axisLabelFontSizePixels: 12,
                    axisLabelFontFamily: 'Verdana, Arial',
                    axisLabelPadding: 20,
                },
                bars : {
                    show: true,
                    barWidth: this.data.result.granularity
                },
                grid: {
                    hoverable: true // IMPORTANT! this is needed for tooltip to
                                    // work
                },
                tooltip: true,
                tooltipOpts: {
                    content: function(label, xval, yval, flotItem){
                        return yval + " responses for " + label + " were between " + xval + " and " + (xval + granularity) + " ms";
                    }
                }
            };
        },
        createGraph: function() {
            var data = this.data;
            var options = this.getOptions();
            prepareOptions(options, data);
            $.plot($("#flotResponseTimeDistribution"), prepareData(data.result.series, $("#choicesResponseTimeDistribution")), options);
        }

};

// Response time distribution
function refreshResponseTimeDistribution() {
    var infos = responseTimeDistributionInfos;
    prepareSeries(infos.data);
    if(infos.data.result.series.length == 0) {
        setEmptyGraph("#bodyResponseTimeDistribution");
        return;
    }
    if (isGraph($("#flotResponseTimeDistribution"))){
        infos.createGraph();
    }else{
        var choiceContainer = $("#choicesResponseTimeDistribution");
        createLegend(choiceContainer, infos);
        infos.createGraph();
        $('#footerResponseTimeDistribution .legendColorBox > div').each(function(i){
            $(this).clone().prependTo(choiceContainer.find("li").eq(i));
        });
    }
};


var syntheticResponseTimeDistributionInfos = {
        data: {"result": {"minY": 75.0, "minX": 0.0, "ticks": [[0, "Requests having \nresponse time <= 500ms"], [1, "Requests having \nresponse time > 500ms and <= 1,500ms"], [2, "Requests having \nresponse time > 1,500ms"], [3, "Requests in error"]], "maxY": 349.0, "series": [{"data": [[0.0, 85.0]], "color": "#9ACD32", "isOverall": false, "label": "Requests having \nresponse time <= 500ms", "isController": false}, {"data": [[1.0, 349.0]], "color": "yellow", "isOverall": false, "label": "Requests having \nresponse time > 500ms and <= 1,500ms", "isController": false}, {"data": [[2.0, 75.0]], "color": "orange", "isOverall": false, "label": "Requests having \nresponse time > 1,500ms", "isController": false}, {"data": [], "color": "#FF6347", "isOverall": false, "label": "Requests in error", "isController": false}], "supportsControllersDiscrimination": false, "maxX": 2.0, "title": "Synthetic Response Times Distribution"}},
        getOptions: function() {
            return {
                legend: {
                    noColumns: 2,
                    show: true,
                    container: '#legendSyntheticResponseTimeDistribution'
                },
                xaxis:{
                    axisLabel: "Response times ranges",
                    axisLabelUseCanvas: true,
                    axisLabelFontSizePixels: 12,
                    axisLabelFontFamily: 'Verdana, Arial',
                    axisLabelPadding: 20,
                    tickLength:0,
                    min:-0.5,
                    max:3.5
                },
                yaxis: {
                    axisLabel: "Number of responses",
                    axisLabelUseCanvas: true,
                    axisLabelFontSizePixels: 12,
                    axisLabelFontFamily: 'Verdana, Arial',
                    axisLabelPadding: 20,
                },
                bars : {
                    show: true,
                    align: "center",
                    barWidth: 0.25,
                    fill:.75
                },
                grid: {
                    hoverable: true // IMPORTANT! this is needed for tooltip to
                                    // work
                },
                tooltip: true,
                tooltipOpts: {
                    content: function(label, xval, yval, flotItem){
                        return yval + " " + label;
                    }
                }
            };
        },
        createGraph: function() {
            var data = this.data;
            var options = this.getOptions();
            prepareOptions(options, data);
            options.xaxis.ticks = data.result.ticks;
            $.plot($("#flotSyntheticResponseTimeDistribution"), prepareData(data.result.series, $("#choicesSyntheticResponseTimeDistribution")), options);
        }

};

// Response time distribution
function refreshSyntheticResponseTimeDistribution() {
    var infos = syntheticResponseTimeDistributionInfos;
    prepareSeries(infos.data, true);
    if (isGraph($("#flotSyntheticResponseTimeDistribution"))){
        infos.createGraph();
    }else{
        var choiceContainer = $("#choicesSyntheticResponseTimeDistribution");
        createLegend(choiceContainer, infos);
        infos.createGraph();
        $('#footerSyntheticResponseTimeDistribution .legendColorBox > div').each(function(i){
            $(this).clone().prependTo(choiceContainer.find("li").eq(i));
        });
    }
};

var activeThreadsOverTimeInfos = {
        data: {"result": {"minY": 255.9017681728878, "minX": 1.6557336E12, "maxY": 255.9017681728878, "series": [{"data": [[1.6557336E12, 255.9017681728878]], "isOverall": false, "label": "Thread Group", "isController": false}], "supportsControllersDiscrimination": false, "granularity": 60000, "maxX": 1.6557336E12, "title": "Active Threads Over Time"}},
        getOptions: function() {
            return {
                series: {
                    stack: true,
                    lines: {
                        show: true,
                        fill: true
                    },
                    points: {
                        show: true
                    }
                },
                xaxis: {
                    mode: "time",
                    timeformat: getTimeFormat(this.data.result.granularity),
                    axisLabel: getElapsedTimeLabel(this.data.result.granularity),
                    axisLabelUseCanvas: true,
                    axisLabelFontSizePixels: 12,
                    axisLabelFontFamily: 'Verdana, Arial',
                    axisLabelPadding: 20,
                },
                yaxis: {
                    axisLabel: "Number of active threads",
                    axisLabelUseCanvas: true,
                    axisLabelFontSizePixels: 12,
                    axisLabelFontFamily: 'Verdana, Arial',
                    axisLabelPadding: 20
                },
                legend: {
                    noColumns: 6,
                    show: true,
                    container: '#legendActiveThreadsOverTime'
                },
                grid: {
                    hoverable: true // IMPORTANT! this is needed for tooltip to
                                    // work
                },
                selection: {
                    mode: 'xy'
                },
                tooltip: true,
                tooltipOpts: {
                    content: "%s : At %x there were %y active threads"
                }
            };
        },
        createGraph: function() {
            var data = this.data;
            var dataset = prepareData(data.result.series, $("#choicesActiveThreadsOverTime"));
            var options = this.getOptions();
            prepareOptions(options, data);
            $.plot($("#flotActiveThreadsOverTime"), dataset, options);
            // setup overview
            $.plot($("#overviewActiveThreadsOverTime"), dataset, prepareOverviewOptions(options));
        }
};

// Active Threads Over Time
function refreshActiveThreadsOverTime(fixTimestamps) {
    var infos = activeThreadsOverTimeInfos;
    prepareSeries(infos.data);
    if(fixTimestamps) {
        fixTimeStamps(infos.data.result.series, -10800000);
    }
    if(isGraph($("#flotActiveThreadsOverTime"))) {
        infos.createGraph();
    }else{
        var choiceContainer = $("#choicesActiveThreadsOverTime");
        createLegend(choiceContainer, infos);
        infos.createGraph();
        setGraphZoomable("#flotActiveThreadsOverTime", "#overviewActiveThreadsOverTime");
        $('#footerActiveThreadsOverTime .legendColorBox > div').each(function(i){
            $(this).clone().prependTo(choiceContainer.find("li").eq(i));
        });
    }
};

var timeVsThreadsInfos = {
        data: {"result": {"minY": 474.0, "minX": 1.0, "maxY": 3687.0, "series": [{"data": [[2.0, 3685.0], [3.0, 3676.0], [4.0, 3676.0], [5.0, 3676.0], [6.0, 3675.0], [11.0, 3672.8], [12.0, 1798.0], [13.0, 1829.0], [14.0, 1827.0], [16.0, 1823.0], [19.0, 1833.0], [21.0, 1824.6], [24.0, 1597.6666666666667], [25.0, 1569.0], [27.0, 1564.5], [30.0, 1566.0], [36.0, 1561.0], [39.0, 1562.5], [38.0, 1563.0], [40.0, 1561.0], [42.0, 1553.0], [45.0, 1560.5], [44.0, 1553.0], [49.0, 1551.0], [48.0, 1555.0], [51.0, 1549.0], [50.0, 1547.0], [52.0, 1555.0], [54.0, 1547.0], [58.0, 1546.0], [61.0, 1548.0], [60.0, 1552.6], [63.0, 1542.0], [66.0, 1540.3333333333333], [75.0, 1528.0], [74.0, 1538.0], [72.0, 1535.1666666666667], [79.0, 568.0], [78.0, 561.0], [77.0, 563.0], [76.0, 562.0], [82.0, 565.0], [80.0, 563.0], [87.0, 558.5], [86.0, 566.0], [84.0, 566.0], [91.0, 565.0], [90.0, 567.75], [94.0, 523.0], [92.0, 518.0], [98.0, 518.0], [97.0, 526.0], [96.0, 523.3333333333334], [103.0, 522.0], [101.0, 521.3333333333334], [106.0, 513.0], [104.0, 519.0], [111.0, 513.5], [109.0, 517.0], [108.0, 511.5], [114.0, 513.0], [113.0, 502.0], [117.0, 510.3333333333333], [123.0, 513.0], [121.0, 507.0], [120.0, 513.0], [125.0, 512.5], [134.0, 496.0], [133.0, 510.0], [131.0, 500.0], [130.0, 497.0], [129.0, 492.0], [128.0, 510.3333333333333], [143.0, 502.0], [142.0, 497.5], [140.0, 509.5], [138.0, 498.5], [136.0, 515.0], [151.0, 498.0], [149.0, 500.0], [148.0, 500.6], [159.0, 494.0], [158.0, 494.0], [157.0, 502.3333333333333], [156.0, 492.0], [154.0, 492.0], [152.0, 496.5], [167.0, 503.8], [162.0, 512.0], [161.0, 495.0], [160.0, 494.0], [174.0, 511.0], [172.0, 491.0], [170.0, 490.5], [168.0, 510.0], [183.0, 485.0], [182.0, 512.0], [181.0, 504.0], [180.0, 488.6666666666667], [177.0, 505.0], [189.0, 494.0], [188.0, 483.0], [187.0, 515.0], [186.0, 500.6666666666667], [199.0, 494.0], [197.0, 480.0], [196.0, 493.6666666666667], [193.0, 492.75], [206.0, 498.75], [202.0, 511.0], [200.0, 516.0], [215.0, 490.0], [214.0, 486.0], [213.0, 495.0], [211.0, 505.5], [210.0, 492.6666666666667], [223.0, 494.0], [222.0, 474.0], [221.0, 497.6], [218.0, 490.0], [231.0, 510.0], [229.0, 503.0], [227.0, 494.5], [225.0, 504.0], [237.0, 494.0], [235.0, 511.5], [233.0, 507.0], [232.0, 516.0], [246.0, 487.0], [245.0, 526.0], [244.0, 511.5], [243.0, 507.0], [242.0, 507.0], [241.0, 505.0], [240.0, 504.5], [253.0, 510.5], [252.0, 506.0], [249.0, 507.3333333333333], [271.0, 496.33333333333337], [265.0, 506.0], [263.0, 505.3333333333333], [260.0, 515.5], [258.0, 505.0], [257.0, 503.0], [256.0, 505.3333333333333], [286.0, 517.0], [285.0, 510.0], [283.0, 502.0], [282.0, 498.0], [281.0, 540.5], [279.0, 514.6666666666666], [277.0, 531.0], [275.0, 512.6666666666666], [272.0, 501.0], [302.0, 528.0], [303.0, 521.0], [301.0, 518.0], [300.0, 539.0], [299.0, 526.0], [297.0, 503.0], [296.0, 512.5], [294.0, 539.0], [289.0, 513.6666666666666], [292.0, 505.3333333333333], [317.0, 517.0], [315.0, 517.0], [314.0, 516.0], [313.0, 516.0], [311.0, 506.0], [309.0, 522.0], [307.0, 539.0], [306.0, 529.0], [305.0, 520.5], [334.0, 509.0], [333.0, 509.0], [321.0, 525.0], [320.0, 521.5], [331.0, 492.0], [330.0, 508.0], [329.0, 510.0], [328.0, 510.0], [327.0, 512.0], [326.0, 508.0], [325.0, 530.0], [324.0, 512.6666666666666], [351.0, 515.5], [347.0, 512.625], [339.0, 523.0], [338.0, 517.6], [365.0, 510.0], [366.0, 515.0], [364.0, 511.3333333333333], [362.0, 503.0], [358.0, 504.0], [357.0, 519.0], [355.0, 507.5], [356.0, 528.0], [353.0, 504.0], [381.0, 516.0], [382.0, 520.0], [380.0, 514.5], [379.0, 513.0], [378.0, 512.0], [377.0, 503.0], [376.0, 488.0], [373.0, 497.0], [372.0, 511.0], [399.0, 496.0], [397.0, 547.5], [394.0, 510.0], [392.0, 538.0], [390.0, 595.0], [389.0, 527.0], [388.0, 555.0], [387.0, 569.5], [385.0, 548.3333333333334], [414.0, 539.0], [415.0, 541.0], [412.0, 527.0], [402.0, 529.0], [400.0, 522.0], [410.0, 507.0], [409.0, 519.0], [408.0, 572.0], [407.0, 579.0], [406.0, 580.0], [405.0, 545.0], [404.0, 545.0], [429.0, 568.0], [428.0, 520.0], [427.0, 564.0], [424.0, 591.0], [423.0, 572.5], [422.0, 569.3333333333334], [418.0, 559.5], [417.0, 555.5], [446.0, 546.0], [447.0, 511.0], [445.0, 555.0], [444.0, 558.5], [443.0, 544.0], [442.0, 542.6666666666666], [441.0, 565.0], [437.0, 582.0], [435.0, 534.0], [434.0, 505.0], [432.0, 550.6666666666666], [436.0, 537.0], [460.0, 530.0], [462.0, 560.0], [463.0, 553.0], [461.0, 560.0], [459.0, 544.5], [457.0, 554.5], [453.0, 539.0], [452.0, 557.0], [451.0, 564.0], [450.0, 579.0], [449.0, 531.0], [448.0, 530.0], [467.0, 536.0], [479.0, 541.0], [476.0, 546.0], [475.0, 548.0], [474.0, 548.0], [472.0, 549.5], [471.0, 551.5], [469.0, 536.0], [466.0, 535.0], [465.0, 557.0], [495.0, 537.75], [491.0, 537.0], [489.0, 539.0], [485.0, 511.0], [484.0, 536.5], [482.0, 544.0], [481.0, 548.0], [480.0, 531.0], [509.0, 526.0], [508.0, 542.0], [507.0, 515.3333333333334], [504.0, 521.0], [503.0, 515.0], [502.0, 521.5], [500.0, 526.0], [498.0, 516.0], [497.0, 516.0], [496.0, 524.0], [1.0, 3687.0]], "isOverall": false, "label": "HTTP Request", "isController": false}, {"data": [[255.9017681728878, 723.7092337917486]], "isOverall": false, "label": "HTTP Request-Aggregated", "isController": false}], "supportsControllersDiscrimination": true, "maxX": 509.0, "title": "Time VS Threads"}},
        getOptions: function() {
            return {
                series: {
                    lines: {
                        show: true
                    },
                    points: {
                        show: true
                    }
                },
                xaxis: {
                    axisLabel: "Number of active threads",
                    axisLabelUseCanvas: true,
                    axisLabelFontSizePixels: 12,
                    axisLabelFontFamily: 'Verdana, Arial',
                    axisLabelPadding: 20,
                },
                yaxis: {
                    axisLabel: "Average response times in ms",
                    axisLabelUseCanvas: true,
                    axisLabelFontSizePixels: 12,
                    axisLabelFontFamily: 'Verdana, Arial',
                    axisLabelPadding: 20
                },
                legend: { noColumns: 2,show: true, container: '#legendTimeVsThreads' },
                selection: {
                    mode: 'xy'
                },
                grid: {
                    hoverable: true // IMPORTANT! this is needed for tooltip to work
                },
                tooltip: true,
                tooltipOpts: {
                    content: "%s: At %x.2 active threads, Average response time was %y.2 ms"
                }
            };
        },
        createGraph: function() {
            var data = this.data;
            var dataset = prepareData(data.result.series, $("#choicesTimeVsThreads"));
            var options = this.getOptions();
            prepareOptions(options, data);
            $.plot($("#flotTimesVsThreads"), dataset, options);
            // setup overview
            $.plot($("#overviewTimesVsThreads"), dataset, prepareOverviewOptions(options));
        }
};

// Time vs threads
function refreshTimeVsThreads(){
    var infos = timeVsThreadsInfos;
    prepareSeries(infos.data);
    if(infos.data.result.series.length == 0) {
        setEmptyGraph("#bodyTimeVsThreads");
        return;
    }
    if(isGraph($("#flotTimesVsThreads"))){
        infos.createGraph();
    }else{
        var choiceContainer = $("#choicesTimeVsThreads");
        createLegend(choiceContainer, infos);
        infos.createGraph();
        setGraphZoomable("#flotTimesVsThreads", "#overviewTimesVsThreads");
        $('#footerTimeVsThreads .legendColorBox > div').each(function(i){
            $(this).clone().prependTo(choiceContainer.find("li").eq(i));
        });
    }
};

var bytesThroughputOverTimeInfos = {
        data : {"result": {"minY": 1001.0333333333333, "minX": 1.6557336E12, "maxY": 2926.75, "series": [{"data": [[1.6557336E12, 2926.75]], "isOverall": false, "label": "Bytes received per second", "isController": false}, {"data": [[1.6557336E12, 1001.0333333333333]], "isOverall": false, "label": "Bytes sent per second", "isController": false}], "supportsControllersDiscrimination": false, "granularity": 60000, "maxX": 1.6557336E12, "title": "Bytes Throughput Over Time"}},
        getOptions : function(){
            return {
                series: {
                    lines: {
                        show: true
                    },
                    points: {
                        show: true
                    }
                },
                xaxis: {
                    mode: "time",
                    timeformat: getTimeFormat(this.data.result.granularity),
                    axisLabel: getElapsedTimeLabel(this.data.result.granularity) ,
                    axisLabelUseCanvas: true,
                    axisLabelFontSizePixels: 12,
                    axisLabelFontFamily: 'Verdana, Arial',
                    axisLabelPadding: 20,
                },
                yaxis: {
                    axisLabel: "Bytes / sec",
                    axisLabelUseCanvas: true,
                    axisLabelFontSizePixels: 12,
                    axisLabelFontFamily: 'Verdana, Arial',
                    axisLabelPadding: 20,
                },
                legend: {
                    noColumns: 2,
                    show: true,
                    container: '#legendBytesThroughputOverTime'
                },
                selection: {
                    mode: "xy"
                },
                grid: {
                    hoverable: true // IMPORTANT! this is needed for tooltip to
                                    // work
                },
                tooltip: true,
                tooltipOpts: {
                    content: "%s at %x was %y"
                }
            };
        },
        createGraph : function() {
            var data = this.data;
            var dataset = prepareData(data.result.series, $("#choicesBytesThroughputOverTime"));
            var options = this.getOptions();
            prepareOptions(options, data);
            $.plot($("#flotBytesThroughputOverTime"), dataset, options);
            // setup overview
            $.plot($("#overviewBytesThroughputOverTime"), dataset, prepareOverviewOptions(options));
        }
};

// Bytes throughput Over Time
function refreshBytesThroughputOverTime(fixTimestamps) {
    var infos = bytesThroughputOverTimeInfos;
    prepareSeries(infos.data);
    if(fixTimestamps) {
        fixTimeStamps(infos.data.result.series, -10800000);
    }
    if(isGraph($("#flotBytesThroughputOverTime"))){
        infos.createGraph();
    }else{
        var choiceContainer = $("#choicesBytesThroughputOverTime");
        createLegend(choiceContainer, infos);
        infos.createGraph();
        setGraphZoomable("#flotBytesThroughputOverTime", "#overviewBytesThroughputOverTime");
        $('#footerBytesThroughputOverTime .legendColorBox > div').each(function(i){
            $(this).clone().prependTo(choiceContainer.find("li").eq(i));
        });
    }
}

var responseTimesOverTimeInfos = {
        data: {"result": {"minY": 723.7092337917486, "minX": 1.6557336E12, "maxY": 723.7092337917486, "series": [{"data": [[1.6557336E12, 723.7092337917486]], "isOverall": false, "label": "HTTP Request", "isController": false}], "supportsControllersDiscrimination": true, "granularity": 60000, "maxX": 1.6557336E12, "title": "Response Time Over Time"}},
        getOptions: function(){
            return {
                series: {
                    lines: {
                        show: true
                    },
                    points: {
                        show: true
                    }
                },
                xaxis: {
                    mode: "time",
                    timeformat: getTimeFormat(this.data.result.granularity),
                    axisLabel: getElapsedTimeLabel(this.data.result.granularity),
                    axisLabelUseCanvas: true,
                    axisLabelFontSizePixels: 12,
                    axisLabelFontFamily: 'Verdana, Arial',
                    axisLabelPadding: 20,
                },
                yaxis: {
                    axisLabel: "Average response time in ms",
                    axisLabelUseCanvas: true,
                    axisLabelFontSizePixels: 12,
                    axisLabelFontFamily: 'Verdana, Arial',
                    axisLabelPadding: 20,
                },
                legend: {
                    noColumns: 2,
                    show: true,
                    container: '#legendResponseTimesOverTime'
                },
                selection: {
                    mode: 'xy'
                },
                grid: {
                    hoverable: true // IMPORTANT! this is needed for tooltip to
                                    // work
                },
                tooltip: true,
                tooltipOpts: {
                    content: "%s : at %x Average response time was %y ms"
                }
            };
        },
        createGraph: function() {
            var data = this.data;
            var dataset = prepareData(data.result.series, $("#choicesResponseTimesOverTime"));
            var options = this.getOptions();
            prepareOptions(options, data);
            $.plot($("#flotResponseTimesOverTime"), dataset, options);
            // setup overview
            $.plot($("#overviewResponseTimesOverTime"), dataset, prepareOverviewOptions(options));
        }
};

// Response Times Over Time
function refreshResponseTimeOverTime(fixTimestamps) {
    var infos = responseTimesOverTimeInfos;
    prepareSeries(infos.data);
    if(infos.data.result.series.length == 0) {
        setEmptyGraph("#bodyResponseTimeOverTime");
        return;
    }
    if(fixTimestamps) {
        fixTimeStamps(infos.data.result.series, -10800000);
    }
    if(isGraph($("#flotResponseTimesOverTime"))){
        infos.createGraph();
    }else{
        var choiceContainer = $("#choicesResponseTimesOverTime");
        createLegend(choiceContainer, infos);
        infos.createGraph();
        setGraphZoomable("#flotResponseTimesOverTime", "#overviewResponseTimesOverTime");
        $('#footerResponseTimesOverTime .legendColorBox > div').each(function(i){
            $(this).clone().prependTo(choiceContainer.find("li").eq(i));
        });
    }
};

var latenciesOverTimeInfos = {
        data: {"result": {"minY": 723.7072691552066, "minX": 1.6557336E12, "maxY": 723.7072691552066, "series": [{"data": [[1.6557336E12, 723.7072691552066]], "isOverall": false, "label": "HTTP Request", "isController": false}], "supportsControllersDiscrimination": true, "granularity": 60000, "maxX": 1.6557336E12, "title": "Latencies Over Time"}},
        getOptions: function() {
            return {
                series: {
                    lines: {
                        show: true
                    },
                    points: {
                        show: true
                    }
                },
                xaxis: {
                    mode: "time",
                    timeformat: getTimeFormat(this.data.result.granularity),
                    axisLabel: getElapsedTimeLabel(this.data.result.granularity),
                    axisLabelUseCanvas: true,
                    axisLabelFontSizePixels: 12,
                    axisLabelFontFamily: 'Verdana, Arial',
                    axisLabelPadding: 20,
                },
                yaxis: {
                    axisLabel: "Average response latencies in ms",
                    axisLabelUseCanvas: true,
                    axisLabelFontSizePixels: 12,
                    axisLabelFontFamily: 'Verdana, Arial',
                    axisLabelPadding: 20,
                },
                legend: {
                    noColumns: 2,
                    show: true,
                    container: '#legendLatenciesOverTime'
                },
                selection: {
                    mode: 'xy'
                },
                grid: {
                    hoverable: true // IMPORTANT! this is needed for tooltip to
                                    // work
                },
                tooltip: true,
                tooltipOpts: {
                    content: "%s : at %x Average latency was %y ms"
                }
            };
        },
        createGraph: function () {
            var data = this.data;
            var dataset = prepareData(data.result.series, $("#choicesLatenciesOverTime"));
            var options = this.getOptions();
            prepareOptions(options, data);
            $.plot($("#flotLatenciesOverTime"), dataset, options);
            // setup overview
            $.plot($("#overviewLatenciesOverTime"), dataset, prepareOverviewOptions(options));
        }
};

// Latencies Over Time
function refreshLatenciesOverTime(fixTimestamps) {
    var infos = latenciesOverTimeInfos;
    prepareSeries(infos.data);
    if(infos.data.result.series.length == 0) {
        setEmptyGraph("#bodyLatenciesOverTime");
        return;
    }
    if(fixTimestamps) {
        fixTimeStamps(infos.data.result.series, -10800000);
    }
    if(isGraph($("#flotLatenciesOverTime"))) {
        infos.createGraph();
    }else {
        var choiceContainer = $("#choicesLatenciesOverTime");
        createLegend(choiceContainer, infos);
        infos.createGraph();
        setGraphZoomable("#flotLatenciesOverTime", "#overviewLatenciesOverTime");
        $('#footerLatenciesOverTime .legendColorBox > div').each(function(i){
            $(this).clone().prependTo(choiceContainer.find("li").eq(i));
        });
    }
};

var connectTimeOverTimeInfos = {
        data: {"result": {"minY": 460.2062868369351, "minX": 1.6557336E12, "maxY": 460.2062868369351, "series": [{"data": [[1.6557336E12, 460.2062868369351]], "isOverall": false, "label": "HTTP Request", "isController": false}], "supportsControllersDiscrimination": true, "granularity": 60000, "maxX": 1.6557336E12, "title": "Connect Time Over Time"}},
        getOptions: function() {
            return {
                series: {
                    lines: {
                        show: true
                    },
                    points: {
                        show: true
                    }
                },
                xaxis: {
                    mode: "time",
                    timeformat: getTimeFormat(this.data.result.granularity),
                    axisLabel: getConnectTimeLabel(this.data.result.granularity),
                    axisLabelUseCanvas: true,
                    axisLabelFontSizePixels: 12,
                    axisLabelFontFamily: 'Verdana, Arial',
                    axisLabelPadding: 20,
                },
                yaxis: {
                    axisLabel: "Average Connect Time in ms",
                    axisLabelUseCanvas: true,
                    axisLabelFontSizePixels: 12,
                    axisLabelFontFamily: 'Verdana, Arial',
                    axisLabelPadding: 20,
                },
                legend: {
                    noColumns: 2,
                    show: true,
                    container: '#legendConnectTimeOverTime'
                },
                selection: {
                    mode: 'xy'
                },
                grid: {
                    hoverable: true // IMPORTANT! this is needed for tooltip to
                                    // work
                },
                tooltip: true,
                tooltipOpts: {
                    content: "%s : at %x Average connect time was %y ms"
                }
            };
        },
        createGraph: function () {
            var data = this.data;
            var dataset = prepareData(data.result.series, $("#choicesConnectTimeOverTime"));
            var options = this.getOptions();
            prepareOptions(options, data);
            $.plot($("#flotConnectTimeOverTime"), dataset, options);
            // setup overview
            $.plot($("#overviewConnectTimeOverTime"), dataset, prepareOverviewOptions(options));
        }
};

// Connect Time Over Time
function refreshConnectTimeOverTime(fixTimestamps) {
    var infos = connectTimeOverTimeInfos;
    prepareSeries(infos.data);
    if(infos.data.result.series.length == 0) {
        setEmptyGraph("#bodyConnectTimeOverTime");
        return;
    }
    if(fixTimestamps) {
        fixTimeStamps(infos.data.result.series, -10800000);
    }
    if(isGraph($("#flotConnectTimeOverTime"))) {
        infos.createGraph();
    }else {
        var choiceContainer = $("#choicesConnectTimeOverTime");
        createLegend(choiceContainer, infos);
        infos.createGraph();
        setGraphZoomable("#flotConnectTimeOverTime", "#overviewConnectTimeOverTime");
        $('#footerConnectTimeOverTime .legendColorBox > div').each(function(i){
            $(this).clone().prependTo(choiceContainer.find("li").eq(i));
        });
    }
};

var responseTimePercentilesOverTimeInfos = {
        data: {"result": {"minY": 474.0, "minX": 1.6557336E12, "maxY": 3687.0, "series": [{"data": [[1.6557336E12, 3687.0]], "isOverall": false, "label": "Max", "isController": false}, {"data": [[1.6557336E12, 1553.0]], "isOverall": false, "label": "90th percentile", "isController": false}, {"data": [[1.6557336E12, 3675.9]], "isOverall": false, "label": "99th percentile", "isController": false}, {"data": [[1.6557336E12, 1568.5]], "isOverall": false, "label": "95th percentile", "isController": false}, {"data": [[1.6557336E12, 474.0]], "isOverall": false, "label": "Min", "isController": false}, {"data": [[1.6557336E12, 519.0]], "isOverall": false, "label": "Median", "isController": false}], "supportsControllersDiscrimination": false, "granularity": 60000, "maxX": 1.6557336E12, "title": "Response Time Percentiles Over Time (successful requests only)"}},
        getOptions: function() {
            return {
                series: {
                    lines: {
                        show: true,
                        fill: true
                    },
                    points: {
                        show: true
                    }
                },
                xaxis: {
                    mode: "time",
                    timeformat: getTimeFormat(this.data.result.granularity),
                    axisLabel: getElapsedTimeLabel(this.data.result.granularity),
                    axisLabelUseCanvas: true,
                    axisLabelFontSizePixels: 12,
                    axisLabelFontFamily: 'Verdana, Arial',
                    axisLabelPadding: 20,
                },
                yaxis: {
                    axisLabel: "Response Time in ms",
                    axisLabelUseCanvas: true,
                    axisLabelFontSizePixels: 12,
                    axisLabelFontFamily: 'Verdana, Arial',
                    axisLabelPadding: 20,
                },
                legend: {
                    noColumns: 2,
                    show: true,
                    container: '#legendResponseTimePercentilesOverTime'
                },
                selection: {
                    mode: 'xy'
                },
                grid: {
                    hoverable: true // IMPORTANT! this is needed for tooltip to
                                    // work
                },
                tooltip: true,
                tooltipOpts: {
                    content: "%s : at %x Response time was %y ms"
                }
            };
        },
        createGraph: function () {
            var data = this.data;
            var dataset = prepareData(data.result.series, $("#choicesResponseTimePercentilesOverTime"));
            var options = this.getOptions();
            prepareOptions(options, data);
            $.plot($("#flotResponseTimePercentilesOverTime"), dataset, options);
            // setup overview
            $.plot($("#overviewResponseTimePercentilesOverTime"), dataset, prepareOverviewOptions(options));
        }
};

// Response Time Percentiles Over Time
function refreshResponseTimePercentilesOverTime(fixTimestamps) {
    var infos = responseTimePercentilesOverTimeInfos;
    prepareSeries(infos.data);
    if(fixTimestamps) {
        fixTimeStamps(infos.data.result.series, -10800000);
    }
    if(isGraph($("#flotResponseTimePercentilesOverTime"))) {
        infos.createGraph();
    }else {
        var choiceContainer = $("#choicesResponseTimePercentilesOverTime");
        createLegend(choiceContainer, infos);
        infos.createGraph();
        setGraphZoomable("#flotResponseTimePercentilesOverTime", "#overviewResponseTimePercentilesOverTime");
        $('#footerResponseTimePercentilesOverTime .legendColorBox > div').each(function(i){
            $(this).clone().prependTo(choiceContainer.find("li").eq(i));
        });
    }
};


var responseTimeVsRequestInfos = {
    data: {"result": {"minY": 515.0, "minX": 11.0, "maxY": 3675.0, "series": [{"data": [[64.0, 1556.0], [11.0, 3675.0], [434.0, 515.0]], "isOverall": false, "label": "Successes", "isController": false}], "supportsControllersDiscrimination": false, "granularity": 1000, "maxX": 434.0, "title": "Response Time Vs Request"}},
    getOptions: function() {
        return {
            series: {
                lines: {
                    show: false
                },
                points: {
                    show: true
                }
            },
            xaxis: {
                axisLabel: "Global number of requests per second",
                axisLabelUseCanvas: true,
                axisLabelFontSizePixels: 12,
                axisLabelFontFamily: 'Verdana, Arial',
                axisLabelPadding: 20,
            },
            yaxis: {
                axisLabel: "Median Response Time in ms",
                axisLabelUseCanvas: true,
                axisLabelFontSizePixels: 12,
                axisLabelFontFamily: 'Verdana, Arial',
                axisLabelPadding: 20,
            },
            legend: {
                noColumns: 2,
                show: true,
                container: '#legendResponseTimeVsRequest'
            },
            selection: {
                mode: 'xy'
            },
            grid: {
                hoverable: true // IMPORTANT! this is needed for tooltip to work
            },
            tooltip: true,
            tooltipOpts: {
                content: "%s : Median response time at %x req/s was %y ms"
            },
            colors: ["#9ACD32", "#FF6347"]
        };
    },
    createGraph: function () {
        var data = this.data;
        var dataset = prepareData(data.result.series, $("#choicesResponseTimeVsRequest"));
        var options = this.getOptions();
        prepareOptions(options, data);
        $.plot($("#flotResponseTimeVsRequest"), dataset, options);
        // setup overview
        $.plot($("#overviewResponseTimeVsRequest"), dataset, prepareOverviewOptions(options));

    }
};

// Response Time vs Request
function refreshResponseTimeVsRequest() {
    var infos = responseTimeVsRequestInfos;
    prepareSeries(infos.data);
    if (isGraph($("#flotResponseTimeVsRequest"))){
        infos.createGraph();
    }else{
        var choiceContainer = $("#choicesResponseTimeVsRequest");
        createLegend(choiceContainer, infos);
        infos.createGraph();
        setGraphZoomable("#flotResponseTimeVsRequest", "#overviewResponseTimeVsRequest");
        $('#footerResponseRimeVsRequest .legendColorBox > div').each(function(i){
            $(this).clone().prependTo(choiceContainer.find("li").eq(i));
        });
    }
};


var latenciesVsRequestInfos = {
    data: {"result": {"minY": 515.0, "minX": 11.0, "maxY": 3675.0, "series": [{"data": [[64.0, 1556.0], [11.0, 3675.0], [434.0, 515.0]], "isOverall": false, "label": "Successes", "isController": false}], "supportsControllersDiscrimination": false, "granularity": 1000, "maxX": 434.0, "title": "Latencies Vs Request"}},
    getOptions: function() {
        return{
            series: {
                lines: {
                    show: false
                },
                points: {
                    show: true
                }
            },
            xaxis: {
                axisLabel: "Global number of requests per second",
                axisLabelUseCanvas: true,
                axisLabelFontSizePixels: 12,
                axisLabelFontFamily: 'Verdana, Arial',
                axisLabelPadding: 20,
            },
            yaxis: {
                axisLabel: "Median Latency in ms",
                axisLabelUseCanvas: true,
                axisLabelFontSizePixels: 12,
                axisLabelFontFamily: 'Verdana, Arial',
                axisLabelPadding: 20,
            },
            legend: { noColumns: 2,show: true, container: '#legendLatencyVsRequest' },
            selection: {
                mode: 'xy'
            },
            grid: {
                hoverable: true // IMPORTANT! this is needed for tooltip to work
            },
            tooltip: true,
            tooltipOpts: {
                content: "%s : Median Latency time at %x req/s was %y ms"
            },
            colors: ["#9ACD32", "#FF6347"]
        };
    },
    createGraph: function () {
        var data = this.data;
        var dataset = prepareData(data.result.series, $("#choicesLatencyVsRequest"));
        var options = this.getOptions();
        prepareOptions(options, data);
        $.plot($("#flotLatenciesVsRequest"), dataset, options);
        // setup overview
        $.plot($("#overviewLatenciesVsRequest"), dataset, prepareOverviewOptions(options));
    }
};

// Latencies vs Request
function refreshLatenciesVsRequest() {
        var infos = latenciesVsRequestInfos;
        prepareSeries(infos.data);
        if(isGraph($("#flotLatenciesVsRequest"))){
            infos.createGraph();
        }else{
            var choiceContainer = $("#choicesLatencyVsRequest");
            createLegend(choiceContainer, infos);
            infos.createGraph();
            setGraphZoomable("#flotLatenciesVsRequest", "#overviewLatenciesVsRequest");
            $('#footerLatenciesVsRequest .legendColorBox > div').each(function(i){
                $(this).clone().prependTo(choiceContainer.find("li").eq(i));
            });
        }
};

var hitsPerSecondInfos = {
        data: {"result": {"minY": 8.483333333333333, "minX": 1.6557336E12, "maxY": 8.483333333333333, "series": [{"data": [[1.6557336E12, 8.483333333333333]], "isOverall": false, "label": "hitsPerSecond", "isController": false}], "supportsControllersDiscrimination": false, "granularity": 60000, "maxX": 1.6557336E12, "title": "Hits Per Second"}},
        getOptions: function() {
            return {
                series: {
                    lines: {
                        show: true
                    },
                    points: {
                        show: true
                    }
                },
                xaxis: {
                    mode: "time",
                    timeformat: getTimeFormat(this.data.result.granularity),
                    axisLabel: getElapsedTimeLabel(this.data.result.granularity),
                    axisLabelUseCanvas: true,
                    axisLabelFontSizePixels: 12,
                    axisLabelFontFamily: 'Verdana, Arial',
                    axisLabelPadding: 20,
                },
                yaxis: {
                    axisLabel: "Number of hits / sec",
                    axisLabelUseCanvas: true,
                    axisLabelFontSizePixels: 12,
                    axisLabelFontFamily: 'Verdana, Arial',
                    axisLabelPadding: 20
                },
                legend: {
                    noColumns: 2,
                    show: true,
                    container: "#legendHitsPerSecond"
                },
                selection: {
                    mode : 'xy'
                },
                grid: {
                    hoverable: true // IMPORTANT! this is needed for tooltip to
                                    // work
                },
                tooltip: true,
                tooltipOpts: {
                    content: "%s at %x was %y.2 hits/sec"
                }
            };
        },
        createGraph: function createGraph() {
            var data = this.data;
            var dataset = prepareData(data.result.series, $("#choicesHitsPerSecond"));
            var options = this.getOptions();
            prepareOptions(options, data);
            $.plot($("#flotHitsPerSecond"), dataset, options);
            // setup overview
            $.plot($("#overviewHitsPerSecond"), dataset, prepareOverviewOptions(options));
        }
};

// Hits per second
function refreshHitsPerSecond(fixTimestamps) {
    var infos = hitsPerSecondInfos;
    prepareSeries(infos.data);
    if(fixTimestamps) {
        fixTimeStamps(infos.data.result.series, -10800000);
    }
    if (isGraph($("#flotHitsPerSecond"))){
        infos.createGraph();
    }else{
        var choiceContainer = $("#choicesHitsPerSecond");
        createLegend(choiceContainer, infos);
        infos.createGraph();
        setGraphZoomable("#flotHitsPerSecond", "#overviewHitsPerSecond");
        $('#footerHitsPerSecond .legendColorBox > div').each(function(i){
            $(this).clone().prependTo(choiceContainer.find("li").eq(i));
        });
    }
}

var codesPerSecondInfos = {
        data: {"result": {"minY": 8.483333333333333, "minX": 1.6557336E12, "maxY": 8.483333333333333, "series": [{"data": [[1.6557336E12, 8.483333333333333]], "isOverall": false, "label": "200", "isController": false}], "supportsControllersDiscrimination": false, "granularity": 60000, "maxX": 1.6557336E12, "title": "Codes Per Second"}},
        getOptions: function(){
            return {
                series: {
                    lines: {
                        show: true
                    },
                    points: {
                        show: true
                    }
                },
                xaxis: {
                    mode: "time",
                    timeformat: getTimeFormat(this.data.result.granularity),
                    axisLabel: getElapsedTimeLabel(this.data.result.granularity),
                    axisLabelUseCanvas: true,
                    axisLabelFontSizePixels: 12,
                    axisLabelFontFamily: 'Verdana, Arial',
                    axisLabelPadding: 20,
                },
                yaxis: {
                    axisLabel: "Number of responses / sec",
                    axisLabelUseCanvas: true,
                    axisLabelFontSizePixels: 12,
                    axisLabelFontFamily: 'Verdana, Arial',
                    axisLabelPadding: 20,
                },
                legend: {
                    noColumns: 2,
                    show: true,
                    container: "#legendCodesPerSecond"
                },
                selection: {
                    mode: 'xy'
                },
                grid: {
                    hoverable: true // IMPORTANT! this is needed for tooltip to
                                    // work
                },
                tooltip: true,
                tooltipOpts: {
                    content: "Number of Response Codes %s at %x was %y.2 responses / sec"
                }
            };
        },
    createGraph: function() {
        var data = this.data;
        var dataset = prepareData(data.result.series, $("#choicesCodesPerSecond"));
        var options = this.getOptions();
        prepareOptions(options, data);
        $.plot($("#flotCodesPerSecond"), dataset, options);
        // setup overview
        $.plot($("#overviewCodesPerSecond"), dataset, prepareOverviewOptions(options));
    }
};

// Codes per second
function refreshCodesPerSecond(fixTimestamps) {
    var infos = codesPerSecondInfos;
    prepareSeries(infos.data);
    if(fixTimestamps) {
        fixTimeStamps(infos.data.result.series, -10800000);
    }
    if(isGraph($("#flotCodesPerSecond"))){
        infos.createGraph();
    }else{
        var choiceContainer = $("#choicesCodesPerSecond");
        createLegend(choiceContainer, infos);
        infos.createGraph();
        setGraphZoomable("#flotCodesPerSecond", "#overviewCodesPerSecond");
        $('#footerCodesPerSecond .legendColorBox > div').each(function(i){
            $(this).clone().prependTo(choiceContainer.find("li").eq(i));
        });
    }
};

var transactionsPerSecondInfos = {
        data: {"result": {"minY": 8.483333333333333, "minX": 1.6557336E12, "maxY": 8.483333333333333, "series": [{"data": [[1.6557336E12, 8.483333333333333]], "isOverall": false, "label": "HTTP Request-success", "isController": false}], "supportsControllersDiscrimination": true, "granularity": 60000, "maxX": 1.6557336E12, "title": "Transactions Per Second"}},
        getOptions: function(){
            return {
                series: {
                    lines: {
                        show: true
                    },
                    points: {
                        show: true
                    }
                },
                xaxis: {
                    mode: "time",
                    timeformat: getTimeFormat(this.data.result.granularity),
                    axisLabel: getElapsedTimeLabel(this.data.result.granularity),
                    axisLabelUseCanvas: true,
                    axisLabelFontSizePixels: 12,
                    axisLabelFontFamily: 'Verdana, Arial',
                    axisLabelPadding: 20,
                },
                yaxis: {
                    axisLabel: "Number of transactions / sec",
                    axisLabelUseCanvas: true,
                    axisLabelFontSizePixels: 12,
                    axisLabelFontFamily: 'Verdana, Arial',
                    axisLabelPadding: 20
                },
                legend: {
                    noColumns: 2,
                    show: true,
                    container: "#legendTransactionsPerSecond"
                },
                selection: {
                    mode: 'xy'
                },
                grid: {
                    hoverable: true // IMPORTANT! this is needed for tooltip to
                                    // work
                },
                tooltip: true,
                tooltipOpts: {
                    content: "%s at %x was %y transactions / sec"
                }
            };
        },
    createGraph: function () {
        var data = this.data;
        var dataset = prepareData(data.result.series, $("#choicesTransactionsPerSecond"));
        var options = this.getOptions();
        prepareOptions(options, data);
        $.plot($("#flotTransactionsPerSecond"), dataset, options);
        // setup overview
        $.plot($("#overviewTransactionsPerSecond"), dataset, prepareOverviewOptions(options));
    }
};

// Transactions per second
function refreshTransactionsPerSecond(fixTimestamps) {
    var infos = transactionsPerSecondInfos;
    prepareSeries(infos.data);
    if(infos.data.result.series.length == 0) {
        setEmptyGraph("#bodyTransactionsPerSecond");
        return;
    }
    if(fixTimestamps) {
        fixTimeStamps(infos.data.result.series, -10800000);
    }
    if(isGraph($("#flotTransactionsPerSecond"))){
        infos.createGraph();
    }else{
        var choiceContainer = $("#choicesTransactionsPerSecond");
        createLegend(choiceContainer, infos);
        infos.createGraph();
        setGraphZoomable("#flotTransactionsPerSecond", "#overviewTransactionsPerSecond");
        $('#footerTransactionsPerSecond .legendColorBox > div').each(function(i){
            $(this).clone().prependTo(choiceContainer.find("li").eq(i));
        });
    }
};

var totalTPSInfos = {
        data: {"result": {"minY": 8.483333333333333, "minX": 1.6557336E12, "maxY": 8.483333333333333, "series": [{"data": [[1.6557336E12, 8.483333333333333]], "isOverall": false, "label": "Transaction-success", "isController": false}, {"data": [], "isOverall": false, "label": "Transaction-failure", "isController": false}], "supportsControllersDiscrimination": true, "granularity": 60000, "maxX": 1.6557336E12, "title": "Total Transactions Per Second"}},
        getOptions: function(){
            return {
                series: {
                    lines: {
                        show: true
                    },
                    points: {
                        show: true
                    }
                },
                xaxis: {
                    mode: "time",
                    timeformat: getTimeFormat(this.data.result.granularity),
                    axisLabel: getElapsedTimeLabel(this.data.result.granularity),
                    axisLabelUseCanvas: true,
                    axisLabelFontSizePixels: 12,
                    axisLabelFontFamily: 'Verdana, Arial',
                    axisLabelPadding: 20,
                },
                yaxis: {
                    axisLabel: "Number of transactions / sec",
                    axisLabelUseCanvas: true,
                    axisLabelFontSizePixels: 12,
                    axisLabelFontFamily: 'Verdana, Arial',
                    axisLabelPadding: 20
                },
                legend: {
                    noColumns: 2,
                    show: true,
                    container: "#legendTotalTPS"
                },
                selection: {
                    mode: 'xy'
                },
                grid: {
                    hoverable: true // IMPORTANT! this is needed for tooltip to
                                    // work
                },
                tooltip: true,
                tooltipOpts: {
                    content: "%s at %x was %y transactions / sec"
                },
                colors: ["#9ACD32", "#FF6347"]
            };
        },
    createGraph: function () {
        var data = this.data;
        var dataset = prepareData(data.result.series, $("#choicesTotalTPS"));
        var options = this.getOptions();
        prepareOptions(options, data);
        $.plot($("#flotTotalTPS"), dataset, options);
        // setup overview
        $.plot($("#overviewTotalTPS"), dataset, prepareOverviewOptions(options));
    }
};

// Total Transactions per second
function refreshTotalTPS(fixTimestamps) {
    var infos = totalTPSInfos;
    // We want to ignore seriesFilter
    prepareSeries(infos.data, false, true);
    if(fixTimestamps) {
        fixTimeStamps(infos.data.result.series, -10800000);
    }
    if(isGraph($("#flotTotalTPS"))){
        infos.createGraph();
    }else{
        var choiceContainer = $("#choicesTotalTPS");
        createLegend(choiceContainer, infos);
        infos.createGraph();
        setGraphZoomable("#flotTotalTPS", "#overviewTotalTPS");
        $('#footerTotalTPS .legendColorBox > div').each(function(i){
            $(this).clone().prependTo(choiceContainer.find("li").eq(i));
        });
    }
};

// Collapse the graph matching the specified DOM element depending the collapsed
// status
function collapse(elem, collapsed){
    if(collapsed){
        $(elem).parent().find(".fa-chevron-up").removeClass("fa-chevron-up").addClass("fa-chevron-down");
    } else {
        $(elem).parent().find(".fa-chevron-down").removeClass("fa-chevron-down").addClass("fa-chevron-up");
        if (elem.id == "bodyBytesThroughputOverTime") {
            if (isGraph($(elem).find('.flot-chart-content')) == false) {
                refreshBytesThroughputOverTime(true);
            }
            document.location.href="#bytesThroughputOverTime";
        } else if (elem.id == "bodyLatenciesOverTime") {
            if (isGraph($(elem).find('.flot-chart-content')) == false) {
                refreshLatenciesOverTime(true);
            }
            document.location.href="#latenciesOverTime";
        } else if (elem.id == "bodyCustomGraph") {
            if (isGraph($(elem).find('.flot-chart-content')) == false) {
                refreshCustomGraph(true);
            }
            document.location.href="#responseCustomGraph";
        } else if (elem.id == "bodyConnectTimeOverTime") {
            if (isGraph($(elem).find('.flot-chart-content')) == false) {
                refreshConnectTimeOverTime(true);
            }
            document.location.href="#connectTimeOverTime";
        } else if (elem.id == "bodyResponseTimePercentilesOverTime") {
            if (isGraph($(elem).find('.flot-chart-content')) == false) {
                refreshResponseTimePercentilesOverTime(true);
            }
            document.location.href="#responseTimePercentilesOverTime";
        } else if (elem.id == "bodyResponseTimeDistribution") {
            if (isGraph($(elem).find('.flot-chart-content')) == false) {
                refreshResponseTimeDistribution();
            }
            document.location.href="#responseTimeDistribution" ;
        } else if (elem.id == "bodySyntheticResponseTimeDistribution") {
            if (isGraph($(elem).find('.flot-chart-content')) == false) {
                refreshSyntheticResponseTimeDistribution();
            }
            document.location.href="#syntheticResponseTimeDistribution" ;
        } else if (elem.id == "bodyActiveThreadsOverTime") {
            if (isGraph($(elem).find('.flot-chart-content')) == false) {
                refreshActiveThreadsOverTime(true);
            }
            document.location.href="#activeThreadsOverTime";
        } else if (elem.id == "bodyTimeVsThreads") {
            if (isGraph($(elem).find('.flot-chart-content')) == false) {
                refreshTimeVsThreads();
            }
            document.location.href="#timeVsThreads" ;
        } else if (elem.id == "bodyCodesPerSecond") {
            if (isGraph($(elem).find('.flot-chart-content')) == false) {
                refreshCodesPerSecond(true);
            }
            document.location.href="#codesPerSecond";
        } else if (elem.id == "bodyTransactionsPerSecond") {
            if (isGraph($(elem).find('.flot-chart-content')) == false) {
                refreshTransactionsPerSecond(true);
            }
            document.location.href="#transactionsPerSecond";
        } else if (elem.id == "bodyTotalTPS") {
            if (isGraph($(elem).find('.flot-chart-content')) == false) {
                refreshTotalTPS(true);
            }
            document.location.href="#totalTPS";
        } else if (elem.id == "bodyResponseTimeVsRequest") {
            if (isGraph($(elem).find('.flot-chart-content')) == false) {
                refreshResponseTimeVsRequest();
            }
            document.location.href="#responseTimeVsRequest";
        } else if (elem.id == "bodyLatenciesVsRequest") {
            if (isGraph($(elem).find('.flot-chart-content')) == false) {
                refreshLatenciesVsRequest();
            }
            document.location.href="#latencyVsRequest";
        }
    }
}

/*
 * Activates or deactivates all series of the specified graph (represented by id parameter)
 * depending on checked argument.
 */
function toggleAll(id, checked){
    var placeholder = document.getElementById(id);

    var cases = $(placeholder).find(':checkbox');
    cases.prop('checked', checked);
    $(cases).parent().children().children().toggleClass("legend-disabled", !checked);

    var choiceContainer;
    if ( id == "choicesBytesThroughputOverTime"){
        choiceContainer = $("#choicesBytesThroughputOverTime");
        refreshBytesThroughputOverTime(false);
    } else if(id == "choicesResponseTimesOverTime"){
        choiceContainer = $("#choicesResponseTimesOverTime");
        refreshResponseTimeOverTime(false);
    }else if(id == "choicesResponseCustomGraph"){
        choiceContainer = $("#choicesResponseCustomGraph");
        refreshCustomGraph(false);
    } else if ( id == "choicesLatenciesOverTime"){
        choiceContainer = $("#choicesLatenciesOverTime");
        refreshLatenciesOverTime(false);
    } else if ( id == "choicesConnectTimeOverTime"){
        choiceContainer = $("#choicesConnectTimeOverTime");
        refreshConnectTimeOverTime(false);
    } else if ( id == "choicesResponseTimePercentilesOverTime"){
        choiceContainer = $("#choicesResponseTimePercentilesOverTime");
        refreshResponseTimePercentilesOverTime(false);
    } else if ( id == "choicesResponseTimePercentiles"){
        choiceContainer = $("#choicesResponseTimePercentiles");
        refreshResponseTimePercentiles();
    } else if(id == "choicesActiveThreadsOverTime"){
        choiceContainer = $("#choicesActiveThreadsOverTime");
        refreshActiveThreadsOverTime(false);
    } else if ( id == "choicesTimeVsThreads"){
        choiceContainer = $("#choicesTimeVsThreads");
        refreshTimeVsThreads();
    } else if ( id == "choicesSyntheticResponseTimeDistribution"){
        choiceContainer = $("#choicesSyntheticResponseTimeDistribution");
        refreshSyntheticResponseTimeDistribution();
    } else if ( id == "choicesResponseTimeDistribution"){
        choiceContainer = $("#choicesResponseTimeDistribution");
        refreshResponseTimeDistribution();
    } else if ( id == "choicesHitsPerSecond"){
        choiceContainer = $("#choicesHitsPerSecond");
        refreshHitsPerSecond(false);
    } else if(id == "choicesCodesPerSecond"){
        choiceContainer = $("#choicesCodesPerSecond");
        refreshCodesPerSecond(false);
    } else if ( id == "choicesTransactionsPerSecond"){
        choiceContainer = $("#choicesTransactionsPerSecond");
        refreshTransactionsPerSecond(false);
    } else if ( id == "choicesTotalTPS"){
        choiceContainer = $("#choicesTotalTPS");
        refreshTotalTPS(false);
    } else if ( id == "choicesResponseTimeVsRequest"){
        choiceContainer = $("#choicesResponseTimeVsRequest");
        refreshResponseTimeVsRequest();
    } else if ( id == "choicesLatencyVsRequest"){
        choiceContainer = $("#choicesLatencyVsRequest");
        refreshLatenciesVsRequest();
    }
    var color = checked ? "black" : "#818181";
    if(choiceContainer != null) {
        choiceContainer.find("label").each(function(){
            this.style.color = color;
        });
    }
}


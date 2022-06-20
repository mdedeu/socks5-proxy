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
        data: {"result": {"minY": 5.0, "minX": 0.0, "maxY": 7765.0, "series": [{"data": [[0.0, 5.0], [0.1, 468.0], [0.2, 471.0], [0.3, 473.0], [0.4, 474.0], [0.5, 475.0], [0.6, 476.0], [0.7, 477.0], [0.8, 477.0], [0.9, 478.0], [1.0, 478.0], [1.1, 479.0], [1.2, 479.0], [1.3, 479.0], [1.4, 480.0], [1.5, 480.0], [1.6, 481.0], [1.7, 481.0], [1.8, 481.0], [1.9, 481.0], [2.0, 482.0], [2.1, 482.0], [2.2, 482.0], [2.3, 482.0], [2.4, 483.0], [2.5, 483.0], [2.6, 483.0], [2.7, 483.0], [2.8, 483.0], [2.9, 484.0], [3.0, 484.0], [3.1, 484.0], [3.2, 484.0], [3.3, 484.0], [3.4, 484.0], [3.5, 485.0], [3.6, 485.0], [3.7, 485.0], [3.8, 485.0], [3.9, 485.0], [4.0, 485.0], [4.1, 486.0], [4.2, 486.0], [4.3, 486.0], [4.4, 486.0], [4.5, 486.0], [4.6, 486.0], [4.7, 486.0], [4.8, 486.0], [4.9, 487.0], [5.0, 487.0], [5.1, 487.0], [5.2, 487.0], [5.3, 487.0], [5.4, 487.0], [5.5, 487.0], [5.6, 487.0], [5.7, 488.0], [5.8, 488.0], [5.9, 488.0], [6.0, 488.0], [6.1, 488.0], [6.2, 488.0], [6.3, 488.0], [6.4, 488.0], [6.5, 488.0], [6.6, 488.0], [6.7, 489.0], [6.8, 489.0], [6.9, 489.0], [7.0, 489.0], [7.1, 489.0], [7.2, 489.0], [7.3, 489.0], [7.4, 489.0], [7.5, 489.0], [7.6, 489.0], [7.7, 489.0], [7.8, 490.0], [7.9, 490.0], [8.0, 490.0], [8.1, 490.0], [8.2, 490.0], [8.3, 490.0], [8.4, 490.0], [8.5, 490.0], [8.6, 490.0], [8.7, 490.0], [8.8, 490.0], [8.9, 490.0], [9.0, 491.0], [9.1, 491.0], [9.2, 491.0], [9.3, 491.0], [9.4, 491.0], [9.5, 491.0], [9.6, 491.0], [9.7, 491.0], [9.8, 491.0], [9.9, 491.0], [10.0, 491.0], [10.1, 491.0], [10.2, 491.0], [10.3, 491.0], [10.4, 492.0], [10.5, 492.0], [10.6, 492.0], [10.7, 492.0], [10.8, 492.0], [10.9, 492.0], [11.0, 492.0], [11.1, 492.0], [11.2, 492.0], [11.3, 492.0], [11.4, 492.0], [11.5, 492.0], [11.6, 492.0], [11.7, 493.0], [11.8, 493.0], [11.9, 493.0], [12.0, 493.0], [12.1, 493.0], [12.2, 493.0], [12.3, 493.0], [12.4, 493.0], [12.5, 493.0], [12.6, 493.0], [12.7, 493.0], [12.8, 493.0], [12.9, 493.0], [13.0, 493.0], [13.1, 493.0], [13.2, 493.0], [13.3, 494.0], [13.4, 494.0], [13.5, 494.0], [13.6, 494.0], [13.7, 494.0], [13.8, 494.0], [13.9, 494.0], [14.0, 494.0], [14.1, 494.0], [14.2, 494.0], [14.3, 494.0], [14.4, 494.0], [14.5, 494.0], [14.6, 494.0], [14.7, 494.0], [14.8, 494.0], [14.9, 494.0], [15.0, 495.0], [15.1, 495.0], [15.2, 495.0], [15.3, 495.0], [15.4, 495.0], [15.5, 495.0], [15.6, 495.0], [15.7, 495.0], [15.8, 495.0], [15.9, 495.0], [16.0, 495.0], [16.1, 495.0], [16.2, 495.0], [16.3, 495.0], [16.4, 495.0], [16.5, 495.0], [16.6, 495.0], [16.7, 495.0], [16.8, 495.0], [16.9, 495.0], [17.0, 496.0], [17.1, 496.0], [17.2, 496.0], [17.3, 496.0], [17.4, 496.0], [17.5, 496.0], [17.6, 496.0], [17.7, 496.0], [17.8, 496.0], [17.9, 496.0], [18.0, 496.0], [18.1, 496.0], [18.2, 496.0], [18.3, 496.0], [18.4, 496.0], [18.5, 496.0], [18.6, 496.0], [18.7, 496.0], [18.8, 496.0], [18.9, 496.0], [19.0, 497.0], [19.1, 497.0], [19.2, 497.0], [19.3, 497.0], [19.4, 497.0], [19.5, 497.0], [19.6, 497.0], [19.7, 497.0], [19.8, 497.0], [19.9, 497.0], [20.0, 497.0], [20.1, 497.0], [20.2, 497.0], [20.3, 497.0], [20.4, 497.0], [20.5, 497.0], [20.6, 497.0], [20.7, 497.0], [20.8, 497.0], [20.9, 497.0], [21.0, 497.0], [21.1, 497.0], [21.2, 498.0], [21.3, 498.0], [21.4, 498.0], [21.5, 498.0], [21.6, 498.0], [21.7, 498.0], [21.8, 498.0], [21.9, 498.0], [22.0, 498.0], [22.1, 498.0], [22.2, 498.0], [22.3, 498.0], [22.4, 498.0], [22.5, 498.0], [22.6, 498.0], [22.7, 498.0], [22.8, 498.0], [22.9, 498.0], [23.0, 498.0], [23.1, 498.0], [23.2, 498.0], [23.3, 498.0], [23.4, 499.0], [23.5, 499.0], [23.6, 499.0], [23.7, 499.0], [23.8, 499.0], [23.9, 499.0], [24.0, 499.0], [24.1, 499.0], [24.2, 499.0], [24.3, 499.0], [24.4, 499.0], [24.5, 499.0], [24.6, 499.0], [24.7, 499.0], [24.8, 499.0], [24.9, 499.0], [25.0, 499.0], [25.1, 499.0], [25.2, 499.0], [25.3, 499.0], [25.4, 499.0], [25.5, 499.0], [25.6, 499.0], [25.7, 499.0], [25.8, 500.0], [25.9, 500.0], [26.0, 500.0], [26.1, 500.0], [26.2, 500.0], [26.3, 500.0], [26.4, 500.0], [26.5, 500.0], [26.6, 500.0], [26.7, 500.0], [26.8, 500.0], [26.9, 500.0], [27.0, 500.0], [27.1, 500.0], [27.2, 500.0], [27.3, 500.0], [27.4, 500.0], [27.5, 500.0], [27.6, 500.0], [27.7, 500.0], [27.8, 500.0], [27.9, 500.0], [28.0, 500.0], [28.1, 500.0], [28.2, 500.0], [28.3, 500.0], [28.4, 500.0], [28.5, 501.0], [28.6, 501.0], [28.7, 501.0], [28.8, 501.0], [28.9, 501.0], [29.0, 501.0], [29.1, 501.0], [29.2, 501.0], [29.3, 501.0], [29.4, 501.0], [29.5, 501.0], [29.6, 501.0], [29.7, 501.0], [29.8, 501.0], [29.9, 501.0], [30.0, 501.0], [30.1, 501.0], [30.2, 501.0], [30.3, 501.0], [30.4, 501.0], [30.5, 501.0], [30.6, 501.0], [30.7, 501.0], [30.8, 501.0], [30.9, 501.0], [31.0, 501.0], [31.1, 502.0], [31.2, 502.0], [31.3, 502.0], [31.4, 502.0], [31.5, 502.0], [31.6, 502.0], [31.7, 502.0], [31.8, 502.0], [31.9, 502.0], [32.0, 502.0], [32.1, 502.0], [32.2, 502.0], [32.3, 502.0], [32.4, 502.0], [32.5, 502.0], [32.6, 502.0], [32.7, 502.0], [32.8, 502.0], [32.9, 502.0], [33.0, 502.0], [33.1, 502.0], [33.2, 502.0], [33.3, 502.0], [33.4, 502.0], [33.5, 502.0], [33.6, 502.0], [33.7, 502.0], [33.8, 503.0], [33.9, 503.0], [34.0, 503.0], [34.1, 503.0], [34.2, 503.0], [34.3, 503.0], [34.4, 503.0], [34.5, 503.0], [34.6, 503.0], [34.7, 503.0], [34.8, 503.0], [34.9, 503.0], [35.0, 503.0], [35.1, 503.0], [35.2, 503.0], [35.3, 503.0], [35.4, 503.0], [35.5, 503.0], [35.6, 503.0], [35.7, 503.0], [35.8, 503.0], [35.9, 503.0], [36.0, 503.0], [36.1, 503.0], [36.2, 503.0], [36.3, 503.0], [36.4, 503.0], [36.5, 504.0], [36.6, 504.0], [36.7, 504.0], [36.8, 504.0], [36.9, 504.0], [37.0, 504.0], [37.1, 504.0], [37.2, 504.0], [37.3, 504.0], [37.4, 504.0], [37.5, 504.0], [37.6, 504.0], [37.7, 504.0], [37.8, 504.0], [37.9, 504.0], [38.0, 504.0], [38.1, 504.0], [38.2, 504.0], [38.3, 504.0], [38.4, 504.0], [38.5, 504.0], [38.6, 504.0], [38.7, 504.0], [38.8, 504.0], [38.9, 504.0], [39.0, 504.0], [39.1, 504.0], [39.2, 504.0], [39.3, 505.0], [39.4, 505.0], [39.5, 505.0], [39.6, 505.0], [39.7, 505.0], [39.8, 505.0], [39.9, 505.0], [40.0, 505.0], [40.1, 505.0], [40.2, 505.0], [40.3, 505.0], [40.4, 505.0], [40.5, 505.0], [40.6, 505.0], [40.7, 505.0], [40.8, 505.0], [40.9, 505.0], [41.0, 505.0], [41.1, 505.0], [41.2, 505.0], [41.3, 505.0], [41.4, 505.0], [41.5, 505.0], [41.6, 505.0], [41.7, 505.0], [41.8, 505.0], [41.9, 505.0], [42.0, 505.0], [42.1, 505.0], [42.2, 505.0], [42.3, 506.0], [42.4, 506.0], [42.5, 506.0], [42.6, 506.0], [42.7, 506.0], [42.8, 506.0], [42.9, 506.0], [43.0, 506.0], [43.1, 506.0], [43.2, 506.0], [43.3, 506.0], [43.4, 506.0], [43.5, 506.0], [43.6, 506.0], [43.7, 506.0], [43.8, 506.0], [43.9, 506.0], [44.0, 506.0], [44.1, 506.0], [44.2, 506.0], [44.3, 506.0], [44.4, 506.0], [44.5, 506.0], [44.6, 506.0], [44.7, 506.0], [44.8, 506.0], [44.9, 506.0], [45.0, 506.0], [45.1, 507.0], [45.2, 507.0], [45.3, 507.0], [45.4, 507.0], [45.5, 507.0], [45.6, 507.0], [45.7, 507.0], [45.8, 507.0], [45.9, 507.0], [46.0, 507.0], [46.1, 507.0], [46.2, 507.0], [46.3, 507.0], [46.4, 507.0], [46.5, 507.0], [46.6, 507.0], [46.7, 507.0], [46.8, 507.0], [46.9, 507.0], [47.0, 507.0], [47.1, 507.0], [47.2, 507.0], [47.3, 507.0], [47.4, 507.0], [47.5, 507.0], [47.6, 507.0], [47.7, 507.0], [47.8, 507.0], [47.9, 507.0], [48.0, 507.0], [48.1, 508.0], [48.2, 508.0], [48.3, 508.0], [48.4, 508.0], [48.5, 508.0], [48.6, 508.0], [48.7, 508.0], [48.8, 508.0], [48.9, 508.0], [49.0, 508.0], [49.1, 508.0], [49.2, 508.0], [49.3, 508.0], [49.4, 508.0], [49.5, 508.0], [49.6, 508.0], [49.7, 508.0], [49.8, 508.0], [49.9, 508.0], [50.0, 508.0], [50.1, 508.0], [50.2, 508.0], [50.3, 508.0], [50.4, 508.0], [50.5, 508.0], [50.6, 508.0], [50.7, 508.0], [50.8, 508.0], [50.9, 508.0], [51.0, 508.0], [51.1, 509.0], [51.2, 509.0], [51.3, 509.0], [51.4, 509.0], [51.5, 509.0], [51.6, 509.0], [51.7, 509.0], [51.8, 509.0], [51.9, 509.0], [52.0, 509.0], [52.1, 509.0], [52.2, 509.0], [52.3, 509.0], [52.4, 509.0], [52.5, 509.0], [52.6, 509.0], [52.7, 509.0], [52.8, 509.0], [52.9, 509.0], [53.0, 509.0], [53.1, 509.0], [53.2, 509.0], [53.3, 509.0], [53.4, 509.0], [53.5, 509.0], [53.6, 509.0], [53.7, 510.0], [53.8, 510.0], [53.9, 510.0], [54.0, 510.0], [54.1, 510.0], [54.2, 510.0], [54.3, 510.0], [54.4, 510.0], [54.5, 510.0], [54.6, 510.0], [54.7, 510.0], [54.8, 510.0], [54.9, 510.0], [55.0, 510.0], [55.1, 510.0], [55.2, 510.0], [55.3, 510.0], [55.4, 510.0], [55.5, 510.0], [55.6, 510.0], [55.7, 510.0], [55.8, 510.0], [55.9, 510.0], [56.0, 510.0], [56.1, 510.0], [56.2, 510.0], [56.3, 510.0], [56.4, 510.0], [56.5, 511.0], [56.6, 511.0], [56.7, 511.0], [56.8, 511.0], [56.9, 511.0], [57.0, 511.0], [57.1, 511.0], [57.2, 511.0], [57.3, 511.0], [57.4, 511.0], [57.5, 511.0], [57.6, 511.0], [57.7, 511.0], [57.8, 511.0], [57.9, 511.0], [58.0, 511.0], [58.1, 511.0], [58.2, 511.0], [58.3, 511.0], [58.4, 511.0], [58.5, 511.0], [58.6, 511.0], [58.7, 511.0], [58.8, 511.0], [58.9, 511.0], [59.0, 512.0], [59.1, 512.0], [59.2, 512.0], [59.3, 512.0], [59.4, 512.0], [59.5, 512.0], [59.6, 512.0], [59.7, 512.0], [59.8, 512.0], [59.9, 512.0], [60.0, 512.0], [60.1, 512.0], [60.2, 512.0], [60.3, 512.0], [60.4, 512.0], [60.5, 512.0], [60.6, 512.0], [60.7, 512.0], [60.8, 512.0], [60.9, 512.0], [61.0, 512.0], [61.1, 512.0], [61.2, 512.0], [61.3, 512.0], [61.4, 513.0], [61.5, 513.0], [61.6, 513.0], [61.7, 513.0], [61.8, 513.0], [61.9, 513.0], [62.0, 513.0], [62.1, 513.0], [62.2, 513.0], [62.3, 513.0], [62.4, 513.0], [62.5, 513.0], [62.6, 513.0], [62.7, 513.0], [62.8, 513.0], [62.9, 513.0], [63.0, 513.0], [63.1, 513.0], [63.2, 513.0], [63.3, 513.0], [63.4, 513.0], [63.5, 513.0], [63.6, 513.0], [63.7, 513.0], [63.8, 514.0], [63.9, 514.0], [64.0, 514.0], [64.1, 514.0], [64.2, 514.0], [64.3, 514.0], [64.4, 514.0], [64.5, 514.0], [64.6, 514.0], [64.7, 514.0], [64.8, 514.0], [64.9, 514.0], [65.0, 514.0], [65.1, 514.0], [65.2, 514.0], [65.3, 514.0], [65.4, 514.0], [65.5, 514.0], [65.6, 514.0], [65.7, 514.0], [65.8, 514.0], [65.9, 514.0], [66.0, 514.0], [66.1, 514.0], [66.2, 514.0], [66.3, 514.0], [66.4, 515.0], [66.5, 515.0], [66.6, 515.0], [66.7, 515.0], [66.8, 515.0], [66.9, 515.0], [67.0, 515.0], [67.1, 515.0], [67.2, 515.0], [67.3, 515.0], [67.4, 515.0], [67.5, 515.0], [67.6, 515.0], [67.7, 515.0], [67.8, 515.0], [67.9, 515.0], [68.0, 515.0], [68.1, 515.0], [68.2, 515.0], [68.3, 515.0], [68.4, 515.0], [68.5, 515.0], [68.6, 516.0], [68.7, 516.0], [68.8, 516.0], [68.9, 516.0], [69.0, 516.0], [69.1, 516.0], [69.2, 516.0], [69.3, 516.0], [69.4, 516.0], [69.5, 516.0], [69.6, 516.0], [69.7, 516.0], [69.8, 516.0], [69.9, 516.0], [70.0, 516.0], [70.1, 516.0], [70.2, 516.0], [70.3, 516.0], [70.4, 516.0], [70.5, 516.0], [70.6, 516.0], [70.7, 516.0], [70.8, 517.0], [70.9, 517.0], [71.0, 517.0], [71.1, 517.0], [71.2, 517.0], [71.3, 517.0], [71.4, 517.0], [71.5, 517.0], [71.6, 517.0], [71.7, 517.0], [71.8, 517.0], [71.9, 517.0], [72.0, 517.0], [72.1, 517.0], [72.2, 517.0], [72.3, 517.0], [72.4, 517.0], [72.5, 517.0], [72.6, 517.0], [72.7, 518.0], [72.8, 518.0], [72.9, 518.0], [73.0, 518.0], [73.1, 518.0], [73.2, 518.0], [73.3, 518.0], [73.4, 518.0], [73.5, 518.0], [73.6, 518.0], [73.7, 518.0], [73.8, 518.0], [73.9, 518.0], [74.0, 518.0], [74.1, 518.0], [74.2, 518.0], [74.3, 518.0], [74.4, 518.0], [74.5, 518.0], [74.6, 518.0], [74.7, 519.0], [74.8, 519.0], [74.9, 519.0], [75.0, 519.0], [75.1, 519.0], [75.2, 519.0], [75.3, 519.0], [75.4, 519.0], [75.5, 519.0], [75.6, 519.0], [75.7, 519.0], [75.8, 519.0], [75.9, 519.0], [76.0, 519.0], [76.1, 519.0], [76.2, 519.0], [76.3, 519.0], [76.4, 519.0], [76.5, 520.0], [76.6, 520.0], [76.7, 520.0], [76.8, 520.0], [76.9, 520.0], [77.0, 520.0], [77.1, 520.0], [77.2, 520.0], [77.3, 520.0], [77.4, 520.0], [77.5, 520.0], [77.6, 520.0], [77.7, 520.0], [77.8, 520.0], [77.9, 520.0], [78.0, 521.0], [78.1, 521.0], [78.2, 521.0], [78.3, 521.0], [78.4, 521.0], [78.5, 521.0], [78.6, 521.0], [78.7, 521.0], [78.8, 521.0], [78.9, 521.0], [79.0, 521.0], [79.1, 521.0], [79.2, 521.0], [79.3, 521.0], [79.4, 521.0], [79.5, 521.0], [79.6, 522.0], [79.7, 522.0], [79.8, 522.0], [79.9, 522.0], [80.0, 522.0], [80.1, 522.0], [80.2, 522.0], [80.3, 522.0], [80.4, 522.0], [80.5, 522.0], [80.6, 522.0], [80.7, 522.0], [80.8, 522.0], [80.9, 522.0], [81.0, 523.0], [81.1, 523.0], [81.2, 523.0], [81.3, 523.0], [81.4, 523.0], [81.5, 523.0], [81.6, 523.0], [81.7, 523.0], [81.8, 523.0], [81.9, 523.0], [82.0, 523.0], [82.1, 523.0], [82.2, 523.0], [82.3, 524.0], [82.4, 524.0], [82.5, 524.0], [82.6, 524.0], [82.7, 524.0], [82.8, 524.0], [82.9, 524.0], [83.0, 524.0], [83.1, 524.0], [83.2, 524.0], [83.3, 524.0], [83.4, 524.0], [83.5, 525.0], [83.6, 525.0], [83.7, 525.0], [83.8, 525.0], [83.9, 525.0], [84.0, 525.0], [84.1, 525.0], [84.2, 525.0], [84.3, 525.0], [84.4, 525.0], [84.5, 525.0], [84.6, 526.0], [84.7, 526.0], [84.8, 526.0], [84.9, 526.0], [85.0, 526.0], [85.1, 526.0], [85.2, 526.0], [85.3, 526.0], [85.4, 526.0], [85.5, 526.0], [85.6, 527.0], [85.7, 527.0], [85.8, 527.0], [85.9, 527.0], [86.0, 527.0], [86.1, 527.0], [86.2, 527.0], [86.3, 527.0], [86.4, 527.0], [86.5, 527.0], [86.6, 528.0], [86.7, 528.0], [86.8, 528.0], [86.9, 528.0], [87.0, 528.0], [87.1, 528.0], [87.2, 528.0], [87.3, 528.0], [87.4, 528.0], [87.5, 529.0], [87.6, 529.0], [87.7, 529.0], [87.8, 529.0], [87.9, 529.0], [88.0, 529.0], [88.1, 529.0], [88.2, 529.0], [88.3, 530.0], [88.4, 530.0], [88.5, 530.0], [88.6, 530.0], [88.7, 530.0], [88.8, 530.0], [88.9, 530.0], [89.0, 531.0], [89.1, 531.0], [89.2, 531.0], [89.3, 531.0], [89.4, 531.0], [89.5, 531.0], [89.6, 531.0], [89.7, 532.0], [89.8, 532.0], [89.9, 532.0], [90.0, 532.0], [90.1, 532.0], [90.2, 533.0], [90.3, 533.0], [90.4, 533.0], [90.5, 533.0], [90.6, 533.0], [90.7, 534.0], [90.8, 534.0], [90.9, 534.0], [91.0, 534.0], [91.1, 534.0], [91.2, 535.0], [91.3, 535.0], [91.4, 535.0], [91.5, 535.0], [91.6, 536.0], [91.7, 536.0], [91.8, 536.0], [91.9, 536.0], [92.0, 537.0], [92.1, 537.0], [92.2, 537.0], [92.3, 537.0], [92.4, 538.0], [92.5, 538.0], [92.6, 539.0], [92.7, 539.0], [92.8, 539.0], [92.9, 540.0], [93.0, 540.0], [93.1, 541.0], [93.2, 541.0], [93.3, 542.0], [93.4, 542.0], [93.5, 543.0], [93.6, 544.0], [93.7, 544.0], [93.8, 545.0], [93.9, 546.0], [94.0, 547.0], [94.1, 548.0], [94.2, 549.0], [94.3, 550.0], [94.4, 551.0], [94.5, 553.0], [94.6, 555.0], [94.7, 557.0], [94.8, 560.0], [94.9, 562.0], [95.0, 566.0], [95.1, 576.0], [95.2, 583.0], [95.3, 591.0], [95.4, 602.0], [95.5, 616.0], [95.6, 627.0], [95.7, 740.0], [95.8, 1494.0], [95.9, 1500.0], [96.0, 1503.0], [96.1, 1506.0], [96.2, 1508.0], [96.3, 1511.0], [96.4, 1512.0], [96.5, 1513.0], [96.6, 1516.0], [96.7, 1517.0], [96.8, 1519.0], [96.9, 1520.0], [97.0, 1521.0], [97.1, 1522.0], [97.2, 1523.0], [97.3, 1525.0], [97.4, 1526.0], [97.5, 1527.0], [97.6, 1529.0], [97.7, 1530.0], [97.8, 1531.0], [97.9, 1532.0], [98.0, 1533.0], [98.1, 1535.0], [98.2, 1536.0], [98.3, 1538.0], [98.4, 1539.0], [98.5, 1542.0], [98.6, 1543.0], [98.7, 1545.0], [98.8, 1548.0], [98.9, 1553.0], [99.0, 1562.0], [99.1, 1710.0], [99.2, 1724.0], [99.3, 1731.0], [99.4, 1736.0], [99.5, 1745.0], [99.6, 1753.0], [99.7, 1930.0], [99.8, 2351.0], [99.9, 3523.0]], "isOverall": false, "label": "HTTP Request", "isController": false}], "supportsControllersDiscrimination": true, "maxX": 100.0, "title": "Response Time Percentiles"}},
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
        data: {"result": {"minY": 1.0, "minX": 0.0, "maxY": 25704.0, "series": [{"data": [[0.0, 1.0], [2300.0, 37.0], [600.0, 100.0], [2500.0, 2.0], [700.0, 26.0], [3100.0, 6.0], [3500.0, 21.0], [3700.0, 13.0], [1200.0, 1.0], [1400.0, 57.0], [1500.0, 1173.0], [400.0, 9510.0], [1600.0, 5.0], [1700.0, 214.0], [1900.0, 47.0], [7700.0, 3.0], [500.0, 25704.0]], "isOverall": false, "label": "HTTP Request", "isController": false}], "supportsControllersDiscrimination": true, "granularity": 100, "maxX": 7700.0, "title": "Response Time Distribution"}},
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
        data: {"result": {"minY": 1.0, "minX": 0.0, "ticks": [[0, "Requests having \nresponse time <= 500ms"], [1, "Requests having \nresponse time > 500ms and <= 1,500ms"], [2, "Requests having \nresponse time > 1,500ms"], [3, "Requests in error"]], "maxY": 24915.0, "series": [{"data": [[0.0, 10491.0]], "color": "#9ACD32", "isOverall": false, "label": "Requests having \nresponse time <= 500ms", "isController": false}, {"data": [[1.0, 24915.0]], "color": "yellow", "isOverall": false, "label": "Requests having \nresponse time > 500ms and <= 1,500ms", "isController": false}, {"data": [[2.0, 1513.0]], "color": "orange", "isOverall": false, "label": "Requests having \nresponse time > 1,500ms", "isController": false}, {"data": [[3.0, 1.0]], "color": "#FF6347", "isOverall": false, "label": "Requests in error", "isController": false}], "supportsControllersDiscrimination": false, "maxX": 3.0, "title": "Synthetic Response Times Distribution"}},
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
        data: {"result": {"minY": 494.93839358363334, "minX": 1.6557288E12, "maxY": 510.0, "series": [{"data": [[1.6557288E12, 510.0], [1.65572886E12, 494.93839358363334]], "isOverall": false, "label": "Thread Group", "isController": false}], "supportsControllersDiscrimination": false, "granularity": 60000, "maxX": 1.65572886E12, "title": "Active Threads Over Time"}},
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
        data: {"result": {"minY": 474.0, "minX": 1.0, "maxY": 1530.0, "series": [{"data": [[3.0, 513.0], [5.0, 507.5], [6.0, 501.0], [8.0, 514.5], [10.0, 510.0], [12.0, 846.3333333333333], [14.0, 507.0], [15.0, 507.0], [17.0, 511.0], [18.0, 518.0], [20.0, 493.0], [21.0, 498.0], [24.0, 508.0], [25.0, 496.0], [26.0, 511.0], [27.0, 511.0], [28.0, 489.0], [29.0, 503.0], [30.0, 1530.0], [31.0, 534.0], [33.0, 501.0], [32.0, 482.0], [34.0, 501.0], [37.0, 509.0], [36.0, 1011.0], [39.0, 502.0], [38.0, 502.0], [41.0, 491.5], [43.0, 1016.0], [44.0, 506.0], [47.0, 840.6666666666667], [49.0, 494.0], [48.0, 499.0], [50.0, 500.0], [53.0, 509.5], [52.0, 506.0], [54.0, 515.0], [56.0, 1008.5], [59.0, 499.5], [58.0, 1497.0], [61.0, 1494.0], [60.0, 541.0], [67.0, 487.0], [66.0, 503.0], [64.0, 494.3333333333333], [71.0, 511.0], [69.0, 500.5], [75.0, 514.0], [74.0, 514.0], [73.0, 507.0], [72.0, 508.0], [79.0, 496.5], [77.0, 499.0], [76.0, 521.0], [83.0, 506.0], [82.0, 520.0], [81.0, 499.5], [87.0, 518.0], [85.0, 527.0], [84.0, 497.0], [89.0, 502.0], [95.0, 500.0], [93.0, 495.75], [99.0, 498.0], [98.0, 497.5], [96.0, 495.0], [103.0, 490.0], [102.0, 500.5], [101.0, 506.0], [106.0, 493.0], [105.0, 497.0], [104.0, 488.0], [111.0, 500.0], [110.0, 497.25], [115.0, 511.0], [114.0, 499.6666666666667], [119.0, 486.0], [118.0, 508.5], [117.0, 498.0], [123.0, 499.0], [122.0, 498.0], [121.0, 505.0], [120.0, 497.0], [127.0, 494.0], [126.0, 504.0], [125.0, 495.5], [135.0, 516.0], [134.0, 508.5], [130.0, 489.0], [129.0, 492.0], [143.0, 513.0], [142.0, 519.0], [141.0, 494.0], [138.0, 493.5], [137.0, 505.0], [150.0, 492.5], [148.0, 493.0], [147.0, 502.5], [145.0, 499.0], [144.0, 505.0], [157.0, 478.0], [156.0, 492.0], [155.0, 501.5], [153.0, 498.0], [152.0, 500.0], [166.0, 512.0], [164.0, 507.0], [163.0, 501.0], [162.0, 482.0], [161.0, 518.0], [160.0, 496.0], [175.0, 507.0], [174.0, 507.0], [173.0, 494.0], [171.0, 502.0], [170.0, 519.0], [183.0, 477.0], [182.0, 487.0], [180.0, 478.0], [179.0, 516.0], [178.0, 483.5], [177.0, 516.0], [176.0, 516.0], [191.0, 508.5], [189.0, 511.6666666666667], [187.0, 474.0], [185.0, 501.0], [184.0, 508.0], [199.0, 497.0], [198.0, 495.5], [196.0, 500.0], [195.0, 506.0], [194.0, 509.5], [192.0, 510.0], [207.0, 478.0], [206.0, 496.0], [205.0, 494.0], [203.0, 511.0], [202.0, 500.3333333333333], [215.0, 519.0], [214.0, 510.6666666666667], [212.0, 490.0], [210.0, 519.0], [208.0, 514.0], [223.0, 509.0], [222.0, 486.0], [221.0, 514.0], [220.0, 500.0], [218.0, 487.0], [217.0, 516.5], [231.0, 495.0], [230.0, 512.0], [229.0, 504.0], [228.0, 521.5], [227.0, 506.0], [225.0, 503.0], [239.0, 509.0], [238.0, 511.0], [237.0, 504.0], [236.0, 1025.5], [234.0, 521.0], [233.0, 488.5], [247.0, 515.0], [246.0, 505.5], [244.0, 489.0], [243.0, 515.0], [242.0, 508.0], [241.0, 503.0], [255.0, 504.5], [254.0, 507.0], [253.0, 499.0], [251.0, 503.0], [249.0, 1522.0], [268.0, 508.0], [270.0, 1506.0], [271.0, 531.0], [269.0, 517.0], [267.0, 492.0], [265.0, 1007.0], [263.0, 499.0], [258.0, 1514.0], [257.0, 1023.0], [262.0, 519.0], [261.0, 509.5], [260.0, 523.0], [286.0, 1001.5], [284.0, 509.0], [275.0, 505.0], [274.0, 512.0], [273.0, 512.0], [283.0, 516.0], [282.0, 1500.0], [281.0, 521.0], [280.0, 1502.0], [279.0, 486.0], [278.0, 507.0], [277.0, 506.0], [276.0, 1505.0], [301.0, 505.0], [302.0, 513.0], [300.0, 517.0], [291.0, 506.5], [289.0, 494.0], [288.0, 505.5], [299.0, 513.0], [298.0, 500.0], [297.0, 514.0], [296.0, 492.0], [294.0, 509.0], [293.0, 505.0], [292.0, 481.0], [318.0, 491.0], [319.0, 506.0], [317.0, 508.0], [316.0, 508.0], [315.0, 502.0], [313.0, 504.0], [312.0, 487.0], [311.0, 486.0], [307.0, 500.5], [305.0, 511.0], [304.0, 510.0], [309.0, 492.0], [334.0, 514.0], [335.0, 519.0], [332.0, 515.0], [331.0, 508.0], [329.0, 515.0], [328.0, 520.0], [326.0, 500.0], [323.0, 535.0], [322.0, 508.0], [321.0, 506.0], [349.0, 518.0], [350.0, 491.0], [348.0, 515.0], [339.0, 510.0], [338.0, 499.0], [337.0, 505.0], [336.0, 495.0], [346.0, 516.0], [345.0, 502.0], [344.0, 507.0], [343.0, 507.0], [342.0, 519.0], [341.0, 503.5], [355.0, 506.0], [365.0, 494.5], [367.0, 508.0], [364.0, 496.0], [362.0, 492.5], [360.0, 510.5], [358.0, 502.0], [357.0, 504.0], [356.0, 497.0], [354.0, 505.0], [353.0, 487.0], [352.0, 504.5], [382.0, 501.5], [383.0, 495.0], [380.0, 496.0], [379.0, 507.5], [377.0, 504.0], [375.0, 506.0], [374.0, 492.0], [372.0, 499.6666666666667], [369.0, 511.0], [368.0, 511.0], [397.0, 498.0], [396.0, 505.0], [395.0, 499.0], [394.0, 504.0], [393.0, 494.0], [392.0, 507.0], [391.0, 501.3333333333333], [390.0, 556.0], [387.0, 517.0], [386.0, 485.0], [385.0, 503.5], [403.0, 526.0], [414.0, 518.0], [413.0, 505.5], [402.0, 506.0], [401.0, 511.0], [400.0, 508.3333333333333], [411.0, 509.0], [409.0, 485.0], [408.0, 510.0], [407.0, 504.0], [406.0, 506.0], [405.0, 488.0], [404.0, 533.0], [430.0, 480.0], [431.0, 507.0], [428.0, 488.0], [419.0, 518.0], [417.0, 502.3333333333333], [427.0, 516.5], [426.0, 510.0], [425.0, 510.0], [423.0, 497.5], [421.0, 511.0], [420.0, 494.0], [445.0, 500.0], [447.0, 519.0], [444.0, 510.5], [442.0, 495.0], [441.0, 490.5], [439.0, 505.0], [438.0, 486.0], [436.0, 509.0], [434.0, 509.0], [432.0, 498.5], [462.0, 523.0], [463.0, 525.0], [461.0, 505.0], [460.0, 506.0], [459.0, 525.0], [458.0, 516.0], [457.0, 537.0], [456.0, 535.0], [455.0, 513.0], [449.0, 515.0], [448.0, 511.0], [451.0, 519.0], [450.0, 519.0], [454.0, 509.0], [453.0, 504.0], [452.0, 501.0], [479.0, 499.0], [476.0, 519.0], [466.0, 514.0], [465.0, 519.0], [464.0, 526.0], [475.0, 518.0], [474.0, 529.0], [473.0, 519.0], [472.0, 527.0], [470.0, 520.0], [469.0, 513.0], [468.0, 495.0], [494.0, 504.0], [495.0, 512.0], [493.0, 512.0], [492.0, 511.5], [491.0, 489.0], [489.0, 510.0], [488.0, 500.0], [486.0, 515.0], [481.0, 508.0], [480.0, 517.0], [485.0, 517.0], [484.0, 522.0], [510.0, 557.9954950005529], [509.0, 514.5454545454545], [506.0, 516.5], [504.0, 501.0], [502.0, 516.0], [501.0, 516.0], [500.0, 516.0], [499.0, 498.0], [497.0, 485.0], [496.0, 487.0], [1.0, 519.0]], "isOverall": false, "label": "HTTP Request", "isController": false}, {"data": [[506.49038461538555, 557.7779794149552]], "isOverall": false, "label": "HTTP Request-Aggregated", "isController": false}], "supportsControllersDiscrimination": true, "maxX": 510.0, "title": "Time VS Threads"}},
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
        data : {"result": {"minY": 16917.266666666666, "minX": 1.6557288E12, "maxY": 162822.75, "series": [{"data": [[1.6557288E12, 162822.75], [1.65572886E12, 49496.01666666667]], "isOverall": false, "label": "Bytes received per second", "isController": false}, {"data": [[1.6557288E12, 55690.1], [1.65572886E12, 16917.266666666666]], "isOverall": false, "label": "Bytes sent per second", "isController": false}], "supportsControllersDiscrimination": false, "granularity": 60000, "maxX": 1.65572886E12, "title": "Bytes Throughput Over Time"}},
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
        data: {"result": {"minY": 548.6712774613494, "minX": 1.6557288E12, "maxY": 560.5446904686248, "series": [{"data": [[1.6557288E12, 560.5446904686248], [1.65572886E12, 548.6712774613494]], "isOverall": false, "label": "HTTP Request", "isController": false}], "supportsControllersDiscrimination": true, "granularity": 60000, "maxX": 1.65572886E12, "title": "Response Time Over Time"}},
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
        data: {"result": {"minY": 548.6650005811947, "minX": 1.6557288E12, "maxY": 560.5402761591985, "series": [{"data": [[1.6557288E12, 560.5402761591985], [1.65572886E12, 548.6650005811947]], "isOverall": false, "label": "HTTP Request", "isController": false}], "supportsControllersDiscrimination": true, "granularity": 60000, "maxX": 1.65572886E12, "title": "Latencies Over Time"}},
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
        data: {"result": {"minY": 297.11217017319734, "minX": 1.6557288E12, "maxY": 309.8254405480807, "series": [{"data": [[1.6557288E12, 309.8254405480807], [1.65572886E12, 297.11217017319734]], "isOverall": false, "label": "HTTP Request", "isController": false}], "supportsControllersDiscrimination": true, "granularity": 60000, "maxX": 1.65572886E12, "title": "Connect Time Over Time"}},
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
        data: {"result": {"minY": 461.0, "minX": 1.6557288E12, "maxY": 7765.0, "series": [{"data": [[1.6557288E12, 7765.0], [1.65572886E12, 3557.0]], "isOverall": false, "label": "Max", "isController": false}, {"data": [[1.6557288E12, 529.0], [1.65572886E12, 533.0]], "isOverall": false, "label": "90th percentile", "isController": false}, {"data": [[1.6557288E12, 1540.0], [1.65572886E12, 1537.0]], "isOverall": false, "label": "99th percentile", "isController": false}, {"data": [[1.6557288E12, 540.0], [1.65572886E12, 550.0]], "isOverall": false, "label": "95th percentile", "isController": false}, {"data": [[1.6557288E12, 461.0], [1.65572886E12, 466.0]], "isOverall": false, "label": "Min", "isController": false}, {"data": [[1.6557288E12, 508.0], [1.65572886E12, 510.0]], "isOverall": false, "label": "Median", "isController": false}], "supportsControllersDiscrimination": false, "granularity": 60000, "maxX": 1.65572886E12, "title": "Response Time Percentiles Over Time (successful requests only)"}},
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
    data: {"result": {"minY": 5.0, "minX": 255.0, "maxY": 535.0, "series": [{"data": [[686.0, 514.0], [789.0, 508.0], [815.0, 507.0], [888.0, 507.0], [876.0, 510.5], [884.0, 511.0], [924.0, 507.0], [901.0, 506.0], [919.0, 517.0], [920.0, 505.0], [917.0, 505.0], [916.0, 508.0], [915.0, 511.0], [933.0, 504.0], [948.0, 501.0], [958.0, 506.0], [929.0, 507.0], [938.0, 511.0], [930.0, 513.0], [931.0, 509.0], [955.0, 507.0], [957.0, 509.0], [945.0, 508.0], [944.0, 510.5], [954.0, 510.0], [953.0, 509.0], [969.0, 504.0], [964.0, 505.0], [960.0, 508.5], [961.0, 510.0], [965.0, 509.0], [972.0, 508.0], [983.0, 510.0], [992.0, 507.0], [998.0, 508.0], [993.0, 513.0], [255.0, 505.0], [495.0, 535.0]], "isOverall": false, "label": "Successes", "isController": false}, {"data": [[958.0, 5.0]], "isOverall": false, "label": "Failures", "isController": false}], "supportsControllersDiscrimination": false, "granularity": 1000, "maxX": 998.0, "title": "Response Time Vs Request"}},
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
    data: {"result": {"minY": 0.0, "minX": 255.0, "maxY": 535.0, "series": [{"data": [[686.0, 514.0], [789.0, 508.0], [815.0, 507.0], [888.0, 507.0], [876.0, 510.5], [884.0, 511.0], [924.0, 507.0], [901.0, 506.0], [919.0, 517.0], [920.0, 505.0], [917.0, 505.0], [916.0, 508.0], [915.0, 511.0], [933.0, 504.0], [948.0, 501.0], [958.0, 506.0], [929.0, 507.0], [938.0, 511.0], [930.0, 513.0], [931.0, 509.0], [955.0, 507.0], [957.0, 509.0], [945.0, 508.0], [944.0, 510.5], [954.0, 510.0], [953.0, 509.0], [969.0, 504.0], [964.0, 505.0], [960.0, 508.5], [961.0, 510.0], [965.0, 509.0], [972.0, 508.0], [983.0, 510.0], [992.0, 507.0], [998.0, 508.0], [993.0, 513.0], [255.0, 505.0], [495.0, 535.0]], "isOverall": false, "label": "Successes", "isController": false}, {"data": [[958.0, 0.0]], "isOverall": false, "label": "Failures", "isController": false}], "supportsControllersDiscrimination": false, "granularity": 1000, "maxX": 998.0, "title": "Latencies Vs Request"}},
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
        data: {"result": {"minY": 134.88333333333333, "minX": 1.6557288E12, "maxY": 480.45, "series": [{"data": [[1.6557288E12, 480.45], [1.65572886E12, 134.88333333333333]], "isOverall": false, "label": "hitsPerSecond", "isController": false}], "supportsControllersDiscrimination": false, "granularity": 60000, "maxX": 1.65572886E12, "title": "Hits Per Second"}},
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
        data: {"result": {"minY": 0.016666666666666666, "minX": 1.6557288E12, "maxY": 471.95, "series": [{"data": [[1.6557288E12, 471.95], [1.65572886E12, 143.36666666666667]], "isOverall": false, "label": "200", "isController": false}, {"data": [[1.65572886E12, 0.016666666666666666]], "isOverall": false, "label": "Non HTTP response code: java.net.SocketException", "isController": false}], "supportsControllersDiscrimination": false, "granularity": 60000, "maxX": 1.65572886E12, "title": "Codes Per Second"}},
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
        data: {"result": {"minY": 0.016666666666666666, "minX": 1.6557288E12, "maxY": 471.95, "series": [{"data": [[1.6557288E12, 471.95], [1.65572886E12, 143.36666666666667]], "isOverall": false, "label": "HTTP Request-success", "isController": false}, {"data": [[1.65572886E12, 0.016666666666666666]], "isOverall": false, "label": "HTTP Request-failure", "isController": false}], "supportsControllersDiscrimination": true, "granularity": 60000, "maxX": 1.65572886E12, "title": "Transactions Per Second"}},
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
        data: {"result": {"minY": 0.016666666666666666, "minX": 1.6557288E12, "maxY": 471.95, "series": [{"data": [[1.6557288E12, 471.95], [1.65572886E12, 143.36666666666667]], "isOverall": false, "label": "Transaction-success", "isController": false}, {"data": [[1.65572886E12, 0.016666666666666666]], "isOverall": false, "label": "Transaction-failure", "isController": false}], "supportsControllersDiscrimination": true, "granularity": 60000, "maxX": 1.65572886E12, "title": "Total Transactions Per Second"}},
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


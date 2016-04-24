
$(document).ready(function () {
    // fade in .navbar
    $(function () {
        $(window).scroll(function () {
            // set distance user needs to scroll before we start fadeIn
            if ($(this).scrollTop() > 100) {
                $('.navbar').fadeOut();
            } else {
                $('.navbar').fadeIn();
            }
        });
    });

    imagesProgress();

    // 顯示圖片載入狀況的進度
    function imagesProgress() {

        var $container = $('#progress'),                    // 1
            $progressBar = $container.find('.progress-bar'),  // 2
            $progressText = $container.find('.progress-text'), // 3
                    //1.進度顯示的整體container
                        //2.進度顯示的bar部分
                        //3.進度顯示的text部分

                        //利用imagesLoaded函式庫監控body元素內圖片載入狀況
                        //同時紀錄body內全部圖片的數量

           // imgLoad       = imagesLoaded('body'),
           // imgTotal      = imgLoad.images.length,

                        //已載入圖片之計數器
                        //和進度顯示的當前數值(最初為0)
            imgLoaded = 0,
            current = 0,

            //1秒60次檢查讀取狀況
            progressTimer = setInterval(updateProgress, 1000 / 60);

        // 利用imagesLoaded，加總每個已載入的圖片，紀錄其個數
        /* imgLoad.on('progress', function () {
             imgLoaded++;
         });*/

        //依照圖片載入狀況更新進度顯示
        //此函數因setInterval()方法每秒呼叫60次
        function updateProgress() {

            // 載入圖片後計算完成的百分比
            var target = (100 / 100) * 100;

            // 以current(當下位置)和target(目的地)的距離計算easing
            current += 5;

            // 顯示bar的寬度與text反應current的值
            // 去除字串小數點的部分取整數
            $progressBar.css({ width: current + '%' });
            $progressText.text(Math.floor(current) + '%');

            // 結束處理
            if (current >= 100) {
                // 停止進度顯示的更新
                clearInterval(progressTimer);
                // 附加完成的CSS樣式
                $container.addClass('progress-complete');

                //同時處理進度顯示bar和字串的動畫
                //將其群組化為1個jQuery物件
                $progressBar.add($progressText)
                    // 等待0.5秒
                    .delay(0)

                    //執行0.25秒的動畫，將進度顯示bar和字串變成透明
                    .animate({ opacity: 0 }, 100, function () {

                        //執行1秒的動畫，將overlap往上方slide out
                        $container.animate({ top: '-200%' }, 500, 'easeInOutQuint');
                        clearInterval(updateProgress);
                    });
            }
            // 若current大於99.9，則視為100並結束處理
            if (current > 99.9) {
                current = 100;
            }
        }
    }
});

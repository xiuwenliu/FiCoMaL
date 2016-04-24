//如果使用者按下取消  復原原本的項目
var cancelData = {};
$(document).on('click', '.modifyBtn',   
    function () {
        // alert(this.id);
        $(this).text("送出").attr('class', 'btn btn-warning modifySubmitBtn');
        $('#' + this.id + '.delBtn').text("取消").attr('class', 'btn btn-warning cancelBtn');
        $('#' + $(this).attr('id') + ' > td').each(
            function () {
                cancelData[$(this).attr('id')] = $(this).text();
                //  alert();
                if ($(this).attr('id') != 'functionGroup') {
                    if ($(this).attr('id') == 'Coach') {

                        var coachListString = '';
                        @foreach (var modelItem in ViewBag.Coachs)
                        {
                            @:coachListString+='<li><a href="#">@modelItem.CoachName</a></li>';
                            // @:coachList.push("@modelItem");
                        }
                        //alert(coachListString);
        
                        $(this).html('<div class="form-group">' +
                        '<div class="dropdown">' +
                            '<button class="btn btn-default dropdown-toggle btn-select2" type="button" data-toggle="dropdown" id="Coach">' +
                            '選教練' +
                                '<span class="caret"></span>' +
                            '</button>' +
                            '<ul class="dropdown-menu">' +
                            coachListString+
                                '</ul>' +
                            '</div>' +
                        '</div>');
                    }
                    else if ($(this).attr('id') == 'StartDate') {
                        $(this).html('<div class="input-group date" id="startDateTimepicker">' +
                                  '<input type="text" class="form-control" data-date-format="" id="StartDate" />' +
                                '<span class="input-group-addon">' +
                                  '<span class="glyphicon glyphicon-calendar"></span>' +
                            '</span>' +
                       '</div>');
                    }
                    else
                        $(this).html('<input type="text" class="form-control input-sm" name="" value="' + $(this).text() + '">');
                }
            }
            );//
    });

$(document).on('click', '.cancelBtn',
    function () {
        $('#' + $(this).attr('id') + ' > td').each(
           function () {
               //  cancelData[$(this).attr('id')] = $(this).text();
               if ($(this).attr('id') != 'functionGroup')
                   $(this).text(cancelData[$(this).attr('id')]);
           }
           );//
        $('#' + this.id + '.cancelBtn').text("刪除").attr('class', 'btn btn-danger delBtn');
        $('#' + this.id + '.modifySubmitBtn').text("修改").attr('class', 'btn btn-info modifyBtn');
    });


$(document).on('click', '.delBtn',
    function () {
        var id = $(this).attr('id');
        delReservation(id);
    });


$(document).on('click', '.modifySubmitBtn',
    function () {
        var data = {};
        data['ReservationId'] = $(this).attr('id');
        $(this).text("修改").attr('class', 'btn btn-info modifyBtn');

        $('#' + $(this).attr('id') + ' > td').each(
            function () {             
                if ($(this).attr('id') != 'functionGroup') {
                    //    alert($(this).attr('id'));
                    data[$(this).attr('id')] = $(this).children().val();
                    //$(this).children().fadeOut(500).delay(8000);
                    $(this).html($(this).children().val());
                }
            }
            );//

        $.ajax({
            url: "@Url.Content("~/Home/ModifyReservation")",
            type: "post",
        async: false,
        data: data,
        success: function (result) {
            alert(result['State']);
            //  $(".table").html(result);
        },
        error: function (xhr) {
            //顯示錯誤訊息
            alert(xhr.responseText);
        }
    });

});
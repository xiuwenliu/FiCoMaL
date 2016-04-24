$(document).ready(function () {


    //modal隱藏之後   必須把值清空
    $('#checkCustomerModal,#createCustomerModal,#newReservationModal').on('hidden.bs.modal', function (e) {
        $(this)
          .find("input[type=text],textarea,select")
             .val('')
             .end();
          /*.find("input[type=checkbox], input[type=radio]")
             .prop("checked", "")
             .end();*/
    })
    

    function setDropDownTextAndID(obj)
    {
        var selText = obj.text();
        obj.parents('.dropdown').find('.dropdown-toggle').attr('id', obj.attr('id')).html(selText + '<span class="caret"></span>');
    }

    //控制dropdown
    $(".dropdown-menu li a[name=Horses]").click(function () {
        setDropDownTextAndID($(this));
    });

    //控制dropdown
    $(".dropdown-menu li a[name=Coachs]").click(function () {
        setDropDownTextAndID($(this));
    });
/*
    //初始化DateTimePicker
    $("#startDateTimepicker").datetimepicker({
        stepping: 30,
        sideBySide: true
    });
    //初始化DateTimePicker
    $("#endDatetimepicker").datetimepicker({
        stepping: 30,
        sideBySide: true
    });
    //生日DateTimePicker
    $("#birthdayDateTimepicker").datetimepicker({        
        stepping: 30,
        sideBySide: true
        
    });*/

});

function ShowNonValidationLabel(res) {
    $("label[id='tipLabel']").text('');
    $.each(res, function () {
        console.log(this.Key + '=' + this.Value);
        $("label[for='" + this.Key + "']").text(this.Value).css('color', '#ff3333');
    });
}
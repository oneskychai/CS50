/* Functions to enable and disable buttons if input fields have been entered or not */

function enableButton()
{
    $('#goBtn').prop('disabled',false);
}

function disableButton()
{
    $('#goBtn').prop('disabled',true);
}

function checkFields(a)
{
    $(a).bind('keydown keyup change mouseup', function() {
        if(allFilled()) enableButton();
        if(allFilled() == false) disableButton();
    });
}

/* https://stackoverflow.com/questions/5614399/disabling-submit-button-until-all-fields-have-values */
function allFilled()
{
    var filled = true;
    $('body input').each(function() {
        if($(this).val() == '') filled = false;
    });
    return filled;
}

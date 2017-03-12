$(function() {

    $("text").html("testing");

    var link = $('a').attr('href');
    link = link.substring(0,25).concat('...');


    $('a').html(link);


});
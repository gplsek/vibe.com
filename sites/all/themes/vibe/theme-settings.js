$(document).ready( function() {
  // Hide the breadcrumb details, if no breadcrumb.
  $('#edit-vibe-breadcrumb').change(
    function() {
      div = $('#div-vibe-breadcrumb-collapse');
      if ($('#edit-vibe-breadcrumb').val() == 'no') {
        div.slideUp('slow');
      } else if (div.css('display') == 'none') {
        div.slideDown('slow');
      }
    }
  );
  if ($('#edit-vibe-breadcrumb').val() == 'no') {
    $('#div-vibe-breadcrumb-collapse').css('display', 'none');
  }
  $('#edit-vibe-breadcrumb-title').change(
    function() {
      checkbox = $('#edit-vibe-breadcrumb-trailing');
      if ($('#edit-vibe-breadcrumb-title').attr('checked')) {
        checkbox.attr('disabled', 'disabled');
      } else {
        checkbox.removeAttr('disabled');
      }
    }
  );
  $('#edit-vibe-breadcrumb-title').change();
} );

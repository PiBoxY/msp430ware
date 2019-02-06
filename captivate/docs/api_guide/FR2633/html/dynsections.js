/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
function toggleVisibility(linkObj)
{
 var base = $(linkObj).attr('id');
 var summary = $('#'+base+'-summary');
 var content = $('#'+base+'-content');
 var trigger = $('#'+base+'-trigger');
 var src=$(trigger).attr('src');
 if (content.is(':visible')===true) {
   content.hide();
   summary.show();
   $(linkObj).addClass('closed').removeClass('opened');
   $(trigger).attr('src',src.substring(0,src.length-8)+'closed.png');
 } else {
   content.show();
   summary.hide();
   $(linkObj).removeClass('closed').addClass('opened');
   $(trigger).attr('src',src.substring(0,src.length-10)+'open.png');
 } 
 return false;
}

function updateStripes()
{
  $('table.directory tr').
       removeClass('even').filter(':visible:even').addClass('even');
}

function toggleLevel(level)
{
  $('table.directory tr').each(function() {
    var l = this.id.split('_').length-1;
    var i = $('#img'+this.id.substring(3));
    var a = $('#arr'+this.id.substring(3));
    if (l<level+1) {
      i.removeClass('iconfopen iconfclosed').addClass('iconfopen');
      a.html('&#9660;');
      $(this).show();
    } else if (l==level+1) {
      i.removeClass('iconfclosed iconfopen').addClass('iconfclosed');
      a.html('&#9658;');
      $(this).show();
    } else {
      $(this).hide();
    }
  });
  updateStripes();
}

function toggleFolder(id)
{
  // the clicked row
  var currentRow = $('#row_'+id);

  // all rows after the clicked row
  var rows = currentRow.nextAll("tr");

  var re = new RegExp('^row_'+id+'\\d+_$', "i"); //only one sub

  // only match elements AFTER this one (can't hide elements before)
  var childRows = rows.filter(function() { return this.id.match(re); });

  // first row is visible we are HIDING
  if (childRows.filter(':first').is(':visible')===true) {
    // replace down arrow by right arrow for current row
    var currentRowSpans = currentRow.find("span");
    currentRowSpans.filter(".iconfopen").removeClass("iconfopen").addClass("iconfclosed");
    currentRowSpans.filter(".arrow").html('&#9658;');
    rows.filter("[id^=row_"+id+"]").hide(); // hide all children
  } else { // we are SHOWING
    // replace right arrow by down arrow for current row
    var currentRowSpans = currentRow.find("span");
    currentRowSpans.filter(".iconfclosed").removeClass("iconfclosed").addClass("iconfopen");
    currentRowSpans.filter(".arrow").html('&#9660;');
    // replace down arrows by right arrows for child rows
    var childRowsSpans = childRows.find("span");
    childRowsSpans.filter(".iconfopen").removeClass("iconfopen").addClass("iconfclosed");
    childRowsSpans.filter(".arrow").html('&#9658;');
    childRows.show(); //show all children
  }
  updateStripes();
}


function toggleInherit(id)
{
  var rows = $('tr.inherit.'+id);
  var img = $('tr.inherit_header.'+id+' img');
  var src = $(img).attr('src');
  if (rows.filter(':first').is(':visible')===true) {
    rows.css('display','none');
    $(img).attr('src',src.substring(0,src.length-8)+'closed.png');
  } else {
    rows.css('display','table-row'); // using show() causes jump in firefox
    $(img).attr('src',src.substring(0,src.length-10)+'open.png');
  }
}


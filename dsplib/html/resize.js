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
var cookie_namespace = 'doxygen'; 
var sidenav,navtree,content,header;

function readCookie(cookie) 
{
  var myCookie = cookie_namespace+"_"+cookie+"=";
  if (document.cookie) 
  {
    var index = document.cookie.indexOf(myCookie);
    if (index != -1) 
    {
      var valStart = index + myCookie.length;
      var valEnd = document.cookie.indexOf(";", valStart);
      if (valEnd == -1) 
      {
        valEnd = document.cookie.length;
      }
      var val = document.cookie.substring(valStart, valEnd);
      return val;
    }
  }
  return 0;
}

function writeCookie(cookie, val, expiration) 
{
  if (val==undefined) return;
  if (expiration == null) 
  {
    var date = new Date();
    date.setTime(date.getTime()+(10*365*24*60*60*1000)); // default expiration is one week
    expiration = date.toGMTString();
  }
  document.cookie = cookie_namespace + "_" + cookie + "=" + val + "; expires=" + expiration+"; path=/";
}
 
function resizeWidth() 
{
  var windowWidth = $(window).width() + "px";
  var sidenavWidth = $(sidenav).outerWidth();
  content.css({marginLeft:parseInt(sidenavWidth)+"px"}); 
  writeCookie('width',sidenavWidth, null);
}

function restoreWidth(navWidth)
{
  var windowWidth = $(window).width() + "px";
  content.css({marginLeft:parseInt(navWidth)+6+"px"});
  sidenav.css({width:navWidth + "px"});
}

function resizeHeight() 
{
  var headerHeight = header.outerHeight();
  var footerHeight = footer.outerHeight();
  var windowHeight = $(window).height() - headerHeight - footerHeight;
  content.css({height:windowHeight + "px"});
  navtree.css({height:windowHeight + "px"});
  sidenav.css({height:windowHeight + "px",top: headerHeight+"px"});
}

function initResizable()
{
  header  = $("#top");
  sidenav = $("#side-nav");
  content = $("#doc-content");
  navtree = $("#nav-tree");
  footer  = $("#nav-path");
  $(".side-nav-resizable").resizable({resize: function(e, ui) { resizeWidth(); } });
  $(window).resize(function() { resizeHeight(); });
  var width = readCookie('width');
  if (width) { restoreWidth(width); } else { resizeWidth(); }
  resizeHeight();
  var url = location.href;
  var i=url.indexOf("#");
  if (i>=0) window.location.hash=url.substr(i);
  var _preventDefault = function(evt) { evt.preventDefault(); };
  $("#splitbar").bind("dragstart", _preventDefault).bind("selectstart", _preventDefault);
  $(document).bind('touchmove',function(e){
    var device = navigator.userAgent.toLowerCase();
    var ios = device.match(/(iphone|ipod|ipad)/);
    if (ios) {
      try {
        var target = e.target;
        while (target) {
          if ($(target).css('-webkit-overflow-scrolling')=='touch') return;
          target = target.parentNode;
        }
        e.preventDefault();
      } catch(err) {
        e.preventDefault();
      }
    }
  });
}



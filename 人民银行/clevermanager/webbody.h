#pragma once

#define MAINBODY       "<html>\
<head>\
<meta http-equiv='Content-Type' content='text/html; charset=gb2312' />\
<link rel='stylesheet' type='text/css' href=main.css />\
<title>clever</title>\
<script language='JavaScript' type='text/JavaScript'>\
function clk(page)\
{\
if(page==1)\
{\
document.getElementById('frm').src = 'tuopu.html';\
}\
else if(page==2)\
{\
document.getElementById('frm').src = 'infostatus.html';\
}\
else if(page==3)\
{\
document.getElementById('frm').src = 'env.html';\
}\
else if(page==4)\
{\
document.getElementById('frm').src = 'list.html';\
}\
else if(page==5)\
{\
document.getElementById('frm').src = 'logs.html';\
}\
}\
window.onresize = function(){\
follow();\
};\
function follow(){\
document.getElementById('body_div').height = document.body.scrollHeight*99/100-120;\
document.getElementById('frm').height = document.body.scrollHeight*99/100-120;\
}\
</script>\
<script language='JavaScript' type='text/JavaScript'>\
<!--\
function MM_reloadPage(init) {  //reloads the window if Nav4 resized\
if (init==true) with (navigator) {if ((appName=='Netscape')&&(parseInt(appVersion)==4)) {\
document.MM_pgW=innerWidth; document.MM_pgH=innerHeight; onresize=MM_reloadPage; }}\
else if (innerWidth!=document.MM_pgW || innerHeight!=document.MM_pgH) location.reload();\
}\
MM_reloadPage(true);\
//-->\
</script>\
</head>\
<body style='margin:0px;' onload='follow();'>\
<div id='main_div' style='position:absolute; width:100%; height:100%; z-index:1'  align='center'>\
<div id='main1_div'>\
<div style='position:relative; width:992px; height:60px; z-index:2;border:1px solid #aaaaFF; left:-1px; top:-1px; background-color:#E9F0F9' align='left'>\
<img src='img/logo.gif' width='32' height='32' style='margin:10px;'><div style='position:absolute;left:50px;top: 20px;'><span style=' font-size:18'>CLEVER Manager</span></div>\
</div>\
<div id='main_div' style='position:relative; width:992px; height:30px; z-index:3; border:1px solid #aaaaFF;left:-1px;top: -2px;'>\
<div id='nav' align='left'>\
<ul>\
<li><a href='#' onClick='clk(1);'>机房拓扑</a></li>\
<li><a href='#' onClick='clk(2);'>统计信息</a></li>\
<li><a href='#' onClick='clk(3);'>环境状态</a></li>\
<li><a href='#' onClick='clk(4);'>NPM设备列表</a></li>\
<li><a href='#' onClick='clk(5);'>日志查看</a></li>\
</ul>\
</div>\
</div>\
<div name='body_div' id='body_div' style='position:relative; width:992px; z-index:3; border:1px solid #aaaaFF;left:-1px;top: -3px;'>\
<iframe name='frm' id='frm' src='tuopu.html' width='100%' height='600px' frameborder='0'></iframe>\
<div id='Layer1' style='position:absolute; width:280px; height:22px; z-index:4; left:700px; bottom:-28px; color:#5555aa;'>Shenzhen CLEVER Electronic Co., Ltd.</div>\
</div>\
</div>\
</div>\
</body>\
</html>"
/////////////////////////////////////////////////
#define TUOPUHEAD				"<html>\
<head>\
<meta http-equiv='Content-Type' content='text/html; charset=gb2312' />\
<title>clever</title>\
<link rel='stylesheet' type='text/css' href=main.css />\
<link rel='stylesheet' type='text/css' href='themes/default/easyui.css'>\
<link rel='stylesheet' type='text/css' href='themes/icon.css'>\
<link rel='stylesheet' type='text/css' href='demo.css'>\
<script type='text/javascript' src='jquery-1.6.min.js'></script>\
<script type='text/javascript' src='jquery.easyui.min.js'></script>\
<script language='JavaScript' type='text/JavaScript' src='js/main.js'></script>\
<script language='JavaScript' type='text/JavaScript'>\
function test(obj){\
var url;\
url = 'cabin.html=';\
url += document.getElementById('jifang').innerHTML;\
url += '=';\
url += obj.id;\
url += '=';\
url += obj.name;\
document.getElementById('cabinfrm').src = url;\
$('#test').window('open');\
		}\
</script>\
</head>\
<body style='margin:0px;'>\
<div id='Layer1' style='position:absolute; border:1px; width:920px; height:670px; z-index:1; left:35px; top:0px;'>\
        <div id='Layer4' style='position:absolute; width:920px; height:30px; z-index:2; top:12px;'>\
            <table width='100%' border='0'>\
                  <tr>"
#define TUOPUTITLE                    "<td width=100 align='left'><a href='tuopu.html=%s=pre'>前一页</a></td>\
                    <td id='jifang' align='center'>%s</td>\
                    <td width=100 align='right'><a href='tuopu.html=%s=back'>后一页</a></td>"
#define TUOPUBODY                   "</tr>\
            </table>\
        </div>\
        <div id='Layer2' style='position:absolute; border:2px solid #aaaaFF; width:910px; height:610px; z-index:3; left:5px; top:40px'>\
       <div id='Layer3' style='position:absolute; border:1px solid #aaaaFF; width:900px; height:600px; z-index:4;left:4px; top:4px'>"
#define TUOPUCABIN                           "<a id=%d name=%d title='%d:%d' href='#' onClick='test(this);'><div class='icon' style='background-color:#%s; left:%dpx; top:%dpx;'>\
						   <img src='img/canbin.png' height='28' width='45' onload='fixPNG(this)'>\
						   </div>"
#define TUOPUEND						   "</a>\
         </div>\
        </div>\
		<div id='test' class='easyui-window' minimizable='false' closed='true' modal='flase' title='设备工作状态' style='width:500px;height:400px;overflow:hidden;'>\
			<iframe id='cabinfrm' width='100%' height=100% frameborder='0'></iframe>\
		</div>\
</div>\
</body>\
</html>"
///////////////////////////////////////
#define CABINHEAD          "<html>\
<head>\
<meta http-equiv='Content-Type' content='text/html; charset=gb2312' />\
<title>clever</title>\
<link rel='stylesheet' type='text/css' href='themes/default/easyui.css'>\
<link rel='stylesheet' type='text/css' href='themes/icon.css'>\
<link rel='stylesheet' type='text/css' href='demo.css'>\
<script type='text/javascript' src='jquery-1.6.min.js'></script>\
<script type='text/javascript' src='jquery.easyui.min.js'></script>\
<script language='JavaScript' type='text/JavaScript' src='js/main.js'></script>\
<script language='JavaScript' type='text/JavaScript'>\
$(function(){\
			$('#tt').datagrid({\
				title: 'NPM工作信息',\
				width: 'auto',\
				height: 'auto',\
				fit:true,\
				fitColumns: true,\
				columns:[[\
					{field:'output',title:'项目',width:80},\
					{field:'value',title:'内容',width:100},\
					{field:'value1',title:'描述',width:100}\
				]]\
			});\
			var users =  {\"total\":0,\"rows\":["
#define CABINBODY     "{'output':'%s','value':'%s','value1':'<span style=\"color:#%s;\">%s</span>'}"
#define CABINEND              "]};\
			$('#tt').datagrid('loadData',users);\
		});\
</script>\
</head>\
<body style='margin-left:0px; margin-top:0px;'>\
<table id='tt'></table>\
</body>\
</html>"
/////////////////////////////////////////////
#define ENVHEAD       "<html>\
<head>\
<title>clever</title>\
<link rel='stylesheet' type='text/css' href=main.css />\
<link rel='stylesheet' type='text/css' href='themes/default/easyui.css'>\
<link rel='stylesheet' type='text/css' href='themes/icon.css'>\
<link rel='stylesheet' type='text/css' href='demo.css'>\
<script type='text/javascript' src='jquery-1.6.min.js'></script>\
<script language='JavaScript' type='text/JavaScript' src='js/main.js'></script>\
<script language='JavaScript' type='text/JavaScript'>\
<!--\
function MM_reloadPage(init) {  //reloads the window if Nav4 resized\
  if (init==true) with (navigator) {if ((appName=='Netscape')&&(parseInt(appVersion)==4)) {\
    document.MM_pgW=innerWidth; document.MM_pgH=innerHeight; onresize=MM_reloadPage; }}\
  else if (innerWidth!=document.MM_pgW || innerHeight!=document.MM_pgH) location.reload();\
}\
MM_reloadPage(true);\
//-->\
</script>\
</head>\
<body style='margin:0px;'>\
<div id='Layer1' style='position:absolute; border:1px; width:920px; height:670px; z-index:1; left:35px; top:0px;'>\
        <div id='Layer4' style='position:absolute; width:920px; height:30px; z-index:2; top:12px;'>\
            <table width='100%' border='0'>\
                  <tr>"
#define ENVTITLE                     "<td width=100 align='left'><a href='env.html=%s=pre'>前一页</a></td>\
                    <td align='center'><span style='color:#5555aa'>%s-温度分布图</span></td>\
                    <td width=100 align='right'><a href='env.html=%s=back'>后一页</a></td>"
#define ENVBODY                  "</tr>\
            </table>\
        </div>\
        <div id='Layer2' style='position:absolute; border:2px solid #aaaaFF; width:910px; height:610px; z-index:3; left:5px; top:40px'>\
       <div id='Layer3' style='position:absolute; border:1px solid #aaaaFF; width:900px; height:600px; z-index:4;left:4px; top:4px'>"
#define ENVICON                           "<div title='%d ℃' class='icon' style='background-color:#%s; left:%dpx; top:%dpx;'>\
						   <img src='img/canbin.png' height='29' width='45' onload='fixPNG(this)'>\
						   </div>"
#define ENVEND						   "<div id='test' style='position:relative; width:45px;height:28px; border:1px solid #aaaaFF; background-color:#ffffff; display:none; text-align:center'></div>\
         </div>\
        </div>\
</div>\
</body>\
</html>"

//////////////////////////////////////////////
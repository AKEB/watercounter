var xmlHttp=createXmlHttpObject();
var set_real_time;

function createXmlHttpObject(){
	if(window.XMLHttpRequest){
		xmlHttp=new XMLHttpRequest();
	}else{
		xmlHttp=new ActiveXObject('Microsoft.XMLHTTP');
	}
	return xmlHttp;
}

function load(){
	console.log('load()');
	if(xmlHttp.readyState==0 || xmlHttp.readyState==4){
		xmlHttp.open('GET','\configs.json',true);
		xmlHttp.send(null);
		xmlHttp.onload = function(e) {
			jsonResponse=JSON.parse(xmlHttp.responseText);
			loadBlock();
		}
	}
}

function loadBlock(data2) {
	console.log('loadBlock()');
	data2 = JSON.parse(xmlHttp.responseText);
	data = document.getElementsByTagName('body')[0].innerHTML;
	var new_string;
	for (var key in data2) {
		new_string = data.replace(new RegExp('{{'+key+'}}', 'g'), data2[key]);
		data = new_string;
	}
	document.getElementsByTagName('body')[0].innerHTML = new_string;
	handleServerResponse();
}

function val(id){
	var v = document.getElementById(id).value;
	return v;
}

function setValue(id, html) {
	console.log('Set value for '+id);
	document.getElementById(id).innerHTML = html;
	return true;
}

function send_request(submit,server){
	request = new XMLHttpRequest();
	request.open("GET", server, true);
	request.send();
	save_status(submit,request);
}

function save_status(submit,request){
	old_submit = submit.value;
	request.onreadystatechange = function() {
		if (request.readyState != 4) return;
		submit.value = request.responseText;
		setTimeout(function(){
			submit.value=old_submit;
			submit_disabled(false);
		}, 1000);
	}
	submit.value = 'Подождите...';
	submit_disabled(true);
}

function submit_disabled(request){
	var inputs = document.getElementsByTagName("input");
	for (var i = 0; i < inputs.length; i++) {
		if (inputs[i].type === 'submit') {inputs[i].disabled = request;}
	}
}

function toggle(target) {
	var curVal = document.getElementById(target).className;
	document.getElementById(target).className = (curVal === 'hidden') ? 'show' : 'hidden';
}

function real_time(hours,min,sec) {
	sec=Number(sec)+1;
	if (sec>=60) { min=Number(min)+1; sec=0; };
	if (min>=60) { hours=Number(hours)+1; min=0; };
	if (hours>=24) { hours=0 };
	document.getElementById("time").innerHTML = hours+":"+min+":"+sec;
	set_real_time = setTimeout("real_time("+hours+","+min+","+sec+");", 1000);
}

function restart(submit,texts){
	if (confirm(texts)) {
		server = "/restart?device=ok";
		send_request(submit,server);
		return true;
	} else {
		return false;
	}
}
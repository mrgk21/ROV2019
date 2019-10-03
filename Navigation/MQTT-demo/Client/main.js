var socket = io();
var msg;
var thresh=0.1;


var haveEvents = 'ongamepadconnected' in window;
var controllers = {};
var prevAxes=[];
var prev_btns=[];


var interval;

var ServerDetectedJoy = [];

function connecthandler(e) {
	console.log("gamepad connection");
  	addgamepad(e.gamepad);
}

function addgamepad(gamepad) {
	controllers[gamepad.index] = gamepad;
	ServerDetectedJoy[gamepad.index] = {id:gamepad.id,status:false};
  	$("#controller1").append("<h1>Gamepad connected at index "+gamepad.index +": "+gamepad.id+". "+gamepad.buttons.length+" buttons, "+gamepad.axes.length+" axes.</h1><br><div id='gp1_axes'></div>");
  	interval = setInterval(updateStatus,1000/60);
}

function disconnecthandler(e) {
  removegamepad(e.gamepad);
}

function removegamepad(gamepad) {
	clearInterval(interval);
	var d = document.getElementById("controller1");
	msg = {buttons:[],axes:[]};
	msg = JSON.stringify(msg);
	console.log(msg);
  	socket.emit('removeGamepad',gamepad.id);
  	$("#controller1").empty();
  	delete controllers[gamepad.index];
}


function updateStatus() 
{
	//console.log(interval);
	if (!haveEvents) {
		scangamepads();
	}

	var i = 0;
	var j;

	for (j in controllers) {
		if(ServerDetectedJoy.length != 0)
		{
			if(!ServerDetectedJoy[j].status)
			{
				socket.emit('newGamepad',ServerDetectedJoy[j].id);
			}
		}
		var count=0;
	    var controller = controllers[j];
	    var buttons= [];

		var axes = [];
		if(window.navigator.platform === "Linux x86_64")
			axes=[controller.axes[0].toFixed(2),-controller.axes[1].toFixed(2),controller.axes[2].toFixed(2),-parseInt(controller.axes[3].toFixed(2))];
		else
			axes = [controller.axes[0].toFixed(2),-controller.axes[1].toFixed(2),controller.axes[5].toFixed(2),-parseInt(controller.axes[6].toFixed(2))];

	    $("#gp1_axes").empty();
	    for(i in axes)
	    {
			$("#gp1_axes").append("<span> axes["+i+"]: "+ axes[i]+"</span><br>");
	    }
	    for(i in controller.buttons)
	    {
	    	buttons.push(controller.buttons[i].value);
	    }
	    if(prev_btns.length==0)
	    {
	    	for(i in buttons)
			{
				prev_btns.push(buttons[i]);
		  	}	
	    }
	    else
	    {
	    	for(i in prev_btns)
	    	{
	    		if(prev_btns[i] != buttons[i])
	    		{
	    			count++;
	    			prev_btns[i] = buttons[i];
	    		}
	    	}
	    }

	    if(prevAxes.length == 0)
		{
			for(i in axes)
			{
				prevAxes.push(axes[i]);
		  	}
		}
		else
		{
			var msg_axes=[];
			
		  	for(j in prevAxes)
		  	{
		  		if(Math.abs(prevAxes[j]-axes[j])> thresh)
		  		{
		  			msg_axes.push(axes[j]);
		  			count++;
		  		}
		  		else
		  		{
		  			msg_axes.push(prevAxes[j]);	
		  		}
		  	}
		  	for(j in msg_axes)
		  	{
		  		prevAxes[j] = msg_axes[j];
		  	}
		}
		if(count>0)
		{
			msg = {buttons: buttons,axes: msg_axes};
		  	//msg = JSON.stringify(msg);
		  	//console.log(msg);
		  	socket.emit('message',msg);
		}
	}
//requestAnimationFrame(updateStatus);
}

function scangamepads() {
  var gamepads = navigator.getGamepads ? navigator.getGamepads() : (navigator.webkitGetGamepads ? navigator.webkitGetGamepads() : []);
  for (var i = 0; i < gamepads.length; i++) {
    if (gamepads[i]) {
      if (gamepads[i].index in controllers) {
        controllers[gamepads[i].index] = gamepads[i];
      } else {
        addgamepad(gamepads[i]);
      }
    }
  }
}


window.addEventListener("gamepadconnected", connecthandler);
window.addEventListener("gamepaddisconnected", disconnecthandler);

if (!haveEvents) {
  setInterval(scangamepads, 500);
}

socket.on('ServerDetectedJoy',function(data)
{
	var c = ServerDetectedJoy.find(function(value){
		return value.id == data;
	});
	console.log(c);
	c.status = true;
});

socket.on('serverOnline',function(){
	console.log("serverOnline!");
	for(j in ServerDetectedJoy)
	{
		ServerDetectedJoy[j].status = false;
	}
});

var mqtt = require('mqtt');
var express = require('express');
var app = express();
app.get('/',function(req,res){
	res.sendFile(__dirname + '/Client/index.html');
});
app.use('/Client',express.static(__dirname + '/Client'));

var server = app.listen(8000,'localhost');
var io = require('socket.io').listen(server);
var client  = mqtt.connect('mqtt://192.168.43.128');

client.on('connect', function () {
    client.subscribe('navFeed', function (err) {
      if (!err) 
      {
      	console.log('subscribed to navFeed');
      }
    });

    client.subscribe('link4D', function (err) {
      if (!err) 
      {
        console.log('subscribed to link4D');
      }
    });

    client.subscribe('link6D', function (err) {
      if (!err)
      {
        console.log('subscribed to link6D');
      }
    });
});

client.on('message', function (topic, message) {
// message is Buffer
    console.log("from mqtt: ");
    console.log(message.toString());
});


var controllers=[];

// Change in data check
var prev_velocities = undefined;
var prev_arm4 = undefined;
var prev_arm6 = undefined;

//DATA elements
var velocities={x:0,y:0,z:0};
var link4D = {'1':0,'2':0,'3':0,'4':0};
var link6D = {'1':0,'2':0,'3':0,'4':0,'5':0,'6':0};

//Link counters
var curr_l4=0;
var curr_l6=0;

var changeLink = false;
var buttonReleased = true;
var toggleButtonReleased = true;

//ToggleButtons
var gripper4 = false;
var gripper6 = false;

//Mode switch booleans
const ModeEnum = Object.freeze({"nav":1, "dof_4":2, "dof_6":3});
var joyMode = undefined;

function map(value,range_min,range_max,out_range_min,out_range_max)
{
	var slope = (out_range_max- out_range_min)/(range_max- range_min);
	var newY = slope*(value - range_min) + out_range_min;
	return newY;
}
//map(30,-100,100,-255,255);


io.on('connection',function(socket){
    socket.emit('serverOnline');

    socket.on('disconnect',function(){
        console.log('connection closed');
        controllers=[];
    });
    socket.on('newGamepad',function(data){
    	if(controllers.length == 0)
    	{
    		console.log("no Gamepads");
    		controllers.push(data);
    		socket.emit("ServerDetectedJoy",data);
    	}
    	else
    	{
    		var conn = controllers.find(function(value){
    			return value == data;
    		});
    		if(conn.length > 0)
    		{
    			console.log("Gamepad with id: " + data +" already exists");
    			socket.emit("ServerDetectedJoy",data);
    		}
    		else
    		{
    			controllers.push(data);
    			socket.emit("ServerDetectedJoy",data);
    		}
    	}
        console.log("Gamepads connected: " + controllers);
    });


    socket.on('removeGamepad',function(data){
        console.log("in removeGamepad ");
        controllers = controllers.filter(function(value){
        	return value != data;
        });
        if(controllers.length>0)
        {
        	console.log("Gamepads connected: " + controllers);
    	}
    	else
    	{
    		console.log(" NO Gamepads connected!");	
    	}

        velocities.x = 0;
        velocities.y = 0;
        velocities.z = 0;
        link4D = {'1':0,'2':0,'3':0,'4':0};
		link6D = {'1':0,'2':0,'3':0,'4':0,'5':0,'6':0};
        var msg = JSON.stringify(velocities);
        client.publish('navFeed1',msg);
        msg = JSON.stringify(link4D);
        client.publish('link4D',msg);
        msg = JSON.stringify(link6D);
        client.publish('link6D',msg);

    });
    socket.on('message',function (data) {
        if(controllers.length>0)
        {
			if(data["buttons"][2])
			{
				joyMode = ModeEnum.nav;
				console.log("NAVIGATION MODE:");
			}
			if(data["buttons"][3])
			{
				joyMode = ModeEnum.dof_4;
				console.log("ARM_4 MODE:");
			}
			if(data["buttons"][4])
			{
				joyMode = ModeEnum.dof_6;
				console.log("ARM_6 MODE:");
			}

			if(joyMode != ModeEnum.nav)
			{
				if(data["buttons"][0] && !changeLink && buttonReleased)
				{
					changeLink = true;
					buttonReleased = false;
				}
				else if(data["buttons"][0] == 0)
				{
					buttonReleased = true;
				}

				if(data["buttons"][5] && toggleButtonReleased)
				{
					if(joyMode == ModeEnum.dof_4)
					{
						gripper4 = !gripper4;
						link4D["4"] = gripper4===true? 1 : 0;
					}
					if(joyMode == ModeEnum.dof_6)
					{
						gripper6 = !gripper6;
						link6D["6"] = gripper6===true?1 : 0;
					}
					toggleButtonReleased = false;
				}
				else if(data["buttons"][5] == 0)
				{
					toggleButtonReleased = true;
				}

			}
			if(joyMode == ModeEnum.nav)
			{
				if(data["axes"][1] > 50)
				{
					velocities.y = 1;
				}
				else if(data["axes"][1] < -50)
				{
					velocities.y = -1;
				}
				else
				{
					velocities.y = 0;
				}
				velocities.z = data["axes"][3];
				var msg = JSON.stringify(velocities);
	            if(prev_velocities == undefined)
	            {
	            	prev_velocities=msg;
	            }
	            else
	            {
	            	if(!(msg===prev_velocities))
	            	{
	            		prev_velocities = msg;
	            		client.publish('navFeed',msg);
	            	}
	            }
        	}

        	if(joyMode == ModeEnum.dof_4)
        	{
        		if(changeLink)
        		{
        			curr_l4++;
        			curr_l4%=3;
        			changeLink = false;
        			link4D = {'1':0,'2':0,'3':0,'4':0};
        		}
        		link4D[(curr_l4+1).toString()] = parseInt(map(data["axes"][1],-100,100,-255,255));

        		var msg = JSON.stringify(link4D);
	            if(prev_arm4 == undefined)
	            {
	            	prev_arm4=msg;
	            }
	            else
	            {
	            	if(!(msg===prev_arm4))
	            	{
	            		prev_arm4 = msg;
	            		client.publish('link4D',msg);
	            	}
	            }
        	}

        	if(joyMode == ModeEnum.dof_6)
        	{
        		if(changeLink)
        		{
        			curr_l6++;
        			curr_l6%=5;
        			changeLink = false;
        			link6D = {'1':0,'2':0,'3':0,'4':0,'5':0,'6':0};
        		}
        		link6D[(curr_l6+1).toString()] = parseInt(map(data["axes"][1],-100,100,-255,255));
        		var msg = JSON.stringify(link6D);
	            if(prev_arm6 == undefined)
	            {
	            	prev_arm6=msg;
	            }
	            else
	            {
	            	if(!(msg===prev_arm6))
	            	{
	            		prev_arm6 = msg;
	            		client.publish('link6D',msg);
	            	}
	            }
        	}
        }
    });
});

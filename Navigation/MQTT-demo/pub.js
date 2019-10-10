var mqtt = require('mqtt');
var express = require('express');
var app = express();
app.get('/',function(req,res){
	res.sendFile(__dirname + '/Client/index.html');
});
app.use('/Client',express.static(__dirname + '/Client'));

var server = app.listen(8000,'localhost');
var io = require('socket.io').listen(server);
var client  = mqtt.connect('mqtt://192.168.0.13');

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

var ChangeLink = false;
var ButtonReleased = true;

//Mode switch booleans
var useArm4 = false;
var useArm6 = false;
var useNav = false;



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
				useNav = true;
				useArm4 = false;
				useArm6 = false;
				console.log("NAVIGATION MODE:");
			}
			if(data["buttons"][3])
			{
				useNav = false;
				useArm4 = true;
				useArm6 = false;
				console.log("ARM_4 MODE:");
			}
			if(data["buttons"][4])
			{
				useNav = false;
				useArm4 = false;
				useArm6 = true;
				console.log("ARM_6 MODE:");
			}

			if(!useNav)
			{
				if(data["buttons"][0] && !ChangeLink && ButtonReleased)
				{
					ChangeLink = true;
					ButtonReleased = false;
				}
				else if(data["buttons"][0] == 0)
				{
					ButtonReleased = true;
				}
			}
			if(useNav)
			{
				if(data["axes"][1] > 0.5)
				{
					velocities.y = 1;
				}
				else if(data["axes"][1] < -0.5)
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

        	if(useArm4)
        	{
        		if(ChangeLink)
        		{
        			curr_l4++;
        			curr_l4%=4;
        			ChangeLink = false;
        			link4D = {'1':0,'2':0,'3':0,'4':0};
        		}
        		link4D[(curr_l4+1).toString()] = data["axes"][1];

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

        	if(useArm6)
        	{
        		if(ChangeLink)
        		{
        			curr_l6++;
        			curr_l6%=6;
        			ChangeLink = false;
        			link6D = {'1':0,'2':0,'3':0,'4':0,'5':0,'6':0};
        		}
        		link6D[(curr_l6+1).toString()] = data["axes"][1];

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

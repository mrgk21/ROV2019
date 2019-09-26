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
var prev_velocities = undefined;
var velocities={x:0,y:0,z:0};
var link4D = {'1':0,'2':0,'3':0,'4':0};
var link6D = {'1':0,'2':0,'3':0,'4':0,'5':0,'6':0};

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
        var msg = JSON.stringify(velocities);
        client.publish('navFeed1',msg);
    });
    socket.on('message',function (data) {
        if(controllers.length>0)
        {
			//velocities={x:0,y:0,z:0};
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
            		var l4 = JSON.stringify(link4D);
            		var l6 = JSON.stringify(link6D);
            		client.publish('navFeed',msg);
            		client.publish('link4D',l4);
            		client.publish('link6D',l6);

            		if(link4D['1']==0)
            		{
            			link4D = {'1':1,'2':1,'3':1,'4':1};
						link6D = {'1':1,'2':1,'3':1,'4':1,'5':1,'6':1};
            		}
            		else if(link4D['1']==1)
            		{
            			link4D = {'1':0,'2':0,'3':0,'4':0};
						link6D = {'1':0,'2':0,'3':0,'4':0,'5':0,'6':0};
            		}
            	}
            }
        }


    });
});

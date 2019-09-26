var mqtt = require('mqtt');
var express = require('express');
var app = express();
app.get('/',function(req,res){
	res.sendFile(__dirname + '/Client/index.html');
});
app.use('/Client',express.static(__dirname + '/Client'));

var server = app.listen(8000,'localhost');
var io = require('socket.io').listen(server);
var client  = mqtt.connect('mqtt://192.168.0.10');

client.on('connect', function () {
    client.subscribe('navFeed1', function (err) {
      if (!err) {
        console.log(err);
      }
    })
});

client.on('message', function (topic, message) {
// message is Buffer
    console.log("from mqtt: ");
    console.log(message.toString());
});


var controllers=0;
var prev_velocities = undefined;
var velocities={x:0,y:0,z:0};

io.on('connection',function(socket){
    console.log('new connection');

    socket.on('disconnect',function(){
        console.log('connection closed');
    });
    socket.on('newGamepad',function(data){
    	console.log(data);
        controllers++;
        console.log("Gamepads connected: " + controllers);
    });
    socket.on('removeGamepad',function(data){
        if(controllers>0)
            controllers--;
        console.log("Gamepads connected: " + controllers);
        velocities.x = 0;
        velocities.y = 0;
        velocities.z = 0;
        var msg = JSON.stringify(velocities);
        client.publish('navFeed1',msg);
    });
    socket.on('message',function (data) {
        if(controllers>0)
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
            		client.publish('navFeed1',msg);
            	}
            }
        }
    });
});

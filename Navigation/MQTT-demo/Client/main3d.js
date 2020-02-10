function d2r(d)
{
	return d*Math.PI/180;
}

function r2d(r)
{
	return r*180/Math.PI;
}


function rotatePoint(x,y,angle)
{
	return {x:x*(Math.cos(d2r(angle)))-y*(Math.sin(d2r(angle))),
			y:x*(Math.sin(d2r(angle)))+y*(Math.cos(d2r(angle)))};
}

function restrictAngle(value,min,max,mode='circular')
{
	switch(mode)
	{
		case 'circular':
		{
			if(value.value>max)
			{
				value.value = min;
			}
			else if(value<min)
			{
				value.value = max;
			}
			console.log(value);
			break;	
		}
		case 'zero':
		{
			if(value.value>max || value.value < min)
			{
				value.value = zero;
			}
			break;
		}
	}
}

var yaw = 0;
var pitch = 20;
var roll = 50;

$(function() {
	// CIRULAR COMPASSES
	var canvas1 = document.getElementById("cnv1");
	var ctx1 = canvas1.getContext("2d");
	var canvas2 = document.getElementById("cnv2");
	var ctx2 = canvas2.getContext("2d");
	var canvas3 = document.getElementById("cnv3");
	var ctx3 = canvas3.getContext("2d");

	ctx1.canvas.width = 150;
	ctx1.canvas.height = 150;
	ctx2.canvas.width = 150;
	ctx2.canvas.height = 150;
	ctx3.canvas.width = 150;
	ctx3.canvas.height = 150;

	function drawCircles(ctx,color1,color2,r1,r2)
	{
		ctx.beginPath();
		ctx.fillStyle = color1;
		ctx.arc(75,75,r1,0,2*Math.PI);
		ctx.closePath();
		ctx.fill();

		ctx.beginPath();
		ctx.fillStyle = color2;
		ctx.arc(75,75,r2,0,2*Math.PI);
		ctx.closePath();
		ctx.fill();
	}

	function AddMarker(ctx,x,y,direction=0)
	{
		ctx.fillStyle = "#ffffff";
		switch(direction)
		{
			case 0:
			{
				ctx.beginPath();
				ctx.moveTo(x-5,y);
				ctx.lineTo(x+5,y);
				ctx.lineTo(x+5,y-10);
				ctx.lineTo(x,y-15);
				ctx.lineTo(x-5,y-10);
				ctx.closePath();
				break;
			}
			case 1:
			{
				ctx.beginPath();
				ctx.moveTo(x-5,y);
				ctx.lineTo(x+5,y);
				ctx.lineTo(x+5,y+10);
				ctx.lineTo(x,y+15);
				ctx.lineTo(x-5,y+10);
				ctx.closePath();
				break;
			}
			case 2:
			{
				ctx.beginPath();
				ctx.moveTo(x,y-5);
				ctx.lineTo(x,y+5);
				ctx.lineTo(x-10,y+5);
				ctx.lineTo(x-15,y);
				ctx.lineTo(x-10,y-5);
				ctx.closePath();
				break;
			}
			case 3:
			{
				ctx.beginPath();
				ctx.moveTo(x,y-5);
				ctx.lineTo(x,y+5);
				ctx.lineTo(x+10,y+5);
				ctx.lineTo(x+15,y);
				ctx.lineTo(x+10,y-5);
				ctx.closePath();
				break;
			}
		}
		ctx.fill();
	}

	function AddText(ctx,text,value,x,y,offset)
	{
		ctx.font = "30px Serif-Sans";
		ctx.fillStyle = "#ffffff";
		ctx.textAlign = "center";
		ctx.fillText(text, x,y);
		ctx.fillText(value, x,y+offset);
	}

	//"#325c80" "#4682b4"
	function ctx1update()
	{
		ctx1.clearRect(0,0,ctx1.canvas.width,ctx1.canvas.height);
		drawCircles(ctx1,"#325c80","#4682b4", 70,60);
		AddMarker(ctx1,75,135,0);
		AddText(ctx1,"Yaw:",yaw.toFixed(2),75,70,30);
		
		//Marker Update
		x_pos = 65 * Math.cos(d2r(yaw) - d2r(90));
		y_pos = 65 * Math.sin(d2r(yaw) - d2r(90));


		/*
		//Circular Marker
		ctx1.beginPath();
		ctx1.fillStyle = "7ab8ff";//"#8fbce3";
		ctx1.arc(75 + x_pos ,75 + y_pos,10,0,2*Math.PI);
		ctx1.fill();
		ctx1.closePath();
		*/

		//PentagonMarker
		var radius = 60;
		var points = [{x:0,y:5,radius:Math.sqrt(radius*radius + 25),angel:r2d(Math.atan2(5,radius))},{x:0,y:-5,radius:Math.sqrt(radius*radius + 25),angel:-r2d(Math.atan2(5,radius))},{x:10,y:-5,radius:Math.sqrt((radius+10)*(radius+10) + 25),angel:-r2d(Math.atan2(5,radius+10))},{x:15,y:0,radius: radius + 15,angel:0},{x:10,y:5,radius:Math.sqrt((radius+10)*(radius+10) + 25),angel:r2d(Math.atan2(5,radius+10))}];
		var newPoints1 = [];
		points.forEach(element => {
			newX1 = element.radius * Math.cos(d2r(element.angel))*Math.cos(-d2r(yaw-90)) - element.radius * Math.sin(d2r(element.angel))*Math.sin(-d2r(yaw-90));
			newY1 = element.radius * Math.cos(d2r(element.angel))*Math.sin(-d2r(yaw-90)) + element.radius * Math.sin(d2r(element.angel))*Math.cos(-d2r(yaw-90));
			newPoints1.push({x: 75 + newX1,y: 75 + newY1});
		});
		
		ctx1.fillStyle = "#ffffff";
		ctx1.beginPath();
		ctx1.moveTo(newPoints1[0].x,newPoints1[0].y);
		for(var i=1;i<5;i++)
		{
			ctx1.lineTo(newPoints1[i].x,newPoints1[i].y);
		}
		ctx1.closePath();
		ctx1.fill();
	}

	function ctx2update()
	{
		ctx2.clearRect(0,0,ctx2.canvas.width,ctx2.canvas.height);
		drawCircles(ctx2,"#325c80","#4682b4", 70,60);
		//AddMarker(ctx2,75,15,1);
		AddText(ctx2,"Pitch:",pitch,75,70,30);
	}

	function ctx3update()
	{
		ctx3.clearRect(0,0,ctx3.canvas.width,ctx3.canvas.height);
		drawCircles(ctx3,"#325c80","#4682b4", 70,60);
		AddMarker(ctx3,135,75,2);
		AddMarker(ctx3,15,75,3);
		AddText(ctx3,"Roll:",roll,75,70,30);

		//Roll Markers
		var radius = 60;
		var points = [{x:0,y:5,radius:Math.sqrt(radius*radius + 25),angel:r2d(Math.atan2(5,radius))},{x:0,y:-5,radius:Math.sqrt(radius*radius + 25),angel:-r2d(Math.atan2(5,radius))},{x:10,y:-5,radius:Math.sqrt((radius+10)*(radius+10) + 25),angel:-r2d(Math.atan2(5,radius+10))},{x:15,y:0,radius: radius + 15,angel:0},{x:10,y:5,radius:Math.sqrt((radius+10)*(radius+10) + 25),angel:r2d(Math.atan2(5,radius+10))}];
		console.log(points);
		var newPoints1 = [];
		var newPoints2 = [];
		points.forEach(element => {
			newX1 = element.radius * Math.cos(d2r(element.angel))*Math.cos(d2r(-roll)) - element.radius * Math.sin(d2r(element.angel))*Math.sin(d2r(-roll));
			newY1 = element.radius * Math.cos(d2r(element.angel))*Math.sin(d2r(-roll)) + element.radius * Math.sin(d2r(element.angel))*Math.cos(d2r(-roll));
			newX2 = element.radius * Math.cos(d2r(element.angel))*Math.cos(d2r(-roll+180)) - element.radius * Math.sin(d2r(element.angel))*Math.sin(d2r(-roll+180));
			newY2 = element.radius * Math.cos(d2r(element.angel))*Math.sin(d2r(-roll+180)) + element.radius * Math.sin(d2r(element.angel))*Math.cos(d2r(-roll+180));
			newPoints1.push({x: 75 + newX1,y: 75 + newY1});
			newPoints2.push({x:75 + newX2,y:75 + newY2});
		});
		console.log(newPoints1);
		
		ctx3.fillStyle = "#ffffff";
		ctx3.beginPath();
		ctx3.moveTo(newPoints1[0].x,newPoints1[0].y);
		for(var i=1;i<5;i++)
		{
			ctx3.lineTo(newPoints1[i].x,newPoints1[i].y);
		}
		ctx3.closePath();
		ctx3.fill();

		ctx3.beginPath();
		ctx3.moveTo(newPoints2[0].x,newPoints2[0].y);
		for(var i=1;i<5;i++)
		{
			ctx3.lineTo(newPoints2[i].x,newPoints2[i].y);
		}
		ctx3.closePath();
		ctx3.fill();
	}

	//RENDERER
	var renderer = new THREE.WebGLRenderer();
	renderer.setSize(400, 400);
	$("td:first").append(renderer.domElement);
	const canvas = renderer.domElement;
	//SCENE
	var scene = new THREE.Scene();
	
	//CAMERA
	var camera = new THREE.PerspectiveCamera( 75, window.innerWidth / window.innerHeight, 0.1, 1000 );
	camera.aspect = canvas.clientWidth / canvas.clientHeight;
  	camera.updateProjectionMatrix();


	//CAMERA CONTROLS
	var controls = new THREE.OrbitControls( camera, renderer.domElement );
	
	camera.position.set( 0, 10, 10 );
	controls.update();

	//GRID
    var size = 10;
	var divisions = 10;
	var gridHelper = new THREE.GridHelper(size, divisions);
	scene.add(gridHelper);
	
	//LIGHTS
	var light = new THREE.AmbientLight( 0xffffff, 0.75);
	var pLight = new THREE.PointLight(0xffffff,1,600);
	pLight.position.set( 0, 10, 0 );
	scene.add( light );
	scene.add( pLight );

	//MATERIALS
	const ROV_mat = new THREE.MeshLambertMaterial({color: 0xffffff});
	const arm1_mat = new THREE.MeshLambertMaterial({color: 0xf57373});
	const arm2_mat = new THREE.MeshLambertMaterial({color: 0xfffd7a});
	const joint_mat = new THREE.MeshLambertMaterial({color: 0x85b5ed});
	const plane_mat = new THREE.MeshLambertMaterial({color: 0x404040, side: THREE.DoubleSide});
	

	//GEOMETRIES
	const gROV = new THREE.BoxGeometry(5,5,5);
	const gArm = new THREE.BoxGeometry(0.5,0.5,2);
	const gArm_1 = new THREE.BoxGeometry(0.5,0.5,1);
	const gRotblock = new THREE.BoxGeometry(0.5,0.5,0.5);
	const gPlane = new THREE.PlaneGeometry(100,100,20);
	const gJoint = new THREE.CylinderGeometry(0.5, 0.5, 0.6, 16);
	
	//MESHES
	var plane = new THREE.Mesh(gPlane,plane_mat);
	var ROV = new THREE.Mesh(gROV,ROV_mat);

	//Arm1
	//	-Links
	var arm1_1 = new THREE.Mesh(gArm,arm1_mat);
	var arm1_2 = arm1_1.clone();
	var arm1_3 = new THREE.Mesh(gRotblock,arm1_mat);
	var arm1_4 = arm1_1.clone();
	//	-Joints
	var jArm1_1 = new THREE.Mesh(gJoint,joint_mat);
	var jArm1_2 = jArm1_1.clone();
	var jArm1_3 = jArm1_1.clone();
	var jArm1_4 = jArm1_1.clone();

	//Arm2
	//	-Links
	var arm2_1 = new THREE.Mesh(gArm,arm2_mat);
	var arm2_2 = arm2_1.clone();
	var arm2_3 = new THREE.Mesh(gArm_1,arm2_mat);
	var arm2_4 = new THREE.Mesh(gRotblock,arm2_mat);
	var arm2_5 = arm2_3.clone();
	var arm2_6 = arm2_4.clone();

	//	-Joints
	var jArm2_1 = new THREE.Mesh(gJoint,joint_mat);
	var jArm2_2 = jArm2_1.clone();
	var jArm2_3 = jArm2_1.clone();
	var jArm2_4 = jArm2_1.clone();
	var jArm2_5 = jArm2_1.clone();
	var jArm2_6 = jArm2_1.clone();


	scene.add(plane);
	scene.add(ROV);
	//Add links and joints to scene
	scene.add(arm1_1);
	scene.add(arm1_2);
	scene.add(arm1_3);
	scene.add(arm1_4);
	scene.add(jArm1_1);
	scene.add(jArm1_2);
	scene.add(jArm1_3);
	scene.add(jArm1_4);

	//Add links and joints to scene
	scene.add(arm2_1);
	scene.add(arm2_2);
	scene.add(arm2_3);
	scene.add(arm2_4);
	scene.add(arm2_5);
	scene.add(arm2_6);

	scene.add(jArm2_1);
	scene.add(jArm2_2);
	scene.add(jArm2_3);
	scene.add(jArm2_4);
	scene.add(jArm2_5);
	scene.add(jArm2_6);


	//ROV
	ROV.position.set(0,3,0);
	//ARM1
	var arm1_y1 = new THREE.Group();
	var arm1_p1 = new THREE.Group();
	var arm1_r1 = new THREE.Group();
	var arm1_p2 = new THREE.Group();

	scene.add(arm1_y1);
	scene.add(arm1_p1);
	scene.add(arm1_r1);
	scene.add(arm1_p2);

	//First Yaw
	arm1_1.position.set(0,0,-0.5);
	arm1_y1.add(jArm1_1);
	arm1_y1.add(arm1_1);
	arm1_y1.position.set(1,-1,-3);

	//First Pitching
	arm1_2.position.set(0,0,-1);
	jArm1_2.rotation.set(0,0,d2r(90));
	arm1_p1.add(jArm1_2);
	arm1_p1.add(arm1_2);
	arm1_p1.position.set(0,0,-1);

	//First Roll
	arm1_3.position.set(0,0,-0.5);
	jArm1_3.rotation.set(d2r(90),0,0);
	arm1_r1.add(jArm1_3);
	arm1_r1.add(arm1_3);
	arm1_r1.position.set(0,0,-2);

	//Second Pitch
	arm1_4.position.set(0,0,-1);
	jArm1_4.rotation.set(0,0,d2r(90));
	arm1_p2.add(jArm1_4);
	arm1_p2.add(arm1_4);
	arm1_p2.position.set(0,0,-1);

	arm1_y1.parent = ROV;
	arm1_p1.parent = arm1_y1;
	arm1_r1.parent = arm1_p1;
	arm1_p2.parent = arm1_r1;
	
	//Initial Angles
	arm1_y1.rotation.set(0,d2r(0),0);
	arm1_p1.rotation.set(d2r(75),0,0);
	arm1_r1.rotation.set(0,0,d2r(0));
	arm1_p2.rotation.set(d2r(-120),0,0);

	//ARM2
	var arm2_y1 = new THREE.Group();
	var arm2_p1 = new THREE.Group();
	var arm2_p2 = new THREE.Group();
	var arm2_r1 = new THREE.Group();
	var arm2_p3 = new THREE.Group();
	var arm2_r2 = new THREE.Group();

	scene.add(arm2_y1);
	scene.add(arm2_p1);
	scene.add(arm2_p2);
	scene.add(arm2_r1);
	scene.add(arm2_p3);
	scene.add(arm2_r2);

	//First Yaw
	arm2_1.position.set(0,0,-0.5);
	arm2_y1.add(jArm2_1);
	arm2_y1.add(arm2_1);
	arm2_y1.position.set(-1,-1,-3);

	//First Pitch
	arm2_2.position.set(0,0,-1);
	jArm2_2.rotation.set(0,0,d2r(90));
	arm2_p1.add(jArm2_2);
	arm2_p1.add(arm2_2);
	arm2_p1.position.set(0,0,-1);

	//Second Pitch
	arm2_3.position.set(0,0,-0.75);
	jArm2_3.rotation.set(0,0,d2r(90));
	arm2_p2.add(jArm2_3);
	arm2_p2.add(arm2_3);
	arm2_p2.position.set(0,0,-2);

	//First Roll
	arm2_4.position.set(0,0,-0.5);
	jArm2_4.rotation.set(d2r(90),0,0);
	arm2_r1.add(jArm2_4);
	arm2_r1.add(arm2_4);
	arm2_r1.position.set(0,0,-1.5);

	//Third Pitch
	arm2_5.position.set(0,0,-0.75);
	jArm2_5.rotation.set(0,0,d2r(90));
	arm2_p3.add(jArm2_5);
	arm2_p3.add(arm2_5);
	arm2_p3.position.set(0,0,-1);

	//Second Roll
	arm2_6.position.set(0,0,-0.5);
	jArm2_6.rotation.set(d2r(90),0,0);
	arm2_r2.add(jArm2_6);
	arm2_r2.add(arm2_6);
	arm2_r2.position.set(0,0,-1.5);


	arm2_y1.parent = ROV;
	arm2_p1.parent = arm2_y1;
	arm2_p2.parent = arm2_p1;
	arm2_r1.parent = arm2_p2;
	arm2_p3.parent = arm2_r1;
	arm2_r2.parent = arm2_p3;


	//Initial Angles
	arm2_y1.rotation.set(0,d2r(0),0);
	arm2_p1.rotation.set(d2r(85),0,0);
	arm2_p2.rotation.set(d2r(-30),0,0);
	arm2_r1.rotation.set(0,0,d2r(0));
	arm2_p3.rotation.set(d2r(-110),0,0);
	arm2_r2.rotation.set(0,d2r(0),0);

	plane.rotation.set(Math.PI * 90/180,0,0);
	
	var animate = function() {
		requestAnimationFrame(animate);
		if(msg != "")
		{
			if(msg.axes[4]> 0.5)
			{
				yaw--;
			}
			else if(msg.axes[4]< -0.5)
			{
				yaw++;
			}
		}
		ROV.rotation.y = d2r(yaw.toFixed(2));
		//ROV.rotation.z += 0.01;
		controls.update();
		renderer.render(scene, camera);
		ctx1update();
		ctx2update();
		ctx3update();
	};

	var isAnyKeyDown = false;
	var keyCode = -1;

	window.addEventListener('keydown',doKeyDown,true);
	window.addEventListener('keyup',doKeyUp,true);
	function doKeyUp(e) 
	{
		if(isAnyKeyDown && keyCode == e.keyCode)
		{
			isAnyKeyDown = false;
		}
	}

	function doKeyDown(e) 
	{  
		if(isAnyKeyDown)
		{
			return;
		}

		if(!isAnyKeyDown)
		{
			console.log(e);
			isAnyKeyDown = true;
			keyCode = e.keyCode;
			if(e.keyCode == 82)
			{
				controls.reset();
				camera.position.set( 0, 10, 10 );
				controls.update();
				console.log(controls);
			}
			
			if(e.keyCode == 97)
			{
				yaw--;
			}

			if(e.keyCode == 98)
			{
				yaw++;
			}
		}
	}

	animate();

});
# Joystick Codes

In this folder I have included the codes required for controlling the ROV with joystick.

> The code structure is as following:
>
> -   [Server Side](#server)
> -   [Client Side](#client)

## <a id='server'></a>Server Side Script:

([Link to the code](https://github.com/mrgk21/ROV2019/blob/PID%2BIK/Navigation/MQTT-demo/pub.js))  
The Server Side Script is used for mainly 2 purposes.

1. Receiving the Joystick data from it's client through [socket.io](https://socket.io/).
2. Sending this data to Nano Pi through [MQTT](http://mqtt.org/).

### **Data Reception**

Server and the client have to do the handshake to establish a communication between them. Especially, Server has to detect whether a joystick is connected or not.

To make this process independent of the sequence in which the client, the server and the joystick are switched on/awaken, I have made server and client send each other requests to one another till joystick connected has been acknowledged from both the sides.

### **Data Transmission**

For data transmission, we are using MQTT protocol which is a publisher-subscriber type of protocol. Our server will publish on certain topics, Raspberry Pi has subscribed to those topics and hence will receive the respective data.

**_Note:_ To make this system more efficient in terms of data rates and network traffic, we are sending data only when there are some changes with respect to the previous data.**

## <a id='client'></a>Client Side Script:

([Link to the code](https://github.com/mrgk21/ROV2019/blob/PID%2BIK/Navigation/MQTT-demo/Client/main.js))  
Client side script uses the _'Gamepad WebAPI'_ for the operation. The API provides us with a javascript object containing information about axes,buttons and hats. All we have to do is to detect whether such an object is available, then read data from it after certain interval, compare data and send it to server if there is any change in the data with respect to previous data.

To know more about **Gamepad API** [here.](https://developer.mozilla.org/en-US/docs/Web/API/Gamepad_API)

# Procedure

1. Start the Mosquitto broker(on linux workstation).
2. Start the [pub.js](https://github.com/mrgk21/ROV2019/blob/PID%2BIK/Navigation/MQTT-demo/pub.js) for starting the Node.js Server.
3. Open the browser and go to _"localhost:8000"_.
4. Connect the joystick and it should be detected on the client side as well as server side.
5. By using the preconfigured buttons choose the mode of operation and use the joystick.

# Commands

-   For [pub.js](https://github.com/mrgk21/ROV2019/blob/PID%2BIK/Navigation/MQTT-demo/pub.js) script:

    > node pub.js -h _(host address)_

-   Buttons:
    -   Button[0]: Change link
    -   Button[2]: Navigation Mode
    -   Button[3]: ARM_4 Mode
    -   Button[4]: ARM_6 Mode
    -   Button[5]: Gripper Toggle

# Possible Issues and solutions

1. If Server is not working.  
   **-> Reason:** Check the host address on which the broker is running. And give according commands.
1. GUI doesn't appear.  
   **-> Reason:** Just move the joystick or press any button.
1. Uncaught TypeError: Cannot read property 'toFixed' of undefined.  
   **-> Reason:** The index of axes is wrong. So check again what all axes are actually having some data and put indices in 'axes' array accordingly.

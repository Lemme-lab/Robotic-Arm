#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"

// Replace with your network credentials
const char* ssid = "technicus";
const char* password = "Werkstatt-G";

const char* PARAM_INPUT_1 = "output";
const char* PARAM_INPUT_2 = "state";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<style>
body {
  background: #0d1117;
}

.tab {
    overflow: hidden;
    border: 1px solid #455559;
    background-color: #010e1d;
    text-align: center;
    width: 90%;
    margin-left: auto;
    margin-right: auto;
  }

  .tablinks{
      font-size: 1.9rem;
      color: white;
  }

  .buttons{
    margin-top: 25px;
    display: flex;
    justify-content: center;
    color: white;
  }

 
  .lower_boxes{
    margin-top: 15px;
    display: flex;
    justify-content: center;
  }

  .Coordinates{
      border: 5px solid #28403D;
      background-color: #455559;
      color: white;
      height: 100px;
      width: 300px;
      margin-right: 10px;
      float:left;
  }

  .status_modes{
      border-color: #2F3D40;
      display: flex;
      justify-content: center;
      float:left;
  }

  .box{
    background-color: rgb(0, 0, 0);
    width: 100px;
    height: 60px;
    padding-top: 40px;
    border-color: black;
    border: 5px solid #2F3D40;
  }

  .power_img_power{
   margin-top: -25px;
   margin-left: 10px;

  }
  
  .power_img_status{
    margin-top: -25px;
    margin-left: 10px;
   }

   #rightbox{
    float:right;
    margin-left: 10px;
    display: flex;
    justify-content: center;
}

    #input1{
      margin-top: +50px;
      margin-left: +25px;
      border-color: rgba(204, 204, 204, 0);
      background-color: #455559;
      color: rgb(255, 255, 255);
      font-size: 1.9em;
    }


    #input3{
      margin-top: +50px;
      margin-left: +15px;
      background-color: #455559;
      border-color: rgba(204, 204, 204, 0);
      color: rgb(255, 255, 255);
      font-size: 1.7em;
    }

    #input4{
      margin-top: +50px;
      margin-left: +15px;
      border-color: rgba(204, 204, 204, 0);
      background-color: #455559;
      color: rgb(255, 255, 255);
      font-size: 1.9em;
      border: 5px solid rgb(39, 39, 39);
    }

    #input1::placeholder {
      color: rgb(255, 255, 255);
      font-size: 0.9em;
    }

    #input3::placeholder {
      color: rgb(255, 255, 255);
      font-size: 1.2em;
    }

    #input4::placeholder {
      color: rgb(255, 255, 255);
      font-size: 1em;
    }
   

  .Voltage{
    border: 5px solid #28403D;
    background-color: #455559;
    color: white;
    height: 100px;
    width: 150px;
  }

  .speed{
    border: 5px solid #28403D;
    background-color: #455559;
    color: white;
    height: 100px;
    width: 150px;
    margin-left: -10px;
  }

  .tab button {
    background-color: inherit;
    border: none;
    outline: none;
    cursor: pointer;
    padding: 10px 80px;
    transition: 0.3s;
    
  }
  
  .tab button:hover {
    color: rgb(0, 0, 0);
    background-color: rgb(224, 224, 224);
    padding: 10px 140px;
    border-radius: 10px;
  }
  
  .tab button.active {
    background-color: rgb(255, 255, 255);
    color: black;
    border-radius: 10px;
  }

  .tabcontent {
    display: none;
    padding: 6px 12px;
    border-top: none;
  }

  .upper_boxes{
    display: flex;
    justify-content: center;
    float:left;
    padding-top: 100px;
    padding-left: 60px;
  }

  .cam{
    background-color: #455559;
    width: 550px; 
    height: 550px; 
    margin-left:auto;
    margin-right:auto;
    margin-top: 50px;
  }

  .cam_img{
    margin-left: 12px;
    margin-top: 12px;
  }

  .path_list{
    margin-left: +15px;
    border: 5px solid #455559;
    background-color: rgb(255, 255, 255);
    width: 212px;
    height: 350px;
    overflow-y: scroll;
  }

  .mid{
    display: flex;
    justify-content: center;
    margin-top: -100px;

  }

  .mid_left{
    padding-top: 100px;
    padding-left: 200px;
    padding-right: 200px;
  }

  .style_element{
    border: 3px solid #455559;
    background-color: rgb(214, 214, 214);
    margin-top: 10px;
    margin-left: 10px;
    margin-right: 10px;
    height: 30px;
    font-size: 0.8em; 
    padding-top: 10px; 
    padding-left: 8px; 
  } 

  .arm_img{
    padding-top: 70px;
    padding-right: 100px;
    float:right;

  }

  .lower{
    margin-top: 25px;
    display: flex;
    justify-content: center;
    color: white;
  }

  .buttons_training{
    height: 85px;
    width: 120px;
    border: 5px solid rgb(39, 39, 39);
  }
  .estop{
    height: 95px;
    width: 115px;
    background: url('data/estop.png');
    background-size: contain;
    background-repeat: no-repeat, repeat;
    background-position: center;
    color: white; 
    background-color: white;  
    border: 5px solid #455559;
  }

  .save{
    height: 95px;
    width: 115px;
    background: url('data/save.png');
    background-size: contain;
    background-repeat: no-repeat, repeat;
    background-position: center;
    color: white; 
    background-color: white;  
    border: 5px solid #455559; 
    padding-left: 20px; padding-right: 20px;
  }

  .test{
    height: 95px;
    width: 115px;
    background: url('data/test.png');
    background-size: contain;
    background-repeat: no-repeat, repeat;
    background-position: center;
    color: white; 
    background-color: white;  
    border: 5px solid #455559;
     padding-left: 20px; 
     padding-right: 20px;
  }

  .add{
    width: 120px;
    height: 44px;
    float: both;
    border-bottom: 5px solid rgb(39, 39, 39);
  }

  .remove{
    width: 120px;
    height: 42px;
    float: both;
  }

  .upper_boxesTraining{
    padding-top: 30px;
    display: flex;
    justify-content: center;
  }

  .lower_boxesTraining{
    padding-top: 30px;
    display: flex;
    justify-content: center;
  }

  .inputAXIS{
    border-color: #28403D;
    color: black;
    font-size: 1.7em;
    margin-left: -4px;
    margin-top: 21px;
    margin-bottom: -4px;
    width: 290px; 
    height: 45px; 
    border-top: 5px solid #28403D;
    border-left: 5px solid #28403D;
    border-right: 5px solid #28403D;
    border-bottom: 5px solid #28403D;
  }
  
  .axisl{
    float: left;
    border: 5px solid #28403D;
    background-color: #455559;
  }

  .axism{
    border: 5px solid #28403D;
    background-color: #455559;
    float: left;
    margin-left: 30px;
  }

  .axisr{
    border: 5px solid #28403D;
    background-color: #455559;
    float: right;
    margin-left: 30px;
  }

  .axisup{
    border: 5px solid #28403D;
    background-color: rgb(255, 255, 255);
    height: 50px;
    width: 300px;
  }

  .axisdown{
    border: 5px solid #28403D;
    background-color: rgb(255, 255, 255);
    height: 50px;
    width: 300px;
  }
  #inaxis1{
    background-color: #455559;
  }
  #inaxis2{
    background-color: #455559;
  }
  #inaxis3{
    background-color: #455559;
  }
  #inaxis4{
    background-color: #455559;
  }
  #inaxis5{
    background-color: #455559;
  }
  #inaxis6{
    background-color: #455559;
  }

  #inaxis1::placeholder{
    background-color: #455559;
    color: white;
  }
  #inaxis2::placeholder{
    background-color: #455559;
    color: white;
  }
  #inaxis3::placeholder{
    background-color: #455559;
    color: white;
  }
  #inaxis4::placeholder{
    background-color: #455559;
    color: white;
  }
  #inaxis5::placeholder{
    background-color: #455559;
    color: white;
  }
  #inaxis6::placeholder{
    background-color: #455559;
    color: white;
  }

  .slidecontainer {
    width: 300px;
    padding-top: 20px;
  }
  
  .slider {
    -webkit-appearance: none;
    width: calc(100% - 10px);
    height: 20px;
    background: #000000;
    outline: none;
    opacity: 0.7;
    -webkit-transition: .2s;
    transition: opacity .2s;
    border-radius: 25px;
    margin-top: 5px;
  }
  
  .slider::-webkit-slider-thumb {
    -webkit-appearance: none;
    appearance: none;
    width: 30px;
    height: 30px;
    border-radius: 50%;
    background-color: #ffffff;
    cursor: pointer;
  }
  
  .slider::-moz-range-thumb {
    width: 25px;
    height: 25px;
    border-radius: 50%;
    background: #000000;
    cursor: pointer;
  }

  .sleep_main {
    display: flex;
    justify-content: center;
  }

  .box_sleep_left {
    padding-top: 50px;
    margin-top: 50px;
    border-radius: 20px;
    border-color: #28403D;
    color: black;
    font-size: 1.7em;
    width: 300px; 
    height: 725px; 
    border: 5px solid #28403D;
  }

  .box_sleep_left_innerboxes {
    margin-top: 40px;
    margin-left: 15px;
    border: 5px solid #28403D;
    border-radius: 20px;
    background-color: white;
    width: 260px;
    height: 70px;
  }

  .box_sleep_left_innerboxes {
    margin-top: 40px;
    margin-left: 15px;
    border: 5px solid #28403D;
    border-radius: 20px;
    background-color: white;
    width: 260px;
    height: 70px;
  }

  .box_sleep_left_innerboxes_small {
    padding-top: 5px;
    margin-top: 10px;
    margin-left: 45px;
    border: 5px solid #28403D;
    border-radius: 20px;
    background-color: white;
    width: 200px;
    height: 70px;
  }

  .box_sleep{
    border-radius: 20px;
    border: 5px solid #28403D;
    margin-top: 50px;
    margin-left: 10px;
    padding-top: 125px;
    padding-right: 30px;
    background-color: rgb(0, 0, 0);
    width: 590px; 
    height: 650px; 
  }

  .mid_boxes_sleep{
    display: flex;
    justify-content: center;
    margin-top: -20px;
  }

  .arm_img_2{
    margin-left: 45px;
    margin-top: -80px;
  }

  .sleep_main{
    display: flex;
    justify-content: center;
  }

  .sleep_rigth_box{
    margin-top: 50px;
    margin-left: 10px;
    border: 5px solid #28403D;
    border-radius: 20px;
    width: 270px;
  }

  .doxc{
    padding-left: 50px;
    margin-top: -20px;
    margin-top: 40px;
    margin-bottom: 10px;
    border-top:5px solid #28403D;
    border-bottom:5px solid #28403D;
    background-color: white;
  }

  .github{
    padding-left: 50px;
    margin-top: -20px;
    margin-top: 30px;
    margin-bottom: 30px;
    border-top:5px solid #28403D;
    border-bottom:5px solid #28403D;
    background-color: white;
  }

  .version{
    height: 180px;
    width: 100%;
    margin-top: 30px;
    margin-bottom: 10px;
    padding-top: 10px;
    border-top:5px solid #28403D;
    border-bottom:5px solid #28403D;
    background-color: white;
  }
</style>
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Controll Panel</title>
    <link rel = "stylesheet" href="styles.css" />

</head>
<body>
    
<nav class = Main >
    <div class = switch_modes>
        <div class="tab" id="tab_change">
            <button class="tablinks" onclick="changemode(event, 'Normal')">Normal</button>
            <button class="tablinks" onclick="changemode(event, 'Training')">Training</button>
            <button class="tablinks" onclick="changemode(event, 'Sleep')">Sleep</button>
          </div>
    </div>


    
    
    <div id="Normal" class="tabcontent">

        <div class = upper_boxes>

            <div class = buttons_side>

                
                <div class = sequence>
                  <button type="button" style="color: white; background-color: rgb(255, 255, 255);  border: 8px solid #455559; margin-bottom: 30px;"> <img src="data/saved.png" height ="120" width="120" /></button>
                </div>
      
                <div class = check>
                    <img name="check" class="check_img" style="color: white; background-color: rgb(255, 255, 255);  border: 8px solid #455559; margin-bottom: 30px; " onclick="next_check()" width="134" height="120" src="data/check_green.png" >
                </div>
      
                <div class = loading>
                    <img src="data/loading_png.png" id="hiddenimageid" style="border: 8px solid #455559;" onclick="character();" height ="135" width="135"/>
                  </div>
      
      
             </div>
         </div>
           

        <div class = cam>
            <!--//Cam-->
           <!--<img name="main" class="cam_img" width="475" height="475" src="http://192.168.1.109">-->

           <!--//test imgae-->
           <img name="main" class="cam_img" width="525" height="525" src="data/shutter.png">
       </div>
    

       <div class = buttons>

          <div class = stop>
            <button type="button" style="color: white; background-color: white;  border: 5px solid #2F3D40;" onclick="stop_programm();"> <img src="data/84381.png" height ="80" width="80" /></button>
          </div>

          <div class = start>
            <button type="button" style="color: white; background-color: white;  border: 5px solid #2F3D40;" onclick="start_programm();"> <img src="data/startsymbol.png" height ="80" width="80" /></button>
          </div>

          <div class = hold>
            <button type="button" style="color: white; background-color: white;  border: 5px solid #2F3D40;" onclick="hold_programm();"> <img src="data/pause.png" height ="80" width="80" /></button>
          </div>


       </div>


       <div class = lower_boxes>

            <div class = Coordinates>
            
            <input id="input1" type="text" style="width: 252px; height: 44px; color: white;" placeholder="20/123/233/65/41/237">
            <h1 style="font-size: 2.6em; margin-top: 0px;margin-left: 20px; margin-top: -100px;">Coordinates: </h1>
            </div>
        
            <div class = status_modes>
   
                <div class = box>
                    <img name="power" class="power_img_power" onclick="next_power()" width="75" height="75" src="data/PowerOn.png" >
                </div>
    
                <div class = box>
                    <img name="status" class="power_img_status" onclick="next_connection()" width="80" height="70" src="data/connection_on.png"> 
                </div>
        
        
            </div>
             
            <div id = "rightbox">
                <div class = Voltage> 
                    <h1 style="font-size: 2em; margin-top: 10px;margin-left: 15px">Voltage: </h1>
                    <h3 style="font-size: 1.7em; margin-top: -20px; margin-left: 25px;" id = "coordinates_text">4.95V</h1>
                </div>
    
                <div class = speed>
                    <input id="input3" type="text" style="width: 110px; height: 44px;" placeholder="20 Steps">
                    <h1 style="font-size: 2em; margin-top: 10px;margin-left: 25px; margin-top: -90px;">Speed: </h1>
                </div>
                
            </div>



        </div>

    
    </div>

    <!--//Hier das ganze vom Matteo einfügen-->
    <div id="Training" class="tabcontent">
    
        <div class = upper_boxesTraining>
            <div class = axisl style="margin-top: 0px;">
                <div class = slidecontainer>
                    <input type="range" min="1" max="360" value="0" class="slider" id="myRange1">
                    <input id="inaxis1" style="width: 295px;" class = "inputAXIS" type="text"  placeholder="              AXIS 1">
                </div>
            </div>
            <div class = axism>
                <div class = slidecontainer>
                    <input type="range" min="1" max="360" value="0" class="slider" id="myRange2">
                    <input id="inaxis2" style="width: 295px;" class = "inputAXIS" type="text"  placeholder="              AXIS 2">
                </div>
            </div>
            <div class = axisr>
                <div class = slidecontainer>
                    <input type="range" min="1" max="360" value="0" class="slider" id="myRange3">
                    <input id="inaxis3" style="width: 295px;" class = "inputAXIS" type="text"  placeholder="              AXIS 3">
                </div>
            </div>
            
        </div>
        <div class = lower_boxesTraining>
            <div class = axisl style="margin-top: 0px;">
                <div class = slidecontainer>
                    <input type="range" min="1" max="360" value="0" class="slider" id="myRange4">
                    <input id="inaxis4" style="width: 295px;" class = "inputAXIS" type="text"  placeholder="              AXIS 4">
                </div>
            </div>
            <div class = axism>
                <div class = slidecontainer>
                    <input type="range" min="1" max="360" value="0" class="slider" id="myRange5">
                    <input id="inaxis5" style="width: 295px;" class = "inputAXIS" type="text"  placeholder="              AXIS 5">
                </div>
            </div>
            <div class = axisr>
                <div class = slidecontainer>
                    <input type="range" min="1" max="360" value="0" class="slider" id="myRange6">
                    <input id="inaxis6" style="width: 295px;" class = "inputAXIS" type="text"  placeholder="              AXIS 6">
                </div>
            </div>
            
        </div>

         <div class = mid>
          <div class = mid_left>
            <div class = EOAT>
                <input id="input4" type="text" style="width: 180px; height: 60px; padding-left: 30px; border: 5px solid #455559; background-color: #2F3D40;" placeholder="    EOAT">
            </div>

            <div class = path_list style="background-color: #ffffff;">
                <div class =list>

                </div>

                <div class = elements id =jselements>
                
                 </div>
                </div>

            </div>

            <div class = arm_img>          
                <img name="status" class="arm_img"  width="430" height="430" style="padding-top: 80px; border-width: 5px;" src="data/Model-of-Articulated-robotic-arm.png"> 
            </div>

         </div>

         <div class = lower>

            
                <button type="button" class = estop onclick="estop_programm();"></button>
    
                <button type="button" class = save onclick="save_programm();"></button>
    
                <button type="button"  class = test onclick="test_programm();"></button>

              <div class = buttons_training>
                <button class = "add" style="color: white; background-color: rgb(0, 255, 179); font-size: 2rem;" onclick="addCoordiantes()">+</button>
                <button class = "remove" style="color: white; background-color: rgb(255, 0, 0); font-size: 2rem;" onclick="removeCoordinates()">-</button>
              </div>
              


         </div>


            

        </div>
        

    </div>

    <div id="Sleep" class="tabcontent" style = "overflow-y: hidden;">
        <div class = mid_boxes_sleep>

            <div class = sleep_main>
                <div class = box_sleep_left>
                    <div class = box_sleep_left_innerboxes>
                        <h1 style="font-size: 1.7em; margin-top: 10px; color: #000000; text-align: center;">ATMEGA</h1>
                    </div>
                    <div class = box_sleep_left_innerboxes_small>
                        <button style="font-size: 1.4em; margin-top: 10px; color: #000000; text-align: center;
                            background: transparent; border: none !important; padding-left: 20px;">Set Sleep</button>
                    </div>
                    <div class = box_sleep_left_innerboxes_small>
                        <h2 style="font-size: 1.4em; margin-top: 10px; color: #000000; text-align: center;">1,3mA</h1>
                    </div>
                    <div class = box_sleep_left_innerboxes>
                        <h1 style="font-size: 1.7em; margin-top: 10px; color: #000000; text-align: center;">ESP32</h1>
                    </div>
                    <div class = box_sleep_left_innerboxes_small>
                        <button style="font-size: 1.4em; margin-top: 10px; color: #000000; text-align: center;
                            background: transparent; border: none !important; padding-left: 20px;">Set Sleep</button>
                    </div>
                    <div class = box_sleep_left_innerboxes_small>
                        <h2 style="font-size: 1.4em; margin-top: 10px; color: #000000; text-align: center;">0,25mA</h1>
                    </div>
                </div>
            <div class = box_sleep>
                <img name="status" class="arm_img_2"  width="550" height="650" src="data/robotic_arm.png">
            </div>

            <div class = sleep_rigth_box>
                   <div class = doxc>
                    <a href="data/documentation.docx"><img src="data/documentation.png" style="width:180px;height:180px;"></a>
                   </div>
                   <div class = github>
                    <a href="https://github.com/Lemme-lab/Robotic-Arm"><img src="data\github.png" style="width:180px;height:180px;"></a>
                   </div>
                   <div class = version>
                       <h1 style="font-size: 1.9rem; padding-left: 45px">Robotic Arm:</h1>
                       <h1 style="font-size: 1.7rem; margin-top: -20px; padding-left: 45px; color: #035951;">Version: 0.1.1</h1>
                       <h1 style="font-size: 1.2rem; margin-top: -25px; padding-left: 52px; color: #035951;">Lenarcic Lucas, <br>Matteo Müller</h1>
                   </div>
            </div>
             
        </div>
    
            
    
        </div>
    
    </div>

    

</nav>
</body>


<script>
var power_img_power = document.querySelector('.power_img_power');
var power_img_status = document.querySelector('.power_img_status');
var check_img = document.querySelector('.check_img');
var images1 = ['PowerOn.png', 'PowerOff.png'];
var images2 = ['connection_off.png', 'connection_on.png'];
var images3 = ['check_green.png', 'check_red.png'];
var i = 0;

var slider1 = document.getElementById("myRange1");
var output1 = document.getElementById("inaxis1");
output1.placeholder = "              AXIS 1";
var slider2 = document.getElementById("myRange2");
var output2 = document.getElementById("inaxis2");
output2.placeholder = "              AXIS 2";
var slider3 = document.getElementById("myRange3");
var output3 = document.getElementById("inaxis3");
output3.placeholder = "              AXIS 3";
var slider4 = document.getElementById("myRange4");
var output4 = document.getElementById("inaxis4");
output4.placeholder = "              AXIS 4";
var slider5 = document.getElementById("myRange5");
var output5 = document.getElementById("inaxis5");
output5.placeholder = "              AXIS 5";
var slider6 = document.getElementById("myRange6");
var output6 = document.getElementById("inaxis6");
output6.placeholder = "              AXIS 6";

var stop_status = false;
var start_status = false;
var hold_status = false;

var input1 = 0;
var input2 = 0;
var input3 = 0;
var input4 = 0;
var input5 = 0;
var input6 = 0;

number = 0;
var loader_img = ["loading_gif.gif", "loading_png.png"];
var counter = 0;


function changemode(evt, cityName) {
  var i, tabcontent, tablinks;

  tabcontent = document.getElementsByClassName("tabcontent");
  for (i = 0; i < tabcontent.length; i++) {
    tabcontent[i].style.display = "none";
  }

  tablinks = document.getElementsByClassName("tablinks");
  for (i = 0; i < tablinks.length; i++) {
    tablinks[i].className = tablinks[i].className.replace(" active", "");
  }

  document.getElementById(cityName).style.display = "block";
  evt.currentTarget.className += " active";
}

slider1.oninput = function () {
  output1.placeholder = this.value;
  input1 = this.value;
}

slider2.oninput = function () {
  output2.placeholder = this.value;
  input2 = this.value;
}

slider3.oninput = function () {
  output3.placeholder = this.value;
  input3 = this.value;
}

slider4.oninput = function () {
  output4.placeholder = this.value;
  input4 = this.value;
}

slider5.oninput = function () {
  output5.placeholder = this.value;
  input5 = this.value;
}

slider6.oninput = function () {
  output6.placeholder = this.value;
  input6 = this.value;
}

function next_power() {
  if (i >= images1.length - 1) i = -1;
  i++;
  return setImg1();
}

function next_connection() {
  if (i >= images2.length - 1) i = -1;
  i++;
  return setImg2();
}

function next_check() {
  if (i >= images3.length - 1) i = -1;
  i++;
  return setImg3();
}


function setImg1() {
  return power_img_power.setAttribute('src', "data/" + images1[i]);
}

function setImg2() {
  return power_img_status.setAttribute('src', "data/" + images2[i]);
}

function setImg3() {
  return check_img.setAttribute('src', "data/" + images3[i]);
}

function character() {
  image = document.getElementById('hiddenimageid');
  image.src = "data/" + loader_img[number];
  if (number == 1) {
    number = 0;
  } else {
    number++;
  }

}




function addCoordiantes() {
  counter++;
  var eoat = document.querySelector("#input4").value;
  var text_in = input1 + "/" + input2 + "/" + input3 + "/" + input4 + "/" + input5 + "/" + input6 + "/" + eoat;
  var tag = document.createElement("H1");
  tag.id = "element" + counter;
  tag.className = "style_element";
  var text = document.createTextNode(text_in);
  tag.appendChild(text);
  var element = document.getElementById("jselements");
  element.appendChild(tag);
  console.log(counter);
}

function removeCoordinates() {
  var child = document.getElementById("element" + counter);
  counter--;
  child.parentNode.removeChild(child);
}

function stop_programm(){
  console.log("Stopped Programm");
  stop_status = true;
  console.log(stop_status);
  
}

function start_programm(){
  console.log("Startet Programm");
  start_status = true;
}

function hold_programm(){
  console.log("Holding Programm");
  hold_status = true;
}

function estop_programm(){
  console.log("E-Stop ");
}

function save_programm(){
  console.log("saved Programm");
}

function test_programm(){
  console.log("testing Programm");
}

const params = new Proxy(new URLSearchParams(window.location.search), {
  get: (searchParams, prop) => searchParams.get(prop),
});

stop_status = params.stop_input;
start_status = params.start_input;
hold_status = params.hold_input;
</script>
</html>
)rawliteral";

void setup(){
  // Serial port for debugging purposes
  Serial.begin(9600);

  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
  pinMode(33, OUTPUT);
  digitalWrite(33, LOW);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });
  
  server.begin();
  

  /*
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });


  // Start server
  server.begin();
  */
}

void loop() {

}

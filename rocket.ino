#include "TimerOne.h"

String g_state = "ST_WAIT_ACK";
String msg_info = "";
bool msg, flag1, flag_timer = 0;
bool g_session_over = 0;
int g_session_over2 = 0;

//criar buffer e flag
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

String input;

//definir tamanho do buffer
void setup() {
  Serial.begin(115200);
  inputString.reserve(200);

  msg_info.reserve(200);
  msg_info = "Hydra\n";
  
  Timer1.initialize(45000);         // initialize timer1, and set a 1/2 second period
  Timer1.attachInterrupt(callback);  // attaches callback() as a timer overflow interrupt

}

void callback()
{
  //Serial.print("controle\n");
  if(g_state == "ST_WAIT_ACK"){
    input = "EV_TOUT_SOT\n";
    Serial.print(input);
  }
  if(g_state == "ST_SENDING_DATA"){
    input = "EV_TOUT_SESSION_TX\n";
  }
  if(g_state == "ST_RECEIVING_DATA"){
    input = "EV_TOUT_NO_RX\n";
  }
    
  flag_timer = true;
  //Serial.print("controle\n");
}

void loop() {


    if(msg_info.length() > 0)
    {
      msg = false;
    }
  
    if (stringComplete == true || flag_timer == true){             
    //Serial.print(inputString);
    //msg = digitalRead(7);
    input = inputString;
    inputString = "";
    stringComplete = false;
    //Serial.print(input);
    if (g_state == "ST_WAIT_ACK"){
      
        if(input == "EV_TOUT_SOT\n") {      
            Serial.print("SoT -- WAIT_ACK\n");
            input = "";
            flag_timer = false;
            
            }
        //Serial.print(input == "EV_ACK\n");
        delay(2);
        if(input == "EV_ACK\n"){ 
            
            if(msg == 1) {  
                Serial.print("Hydra -- WAIT_ACK\n");
                g_state = "ST_SENDING_DATA";
                Timer1.initialize(1000000);
            } else {      
                Serial.print("EOT -- WAIT_ACK\n");
                g_state = "ST_RECEIVING_DATA"; 
                Timer1.initialize(5000000);
            }
        }
        /*else{
        Serial.print("Error-wait-ack\n");
            }*/
    }
    if(g_state == "ST_SENDING_DATA"){

        if(input == "EV_TOUT_SESSION_TX\n") {
            g_session_over = 1;
            input = "";
            flag_timer = false;
            //Serial.print("DALE\n");
        }
        if(g_session_over == 1 || msg == false) {
                g_session_over = 0;
                Serial.print("EOT -- SENDING_DATA\n");
                g_state = "ST_RECEIVING_DATA";
            } 
         /*else
                  {
                    Serial.print("Hydra -- SENDING_DATA\n");
                    //delay(100);
                  }*/
        }
        /*else{
        Serial.print("Error-sending-data\n");
            }*/        
        
    if(g_state == "ST_RECEIVING_DATA"){
        if(input == "EV_TOUT_NO_RX\n") {
            input = "";
            flag_timer = false;
            Serial.print("SoT -- RECEIVING_DATA\n");
            Timer1.initialize(30000);
            g_state = "ST_WAIT_ACK";
            }
       
        if(input == "EV_EOT\n"){ //11
            if (msg == true) { //12
                Serial.print("Hydra -- RECEIVING_DATA\n");
                g_state = "ST_SENDING_DATA"; 
            } 
            else
            {
                Serial.print("EOT -- RECEIVING_DATA\n");
            }
        }
        if(msg == true){ //11
          //processar dados recebidos
            //processa_dados();
        }
        /*else
        {
        Serial.print("Error-rec-data\n");
        }*/
                
    }
    //input = "";
    //delay(1000);
  }           
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar; 
    
      if (inChar == '\n') 
      {
      stringComplete = true;
      flag_timer == false;
      }
                            
  }
}

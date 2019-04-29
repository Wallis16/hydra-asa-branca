#include "TimerOne.h"

//#define ARRAYSIZE 70
//#define ARRAYSIZE2 140

String g_state = "ST_WAIT_ACK";
bool msg, flag1, flag_timer = 0;
bool g_session_over = 0;

//String ST_names[ARRAYSIZE] = { "ST_INIT", "ST_WAIT_ACK", "ST_SENDING_DATA", "ST_RECEIVING_DATA",
//                          "desconhecido" };

//String EV_NAMES[ARRAYSIZE2] = {"EV_ACK", "EV_SOT", "EV_TOUT_SOT", "EV_TOUT_SESSION_TX",
//    "EV_TOUT_NO_RX", "EV_TOUT_SESSION_RX", "EV_EOT", "EV_DADOS", "EV_DATA_SENT",
//    "EV_NO_DATA", "EV_INVALID", "EV_MAX", "desconhecido"
//};

//criar buffer e flag
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

String input;

//definir tamanho do buffer
void setup() {
  Serial.begin(115200);
  inputString.reserve(200);
  
  Timer1.initialize(1000000);         // initialize timer1, and set a 1/2 second period
  Timer1.attachInterrupt(callback);  // attaches callback() as a timer overflow interrupt

  pinMode(7, INPUT);
}

void callback()
{
  if(g_state == "ST_WAIT_ACK"){
    input = "EV_TOUT_SOT\n";
  }
  if(g_state == "ST_SENDING_DATA"){
    input = "EV_TOUT_SESSION_TX\n";
  }
  
  flag_timer = true;
}

void loop() {

    if (stringComplete == true || flag_timer == true){             
//Serial.print(inputString);
    msg = digitalRead(7);
    input = inputString;
    inputString = "";
    stringComplete = false;
    
    if (g_state == "ST_WAIT_ACK"){
        //timer_tout_sot.resetTimer();

    //flag1 = true;
  //Serial.print(input);
        if(input == "EV_TOUT_SOT\n") {
            //Serial.print("Sot timeout. Reenviando SoT\n");
            Serial.print("***SoT***\n");
            input = "";
            flag_timer = false;
            //send_sot();
            }

        if(input == "EV_ACK\n"){  //7
            //Serial.print("Recebido EV_ACK.\n");
            //timer_reset(&g_timer_mgr, SOT_TIMER_ID); //assim que o timer é resetado, necessita ser setado novamente, isso ocorre apenas
            //Timer1.stop();
            if(msg == 1) {  //8              // quando send_sot() for requisitada
                Serial.print("Sending first data after ACK\n");
                //send_first_data();
                g_state = "ST_SENDING_DATA";
                Timer1.initialize(100000);
            } else {      //8
                Serial.print("No data, sending an EOT\n");
                //send_eot(); //9
                g_state = "ST_RECEIVING_DATA"; //10
                Timer1.initialize(500000);
            }
        }
        /*else{
        Serial.print("Error-wait-ack\n");
            }*/
    }
    if(g_state == "ST_SENDING_DATA"){

      //Timer1.initialize(1000000);
        if(input == "EV_TOUT_SESSION_TX\n") {
            g_session_over = 1;
            input = "";
            flag_timer = false;
        }
        if(input == "EV_DATA_SENT\n") {
            if (g_session_over || !msg) {
                //timer_reset(&g_timer_mgr, DOWNLINK_TIMER_ID);
                g_session_over = 0;
                Serial.print("EOT\n");
                //send_eot();
                g_state = "ST_RECEIVING_DATA";
            } else
                  {
                //send_data();
                
                  }
        }
        /*else{
        Serial.print("Error-sending-data\n");
            }*/        
     }
        
    if(g_state == "ST_RECEIVING_DATA"){
        if(input == "EV_TOUT_NO_RX\n") {
            //timer_reset(&g_timer_mgr, UPLINK_TIMER_ID); //12 reseta o tout_session
            //send_sot();
            g_state = "ST_WAIT_ACK";
        }
        if(input == "EV_EOT\n"){ //11
            // Resetar o temporizador de sessão
            if (msg) { //12
                //timer_reset(&g_timer_mgr, UPLINK_TIMER_ID);
                //timer_reset(&g_timer_mgr, UPLINK_RX_TIMER_ID);
                //send_first_data();
                g_state = "ST_SENDING_DATA"; 
            } else
                {
                //send_eot();//13
                }
        }
        if(input == "EV_DADOS\n"){ //11
            //processa_dados();
        }
        if(input == "EV_TOUT_SESSION_RX\n"){ //11
            //tout_rx_sessao();
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

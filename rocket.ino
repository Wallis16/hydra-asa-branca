#include<NoDelayTimerTask.h>

//#define ARRAYSIZE 70
//#define ARRAYSIZE2 140

NoDelayTimerTask timer_tout_sot(2000);

String g_state = "ST_WAIT_ACK";
bool msg = 0;
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

  pinMode(7, INPUT);
}

void loop() {

    if (stringComplete) {             
//Serial.print(inputString);
    msg = digitalRead(7);
    input = inputString;
    inputString = "";
    stringComplete = false;
    }
        
    if (g_state == "ST_WAIT_ACK"){
        //timer_tout_sot.resetTimer();

        if (timer_tout_sot.isTimeUp())
          {
            timer_tout_sot.resetTimer();
            input = "EV_TOUT_SOT\n";
          }
      
        if(input == "EV_TOUT_SOT\n") {
            Serial.print("Sot timeout. Reenviando SoT\n");
            //Serial.print("***SoT***\n");
            //send_sot();
            }

        if(input == "EV_ACK\n"){  //7
            Serial.print("Recebido EV_ACK.\n");
            //timer_reset(&g_timer_mgr, SOT_TIMER_ID); //assim que o timer é resetado, necessita ser setado novamente, isso ocorre apenas
            if(msg == 1) {  //8              // quando send_sot() for requisitada
                Serial.print("Sending first data after ACK\n");
                //send_first_data();
                g_state = "ST_SENDING_DATA";
            } else {      //8
                Serial.print("No data, sending an EOT\n");
                //send_eot(); //9
                g_state = "ST_RECEIVING_DATA"; //10
            }
        }
        /*else{
        Serial.print("Error-wait-ack\n");
            }*/
    }
    if(g_state == "ST_SENDING_DATA"){
      
        if(input == "EV_TOUT_SESSION_TX\n") {
            g_session_over = 1;
        }
        if(input == "EV_DATA_SENT\n") {
            if (g_session_over || !msg) {
                //timer_reset(&g_timer_mgr, DOWNLINK_TIMER_ID);
                g_session_over = 0;
                //send_eot();
                g_state = "ST_RECEIVING_DATA";
            } else
                  {
                //send_data();
                
                  }
        }
        else{
        Serial.print("Error-sending-data\n");
            }        
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
        else
        {
        Serial.print("Error-rec-data\n");
        }
                
    }
    input = "";
    //delay(1000);
  //}           
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar; 
    
      if (inChar == '\n') 
      {
      stringComplete = true;
      }
                            
  }
}

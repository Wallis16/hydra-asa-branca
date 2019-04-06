//const char* EV_NAMES[] = {"EV_ACK", "EV_SOT", "EV_TOUT_SOT"," EV_TOUT", "EV_EOT", "EV_DATA_RECEIVED",
// "EV_SEND_DATA_BASE", "EV_TOUT_SESSAO", "EV_MAX", "desconhecido"
//};

//const char* ST_NAMES[] = {"ST_INIT", "ST_WAIT_SOT", "ST_WAIT_DATA", "ST_SEND_DATA", "desconhecido"
//};

String g_state = "ST_WAIT_SOT";

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

String input;

void setup() {
  Serial.begin(115200);
  inputString.reserve(200);

  //pinMode(7, INPUT);
}

void loop() {

    if (stringComplete) {

    //msg = digitalRead(7);
    input = inputString;
    inputString = "";
    stringComplete = false;

 if (g_state == "ST_WAIT_SOT") {
        if (input == "EV_SOT\n") {
            Serial.print("Recebido SoT, enviando ACK, indo para ST_WAIT_DATA.\n");
            //processa_sot();
            g_state = "ST_WAIT_DATA";
            }
         else
            {
              Serial.print("Error-wait-sot\n");
            }
 }

 if (g_state == "ST_WAIT_DATA") {
        if (input == "EV_SOT2\n"){
            Serial.print("Recebido um SoT nao-esperado, indo para ST_WAIT_SOT\n");
            //unexpected_sot();
            g_state = "ST_WAIT_SOT";
            }
        if (input == "EV_TOUT_NO_RX\n") {
            /* TODO: por seguranÃ§a e informaÃ§Ã£o, se nÃ£o recebermos nada em um determinado perÃ­odo de tempo, voltamos para o estado de espera por link. */
            g_state = "ST_WAIT_SOT";
        }
        if (input == "EV_TOUT_SESSAO\n"){
            //tout_sessao();
            }
        if (input == "EV_DATA_RECEIVED\n"){
            //processa_dados();
        }
        if (input == "EV_EOT\n"){
            Serial.print("Recebido EOT, indo para ST_SEND_DATA\n");
            //processa_send_data_base_begin();
            //g_state = ST_SEND_DATA;
        }
        else
        {
          Serial.print("Error-wait-data\n");
        }
   }

    if (g_state == "ST_SEND_DATA"){
        
        if (input == "EV_SEND_DATA_BASE\n"){
            //processa_send_data_base();
        }
        if (input == "EV_TOUT\n"){
            Serial.print("Timeout ocorrido, indo para ST_WAIT_DATA\n");
            //processa_tout();
            g_state = "ST_WAIT_DATA";
        }
        if (input == "EV_EOT\n"){
            Serial.print("Enviando EOT, indo para ST_WAIT_DATA\n");
            //processa_eot_base();
            g_state = "ST_WAIT_DATA";
        }
        else
        {
          Serial.print("Error-send-data\n");
        }    
    }
    
  }
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

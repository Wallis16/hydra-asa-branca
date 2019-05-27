//ASA BRANCA - HYDRA -- DIÓGENES                  ---- ESTOU LENDO UM PUTA LIVRO SOBRE UM CANADENSE QUE SE TORNOU UM ASTRONAUTA        --- 26/05/2019 -- ao som de nada e estou sem internet  
**************vale a pena usar algum artifício para indicar que o canal está estabelecido, mesmo sem haver dados a serem enviados?
String g_state = "ST_WAIT_SOT";
String msg_info = "";
bool msg, flag1, flag_timer = 0;
bool g_session_over = 0;
int g_session_over2 = 0;

//criar buffer e flag
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

String input;

void setup() {
  
  Serial.begin(115200);
  inputString.reserve(200);

  msg_info.reserve(200);
  msg_info = "Hydra\n";
  
  cli();//stop interrupts

//set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12 and CS10 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  
  sei();//allow interrupts

}

ISR(TIMER1_COMPA_vect){//timer1 interrupt 1Hz toggles pin 13 (LED)
//generates pulse wave of frequency 1Hz/2 = 0.5kHz (takes two cycles for full wave- toggle high then toggle low)

/*  if(g_state == "ST_WAIT_SOT"){
    input = "EV_TOUT_SOT\n";
    //Serial.print(input);
  }*/
  if(g_state == "ST_WAIT_DATA"){
    input = "EV_TOUT_NO_RX\n";
  }
  if(g_state == "ST_SEND_DATA"){
    input = "EV_TOUT_NO_RX\n";
  }
    
  //flag_timer = true;

}

void loop() {


    if(msg_info.length() > 0)
    {
      msg = true;
    }

    if (stringComplete) {

    //msg = digitalRead(7);
    input = inputString;
    inputString = "";
    stringComplete = false;
    }

 if (g_state == "ST_WAIT_SOT") {
        if (input == "EV_SOT\n") {
            Serial.print("Recebido SoT, enviando ACK, indo para ST_WAIT_DATA.\n");
            //processa_sot();
            g_state = "ST_WAIT_DATA";
            }
         else
            {
              Serial.print("Error-wait-sot\n");
              delay(600);
            }
 }

 if (g_state == "ST_WAIT_DATA") {
        if (input == "EV_SOT2\n"){
            Serial.print("Recebido um SoT nao-esperado, indo para ST_WAIT_SOT\n");
            //unexpected_sot();
            g_state = "ST_WAIT_SOT";
            }
        /*if (input == "EV_TOUT_NO_RX\n") {
            // TODO: por seguranÃ§a e informaÃ§Ã£o, se nÃ£o recebermos nada em um determinado perÃ­odo de tempo, voltamos para o estado de espera por link. 
            g_state = "ST_WAIT_SOT";
        }
        if (input == "EV_TOUT_SESSAO\n"){
            Serial.print("Recebido EOT, indo para ST_SEND_DATA\n");
            }*/

        if(msg == true){ //11
          //processar dados recebidos
            //processa_dados();
        }

        if (input == "EV_EOT\n"){
            Serial.print("Recebido EOT, indo para ST_SEND_DATA\n");
            //processa_send_data_base_begin();
            g_state = ST_SEND_DATA;
        }
        else
        {
          Serial.print("Error-wait-data\n");
          delay(600);
        }
   }

    if (g_state == "ST_SEND_DATA"){


        if(input == "EV_TOUT_SESSION_TX\n") {
            g_session_over = 1;
            input = "";
            flag_timer = false;
            //Serial.print("DALE\n");
        }
        if(g_session_over == 1 || msg == false) {
                g_session_over = 0;
                Serial.print("EOT -- SEND_DATA\n");
                g_state = "ST_RECEIVING_DATA";
            } 

        /*if (input == "EV_SEND_DATA_BASE\n"){
            //processa_send_data_base();
        }*/
        /*
        if (input == "EV_TOUT\n"){
            Serial.print("Timeout ocorrido, indo para ST_WAIT_DATA\n");
            //processa_tout();
            g_state = "ST_WAIT_DATA";
        }
        if (input == "EV_EOT\n"){
            Serial.print("Enviando EOT, indo para ST_WAIT_DATA\n");
            //processa_eot_base();
            g_state = "ST_WAIT_DATA";
        }*/
        else
        {
          Serial.print("Error-send-data\n");
          delay(600);
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

//ASA BRANCA - HYDRA -- DIÓGENES                     ---- VOCÊ SÓ TEM UMA CHANCE, ESTEJA PRONTO E CORRA ATRÁS DO QUE É SEU        --- 24/05/2019 -- ao som de um bom Jazz  

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

 //Serial.print("controle\n");
  if(g_state == "ST_WAIT_ACK"){
    input = "EV_TOUT_SOT\n";
    //Serial.print(input);
  }
  if(g_state == "ST_SENDING_DATA"){
    input = "EV_TOUT_SESSION_TX\n";
  }
  if(g_state == "ST_RECEIVING_DATA"){
    input = "EV_TOUT_NO_RX\n";
  }
    
  //flag_timer = true;

}

void loop() {


    if(msg_info.length() > 0)
    {
      msg = true;
    }
  
    if (stringComplete == true){             
    
    input = inputString;
    inputString = "";
    stringComplete = false;
    }
    
    if (g_state == "ST_WAIT_ACK"){
      
        if(input == "EV_TOUT_SOT\n") {      
            Serial.print("SoT -- WAIT_ACK\n");
            input = "";
            flag_timer = false;
            
            }
        
        if(input == "EV_ACK\n"){ 
            
            if(msg == 1) {  
                Serial.print("Hydra -- WAIT_ACK\n");
                g_state = "ST_SENDING_DATA";
                OCR1A = 15624;
            } else {      
                Serial.print("EOT -- WAIT_ACK\n");
                g_state = "ST_RECEIVING_DATA"; 
                OCR1A = 60000;
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
            OCR1A = 50000;
            g_state = "ST_WAIT_ACK";
            }
       
        if(input == "EV_EOT\n"){ //11
            if (msg == true) { //12
                Serial.print("Hydra -- RECEIVING_DATA\n");
                g_state = "ST_SENDING_DATA";
                input=""; 
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

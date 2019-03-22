#define ARRAYSIZE 300
#define ARRAYSIZE2 500

String g_state;

String ST_names[ARRAYSIZE] = { "ST_INIT", "ST_WAIT_ACK", "ST_SENDING_DATA", "ST_RECEIVING_DATA",
                          "desconhecido" };

String EV_NAMES[ARRAYSIZE2] = {"EV_ACK", "EV_SOT", "EV_TOUT_SOT", "EV_TOUT_SESSION_TX",
    "EV_TOUT_NO_RX", "EV_TOUT_SESSION_RX", "EV_EOT", "EV_DADOS", "EV_DATA_SENT",
    "EV_NO_DATA", "EV_INVALID", "EV_MAX", "desconhecido"
};

//criar buffer e flag
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

//definir tamanho do buffer
void setup() {
  // initialize serial:
  Serial.begin(115200);
  // reserve 200 bytes for the inputString:
  //inputString.reserve(200);
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);  
  
String g_state = "ST_WAIT_ACK";
}

//loop para printar o byte recebido
//uma vez recebido o byte, mostra-se na tela e limpa-se o buffer e desabilita o flag

void loop() {
  // print the string when a newline arrives:
  /*if (stringComplete) {
    Serial.println(inputString);
  if(inputString == "a\n"){
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
    }
    */
    // clear the string:
    if (stringComplete) {
    inputString = "";
    stringComplete = false;
  }
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/


void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar; //inChar recebe caractere por carctere e assim são add para formar uma string
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:

    //switch (g_state) {
    if (g_state == "ST_WAIT_ACK"):
        switch (inputString) {
        case "EV_TOUT_SOT\n": //7
            Serial.print("Sot timeout. Reenviando SoT\n");
            //send_sot();
            break;
        case "EV_ACK\n":  //7
            Serial.print("Recebido EV_ACK.\n");
            //timer_reset(&g_timer_mgr, SOT_TIMER_ID); //assim que o timer é resetado, necessita ser setado novamente, isso ocorre apenas
            //if(has_message(&g_msg_mgr)) {  //8              // quando send_sot() for requisitada
                //printf("Sending first data after ACK\n");
                //send_first_data();
                //g_state = ST_SENDING_DATA;
            //} else {      //8
                //printf("No data, sending an EOT\n");
                //send_eot(); //9
                //g_state = "ST_RECEIVING_DATA"; //10
            //}
            break;
        default:
            Serial.print("Em WaitACK recebeu e ignorou (%s)\n", EV_NAMES[12]);
            break;
        }
        break;

    case "ST_SENDING_DATA":
        switch (inputString) {
        case "EV_TOUT_SESSION_TX\n":
            //g_session_over = 1;
            break;
        case "EV_DATA_SENT\n":
            /*if (g_session_over || !has_message(&g_msg_mgr)) {
                timer_reset(&g_timer_mgr, DOWNLINK_TIMER_ID);
                g_session_over = 0;
                send_eot();
                g_state = ST_RECEIVING_DATA;
            } else
                send_data();*/
            break;
        default:
            Serial.print("Em ST_SENDING_DATA recebeu (%s)\n", EV_NAMES[12]);
            break;
        }
        break;

    case "ST_RECEIVING_DATA":
        switch (inputString) {
        case "EV_TOUT_NO_RX\n": //11
            //timer_reset(&g_timer_mgr, UPLINK_TIMER_ID); //12 reseta o tout_session
            //send_sot();
            g_state = "ST_WAIT_ACK";
            break;
        case "EV_EOT\n": //11
            // Resetar o temporizador de sessão
   /*         if (has_message(&g_msg_mgr)) { //12
                timer_reset(&g_timer_mgr, UPLINK_TIMER_ID);
                timer_reset(&g_timer_mgr, UPLINK_RX_TIMER_ID);
                send_first_data();
                g_state = "ST_SENDING_DATA"; 
            } else
                send_eot();//13  */
            break;
        case "EV_DADOS\n": //11
            //processa_dados();
            break;
        case "EV_TOUT_SESSION_RX\n": //11
            //tout_rx_sessao();
            break;
        default:
            Serial.print("Em ST_RECEIVING_DATA recebeu (%s)\n", EV_NAMES[12]);
            break;
        }
        break;

    default:
        break;
    }
  
    if (inChar == '\n') {
      stringComplete = true;

    }
  }
}

int byte_recebido = 0;
bool aux = 0;

void setup() {
  Serial.begin(115200);
  pinMode(7, INPUT);
}


void loop() {

  aux = digitalRead(7);

  if (aux == 1){
  delay(1000);
  Serial.write(23);
  }

  else{
    
  if(Serial.available()>0){
    byte_recebido = Serial.read();
    Serial.print(byte_recebido); 
  }
  
  }

  
}

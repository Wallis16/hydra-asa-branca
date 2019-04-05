int byte_recebido = 0;
bool aux = 0;

void setup() {
  Serial.begin(115200);
  pinMode(8, INPUT);
}


void loop() {

  aux = digitalRead(8);

  if (aux == 0){
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



void setup() 
{
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  //Serial.begin(9600);
}
void loop() 
{ 
  if(Serial.available())
  {
    digitalWrite(13, HIGH);
    String a = Serial.readString();
    Serial.println(a);
  }
}

void setup() {
  Serial.begin(115200); 
}

String READ() {
  String out = "";
  
  if (Serial.available() > 0) {
    delay(10); 
    
    while (Serial.available() > 0) {
      char c = Serial.read(); 
      out += c;
    }
    //Serial.println(out);
  }
  return out;
}

void Blink(int times){
  for(int i=0;i++;i<times){
    digitalWrite( , HIGH);
    delay(500);
    digitalWrite( , LOW);
    delay(500);
  }
}

void M1(String in){
  Serial.print("This is Module 1:");
  String SubStr = in.substring(3,in.length()); 
  int val = SubStr.toInt(); 

  Serial.print(val);
  Blink(val);
  Serial.println();
  }

void M2(){Serial.println("This is Module 2");}
void M3(){Serial.println("This is Module 3");}

void loop() {
  String out = READ();

  if(out.length() > 0){
    if(out.substring(0,2).equals("M1")){M1(out);}
    if(out.substring(0,2).equals("M2")){M2();}
    if(out.substring(0,2).equals("M3")){M3();}
  }
}






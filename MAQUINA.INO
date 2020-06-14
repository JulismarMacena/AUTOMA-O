#include <avr/wdt.h>
#include <LCD5110_Graph.h>

LCD5110 lcd(8, 9, 10, 12, 11);
extern unsigned char MediumFont[];
extern unsigned char TinyFont[];
extern uint8_t maquina[];
extern uint8_t imgMOLHO1[];
extern uint8_t lavar1[];
extern uint8_t cretifugar[];
extern uint8_t agua1[];
extern uint8_t jie[];

float y;
uint8_t* bm;
extern uint8_t SmallFont[];


byte BOTAO = A0; //SELECIONA A FUNÇÃO
byte SENSOR = 2; //SENSOR
byte BOMBAAGUA = 3; //LED PINO 3
byte MOTORD = 4; //LED PINO 4
byte MOTORC = 5; //LED PINO 5
byte BOMBASAIDA = 6; //BOMBA DE RETIRA DE AGUA
byte CENTRIFUGA = 7; //BOMBA DE RETIRA DE AGUA
int vezes = 1; //quantidade de loop
int vezes2 = 1; //quantidade de loop


int LM = 2000;
int DM = 2000;
int ESTADOSENSOR = 0;

boolean LAVAR = false;
boolean ENXAGUAR = false;
boolean CENTRIFUGAR = false;
boolean MOLHO1 = false;
boolean MOLHO2 = false;

byte digitalReadBOTAO(byte val) {
  static byte lastVal = HIGH;
  static unsigned long m = 0;
  if (lastVal != val && millis() > (m + 100) ) {
    lastVal = val;
    m = millis();
    return lastVal;
  }
  return HIGH;
}

int getCommand() {
  static unsigned long m1 = 0;  //millis no momento inicia de pressionar o botão
  static unsigned long m2 = 0;  //millis após soltar o botão
  static byte count = 0;

  byte r2 = digitalRead(SENSOR);
  byte r = digitalRead(BOTAO);
  if (digitalReadBOTAO(r) == LOW) {
    m1 = millis();
    count++;
  }

  if (r == LOW) {
    m2 = millis();
  } else {
    if (! (m2 > 0 && m2 - m1 < 2000) ) { //o botão deve ser pressionado por menos de 1 segundo, senão cancela o comando
      count = 0;
      m1 = 0;
      m2 = 0;
    }

    if (m2 > 0 && millis() - m2 > 2500) { //após a ultima vez pressionado o botao, aguarda 1,5 segundos para finalizar e retornar o comando.
      byte c = count;
      count = 0;
      m1 = 0;
      m2 = 0;

      return c;
    }
  }

  return 0;
}


void setup() {
  Serial.begin(115200);
  Serial.println("Iniciando a Maquina");

  pinMode(BOTAO, INPUT_PULLUP); //BOTAO DE ENTRADA DE COMANDOA[
  pinMode(SENSOR, INPUT); //ENTRADA SENSOR PORTA D3
  lcd.InitLCD();
  lcd.setFont(SmallFont);


  pinMode(BOMBAAGUA, OUTPUT); //LED PINO 3
  digitalWrite(BOMBAAGUA, LOW);
  pinMode(MOTORD, OUTPUT); //LED PINO 4
  digitalWrite(MOTORD, LOW);
  pinMode(MOTORC, OUTPUT); //LED PINO 5
  digitalWrite(MOTORC, LOW);
  pinMode(BOMBASAIDA, OUTPUT); //LED PINO 6
  digitalWrite(BOMBASAIDA, LOW);
  pinMode(CENTRIFUGA, OUTPUT); //LED PINO 6
  digitalWrite(CENTRIFUGA, LOW);

  //while (vezes <= 1) {
  lcd.clrScr();
  lcd.drawBitmap(0, 0, jie, 84, 48);
  lcd.print("OLA!", 2, 2);
  lcd.drawRect(0, 0, 83, 47);
  for (int i = 0; i < 29; i++)
  {
    lcd.invPixel(i, (sin(y * 1) * 2) + 1);
    lcd.invPixel(i, (sin(y * 1) * 2) + 2);
    lcd.invPixel(i, (sin(y * 1) * 2) + 3);
    lcd.invPixel(i, (sin(y * 1) * 2) + 4);
    lcd.invPixel(i, (sin(y * 1) * 2) + 5);
    lcd.invPixel(i, (sin(y * 1) * 2) + 6);
    lcd.invPixel(i, (sin(y * 1) * 2) + 7);
    lcd.invPixel(i, (sin(y * 1) * 2) + 8);
    lcd.invPixel(i, (sin(y * 1) * 2) + 9);
    lcd.invPixel(i, (sin(y * 1) * 2) + 10);
    lcd.update();
    delay(1);
    //   }
    lcd.update();
    delay(100);
  }
}

void loop() {

  int command = getCommand();
  //INICIAR FUNÇÃO LAVAR
  if (command == 1) //LAVAR
  {
    Serial.println("ENCHENDO A MAQUINA DE AGUA");
    MOLHO1 = true;
    digitalWrite(BOMBAAGUA, HIGH);
    lcd.clrScr();
    lcd.drawBitmap(0, 0, agua1, 84, 48);
    lcd.print("AGUA!", 50, 0);
    Serial.println("AGUARDANDO NIVEL DE AGUA 1");
    lcd.update();
  }
  //INICIAR ENXAGUE
  if (command == 2) //ENXAGUE
  {
    Serial.println("ENCHENDO A MAQUINA DE AGUA 2");
    MOLHO2 = true;
    digitalWrite(BOMBAAGUA, HIGH);
    lcd.clrScr();
    lcd.drawBitmap(0, 0, agua1, 84, 48);
    lcd.print("AGUA 2!", 39, 0);
    Serial.println("AGUARDANDO NIVEL DE AGUA 2");
    lcd.update();
  }

  if (command == 3) //ENXAGUE
  {
    CENTRIFUGAR = true;
  }

  //FUNÇÃO MOLHO TEMPO DO MOLHO NO DALEY
  //FUNÇÃO LAVA 1X
  if (digitalRead(2) == HIGH && MOLHO1 == true)
  {
    Serial.println("MOLHO 1...");
    digitalWrite(BOMBAAGUA, LOW);
    lcd.clrScr();
    lcd.drawBitmap(0, 0, imgMOLHO1, 84, 48);
    lcd.print("MOLHO!", 50, 0);
    lcd.update();
    delay(5000);
    LAVAR = true;
    MOLHO1 = false;


  }
  if (CENTRIFUGAR == true)
  {

    digitalWrite(CENTRIFUGA, HIGH);
    delay(300);
    digitalWrite(MOTORD, HIGH); //Botão pressionado, acende o led.
    delay(10000);
    digitalWrite(MOTORD, LOW); //Botão pressionado, acende o led.
    delay(10000);
    digitalWrite(BOMBASAIDA, LOW);
    digitalWrite(CENTRIFUGA, LOW);
    Serial.println("REINICIANDO");
    delay(5000);
    asm volatile ( "jmp 0");
  }

  if (LAVAR == true)
  {
    LAVAR = false;
    lcd.clrScr();
    lcd.drawBitmap(0, 0, lavar1, 84, 48);
    lcd.print("LAVANDO!", 39, 0);
    lcd.update();
    Serial.println("LAVANDO...");
    lcd.clrScr();
    lcd.drawBitmap(0, 0, lavar1, 84, 48);
    lcd.print("LAVANDO!", 39, 0);
    lcd.update();
    while (vezes <= 1)
    {
      digitalWrite(MOTORD, HIGH); //Botão pressionado, acende o led.
      Serial.println("M1");
      delay (LM);
      digitalWrite(MOTORD, LOW);
      Serial.println("M2");
      delay (DM);
      digitalWrite(MOTORC, HIGH); //Botão pressionado, acende o led.
      Serial.println("M3");
      delay (LM);
      digitalWrite(MOTORC, LOW);
      Serial.println("M4");
      delay (DM);
      vezes++;
    }
    digitalWrite(BOMBASAIDA, HIGH);
    Serial.println("REMOVENDO AGUA 1!");
    lcd.clrScr();
    lcd.drawBitmap(0, 0, maquina , 84, 48);
    lcd.print("REMORVENDO!", 39, 0);
    lcd.print("AGUA!", 39, 10);
    lcd.update();
    delay(10000);

    digitalWrite(BOMBASAIDA, LOW);
    lcd.clrScr();
    lcd.drawBitmap(0, 0, maquina , 84, 48);
    lcd.print("LAVADO!", 39, 0);
    lcd.update();
    Serial.println("LAVADO!");
    delay(5000);
    MOLHO2 = true;
    digitalWrite(BOMBAAGUA, HIGH);
    lcd.clrScr();
    lcd.drawBitmap(0, 0, agua1 , 84, 48);
    lcd.update();
    Serial.println("AGUARDANDO NIVEL DE AGUA!");

  }
  // ENXAGUE
  if (digitalRead(2) == HIGH && MOLHO2 == true)
  {
    Serial.println("MOLHO 2...");
    digitalWrite(BOMBAAGUA, LOW);
    lcd.clrScr();
    lcd.drawBitmap(0, 0, imgMOLHO1, 84, 48);
    lcd.print("MOLHO!", 45, 0);
    lcd.update();
    delay(5000);
    MOLHO2 = false;
    lcd.clrScr();
    lcd.drawBitmap(0, 0, lavar1, 84, 48);
    lcd.print("ENXAGUE !", 39, 0);
    lcd.update();

    while (vezes2 <= 1 )
    {
      digitalWrite(MOTORD, HIGH); //Botão pressionado, acende o led.
      Serial.println("M1 ENXAGUE");
      delay (LM);

      digitalWrite(MOTORD, LOW);
      Serial.println("M2 ENXAGUE");
      delay (DM);

      digitalWrite(MOTORC, HIGH); //Botão pressionado, acende o led.
      Serial.println("M3 ENXAGUE");
      delay (LM);

      digitalWrite(MOTORC, LOW);
      Serial.println("M4 ENXAGUE");
      delay (DM);
      vezes2++;
    }
    digitalWrite(BOMBASAIDA, HIGH);
    Serial.println("REMOVENDO AGUA 2...");
    lcd.clrScr();
    lcd.drawBitmap(0, 0, maquina , 84, 48);
    lcd.print("REMORVENDO!", 39, 0);
    lcd.print("AGUA!", 39, 10);
    lcd.update();
    delay(10000);
    Serial.println("FIM DO MOLHO 2");
    CENTRIFUGAR = true;
    Serial.println("INICIANDO CENTRIFUGAÇÃO!");
    lcd.clrScr();
    lcd.drawBitmap(0, 0, maquina , 84, 48);
    lcd.print("CENTRIFUGANDO", 29, 0);
    lcd.update();
    delay(5000);
    lcd.clrScr();
    lcd.drawBitmap(0, 0, cretifugar , 84, 48);
    lcd.print("", 39, 0);
    lcd.update();
  }

  //FUNÇÃO RESET
  if (command == 4) //comando reseta arduino
  {
    Serial.println("REINICIANDO");
    delay(5000);
    asm volatile ( "jmp 0");
  }
}

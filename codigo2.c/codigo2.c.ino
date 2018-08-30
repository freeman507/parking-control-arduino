#include <SPI.h>
#include <Ethernet.h>
#include <MFRC522.h>
#include <Ultrasonic.h> 

#define pino_trigger 5
#define pino_echo 6

#define SS_PIN 2
#define RST_PIN 3

Ultrasonic ultrasonic(pino_trigger, pino_echo);

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 0, 101);

IPAddress server(192, 168, 0, 102);

EthernetClient client;
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.

void setup()
{

    SPI.begin(); // Inicia  SPI bus
    mfrc522.PCD_Init(); // Inicia MFRC522
    Serial.println("Aproxime o seu cartao do leitor...");
    Serial.println();

    // start the Ethernet connection:
    Ethernet.begin(mac, ip);
    // Open serial communications and wait for port to open:
    Serial.begin(9600);
    while (!Serial)
    {
        ; // wait for serial port to connect. Needed for native USB port only
    }


    // give the Ethernet shield a second to initialize:
    delay(1000);
    Serial.println("connecting...");

    // if you get a connection, report back via serial:
    if (client.connect(server, 10002))
    {
        Serial.println("connected");
    }
    else
    {
        // if you didn't get a connection to the server:
        Serial.println("connection failed");
    }
}

bool verificaBotton()
{
    return mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial();
}

boolean verificaVaga() {
   long distancia = ultrasonic.Ranging(CM);
   //Serial.print("distancia: ");
   //Serial.println(distancia);
   return distancia < 5;
}

void loop()
{
    bool ocupado = verificaVaga() || verificaVaga() || verificaVaga() ||  verificaBotton();

    if(ocupado)
    {
        Serial.println("ocupado");
        client.print("1");
    }
    else
    {
        Serial.println("disponivel");
        client.print("0");
    }

    // if the server's disconnected, stop the client:
    if (!client.connected())
    {
        Serial.println();
        Serial.println("disconnecting.");
        client.stop();
        // do nothing:
        //while (true);
        setup();
        loop();
    }
    delay(500);
}


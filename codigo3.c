#include <SPI.h>
#include <Ethernet.h>
#include <MFRC522.h>

#define SS_PIN 2
#define RST_PIN 3

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 0, 110);

IPAddress server(192, 168, 0, 100);

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

void loop()
{
    bool ocupado = verificaBotton();

    // if there are incoming bytes available
    // from the server, read them and print them:
    if (client.available())
    {
        char c = client.read();
        Serial.print(c);

    }

    if(ocupado)
    {
        Serial.println("1");
        client.print("1");
    }
    else
    {
        Serial.println("0");
        client.print("0");
    }



    // if the server's disconnected, stop the client:
    if (!client.connected())
    {
        Serial.println();
        Serial.println("disconnecting.");
        client.stop();
        // do nothing:
        while (true);
    }
}

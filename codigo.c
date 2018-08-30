#include <SPI.h>
#include <Ethernet.h>
#include <MFRC522.h>

#define SS_PIN 2
#define RST_PIN 3

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 0, 110);
IPAddress dns(192,168,0, 1);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

EthernetClient client;

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.

void setup()
{

    SPI.begin(); // Inicia  SPI bus
    mfrc522.PCD_Init(); // Inicia MFRC522
    Serial.println("Aproxime o seu cartao do leitor...");
    Serial.println();

    // initialize the ethernet device
    Ethernet.begin(mac, ip, dns, gateway, subnet);
    // start listening for clients
    server.begin();
    // Open serial communications and wait for port to open:
    Serial.begin(9600);
    while (!Serial)
    {
        ; // wait for serial port to connect. Needed for native USB port only
    }


    Serial.print("Chat server address:");
    Serial.println(Ethernet.localIP());
}

bool verificaBotton()
{
    return mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial();
}

void loop()
{
    bool ocupado = verificaBotton();

    // wait for a new client:
    EthernetClient client = server.available();

    // when the client sends the first byte, say hello:
    if (client)
    {
        if (!alreadyConnected)
        {
            // clear out the input buffer:
            client.flush();
            Serial.println("We have a new client");
            client.println("Hello, client!");
            alreadyConnected = true;
        }

        if (client.available() > 0)
        {
            // read the bytes incoming from the client:
            char thisChar = client.read();
            // echo the bytes back to the client:
            server.write(thisChar);
            // echo the bytes to the server as well:
            Serial.write(thisChar);
        }
    }
}

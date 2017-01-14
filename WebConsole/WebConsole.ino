/*
 Web Console

 Based on the Arduino Web Server example by Mellis, Igoe 
 and Guadalupi.

 This sketch targets the Freetronics EtherTen board which
 is an Uno with onboard ethernet and microSD card.
 
 Circuit:
 * Ethernet attached to pins 10, 11, 12, 13
 * MicroSD attached to 4 (select), 11, 12, 13

 created 14 Jan, 2017
 by Stephen Davies
 
 */

#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>

const int sdSelect = 4;

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 177);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

boolean sdAvailable = true;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());

  // see if the SD card is present and can be initialized
  if (!SD.begin(sdSelect)) {
    sdAvailable = false;
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
}

void loop() {
  if (sdAvailable)
  {
    // listen for incoming clients
    EthernetClient client = server.available();
    if (client)
    {
      Serial.println("new client");
      // an http request ends with a blank line
      boolean currentLineIsBlank = true;
      while (client.connected())
      {
        if (client.available())
        {
          char c = client.read();
          Serial.write(c);
          // if you've gotten to the end of the line (received a newline
          // character) and the line is blank, the http request has ended,
          // so you can send a reply
          if (c == '\n' && currentLineIsBlank)
          {
            File dataFile = SD.open("index.htm");

            if (dataFile)
            {
              sendHeader(&client, "200", "OK", "text/html");
              sendData(&client, &dataFile);
            }
            break;
          }
          if (c == '\n')
          {
            // you're starting a new line
            currentLineIsBlank = true;
          } else if (c != '\r')
          {
            // you've gotten a character on the current line
            currentLineIsBlank = false;
          }
        }
      }
      // give the web browser time to receive the data
      delay(1);
      // close the connection:
      client.stop();
      Serial.println("client disconnected");
    }
  }
}

/*
 * Send the entire HTTP response header.
 * Content type heade will be supressed if type is null.
 */
void sendHeader(EthernetClient * client, char * code, char * phrase, char * type)
{
  sendStatusLine(client, code, phrase);
  if (type)
  {
    sendHeaderLine(client, "Content-Type", type);
  }
  sendHeaderLine(client, "Connection", "close");  // the connection will be closed after completion of the response
  client->println();
}

/*
 * Sends first line of HTTP response,
 * E.g. HTTP/1.1 200 OK
 */
void sendStatusLine(EthernetClient * client, char * code, char * phrase)
{
  client->print("HTTP/1.1 ");
  client->print(code);
  client->print(" ");
  client->println(phrase); 
}

/*
 * Send single HTTP response header line.
 */
void sendHeaderLine(EthernetClient * client, char * name, char * value)
{
  client->print(name);
  client->print(": ");
  client->println(value);
}

/*
 * Perform a file transfer from the server to the client.
 */
void sendData(EthernetClient * client, File * dataFile)
{
  while (dataFile->available())
  {
    client->write(dataFile->read());
  }
  dataFile->close();
}


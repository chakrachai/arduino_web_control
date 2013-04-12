#include <SPI.h>
#include <Ethernet.h>


byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 137, 20);
EthernetServer server(8080);
String HTTP_req;
boolean LED_status = 0;
int led = 13;


void setup()
{
    Ethernet.begin(mac, ip);
    server.begin();
    Serial.begin(9600);
    pinMode(led, OUTPUT);
}


void loop()
{
    EthernetClient client = server.available();
    if (client) {
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                //Serial.println(c);
                HTTP_req += c;
                if (c == '\n' && currentLineIsBlank) {
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println("Connection: close");
                    client.println();
                    client.println("<!DOCTYPE html>");
                    client.println("<html>");
                    client.println("<head>");
                    client.println("<title>control led</title>");
                    client.println("</head>");
                    client.println("<body bgcolor=#6666ff>");
                    client.println("<h1><font color=#ffff33>Control LED</font></h1>");
                    client.println("<p><font color=#ffffff>pass key to led</font></p>");
                    client.println("<form method=\"get\">");
                    client.println("<button name=\"Ledon\" value=\"true\" type=\"submit\">onled</button>");
                    client.println("<button name=\"Ledon\" value=\"false\" type=\"submit\">offled</button>");
                    ProcessPinButton(client);
                    client.println("</form>");
                    client.println("</body>");
                    client.println("</html>");
                    Serial.print(HTTP_req);
                    HTTP_req = "";
                    break;
                }
                if (c == '\n') {
                    currentLineIsBlank = true;
                } 
                else if (c != '\r') {
                    currentLineIsBlank = false;
                }
            }
        }
        delay(1);
        client.stop();
    }
}
void ProcessPinButton(EthernetClient cl)
{
    String s = HTTP_req.substring(6,18);
    if (s.indexOf("Ledon=true") == 0) {
            cl.println(s.indexOf("Ledon=false"));
            digitalWrite(13, HIGH);
    }
    else {
            cl.println(s.indexOf("Ledon=false"));
            digitalWrite(13, LOW);
    }
}

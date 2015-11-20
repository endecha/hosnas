#include <UIPEthernet.h>

byte mac[] = { 0x54, 0x34, 0x41, 0x30, 0x30, 0x31 };
IPAddress ip(192, 168, 0, 10);
IPAddress gateway(192, 168, 0, 252);
IPAddress mask(255, 255, 255, 0);
IPAddress domain(8, 8, 8, 8);
EthernetServer server(80);

/* Sensor values */
unsigned int temp_in;
unsigned int temp_out;
unsigned int hum;
unsigned int co2;
unsigned int dust;
unsigned int gas;

/* Fan status */
unsigned int fan_air;
unsigned int fan_hum;

void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac, ip, domain, gateway, mask);
  server.begin();
  temp_in = 0;
  temp_out = 0;
  hum = 0;
  co2 = 0;
  dust = 0;
  gas = 0;
  fan_air = 0;
  fan_hum = 0;
  //Serial.print("IP Address: ");
  //Serial.println(Ethernet.localIP());
}

void loop() {
  EthernetClient client = server.available();
  int his;

  temp_in++;
  temp_out++;
  hum++;
  co2++;
  dust++;
  gas++;

  if (client) 
  {  
    Serial.println("-> New Connection");

    boolean currentLineIsBlank = true;

    while (client.connected()) 
    {
      if (client.available()) 
      {
        char c = client.read();

        if (c == '\n' && currentLineIsBlank) 
        {
          client.println("<html><title>Air Conditioning</title><body>");

          /* Fan status */
          client.println("<tr>");
          client.println("<table border=5 width=100% bordercolor=white>");
          client.println("<tr>");
          client.println("<td height=100px width=50%>");
          client.println("<font size=15><b>Air Fan</b></font>");
          client.println("</td>");
          client.println("<td height=100px>");
          client.println("<font size=15><b>Hum. Fan</b></font>");
          client.println("</td>");
          client.println("</tr>");
          client.println("<tr>");
          if(dust%5 > 3) {
            client.println("<td align=center bgcolor=yellow>");
            client.println("<font size=10><b>Run</b></font>");
            client.println("</td>");
          }
          else {
            client.println("<td align=center bgcolor=green>");
            client.println("<font size=10><b>Stop</b></font>");
            client.println("</td>");
          }
          if(dust%5 < 3) {
            client.println("<td align=center bgcolor=yellow>");
            client.println("<font size=10><b>Run</b></font>");
            client.println("</td>");
          }
          else {
            client.println("<td align=center bgcolor=green>");
            client.println("<font size=10><b>Stop</b></font>");
            client.println("</td>");
          }
          client.println("</tr>");
          client.println("</table>");

          client.println("<br><br><br>");

          /* Current status */
          client.println("<table border=5 width=100% bordercolor=white>");
          client.println("<tr>");
          client.println("<td height=150px width=50%>");
          client.println("<font size=15><b>Temperature</b></font> <font size = 10>In</font>");
          client.println("</td>");
          client.println("<td align=center>");
          client.print("<font size=15><b>");
          client.print(temp_in);
          client.println("</b></font>");
          client.println("</td>");
          client.println("</tr>");

          client.println("<tr>");
          client.println("<td height=150px>");
          client.println("<font size=15><b>Temperature</b></font> <font size = 10> Out</font>");
          client.println("</td>");
          client.println("<td align=center>");
          client.print("<font size=15><b>");
          client.print(temp_out);
          client.println("</b></font>");
          client.println("</td>");
          client.println("</tr>");

          client.println("<tr>");
          client.println("<td height=150px>");
          client.println("<font size=15><b>Humidity</b></font>");
          client.println("</td>");
          client.println("<td align=center>");
          client.print("<font size=15><b>");
          client.print(hum);
          client.println("</b></font>");
          client.println("</td>");
          client.println("</tr>");

          client.println("<tr>");
          client.println("<td height=150px>");
          client.println("<font size=15><b>Co2</b></font>");
          client.println("</td>");
          client.println("<td align=center>");
          client.print("<font size=15><b>");
          client.print(co2);
          client.println("</b></font>");
          client.println("</td>");
          client.println("</tr>");

          client.println("<tr>");
          client.println("<td height=150px>");
          client.println("<font size=15><b>Dust</b></font>");
          client.println("</td>");
          client.println("<td align=center>");
          client.print("<font size=15><b>");
          client.print(dust);
          client.println("</b></font>");
          client.println("</td>");
          client.println("</tr>");

          client.println("<tr>");
          client.println("<td height=150px>");
          client.println("<font size=15><b>Gas</b></font>");
          client.println("</td>");
          client.println("<td align=center>");
          client.print("<font size=15><b>");
          client.print(gas);
          client.println("</b></font>");
          client.println("</td>");
          client.println("</tr>");
          client.println("</table><br><br>");

          client.println("</body></html>");
          break;
        }

        if (c == '\n') {
          currentLineIsBlank = true;
        }
        else if (c != '\r') 
        {
          currentLineIsBlank = false;
        }
      } 
    }

    delay(10);
    client.stop();
    Serial.println("   Disconnected\n");
  }
  delay(10);
}

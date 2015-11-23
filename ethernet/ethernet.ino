/*
  Web Server
 
 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield. 
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)
 
 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 
 */

#include <SPI.h>
#include <Ethernet.h>

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

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,0,100);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);

void setup() {
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  temp_in = 0;
  temp_out = 0;
  hum = 0;
  co2 = 0;
  dust = 0;
  gas = 0;
  fan_air = 0;
  fan_hum = 0;

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  int his;

  temp_in++;
  temp_out++;
  hum++;
  co2++;
  dust++;
  gas++;

  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
	      //client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<body>");

          /* Fan status */
          client.println("<table border=5 width=100% bordercolor=white>");
          client.println("<tr>");
          client.println("<td align=center height=100px width=50%>");
          client.println("<font size=15><b>Air Fan</b></font>");
          client.println("</td>");
          client.println("<td align=center height=100px>");
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
          client.println("<br>");

          /* Current status */
          client.println("<table border=5 width=100% bordercolor=white>");
          client.println("<tr>");
          client.println("<td height=100px width=50%>");
          client.println("<font size=20><b>Temperature</b></font> <font size = 10>In</font>");
          client.println("</td>");
          client.println("<td align=center>");
          client.print("<font size=20><b>");
          client.print(temp_in);
          client.println("</b></font>");
          client.println("</td>");
          client.println("</tr>");

          client.println("<tr>");
          client.println("<td height=100px>");
          client.println("<font size=20><b>Temperature</b></font> <font size = 10> Out</font>");
          client.println("</td>");
          client.println("<td align=center>");
          client.print("<font size=20><b>");
          client.print(temp_out);
          client.println("</b></font>");
          client.println("</td>");
          client.println("</tr>");

          client.println("<tr>");
          client.println("<td height=100px>");
          client.println("<font size=20><b>Humidity</b></font>");
          client.println("</td>");
          client.println("<td align=center>");
          client.print("<font size=20><b>");
          client.print(hum);
          client.println("</b></font>");
          client.println("</td>");
          client.println("</tr>");

          client.println("<tr>");
          client.println("<td height=100px>");
          client.println("<font size=20><b>Co2</b></font>");
          client.println("</td>");
          client.println("<td align=center>");
          client.print("<font size=20><b>");
          client.print(co2);
          client.println("</b></font>");
          client.println("</td>");
          client.println("</tr>");

          client.println("<tr>");
          client.println("<td height=100px>");
          client.println("<font size=20><b>Dust</b></font>");
          client.println("</td>");
          client.println("<td align=center>");
          client.print("<font size=20><b>");
          client.print(dust);
          client.println("</b></font>");
          client.println("</td>");
          client.println("</tr>");

          client.println("<tr>");
          client.println("<td height=100px>");
          client.println("<font size=20><b>Gas</b></font>");
          client.println("</td>");
          client.println("<td align=center>");
          client.print("<font size=20><b>");
          client.print(gas);
          client.println("</b></font>");
          client.println("</td>");
          client.println("</tr>");
          client.println("</table><br><br>");

          client.println("<font size=10><b>Temperature</b></font><br>");
            client.println("<table border=0 width=100% bordercolor=white>");
            client.println("<tr>");
		  for(his=0 ; his<24 ; his++) {
			if(his%5<3)
              client.println("<td height=30px bgcolor=yellow>");
			else
              client.println("<td height=30px bgcolor=red>");
            client.println("</td>");
	      }
            client.println("</tr>");
            client.println("</table>");

          client.println("<font size=10><b>Humidity</b></font><br>");
            client.println("<table border=0 width=100% bordercolor=white>");
            client.println("<tr>");
		  for(his=0 ; his<24 ; his++) {
			if(his%5==3)
              client.println("<td height=30px bgcolor=yellow>");
			else
              client.println("<td height=30px bgcolor=red>");
            client.println("</td>");
	      }
            client.println("</tr>");
            client.println("</table>");

          client.println("<font size=10><b>Co2</b></font><br>");
            client.println("<table border=0 width=100% bordercolor=white>");
            client.println("<tr>");
		  for(his=0 ; his<24 ; his++) {
			if(his%5>1)
              client.println("<td height=30px bgcolor=yellow>");
			else
              client.println("<td height=30px bgcolor=red>");
            client.println("</td>");
	      }
            client.println("</tr>");
            client.println("</table>");

          client.println("<font size=10><b>Dust</b></font><br>");
            client.println("<table border=0 width=100% bordercolor=white>");
            client.println("<tr>");
		  for(his=0 ; his<24 ; his++) {
			if(his%5==1)
              client.println("<td height=30px bgcolor=yellow>");
			else
              client.println("<td height=30px bgcolor=red>");
            client.println("</td>");
	      }
            client.println("</tr>");
            client.println("</table>");

          client.println("<font size=10><b>Gas</b></font><br>");
            client.println("<table border=0 width=100% bordercolor=white>");
            client.println("<tr>");
		  for(his=0 ; his<24 ; his++) {
			if(his%5>2)
              client.println("<td height=30px bgcolor=yellow>");
			else
              client.println("<td height=30px bgcolor=red>");
            client.println("</td>");
	      }
            client.println("</tr>");
            client.println("</table>");
          /* End*/

          client.println("</body>");
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}


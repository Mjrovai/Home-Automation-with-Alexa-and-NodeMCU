
/*********************************************** 
 *  Connect to WiFi and UDP
 **********************************************/

void startConnections()
{
  int counter = 0;
  while (WiFi.status() != WL_CONNECTED)   // Wait til WiFi is connected properly
  {
    delay(500);
    Serial.print(".");
    counter++;
  }
  Serial.println("Connected to WiFi");

  bool udpConnected = connectUDP();   // Connect to UDP:
  if (udpConnected){
    startHttpServer(); // Start the HTTP Server
  }
}

void parsePackets()
{
  int packetSize = UDP.parsePacket();
  if(packetSize) {
    if (debug) {
      Serial.println("");
      Serial.print("Received packet of size ");
      Serial.println(packetSize);
      Serial.print("From ");
      IPAddress remote = UDP.remoteIP();

      for (int i =0; i < 4; i++) {
        Serial.print(remote[i], DEC);
        if (i < 3) {
          Serial.print(".");
        }
      }

      Serial.print(", port ");
      Serial.println(UDP.remotePort());
    }

    int len = UDP.read(packetBuffer, 255);

    if (len > 0) {
      packetBuffer[len] = 0;
    }

    String request = packetBuffer;

    if(request.indexOf('M-SEARCH') > 0) {
      if(request.indexOf("urn:Belkin:device:**") > 0) {
        if (debug) {
          Serial.println("Responding to search request ...");
        }
        respondToSearch();
      }
    }
  }
}

/*********************************************** 
 *  Connect UDP
 **********************************************/
bool connectUDP()
{
  boolean state = false;
  Serial.println("Connecting to UDP");

  if(UDP.beginMulticast(WiFi.localIP(), ipMulti, portMulti)) 
  {
    Serial.println("Connection successful");
    state = true;
  }
  else
  {
    Serial.println("Connection failed");
  }
  return state;
}


/*
  With this program you can make a Maneki-Neko (Beckoning Cat, Luck Cat, Winkekatze) to an IoT Maneki-Neko.
  Let's make a thing to an thing with internet or internet of things.

  You need an ESP8266.
  Connect D0 and D1 each with one LED.
  Connect D2 to the motor (magnet).

  More details: https://github.com/Cyclenerd/iot-maneki-neko
*/

#include <ESP8266WiFi.h>  // https://github.com/esp8266/Arduino
#include <PubSubClient.h> // https://pubsubclient.knolleary.net/


/***************************************************************************************
   Configuration Section
 ***************************************************************************************/

// Cat Name (CHANGE THIS!!!!11)
const char* cat_name = "iotkatze";

// WiFi SSID Name
const char* ssid     = "YOUR-WIFI-SSID";
// WiFi Password
const char* password = "YOUR-WIFI-PASSWORD";

// MQTT Server
// This can stay iot.eclipse.org for the global clowder ;)
const char* mqtt_server            = "iot.eclipse.org";
const short unsigned int mqtt_port = 1883; // unencrypted

/***************************************************************************************
   End Configuration Section
 ***************************************************************************************/

WiFiClient espClient;
PubSubClient client(espClient);

// The setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  delay(1000); // wait 1 seconds

  // Initialize digital pins an output.
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);

  test_led();
  
  print_logo();

  Serial.print("My name is ");
  Serial.println(cat_name);

  connect();
}

// The connect function makes a connection with the WiFi and the MQTT server
void connect() {
  digitalWrite(D0, LOW); // LED D0 off
  digitalWrite(D1, LOW); // LED D1 off
  
  // Start connecting to WiFi network
  Serial.println();
  Serial.print("Connecting to WiFi name ");
  Serial.println(ssid);

  // WiFi fix: https://github.com/esp8266/Arduino/issues/2186
  WiFi.persistent(false);
  WiFi.mode(WIFI_OFF);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    if (WiFi.status() == WL_CONNECT_FAILED) {
      Serial.println("Failed to connect to WIFI. Please verify credentials!");
      Serial.println();
    }
    delay(500);
    Serial.print(".");
  }

  // Connected to WiFi
  digitalWrite(D0, HIGH); // LED D0 on
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Setup MQTT client
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  
  Serial.println();
  Serial.print("Connecting to MQTT server ");
  Serial.println(mqtt_server);

  // Loop until we're reconnected
  while (!client.connected()) {
    // Attempt to connect
    if (client.connect("iot-maneki-neko")) {
      digitalWrite(D1, HIGH); // LED D1 on
      Serial.println();
      Serial.println("MQTT connected");
      // ... and subscribe to topic
      client.subscribe( ("winkekatze/" +  String(cat_name) + "/command" ).c_str() );
      meow(); // Meow!
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000); // Wait 5 seconds before retrying
    }
  }
}

// The meow function sends a meow
void meow() {
  client.publish( ("winkekatze/" +  String(cat_name) + "/status").c_str(), "Meow!");
}

// The callback funtions is called when an MQTT message (subscription) is received
void callback(char* topic, byte* payload, unsigned int length) {
  if ( "winkekatze/" +  String(cat_name) + "/command" ) {
    char* p = (char*)malloc(length + 1);
    p[length] = 0;
    // Copy the payload to the new buffer
    memcpy(p, payload, length);
    String command(p);
    
    Serial.print("[");
    Serial.print(topic);
    Serial.print("] ");
    Serial.print(command);
    Serial.println();

    // Wave ?
    if (command == "wave") {
      wave(true); // yes
      meow();
    } else {
      wave(false); // no
      meow();
    }
  }
}


// The print_logo functions prints a nice ascii cat
void print_logo() {
  Serial.println();
  Serial.println();
  Serial.println("      /\\_/\\  ");
  Serial.println(" IoT (o . o) Maneki-Neko");
  Serial.println("      > ^ <  ");
  Serial.println();
  Serial.println();
}

// The test_led function tests the connected LEDs on D0 and D1
void test_led() {
  // test D0, D1
  digitalWrite(D0, HIGH);
  digitalWrite(D1, HIGH);
  delay(5000); // wait 5 seconds
  digitalWrite(D0, LOW);
  digitalWrite(D1, LOW);
}

// The wave functions starts or stops the waving
void wave(boolean command) {
  if (command == true) {
    digitalWrite(D2, HIGH); // on
  } else {
    digitalWrite(D2, LOW); // off
  }
}

// The loop function runs over and over again forever
void loop() {
  bool reconnect = false;
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Disconnected from WiFi");
    reconnect = true;
  }
  if (!client.connected()) {
    Serial.print("Disconnected from MQTT, rc=");
    Serial.println(client.state());
    reconnect = true;
  }
  if (reconnect) {
    connect();
  }
  
  client.loop();
}

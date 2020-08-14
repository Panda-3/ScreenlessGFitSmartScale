bool send_push(String topic, String title, String message) {
  topic.toLowerCase();

  //send_push_gmessages(topic, title, message);  //uncomment the line to use Firebase Cloud Messages Instead

  //send_push_pushbullet(topic, title, message);
}

void send_push_pushbullet(String topic, String title, String message) {
  const int httpPort = 443;
  String host = "api.pushbullet.com";

  WiFiClientSecure client;
  Serial.println("---------------REFRESH TOKEN---------------\n\nConnecting to host\n");

  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  //String mBody = String("{\"body\":\"" + message + "\",\"title\":\"" + title + "\",\"type\":\"note\",\"device_iden\":\"" + [device_iden] + "\"}";
  //uncomment the previous line if you want to target a specific device

  String mBody = String("{\"body\":\"") + message + "\",\"title\":\"" + title + "\",\"type\":\"note\"}";

  String payload = String("POST ") + "/v2/pushes HTTP/1.1\n" +
                   "Host: " + host + "\n" +
                   "User-Agent: SmartScale\n" +
                   "Access-Token: " + get_pushbulet_token() + "\n" +
                   "Content-length: " + mBody.length() + "\n" +
                   "Accept: */*\n" +
                   "Content-type: application/json\n\n" +
                   mBody + "\r\n";

  client.println(payload);

  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(F("\n>>> Client Timeout !"));
      client.stop();
      return;
    }
  }

  char status[32] = {0};
  client.readBytesUntil('\r', status, sizeof(status));
  if (strcmp(status, "HTTP/1.1 200 OK") != 0) {
    Serial.println(F("Unexpected response: "));
    Serial.println(status);
    return;
  }

  Serial.println(F("Correclty sent pushbullet message\n"));

  // Disconnect
  client.stop();
}

int refreshTokenMessages() {
  WiFiClientSecure client;
  String mbodyRefreshMess = "client_secret=[MESSAGE_CLIENT_SECRET]&grant_type=refresh_token&refresh_token=[MESSAGE_REFRESH_TOKEN]&client_id=[MESSAGE_CLIENT_ID]";
  Serial.println("REFRESH TOKEN MESSAGES\n\nConnecting to host\n");

  if (!client.connect(host_google, httpPort)) {
    Serial.println("Connection failed");
    return -5;
  }

  String payload = String("POST ") +  "/oauth2/v4/token HTTP/1.1\n" +
                   "Host: " + host_google + "\n" +
                   "Content-length: " + mbodyRefreshMess.length() + "\n" +
                   "content-type: application/x-www-form-urlencoded\n\n" +
                   mbodyRefreshMess + "\r\n";

  client.println(payload);
  Serial.println(payload);

  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(F("\n>>> Client Timeout!"));
      client.stop();
      return -1;
    }
  }

  char status[32] = {0};
  client.readBytesUntil('\r', status, sizeof(status));
  if (strcmp(status, "HTTP/1.1 200 OK") != 0) {
    Serial.println(F("Unexpected response: "));
    Serial.println(status);
    return -2;
  }

  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) return -3;

  char carriageReturn[] = "\r\n";
  if (!client.find(carriageReturn)) return -4;

  const size_t capacity = JSON_OBJECT_SIZE(3);
  DynamicJsonDocument doc(capacity);

  // Parse JSON object
  DeserializationError error = deserializeJson(doc, client);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return -5;
  }

  // Extract values
  JsonObject root = doc.as<JsonObject>();
  Serial.println(F("Response:"));
  authKeyMessages = root["access_token"].as<char*>();
  Serial.println(authKeyMessages);

  if (!authKeyMessages.length() > 0) return -6;

  Serial.println(F("Correclty got the MESSAGE TOKEN\n"));

  // Disconnect
  client.stop();
  return 1;
}

bool send_push_gmessages(String topic, String title, String message) {
  WiFiClientSecure client;
  Serial.println("\nPOSTING MESSAGE\n\nConnecting to host\n");
  String host = "fcm.googleapis.com";

  if (!client.connect(host, httpPort)) {
    Serial.println("Connection failed");
    return false;
  }

  String mbody = String("{") + "\"message\":{\"topic\":\"" + topic + "\",\"notification\":{\"body\":\"" + message + "\",\"title\":\"" + title + "\"}}}";

  String payload = String("POST ") +  "/v1/projects/[FIREBASE_PROJECT_NAME]/messages:send" + " HTTP/1.1\n" +
                   "Host: " + host + "\n" +
                   "Content-length: " + mbody.length() + "\n" +
                   "Content-type: application/json\n" +
                   "Authorization: Bearer " + authKeyMessages + "\n\n" +
                   mbody + "\n";

  client.println(payload);
  Serial.println(payload);

  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(F("\n>>> Client Timeout !"));
      client.stop();
      return false;
    }
  }

  char status[32] = {0};
  client.readBytesUntil('\r', status, sizeof(status));
  if (strcmp(status, "HTTP/1.1 200 OK") != 0) {
    Serial.print(F("Unexpected response: "));
    Serial.println(status);
    return false;
  }

  Serial.println(F("\nPosted succesfully"));
  client.stop();
  return true;
}

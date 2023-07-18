//Configurações da rede Wifi

WiFiManager wm;
WiFiManagerParameter custom_field;
int customFieldLength = 40;
bool wm_nonblocking = false;
bool res;

String getParam(String name){
  String value;
  if(wm.server->hasArg(name)) {
    value = wm.server->arg(name);
  }
  return value;
}

void saveParamCallback(){
  Serial.println("[CALLBACK] saveParamCallback fired");
  Serial.println("PARAM customfieldid = " + getParam("customfieldid"));
}

//Função principal de configuração da rede.
void wifiConfigESP(){
  WiFi.mode(WIFI_STA);
  Serial.setDebugOutput(true);  
  delay(3000);
  Serial.println("\n Iniciando");

  if(wm_nonblocking) wm.setConfigPortalBlocking(false);

  const char* custom_radio_str = "<br/><label for='customfieldid'>Custom Field Label</label><input type='radio' name='customfieldid' value='1' checked> One<br><input type='radio' name='customfieldid' value='2'> Two<br><input type='radio' name='customfieldid' value='3'> Three";
  new (&custom_field) WiFiManagerParameter(custom_radio_str);
  std::vector<const char *> menu = {"wifi","info","param","sep","restart","exit"};

  wm.addParameter(&custom_field);
  wm.setSaveParamsCallback(saveParamCallback);
  wm.setMenu(menu);
  wm.setClass("invert");
  wm.setConfigPortalTimeout(60); 
  res = wm.autoConnect("ESP32_CONFIG","12345678");    //Change the password as needed

  if(!res) {
    Serial.println("Failed to connect or timed out :(");
  } 
  else {
    Serial.println("Connected...yeey :)");
  }

}

//Function to reset the network, when pressing the button the access point is activated, this will open the portal for configuring a new Wifi network.
void changeNetwork(){
    if( digitalRead(BUTTON_RESET_WIFI) == LOW ){
      Serial.println("Pressed button");
      delay(180);
      if( digitalRead(BUTTON_RESET_WIFI) == LOW ){
        Serial.println("Retained button");
        Serial.println("Erasing Settings, Restoring...");
        wm.resetSettings();
        ESP.restart();
      }

    }

}

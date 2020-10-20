//JSON que será utilizado na criação de uma requisição em connection.ino
extern String write_api_key;
//String json = "{\"write_api_key\":\"" + (String)write_api_key + "\",\"updates\":[{\"delta_t\":0,\"field1\":\"13\",\"field2\":\"14\",\"field3\":\"15\"}]}";
String json = "{\"write_api_key\":\"" + (String)write_api_key + "\",\"updates\":[{\"delta_t\":0,\"field1\":\"" + (String)dht.readTemperature() + "\",\"field2\":\"" + (String)dht.readHumidity() + "\",\"field3\":\"" + convertToLux(analogRead(LDR_PIN)) + "\"}]}";

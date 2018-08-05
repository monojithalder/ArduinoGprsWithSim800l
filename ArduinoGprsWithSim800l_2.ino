 #include<SoftwareSerial.h>

SoftwareSerial client(10,11);

String gpsData="";
void setup()
{
  Serial.begin(9600);
  client.begin(9600);
  delay(500);

  if(client.available())
  {
    Serial.print("Connected");
  }
  else
  {
    Serial.print("NotConnected");
  }
  //delay(50000);
}

void loop()
{
    connectGPRS(); 
  client.println("AT+CIPGSMLOC=1,1");
  delay(1000);
  //ShowSerialData();
  gpsData = getGpsData();
  //Serial.println(gpsData);
  delay(1000);
  connectHTTP(gpsData);
  delay(20000);
}



void connectGPRS()
{ 
  client.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\"");
  delay(1000);
  ShowSerialData();
  //Serial.println(getGpsData());
  client.println("AT+SAPBR=3,1,\"APN\",\"www\"");//APN
  delay(1000);
  ShowSerialData();

  client.println("AT+SAPBR=1,1");
  delay(1000);
  ShowSerialData();

  client.println("AT+SAPBR=2,1");
  delay(1000);
  ShowSerialData();
}

void connectHTTP(String testData)
{
  //Serial.println(testData);
  
  client.println("AT+HTTPINIT");
  delay(1000);
  ShowSerialData();

  client.println("AT+HTTPPARA=\"CID\",1");
  delay(1000);
  ShowSerialData();
  testData.replace("AT+CIPGSMLOC=1,1","");
  testData.replace("+CIPGSMLOC: ","");
  testData.replace("OK","");
  testData.replace("\r\n","");
  testData.trim();
  Serial.println(testData);
  client.println("AT+HTTPPARA=\"URL\",\"iot-tracking.amitmaity.com/api/add-location/1?data="+testData+"\"");
  delay(1000);
  ShowSerialData();
 
  client.println("AT+HTTPACTION=0");
  delay(1000);
  ShowSerialData();

  client.println("AT+HTTPREAD");
  delay(1000);
  ShowSerialData();

  client.println("AT+HTTPTERM");
  delay(1000);
  ShowSerialData;
  Serial.println("Done");
}

void ShowSerialData()
{
  while(client.available()!=0)
  {
  Serial.write(client.read());
  delay(100);
  }
}

String getGpsData() {
  String data = "";
  while(client.available()!=0)
  {
    delay(1000);
   data = client.readString();
   
  }
  return data;
}


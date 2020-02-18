#include <Adafruit_Fingerprint.h>

/*
PINO 11 - TX (fio branco)  
PINO 10 - TX (fio cinza)
 */
#include <SoftwareSerial.h>
SoftwareSerial mySerial(11, 10);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

void setup()  
{
  Serial.begin(9600);
  while (!Serial);
  delay(100);
  Serial.println("\n\nTESTANDO LEITOR DE DIGITAIS");

  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Sensor encontrado!");
  } else {
    Serial.println("Sensor não encontrado =(");
    while (1) { delay(1); }
  }

  finger.getTemplateCount();
  Serial.print("Sensor possui "); Serial.print(finger.templateCount); Serial.println(" digitais cadastradas.");
  Serial.println("Aguardando por digital válida...");
}

void loop()
{
  getFingerprintIDez();
  delay(50);
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Imagem capturada");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("Nenhum dedo detectado");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Erro de comunicação");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Erro de imagem");
      return p;
    default:
      Serial.println("Erro desconhecido");
      return p;
  }

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Imagem convertida");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Imagem bagunçada");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Erro de comunicação");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Não foi possível encontrar os recursos de impressão digital");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Imagem inválida");
      return p;
    default:
      Serial.println("Erro desconhecido");
      return p;
  }
  
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Encontrada uma imagem correspondente!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Erro de comunicação");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Nenhuma imagem correspondente");
    return p;
  } else {
    Serial.println("Erro desconhecido");
    return p;
  }   
  
  Serial.print("Encontrado ID #"); Serial.print(finger.fingerID); 
  Serial.print(" com a confiança de "); Serial.println(finger.confidence); 

  return finger.fingerID;
}

int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  Serial.print("Encontrado ID #"); Serial.print(finger.fingerID); 
  Serial.print(" com a confiança de "); Serial.println(finger.confidence);
  return finger.fingerID; 
}

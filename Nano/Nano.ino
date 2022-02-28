#include <SD.h> //SD kart kullanmamıza olanak veren kütüphaneyi çağrıyoruz.
#include <SPI.h> //SPI cihazlarını kullanabilmemize yarayan SPI haberleşme kütüphanesini çağırıyoruz
#include <virtuabotixRTC.h> //RTC modülünü kullanmamıza yarayan kütüphaneyi çağırıyoruz.

virtuabotixRTC Saat(7, 8, 9); //RTC modülünün pin girişlerini belirliyoruz. (CLK=7, Data=8, RST=9)

int yesil = 5; // SD kart kontorlünde kullanacağımız ledlerin pin tanımlamasını yapıyoruz.
int kirmizi = 4; 

int i=0; //Değişkenlerimizi tanımlıyoruz. 
int yeni=0;

int pinCS = 10;//SD kart modülünün CS pinine dijital pinlerden 10. pini tanımlıyoruz.

File sdkart; //SD içerisine bilgi yazabilmek için yada SD kartı kontrol edebilmek için modüle isim veriyoruz. 

void setup() {
  Serial.begin(9600); // Seri haberleşmeyi başlatıyoruz.
  pinMode(kirmizi, OUTPUT); // Bu pinlerin çıkış pini olduğunu tanımlıyoruz.
  pinMode(yesil, OUTPUT);
  kontrol(); //Kontrol fonksiyonunu çağırarak SD kartın çalışıp çalışmadığını kontrol ediyoruz.
}

void loop() {
  if (Serial.available() > 0){//Seri haberleşme üzerindne 0 dan büyük bir değer geldiği zaman okumayı başlatıyoruz.
    i=Serial.read(); //Seri haberleşmeden gelen veriyi "i" değişkenine atıyoruz.
    yeni=map(i,0,255,0,1800); //Alıcı tarafında yaptığımız haritalandırmayı bu sefer tam tersi şeklinde yapıyoruz.
    /*map komutu kullanma sebebimiz seri haberleşme sadece 8 bitlik veri taşıyabilir. Veriyi char yada string olarak
    taşıyabiliriz. Ama buda gecikmelere ve parazitlere sebebiyet verir. Bundan dolayı alıcı kısmında yaptığımız haritalandırmayı
    burda ters çevirerek asıl değeri alıyoruz.*/
    Serial.println(yeni); //"yeni" değişkenini ekrana yazdırıyoruz.
    Saat.updateTime(); //Saat verisini güncelliyoruz.
    SDkart(); //SD kart fonksiyonunu çağırıyoruz. 
  }
}
void kontrol(){ //SD kart kontrol etmek için oluşturuduğumuz fonksiyon.
  if(SD.begin()){ //SD kart ile haberleşmenin başlayıp başlmadığını sorguluyoruz.
    Serial.println("SD Kart Okundu"); // Sorgulama sonucu "1" ise ekrana okundu bilgisini gönderiyoruz.
    digitalWrite(yesil,HIGH); // SD kart okunduğu durumda yeşil led aktif kırmızı led pasif olmasını sağlıyoruz.
    digitalWrite(kirmizi,LOW);
    }
  else{
    Serial.println("SD Kart Okunamadi");// Sorgulama sonucu "0" ise ekrana okundu bilgisini gönderiyoruz.
    digitalWrite(yesil,LOW); // SD kart okunduğu durumda kırmızı led aktif yeşil led pasif olmasını sağlıyoruz.
    digitalWrite(kirmizi,HIGH);
    return;
    }
}
void SDkart(){ //SD karta yazma işlemini yaptığımız fonksiyon.
  sdkart = SD.open("data.txt", FILE_WRITE); //SD kart içerisinde .txt uzantılı "Data" dosyasını oluşturuyoruz. Hali hazırda varsa açıyoruz.
  
  if(sdkart){ //Dosya oluşması sırasıda sorun olmadıysa yazma işlemini başlatıyoruz.
    sdkart.print("|");
    sdkart.print(Saat.hours);  //Saat bilgisini yazıyoruz.
    sdkart.print(":");
    sdkart.print(Saat.minutes);  //Dakika bilgisini yazıyoruz.
    sdkart.print(":");
    sdkart.print(Saat.seconds);  //Saniye bilgisini yazıyoruz.
    sdkart.print("|");
    sdkart.print(" ");
    sdkart.print("|");
    sdkart.print(Saat.dayofmonth); //Gün bilgisini yazıyoruz.
    sdkart.print("/");
    sdkart.print(Saat.month);  //Ay bilgisini yazıyoruz.
    sdkart.print("/");
    sdkart.print(Saat.year); //Yıl bilgisini yazıyoruz.
    sdkart.print("|");
    sdkart.print(" ");
    sdkart.print("|");  
    sdkart.print("Agirlik: ");
    sdkart.print(yeni); //Ağırlık bilgisini yazıyoruz
    sdkart.print("g");
    sdkart.println("|");
    sdkart.close();  // Dosyayı kapatıyoruz.
  }
  else{
    Serial.println("Dosya Acilamadi"); //Dosya açılamamısı durumunda seri monütöre bilgi gönderiyoruz.
    digitalWrite(kirmizi,HIGH);  //Dosya açılamadığı durumda kırmızı ledi aktif hale getiriyoruz. Hem kırmızı hem yeşil led yanarsa SD kart okundu ama dosya açılamadığını anlıyoruz.
  
  }
 }

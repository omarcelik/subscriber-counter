// Ugradbeni sistemi
// Projekat: YouTube subscriber counter
//
// Benjamin Emirhafizović
//
// juni 2020



#define TEMASUBINFO "brude/info"
#define TEMASUBADD "brude/add"
#define TEMAPUBKANAL "brude/kanal"

#include "mbed.h"
#include "stm32f413h_discovery_lcd.h"
#include <stdlib.h>
#include <stdio.h>
#define MQTTCLIENT_QOS2 0
#include "easy-connect.h"
#include "MQTTNetwork.h"
#include "MQTTmbed.h"
#include "MQTTClient.h"
#include <string.h>
#include <string>
#include <iostream>
#include <vector>

InterruptIn  taster1(p5, PullDown);
InterruptIn  taster2(p6, PullDown);
InterruptIn  taster3(p7, PullDown);

Ticker auto_refresh;


char podatak[40];

char* str1;
char* str2;

struct Jutuber {
    char link[200];
    char ime[40];
    char subovi[40];
    char videi[40];
    char pregledi[40];
    char drzava[40];
    char datum[50];
};


std::vector<Jutuber> V;

int i, broj_kanala = 0;
bool salji = 0;


void next() {
    if (broj_kanala < V.size()-1) broj_kanala++;
    salji = 1;
}

void refresh() {
    salji = 1;
}

void previous() {
    if (broj_kanala > 0) broj_kanala--;
    salji = 1;
}


void prikazi(int n) {
    
    BSP_LCD_Init();
    
    BSP_LCD_Clear(LCD_COLOR_WHITE);  //ocisti displej
    
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 40);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_SetBackColor(LCD_COLOR_RED);
    BSP_LCD_SetFont(&Font12);
    BSP_LCD_DisplayStringAt(0, 15, (uint8_t *)"YouTube subscriber counter", CENTER_MODE);
    
    BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
    BSP_LCD_FillRect(0, 41, BSP_LCD_GetXSize(), 20);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_SetBackColor(LCD_COLOR_YELLOW);
    BSP_LCD_SetFont(&Font16);
    BSP_LCD_DisplayStringAt(7, 45, (uint8_t *)"Channel: ", LEFT_MODE);
    char broj[5];
    sprintf(broj, "%d", broj_kanala);
    BSP_LCD_DisplayStringAt(100, 45, (uint8_t *)broj, LEFT_MODE);
    
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetFont(&Font12);

    BSP_LCD_DisplayStringAt(7, 75, (uint8_t *)V[n].ime, LEFT_MODE);
    BSP_LCD_DisplayStringAt(7, 91, (uint8_t *)V[n].drzava, LEFT_MODE);
    BSP_LCD_DisplayStringAt(7, 107, (uint8_t *)V[n].subovi, LEFT_MODE);
    BSP_LCD_DisplayStringAt(7, 123, (uint8_t *)V[n].pregledi, LEFT_MODE);
    BSP_LCD_DisplayStringAt(7, 139, (uint8_t *)V[n].videi, LEFT_MODE);
    BSP_LCD_DisplayStringAt(7, 155, (uint8_t *)V[n].datum, LEFT_MODE);
    
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_FillRect(0, 190, BSP_LCD_GetXSize(), 60);
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_SetFont(&Font12);
    BSP_LCD_DisplayStringAt(7, 195, (uint8_t *)"Button 1: Next channel", LEFT_MODE);
    BSP_LCD_DisplayStringAt(7, 208, (uint8_t *)"Button 2: Refresh", LEFT_MODE);
    BSP_LCD_DisplayStringAt(7, 221, (uint8_t *)"Button 3: Previous channel", LEFT_MODE);
}

void clear() {
    BSP_LCD_Init();
    
    BSP_LCD_Clear(LCD_COLOR_WHITE);  //ocisti displej
    
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 40);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_SetBackColor(LCD_COLOR_RED);
    BSP_LCD_SetFont(&Font12);
    BSP_LCD_DisplayStringAt(0, 15, (uint8_t *)"YouTube subscriber counter", CENTER_MODE);
    
    BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
    BSP_LCD_FillRect(0, 41, BSP_LCD_GetXSize(), 20);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_SetBackColor(LCD_COLOR_YELLOW);
    BSP_LCD_SetFont(&Font16);
    BSP_LCD_DisplayStringAt(7, 45, (uint8_t *)"Channel:", LEFT_MODE);
    
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetFont(&Font12);

    BSP_LCD_DisplayStringAt(7, 75, (uint8_t *)"Name: ", LEFT_MODE);
    BSP_LCD_DisplayStringAt(7, 91, (uint8_t *)"Country: ", LEFT_MODE);
    BSP_LCD_DisplayStringAt(7, 107, (uint8_t *)"Subscribers: ", LEFT_MODE);
    BSP_LCD_DisplayStringAt(7, 123, (uint8_t *)"Total views: ", LEFT_MODE);
    BSP_LCD_DisplayStringAt(7, 139, (uint8_t *)"Video uploads: ", LEFT_MODE);
    BSP_LCD_DisplayStringAt(7, 155, (uint8_t *)"Date registered: ", LEFT_MODE);
    
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_FillRect(0, 190, BSP_LCD_GetXSize(), 60);
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_SetFont(&Font12);
    BSP_LCD_DisplayStringAt(7, 195, (uint8_t *)"Button 1: Next channel", LEFT_MODE);
    BSP_LCD_DisplayStringAt(7, 208, (uint8_t *)"Button 2: Refresh", LEFT_MODE);
    BSP_LCD_DisplayStringAt(7, 221, (uint8_t *)"Button 3: Previous channel", LEFT_MODE);
}



void uzmi_podatak(int max) {
    int j = 0;
    
    while (str1[i] != '*' && str1[i] != '\0' && i < max) {
        podatak[j] = str1[i];
        i++;
        j++;
    }
    podatak[j]='\0';
    
    i++;
}



void update(MQTT::MessageData& md) {
    MQTT::Message &message = md.message;
    printf("\r\n Povratne informacije: %.*s\r\n", (int)message.payloadlen, (char*)message.payload);
    sprintf(str1, "%.*s", (int)message.payloadlen, (char*)message.payload);
    i = 0;
    
    uzmi_podatak(message.payloadlen);
    sprintf(V[broj_kanala].ime, "Name: %s", podatak);
        
    uzmi_podatak(message.payloadlen);
    sprintf(V[broj_kanala].subovi, "Subscribers: %s", podatak);
        
    uzmi_podatak(message.payloadlen);
    sprintf(V[broj_kanala].videi, "Video uploads: %s", podatak);
        
    uzmi_podatak(message.payloadlen);
        sprintf(V[broj_kanala].pregledi, "Total views: %s", podatak);
        
    uzmi_podatak(message.payloadlen);
    sprintf(V[broj_kanala].drzava, "Country: %s", podatak);
        
    uzmi_podatak(message.payloadlen);
    sprintf(V[broj_kanala].datum, "Date registered: %s", podatak);
        
        
    prikazi(broj_kanala);
}


void dodaj_brisi(MQTT::MessageData& md) {
    
    
    MQTT::Message &message = md.message;
    printf("\r\n dodaj_obrisi: %.*s\r\n", (int)message.payloadlen, (char*)message.payload);
    sprintf(str2, "%.*s", (int)message.payloadlen, (char*)message.payload);
    if (message.payloadlen >= 5) {
        if (str2[0]=='a' && str2[1]=='d' && str2[2]=='d' && str2[3]==' ') {
            str2=&str2[4]; //odsijeci "add_"
            struct Jutuber J;
            strcpy(J.link, str2);   //J.link = str2;
            V.push_back(J);
            printf("Kanal dodat!\r\n");
            broj_kanala = V.size()-1;
            salji = 1; //refresh
        }
        else if (str2[0]=='d' && str2[1]=='e' && str2[2]=='l' && str2[3]==' ') {
            str2=&str2[4]; //odsijeci "del_"
            int broj = atoi(str2);  //string to int
            if (V.size() > 0 && broj >= 0 && broj <= V.size()-1) {
                V.erase(V.begin() + broj);
                if (broj_kanala > 0) broj_kanala--;
                salji = 1; //refresh
                printf("Kanal obrisan!\r\n");
            }
            else if (broj > V.size()-1) printf("Nepostojeci kanal za brisati!\r\n");
            if (V.size() == 0) clear();  //ako je orbisan zadnji kanal displej se ocisti
        }
        else printf("Neispravan format!\r\n");
    }
    else printf("Prekratak unos!\r\n");
}







int main() {
    
    taster1.rise(&next);
    taster2.rise(&refresh);
    taster3.rise(&previous);
    
    auto_refresh.attach(&refresh, 30);
    
    printf("Ugradbeni sistemi\r\n");
    printf("YouTube subscriber counter\r\n\r\n");

    NetworkInterface *network;
    network = NetworkInterface::get_default_instance();
    
    if (!network)
        return -1;

    MQTTNetwork mqttNetwork(network);

    MQTT::Client<MQTTNetwork, Countdown> client(mqttNetwork);

    const char* hostname = "broker.hivemq.com";
    int port = 1883;
    //printf("Connecting to %s:%d\r\n", hostname, port);
    int rc = mqttNetwork.connect(hostname, port);
    if (rc != 0)
        printf("rc from TCP connect is %d\r\n", rc);

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
    data.MQTTVersion = 3;
    data.clientID.cstring = "brude";
    data.username.cstring = "";
    data.password.cstring = "";
    if ((rc = client.connect(data)) != 0)
        printf("rc from MQTT connect is %d\r\n", rc);

    if ((rc = client.subscribe(TEMASUBINFO, MQTT::QOS2, update)) != 0)
        printf("rc from MQTT subscribe is %d\r\n", rc);
        
    if ((rc = client.subscribe(TEMASUBADD, MQTT::QOS2, dodaj_brisi)) != 0)
        printf("rc from MQTT subscribe is %d\r\n", rc);

    clear();
    
    while(1) {
        
        if (salji == 1 && V.size() > 0) {
            
            MQTT::Message message;
            // QoS 0
            message.qos = MQTT::QOS0;
            message.retained = false;
            message.dup = false;
            message.payload = V[broj_kanala].link;
            message.payloadlen = strlen(V[broj_kanala].link);
            client.publish(TEMAPUBKANAL, message);
            
            salji = 0;
        }
        
        rc = client.subscribe(TEMASUBINFO, MQTT::QOS0, update);
        rc = client.subscribe(TEMASUBADD, MQTT::QOS0, dodaj_brisi);
        
        wait_ms(50);
    }
}

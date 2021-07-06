
#include <SPI.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "xbm.h"
#include <TFT_eSPI.h> // Hardware-specific library


TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

// Wifi Credentials

const char* ssid     = "SSID";
const char* password = "PASS";


//Authentication Tokens
String Auth_token_board1 =  "AUTH TOKEN 1";
String Auth_token_board2 =  "AUTH TOKEN 2";

// variables

String auth_token_board;
String pin_number;
String value; // Data to be sent
uint8_t switch_state;
uint8_t board = 1; // board = 1 || board = 2

uint16_t calData[5] = { 569, 3299, 1110, 1528, 7 };


// Switch position and size
#define FRAME_X 20
#define FRAME_Y 10
#define FRAME_W 120
#define FRAME_H 50

// Red zone size
#define REDBUTTON_X FRAME_X
#define REDBUTTON_Y FRAME_Y
#define REDBUTTON_W (FRAME_W/2)
#define REDBUTTON_H FRAME_H

// Green zone size
#define GREENBUTTON_X (REDBUTTON_X + REDBUTTON_W)
#define GREENBUTTON_Y FRAME_Y
#define GREENBUTTON_W (FRAME_W/2)
#define GREENBUTTON_H FRAME_H

#define X_Button_Margin 150
#define Y_Button_Margin 100



//------------------------------------------------------------------------------------------
void setup(void)
{
  Serial.begin(9600);
  tft.init();

  tft.setRotation(2);
  tft.setCursor(0, 0);
  tft.fillScreen(TFT_WHITE);
  print_logo();

  delay(1000);

  // clear screen
  tft.fillScreen(TFT_BLACK);

  tft.setRotation(3);
  show_wifi_connectivity();

  // clear screen
  tft.fillScreen(TFT_BLACK);
  buttons(); // print buttons on screen
}

//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
void loop()
{
  uint16_t x, y;


  // See if there's any touch data for us
  if (tft.getTouch(&x, &y))
  {
    int Touch_Y_Cor = map(y, 50, 150, 0, 240);

    Serial.print("x - "); Serial.println(x);
    Serial.println("y - "); Serial.println(y);


    {
      if ((x > REDBUTTON_X + X_Button_Margin) && (x < (REDBUTTON_X + REDBUTTON_W + X_Button_Margin ) )) {
        if ((Touch_Y_Cor > REDBUTTON_Y) && (Touch_Y_Cor <= (REDBUTTON_Y + REDBUTTON_H))) {
          Serial.println("Switch 1 ON");
          switch_state = 11;

          SW_control();
        }
      }

      if ((x > REDBUTTON_X) && (x < (REDBUTTON_X + REDBUTTON_W))) {
        if ((Touch_Y_Cor > REDBUTTON_Y) && (Touch_Y_Cor <= (REDBUTTON_Y + REDBUTTON_H))) {
          Serial.println("Switch 2 ON");
          switch_state = 21;

          SW_control();
        }
      }

      if ((x > REDBUTTON_X + X_Button_Margin) && (x < (REDBUTTON_X + REDBUTTON_W + X_Button_Margin ) )) {
        if ((Touch_Y_Cor > REDBUTTON_Y + Y_Button_Margin ) && (Touch_Y_Cor <= ((REDBUTTON_Y + Y_Button_Margin) + REDBUTTON_H))) {
          Serial.println("Switch 3 ON");
          switch_state = 31;

          SW_control();
        }
      }

      if ((x > REDBUTTON_X) && (x < (REDBUTTON_X + REDBUTTON_W))) {
        if ((Touch_Y_Cor > REDBUTTON_Y + Y_Button_Margin ) && (Touch_Y_Cor <= (REDBUTTON_Y + REDBUTTON_H + Y_Button_Margin))) {
          Serial.println("Switch 4 ON");
          switch_state = 41;

          SW_control();
        }
      }

      if ((x > 0 ) && (x < 160)) {
        if ((Touch_Y_Cor > REDBUTTON_Y + Y_Button_Margin + 70) && (Touch_Y_Cor <= (REDBUTTON_Y + REDBUTTON_H + Y_Button_Margin + 70))) {
          Serial.println("Board 1 Selected");
          board = 1;

        }
      }
    }

    {
      if ((x > GREENBUTTON_X + X_Button_Margin ) && (x < (GREENBUTTON_X + GREENBUTTON_W + X_Button_Margin ))) {
        if ((Touch_Y_Cor > GREENBUTTON_Y) && (Touch_Y_Cor <= (REDBUTTON_Y + GREENBUTTON_H))) {
          Serial.println("Switch 1 OFF");
          switch_state = 10;

          SW_control();

        }
      }

      if ((x > GREENBUTTON_X) && (x < (GREENBUTTON_X + GREENBUTTON_W))) {
        if ((Touch_Y_Cor > GREENBUTTON_Y) && (Touch_Y_Cor <= (REDBUTTON_Y + GREENBUTTON_H))) {
          Serial.println("Switch 2 OFF");
          switch_state = 20;

          SW_control();

        }
      }

      if ((x > GREENBUTTON_X + X_Button_Margin ) && (x < (GREENBUTTON_X + GREENBUTTON_W + X_Button_Margin ))) {
        if ((Touch_Y_Cor > GREENBUTTON_Y + Y_Button_Margin) && (Touch_Y_Cor <= (REDBUTTON_Y + GREENBUTTON_H + Y_Button_Margin))) {
          Serial.println("Switch 3 OFF");
          switch_state = 30;

          SW_control();

        }
      }
      if ((x > GREENBUTTON_X) && (x < (GREENBUTTON_X + GREENBUTTON_W))) {
        if ((Touch_Y_Cor > GREENBUTTON_Y + Y_Button_Margin) && (Touch_Y_Cor <= (REDBUTTON_Y + GREENBUTTON_H + Y_Button_Margin))) {
          Serial.println("Switch 4 OFF");
          switch_state = 40;

          SW_control();

        }
      }

      if ((x > 160) && (x < 320))
      {
        if ((Touch_Y_Cor > GREENBUTTON_Y + Y_Button_Margin + 70) && (Touch_Y_Cor <= (REDBUTTON_Y + GREENBUTTON_H + Y_Button_Margin + 70 ))) {
          Serial.println("Board 2 Selected");
          board = 2;
        }
      }
    }

  }


}
//------------------------------------------------------------------------------------------

void buttons()
{
  tft.fillRect(REDBUTTON_X, REDBUTTON_Y, REDBUTTON_W, REDBUTTON_H, TFT_CYAN);
  tft.fillRect(GREENBUTTON_X, GREENBUTTON_Y, GREENBUTTON_W, GREENBUTTON_H, TFT_WHITE);

  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("OFF", GREENBUTTON_X + (GREENBUTTON_W / 2), GREENBUTTON_Y + (GREENBUTTON_H / 2));
  tft.drawString("ON", (REDBUTTON_X) + (REDBUTTON_W / 2) + 1, REDBUTTON_Y + (REDBUTTON_H / 2));

  tft.fillRect((REDBUTTON_X + X_Button_Margin ), REDBUTTON_Y, REDBUTTON_W, REDBUTTON_H, TFT_CYAN);
  tft.fillRect((GREENBUTTON_X + X_Button_Margin), GREENBUTTON_Y, GREENBUTTON_W, GREENBUTTON_H, TFT_WHITE);
  tft.setTextColor(TFT_BLACK);

  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("OFF", (GREENBUTTON_X + X_Button_Margin) + (GREENBUTTON_W / 2), GREENBUTTON_Y + (GREENBUTTON_H / 2));
  tft.drawString("ON", (REDBUTTON_X + X_Button_Margin) + (REDBUTTON_W / 2) + 1, REDBUTTON_Y + (REDBUTTON_H / 2));

  tft.fillRect(REDBUTTON_X, (REDBUTTON_Y + Y_Button_Margin) , REDBUTTON_W, REDBUTTON_H, TFT_CYAN);
  tft.fillRect(GREENBUTTON_X, (GREENBUTTON_Y + Y_Button_Margin) , GREENBUTTON_W, GREENBUTTON_H, TFT_WHITE);
  tft.setTextColor(TFT_BLACK);

  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("OFF", GREENBUTTON_X + (GREENBUTTON_W / 2), ( GREENBUTTON_Y + Y_Button_Margin) + (GREENBUTTON_H / 2));
  tft.drawString("ON", (REDBUTTON_X) + (REDBUTTON_W / 2) + 1, (REDBUTTON_Y + Y_Button_Margin) + (REDBUTTON_H / 2));


  tft.fillRect((REDBUTTON_X + X_Button_Margin ), (REDBUTTON_Y + Y_Button_Margin), REDBUTTON_W, REDBUTTON_H, TFT_CYAN);
  tft.fillRect((GREENBUTTON_X + X_Button_Margin), (GREENBUTTON_Y + Y_Button_Margin), GREENBUTTON_W, GREENBUTTON_H, TFT_WHITE);
  tft.setTextColor(TFT_BLACK);

  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("OFF", (GREENBUTTON_X + X_Button_Margin) + (GREENBUTTON_W / 2), (GREENBUTTON_Y + Y_Button_Margin) + (GREENBUTTON_H / 2));
  tft.drawString("ON", (REDBUTTON_X + X_Button_Margin) + (REDBUTTON_W / 2) + 1, (REDBUTTON_Y + Y_Button_Margin) + (REDBUTTON_H / 2));


  tft.fillRect(0, (REDBUTTON_Y + Y_Button_Margin + 70), 160, REDBUTTON_H, TFT_GREEN);
  tft.fillRect(160, (GREENBUTTON_Y + Y_Button_Margin + 70), 160, GREENBUTTON_H, TFT_RED);
  tft.setTextColor(TFT_BLACK);

  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("Room 1", 80, (REDBUTTON_Y + Y_Button_Margin + 70) + (REDBUTTON_H / 2));
  tft.drawString("Room 2", 160 + 80, (GREENBUTTON_Y + Y_Button_Margin + 70 ) + (GREENBUTTON_H / 2));

}

void SW_control ()
{
  if (board == 1)
  {
    if (switch_state == 10 )
    {
      auth_token_board = Auth_token_board1;
      pin_number = "V1";
      value = "0";
    }
    if (switch_state == 11)
    {
      auth_token_board = Auth_token_board1;
      pin_number = "V1";
      value = "1";
    }
    if (switch_state == 20)
    {
      auth_token_board = Auth_token_board1;
      pin_number = "V2";
      value = "0";
    }
    if (switch_state == 21 )
    {
      auth_token_board = Auth_token_board1;
      pin_number = "V2";
      value = "1";
    }
    if (switch_state == 30 )
    {
      auth_token_board = Auth_token_board1;
      pin_number = "V3";
      value = "0";
    }
    if (switch_state == 31)
    {
      auth_token_board = Auth_token_board1;
      pin_number = "V3";
      value = "1";
    }
    if (switch_state == 40)
    {
      auth_token_board = Auth_token_board1;
      pin_number = "V4";
      value = "0";
    }
    if (switch_state == 41 )
    {
      auth_token_board = Auth_token_board1;
      pin_number = "V4";
      value = "1";
    }
  }
  if (board == 2)
  {
    if (switch_state == 10 )
    {
      auth_token_board = Auth_token_board2;
      pin_number = "V1";
      value = "1";
    }
    if (switch_state == 11)
    {
      auth_token_board = Auth_token_board2;
      pin_number = "V1";
      value = "0";
    }
    if (switch_state == 20)
    {
      auth_token_board = Auth_token_board2;
      pin_number = "V2";
      value = "1";
    }
    if (switch_state == 21 )
    {
      auth_token_board = Auth_token_board2;
      pin_number = "V2";
      value = "0";
    }
    if (switch_state == 30 )
    {
      auth_token_board = Auth_token_board2;
      pin_number = "V3";
      value = "1";
    }
    if (switch_state == 31)
    {
      auth_token_board = Auth_token_board2;
      pin_number = "V3";
      value = "0";
    }
    if (switch_state == 40)
    {
      auth_token_board = Auth_token_board2;
      pin_number = "V0";
      value = "4";
    }
    if (switch_state == 41 )
    {
      auth_token_board = Auth_token_board2;
      pin_number = "V0";
      value = "0";
    }
  }



  if ((WiFi.status()  == WL_CONNECTED))
  {

    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    // configure traged server and url
    //http.begin("https://www.howsmyssl.com/a/check", ca); //HTTPS
    http.begin("http://blynk-cloud.com/" + auth_token_board + "/update/" + pin_number + "?value=" + value); //HTTP
    Serial.println("http://blynk-cloud.com/" + auth_token_board + "/update/" + pin_number + "?value=" + value);

    Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println(payload);
      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }


}

void print_logo()
{
  int x = 0;
  int y = 0;

  // Draw bitmap with top left corner at x,y with foreground only color
  // Bits set to 1 plot as the defined color, bits set to 0 are not plotted
  //              x  y  xbm   xbm width  xbm height  color
  tft.drawXBitmap(x, y, logo, logoWidth, logoHeight, TFT_CYAN);

}

void show_wifi_connectivity()
{
  WiFi.begin(ssid, password);
  tft.setCursor(110, 100);
  tft.setTextFont(2);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  tft.println("Connecting");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");

  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  tft.fillScreen(TFT_BLACK);
  tft.setCursor(105, 100);
  tft.setTextFont(2);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);


  tft.setCursor(105, 100);
  tft.setTextFont(2);
  tft.setTextSize(1);

  tft.println(WiFi.localIP());
  tft.setCursor(105, 120);
  tft.setTextFont(2);
  tft.setTextSize(1);
  tft.println("Connected");
  delay(2000);


}

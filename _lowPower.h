#include "_Definitions.h"
#include "esp_sleep.h"

#include "esp_pm.h"
#include "esp_clk.h"
#include "esp_timer.h"
#include "rom/uart.h"




int lastPowerReadTimer = 0;

void _setupPower(){
  adcAttachPin(BATTERY_PIN);
  Serial.println("_setupPower called");

#if _ROLE == 0 //DISABLE WIFI AND BLUETOOTH IN SENDER MODE
  WiFi.mode(WIFI_OFF);
  btStop();
#endif
}


bool _readPowerTimer(){
  if (millis() - lastPowerReadTimer > _POWER_READ_INTERVAL){
      //time since last send
      lastPowerReadTimer = millis();
      return true;
 } 
 else return false;

}

double readPrecisionVoltage(byte pin){
  double reading = analogRead(pin); // Reference voltage is 3v3 so maximum reading is 3v3 = 4095 in range 0 to 4095
  if(reading < 1 || reading >= 4095)
    //return 0;
    // Added an improved polynomial, use either, comment out as required
  // return -0.000000000009824 * pow(reading,3) + 0.000000016557283 * pow(reading,2) + 0.000854596860691 * reading + 0.065440348345433;
  return -0.000000000000016 * pow(reading,4) + 0.000000000118171 * pow(reading,3)- 0.000000301211691 * pow(reading,2)+ 0.001109019271794 * reading + 0.034143524634089;
} 

void _readBatteryPower(){


   double dVBAT  =  readPrecisionVoltage(BATTERY_PIN);// OR analogRead(BATTERY_PIN)*XS*MUL/_FULL_BATTERY;
   char TempString[10];  //  Hold The Convert Data
   dtostrf(dVBAT,3,2,TempString);
   sVBAT = String(TempString);
    // dtostrf( [doubleVar] , [sizeBeforePoint] , [sizeAfterPoint] , [WhereToStoreIt] )
   Serial.println("Remaining battery still has VBAT:"+sVBAT+"(mV)");

}





void _switch_freq(int mhz)
{
    Serial.println("switch_freq called for:"+(String)mhz);
     setCpuFrequencyMhz(mhz);
    /*
    rtc_cpu_freq_t max_freq;
    assert(rtc_clk_cpu_freq_from_mhz(mhz, &max_freq));
    esp_pm_config_esp32_t pm_config;
    pm_config.max_cpu_freq=RTC_CPU_FREQ_80M;
    pm_config.min_cpu_freq=RTC_CPU_FREQ_XTAL;
    pm_config.light_sleep_enable=true;
 
  
    esp_err_t ret;
    if((ret = esp_pm_configure(&pm_config)) != ESP_OK) {
        printf("pm config error %s\n", \
                ret == ESP_ERR_INVALID_ARG ? \
                "ESP_ERR_INVALID_ARG":"ESP_ERR_NOT_SUPPORTED");
    }

    
    ESP_ERROR_CHECK( esp_pm_configure(&pm_config) );
    
    Serial.println("Waiting for frequency to be set to  MHz..."+ (String)mhz);
    while (getCpuFrequencyMhz() != mhz) {
        vTaskDelay(10);
    }
    */
    Serial.println("Frequency is set to MHz..."+ (String)mhz);
}


/*
Method to print the reason by which ESP32
has been awaken from sleep
*/
void print_wakeup_reason()
{
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}

void _light_sleep(){
      /* Wake up in 2 seconds, or when button is pressed */
     esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
     Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) + " Seconds"); 
      /* To make sure the complete line is printed before entering sleep mode,
       * need to wait until UART TX FIFO is empty:
       */
      uart_tx_wait_idle(CONFIG_ESP_CONSOLE_UART_NUM);

      /* Get timestamp before entering sleep */
      int64_t t_before_us = esp_timer_get_time();

      /* Enter sleep mode */
      esp_light_sleep_start();
      /* Execution continues here after wakeup */
      print_wakeup_reason();
      /* Get timestamp after waking up from sleep */
      int64_t t_after_us = esp_timer_get_time();
     Serial.println("Returned from light sleep at: "+_printInt64(t_after_us / 1000, DEC)+ "ms, slept for:"+_printInt64((t_after_us - t_before_us) / 1000, DEC)+ " ms\n");

}


  

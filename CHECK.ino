static int CHECK_entry(struct pt *pt) {
  PT_BEGIN(pt);
  while (1)
  {
    if (unPower < EEPROM.read(15))
    {
      //Display LOW BATTERY
      PT_SEM_WAIT(pt, &OLED);
      for (static int Tnib = 0; Tnib < 10000; Tnib++ )
      {
        tone(Buzzer_OUTPIN, 3200);

        display.clear();
        display.setTextColor(WHITE);
        display.setTextSize(2);
        display.setCursor(3 , 24);
        display.print("LOW BATTERY");
        display.update();

        if (bKEY_INTERRUPT == true)
        {
          bKEY_INTERRUPT = false;
          break;
        }
        delay(1);
      }
      noTone(Buzzer_OUTPIN);
      PT_SEM_SIGNAL(pt, &OLED);
      fnPOWER_DOWN();
    }

    if (unPower >= 4250)
    {
      //Display Charging
      PT_SEM_WAIT(pt, &OLED);
      for (static int Tnib = 0; Tnib < 3000; Tnib++ )
      {
        tone(Buzzer_OUTPIN, Tnib);

        display.clear();
        display.setTextColor(WHITE);
        display.setTextSize(1);
        display.setCursor(16 , 28);
        display.print("Full Or Charging");
        display.update();

        if (bKEY_INTERRUPT == true)
        {
          bKEY_INTERRUPT = false;
          break;
        }
        delay(1);
      }
      noTone(Buzzer_OUTPIN);
      PT_SEM_SIGNAL(pt, &OLED);
    }

    dPressure = fnREAD_BMP180();
    dBaseline = EEPROM.read(14);
    dHeight = pressure.altitude(dPressure, dBaseline);
    dHumidity = AM2320.getHumidity();
    dTemperature = AM2320.getTemperature();
    unFinger = EEPROM.read(18);
    PT_TIMER_DELAY(pt, EEPROM.read(17) * 1000);
  }
  PT_END(pt);
}


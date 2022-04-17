static int TIME_entry(struct pt *pt) {
  PT_BEGIN(pt);
  while (1)
  {
    PT_WAIT_UNTIL(pt, bTIME_Switch == true);
    PT_SEM_WAIT(pt, &OLED);
    static byte byTIME_WINDOW = 1;
    bySecond = Clock.getSecond();
    byMinute = Clock.getMinute();
    byHour = Clock.getHour(bH12, bPM);
    byDate = Clock.getDate();
    byMonth = Clock.getMonth(bCentury);
    byYear = Clock.getYear() + 2000;
    byDoW = Clock.getDoW();
    unPower = fnREAD_VCC();
    fnREAD_GY271();

    if (byTIME_WINDOW == 1)
    {
      display.clear();
      display.setTextColor(WHITE);
      display.setTextSize(2);
      display.setCursor(0, 14);
      if (bH12 == true)
      {
        if (bPM == true)
        {
          display.print("PM");
        }
        else
        {
          display.print("AM");
        }
      }
      display.setTextSize(3);
      display.setCursor(23, 14);
      display.print(byHour);
      display.print(":");
      display.println(byMinute);
      display.setTextSize(1);
      display.setCursor(11, 36);
      display.print(dTemperature);
      display.print("DC");
      display.setCursor(76, 45);
      display.print(dHumidity);
      display.println("PC");
      display.setCursor(1, 44);
      if (dPressure == 0)
      {
        display.print("   ");
      }
      if (dPressure > 0 && dPressure < 1000)
      {
        display.print(" ");
      }
      display.print(dPressure);
      display.print("hPa");
      display.setCursor(71, 44);
      if (dHeight == 0)
      {
        display.print("   ");
      }
      if (dHeight > 0 && dHeight < 1000)
      {
        display.print(" ");
      }
      display.print(dHeight);
      display.print("M");
      display.drawLine(0, 63, unPower / 5000 * 127, 63, WHITE);
      display.update();

      PT_SEM_SIGNAL(pt, &OLED);
      PT_TIMER_DELAY(pt, 100);

      if (bBACKGROUND_REQUEST != true)
      {
        if (bKEY_INTERRUPT == true)
        {
          if (analogRead(Finger_PIN) >= unFinger)
          {
            byTIME_WINDOW = 2;
            bKEY_INTERRUPT = false;
          }
          else
          {
            bSTOPWATCH_Switch = true;
            bTIME_Switch = false;
            bKEY_INTERRUPT = false;
            PT_YIELD(pt);
          }
        }
      }
    }

    if (byTIME_WINDOW == 2)
    {
      static byte Th = byHour;
      if (bH12 == false)
      {
        bH12 = true;
        Clock.setClockMode(bH12);
        Th = Clock.getHour(bH12, bPM);
        bH12 = false;
        Clock.setClockMode(bH12);
      }
      display.clear();
      display.drawCircle(32, 32, 31, WHITE);//Circle
      display.drawCircle(32, 32, 1, BLACK);//Centre
      display.drawFastVLine(32, 2, 3, WHITE);// 12h
      display.drawFastVLine(32, 60, 3, WHITE);//3h
      display.drawFastHLine(2, 32, 3, WHITE);//6h
      display.drawFastHLine(60, 32, 3, WHITE);//9h
      display.drawPixel(Hour_X[Th], Hour_Y[Th], WHITE); //Hour
      display.drawPixel(Min_X[byMinute], Min_Y[byMinute], WHITE); //Min
      display.drawPixel(Sec_X[bySecond], Sec_Y[bySecond], WHITE); //Sec
      display.setTextColor(WHITE);
      display.setTextSize(1);
      display.setCursor(65, 0);
      display.print(byYear);
      display.print(".");
      display.print(byMonth);
      display.print(".");
      display.print(byDate);
      display.setTextSize(2);
      display.setCursor(9, 8);
      if (byDoW == 1)
      {
        display.println("Mon");
      }
      if (byDoW == 2)
      {
        display.println("Tues");
      }
      if (byDoW == 3)
      {
        display.println("Wed");
      }
      if (byDoW == 4)
      {
        display.println("Thur");
      }
      if (byDoW == 5)
      {
        display.println("Fri");
      }
      if (byDoW == 6)
      {
        display.println("Sat");
      }
      if (byDoW == 7)
      {
        display.println("Sun");
      }
      display.setTextSize(1);
      display.print("X=");
      display.println(TnX);
      display.print("Y=");
      display.println(TnY);
      display.print("Z=");
      display.println(TnZ);
      display.drawLine(0, 63, unPower / 5000 * 127, 63, WHITE);
      display.update();

      PT_SEM_SIGNAL(pt, &OLED);
      PT_TIMER_DELAY(pt, 100);

      if (bBACKGROUND_REQUEST != true)
      {
        if (bKEY_INTERRUPT == true)
        {
          if (analogRead(Finger_PIN) >= unFinger)
          {
            byTIME_WINDOW = 1;
            bKEY_INTERRUPT = false;
          }
          else
          {
            bSTOPWATCH_Switch = true;
            bTIME_Switch = false;
            bKEY_INTERRUPT = false;
            PT_YIELD(pt);
          }
        }
      }
    }
  }
  PT_END(pt);
}


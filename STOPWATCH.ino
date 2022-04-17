static int STOPWATCH_entry(struct pt *pt) {
  PT_BEGIN(pt);
  while (1)
  {
    PT_WAIT_UNTIL(pt, bSTOPWATCH_Switch == true);
    PT_SEM_WAIT(pt, &OLED);

    static bool bSTART = false;

    if (bSTART = true)
    {
      display.clear();
      display.setTextColor(WHITE);
      display.setTextSize(1);
      display.setCursor(0, 0);
      display.println("QuinJet Has Taken Off");
      display.setTextSize(2);
      display.println(lWDT_TIMER);
      display.update();
    }

    if (bBACKGROUND_REQUEST != true)
    {
      if (bKEY_INTERRUPT == true)
      {
        if (analogRead(Finger_PIN) >= unFinger)
        {
          if (bSTART = false)
          {
            wdt_enable(WDTO_1S);
            WDTCSR |= _BV(WDIE);
            lWDT_TIMER = 0;
            bSTART = true;
          }
          else
          {
            wdt_enable(WDTO_8S);
            WDTCSR |= _BV(WDIE);
            lWDT_TIMER = 0;
            bSTART = false;
            display.clear();
            display.setTextColor(WHITE);
            display.setTextSize(1);
            display.setCursor(15, 0);
            display.println("Misson Completed");
            display.setTextSize(2);
            display.println(lWDT_TIMER);
            display.update();
          }
          bKEY_INTERRUPT = false;
        }
        else
        {
          bTIMER_Switch = true;
          bSTOPWATCH_Switch = false;
          bKEY_INTERRUPT = false;
          PT_YIELD(pt);
        }
      }

      unPower = fnREAD_VCC();
      display.drawLine(0, 63, unPower / 5000 * 127, 63, WHITE);
      display.update();

    }
  }
  PT_END(pt);
}


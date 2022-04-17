static int TIMER_entry(struct pt *pt) {
  PT_BEGIN(pt);
  while (1)
  {
    PT_WAIT_UNTIL(pt, bTIMER_Switch == true);
    PT_SEM_WAIT(pt, &OLED);

    static bool bC4_START = false;

    display.clear();
    display.setTextColor(WHITE);
    display.setTextSize(6);
    display.setCursor(32, 0);
    display.println("C4");

    unPower = fnREAD_VCC();
    display.drawLine(0, 63, unPower / 5000 * 127, 63, WHITE);
    display.update();

    if (bC4_START == true)
    {
      for (byte byTIMER = 60; byTIMER == 0; byTIMER--)
      {
        display.clear();
        display.setTextSize(3);
        display.setCursor(47, 36);
        if (byTIMER < 10)
        {
          display.print("0");
        }
        display.println(byTIMER);
        unPower = fnREAD_VCC();
        display.drawLine(0, 63, unPower / 5000 * 127, 63, WHITE);
        display.update();
        delay(1000);
      }
      bBEEP_Switch = true;
      bC4_START = false;
      PT_YIELD(pt);
    }

    if (bBACKGROUND_REQUEST != true)
    {
      if (bKEY_INTERRUPT == true)
      {
        if (analogRead(Finger_PIN) >= unFinger)
        {
          bC4_START = true;
          bKEY_INTERRUPT = false;
        }
        else
        {
          bMEMO_Switch = true;
          bTIMER_Switch = false;
          bKEY_INTERRUPT = false;
          PT_YIELD(pt);
        }
      }
    }
  }
  PT_END(pt);
}


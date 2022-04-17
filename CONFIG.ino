static int CONFIG_entry(struct pt *pt) {
  PT_BEGIN(pt);
  while (1)
  {
    PT_WAIT_UNTIL(pt, bCONFIG_Switch == true);
    PT_SEM_WAIT(pt, &OLED);

    display.clear();
    display.setTextColor(WHITE);
    display.setTextSize(3);
    display.setCursor(8, 0);
    display.println("CONFIG:");
    display.setTextSize(1);
    display.setCursor(10, 22);
    display.println("Prepare To Connect");
    if (unPower <= 4250)
    {
      display.print(unPower / 4200 * 100);
      display.println("Percent");
    }
    else
    {
      display.println("Full Or Charging");
    }
    display.drawLine(0, 63, unPower / 5000 * 127, 63, WHITE);
    display.update();

    if (bBACKGROUND_REQUEST != true)
    {
      if (bKEY_INTERRUPT == true)
      {
        if (analogRead(Finger_PIN) >= unFinger)
        {
          display.clear();
          display.println("Config Or Reboot?");
          display.update();
          static int Tnid = 0;
          for (Tnid = 0; Tnid < 5000; Tnid++)
          {
            display.println(Tnid);
            if (bKEY_INTERRUPT = true)
            {
              fnCONFIG();
            }
            delay(1);
          }
          bKEY_INTERRUPT = false;
          fnReset();
        }
        else
        {
          bTIME_Switch = true;
          bCONFIG_Switch = false;
          bKEY_INTERRUPT = false;
          PT_YIELD(pt);
        }
      }
    }
    PT_TIMER_DELAY(pt, 500);
  }
  PT_END(pt);
}

void fnCONFIG() {

}


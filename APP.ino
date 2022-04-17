static int APP_entry(struct pt *pt) {
  PT_BEGIN(pt);
  while (1)
  {
    PT_WAIT_UNTIL(pt, bAPP_Switch == true);
    PT_SEM_WAIT(pt, &OLED);

    display.clear();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println(" ");

    display.drawLine(0, 63, unPower / 5000 * 127, 63, WHITE);
    display.update();

    if (bBACKGROUND_REQUEST != true)
    {
      if (bKEY_INTERRUPT == true)
      {
        if (analogRead(Finger_PIN) >= unFinger)
        {
          PT_RESTART(pt);
        }
        bKEY_INTERRUPT = false;
      }
      else
      {
        bCONFIG_Switch = true;
        bAPP_Switch = false;
        bKEY_INTERRUPT = false;
        PT_YIELD(pt);
      }
    }
  }
  PT_END(pt);
}


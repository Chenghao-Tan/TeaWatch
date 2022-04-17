static int SLEEP_entry(struct pt *pt) {
  PT_BEGIN(pt);
  while (1)
  {
    PT_WAIT_UNTIL(pt, bSLEEP_Switch == true);

    if (TbyKEY_TIMER == EEPROM.read(16) / 10)
    {
      TbyKEY_TIMER = 0;
      fnPOWER_DOWN();
    }
    TbyKEY_TIMER++;
    PT_TIMER_DELAY(pt, 10000);
  }
  PT_END(pt);
}

void fnPOWER_DOWN() {
  while (1)
  {
    delay(100);
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    sleep_mode();
    //Return From Watchdog Interrupt
    sleep_disable();
    power_all_enable();
    delay(100);

    fnALARM();
    fnQUICK_SHOW();

    if (bKEY_INTERRUPT == true || bBEEP_Switch == true)
    {
      bKEY_INTERRUPT = false;
      break;
    }
  }
}


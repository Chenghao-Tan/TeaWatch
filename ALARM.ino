static int ALARM_entry(struct pt *pt) {
  PT_BEGIN(pt);
  while (1)
  {
    PT_WAIT_UNTIL(pt, bALARM_Switch == true);
    fnALARM();
    PT_TIMER_DELAY(pt, 20000);
  }
  PT_END(pt);
}

void fnALARM() {
  if (EEPROM.read(11) == 1 || EEPROM.read(12) == 1 || EEPROM.read(13) == 1)
  {
    byMinute = Clock.getMinute();
    byHour = Clock.getHour(bH12, bPM);

    if (bH12 == true)
    {
      bH12 = false;
      Clock.setClockMode(bH12);
      byHour = Clock.getHour(bH12, bPM);
      bH12 = true;
      Clock.setClockMode(bH12);
      byHour = Clock.getHour(bH12, bPM);
    }

    if (EEPROM.read(3) == byHour && EEPROM.read(4) == byMinute)
    {
      bBEEP_Switch = true;
    }

    if (EEPROM.read(5) == byHour && EEPROM.read(6) == byMinute)
    {
      bBEEP_Switch = true;
    }

    if (EEPROM.read(7) == byHour && EEPROM.read(8) == byMinute)
    {
      bBEEP_Switch = true;
    }
  }
}


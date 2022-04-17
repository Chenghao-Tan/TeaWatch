static int BEEP_entry(struct pt *pt) {
  PT_BEGIN(pt);
  while (1)
  {
    PT_WAIT_UNTIL(pt, bBEEP_Switch == true);
    bBACKGROUND_REQUEST = true;
    PT_SEM_WAIT(pt, &OLED);

    display.clear();
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(4 , 28);
    display.print("TIME'S UP!");
    display.update();
    PT_TIMER_DELAY(pt, 2000);
    PT_SEM_SIGNAL(pt, &OLED);

    static int Tnic;
    for (Tnic = 0; Tnic < 6; Tnic++)
    {
      static int Tnia;
      for (Tnia = 200; Tnia <= 800; Tnia++)
      {
        tone(Buzzer_OUTPIN, Tnia);
        analogWrite(Motor_OUTPIN, Tnia / 800 * 255);
        display.drawLine(0, 0, Tnia / 800 * 127, 0, WHITE);
        display.update();
        PT_TIMER_DELAY(pt, 5);
      }
      PT_TIMER_DELAY(pt, 4000);
      for (Tnia = 800; Tnia >= 200; Tnia--)
      {
        tone(Buzzer_OUTPIN, Tnia);
        analogWrite(Motor_OUTPIN, 255);
        PT_TIMER_DELAY(pt, 10);
      }

      if (bKEY_INTERRUPT == true)
      {
        digitalWrite(Led_OUTPIN, LOW);
        noTone(Buzzer_OUTPIN);
        analogWrite(Motor_OUTPIN, 0);
        bBEEP_Switch = false;
        bBACKGROUND_REQUEST = false;
        PT_YIELD(pt);
      }
    }

    digitalWrite(Led_OUTPIN, LOW);
    noTone(Buzzer_OUTPIN);
    analogWrite(Motor_OUTPIN, 0);
    bBEEP_Switch = false;
    bBACKGROUND_REQUEST = false;
    PT_YIELD(pt);
  }
  PT_END(pt);
}


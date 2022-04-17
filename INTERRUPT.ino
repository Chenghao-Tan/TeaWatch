void fnKEY() {
  TbyKEY_TIMER = 0;
  bKEY_INTERRUPT = true;
}

ISR(WDT_vect) {
  WDTCSR |= _BV(WDIE);
  lWDT_TIMER++;
}


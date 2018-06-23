/************************************************************************/
/* Threads static table, one entry per thread.  A thread's priority is  */
/* determined by its position in the table with highest priority first. */
/*                                                                      */
/* These threads start with a null argument.  A thread's name may also  */
/* be null to save RAM since the name is currently not used.            */
/************************************************************************/
// THD_TABLE_ENTRY(waThread1, "thread1", Thread1, NULL) Name is not necessary
THD_TABLE_BEGIN
//#ifdef THR_PRESSURE
//  THD_TABLE_ENTRY(waPressure, NULL, ThPressure, NULL)
//#endif  // THR_PRESSURE
#ifdef THR_PT100
  THD_TABLE_ENTRY(waPT100, NULL, ThPT100, NULL)
#endif  // THR_PT100
#ifdef THR_SERIAL
  THD_TABLE_ENTRY(waSerial, NULL, ThSerial, NULL)
#endif  // THR_SERIAL
//#ifdef THR_OLED
//  THD_TABLE_ENTRY(waOLED, NULL, ThOLED, NULL)
//#endif  // THR_OLED
THD_TABLE_END

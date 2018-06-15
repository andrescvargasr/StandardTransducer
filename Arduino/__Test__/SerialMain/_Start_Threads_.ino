/************************************************************************/
/* Threads static table, one entry per thread.  A thread's priority is  */
/* determined by its position in the table with highest priority first. */
/*                                                                      */
/* These threads start with a null argument.  A thread's name may also  */
/* be null to save RAM since the name is currently not used.            */
/************************************************************************/
// THD_TABLE_ENTRY(waThread1, "thread1", Thread1, NULL) Name is not necessary
THD_TABLE_BEGIN
THD_TABLE_ENTRY(waSerial, NULL, ThrSerial, NULL)
//THD_TABLE_ENTRY(waOLED, NULL, ThrOLED, NULL)
THD_TABLE_END

#ifndef COMMON_HH
#define COMMON_HH

// period of loop() repeat
// every single TICK_TIME msec banks are checked against being disposed
#define TICK_TIME 1000

#define NUM_BANKS 5
#define NUM_WEEK_DAYS 7
#define NUM_MAX_N_TIMES 10
#define DATE_SET_FLAG 1

bool json_to_DateTime(const char *json_str);
void try_dispense_pill();
unsigned int dt_to_weekday(struct tm *t);
void initWiFi();
void mqtt_setup();
void printTime();

void openColumn1();
void openColumn2();
void openColumn3();
void openColumn4();
void openColumn5();
void closeColumn1();
void closeColumn2();
void closeColumn3();
void closeColumn4();
void closeColumn5();

extern const char *ntpServer;

#endif

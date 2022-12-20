#include "common.hh"
#include "cJSON.h"
#include <Arduino.h>
#include <WiFi.h>
#include "mqtt_client.h"

struct tm schedule[NUM_BANKS][NUM_WEEK_DAYS][NUM_MAX_N_TIMES];
struct tm last_time_dispensed[NUM_BANKS];

// Replace with your network credentials (STATION)
const char *ssid = "test1";
const char *password = "test1234";

const char *ntpServer = "time.google.com";

extern const char *srv_cert;

void initWiFi()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi ..");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print('.');
        delay(1000);
    }
    Serial.println(WiFi.localIP());
}

// Test Mosquitto server, see: https://test.mosquitto.org
// const char *server = "mqtt://test.mosquitto.org:1883";
const char *server = "mqtts://4e2bde16ffff46d9a8f341015b213f81.s2.eu.hivemq.cloud:8883";
const char *usr = "IoT_2022_drug_dealer";
const char *pwd = "8b70ty34ft2inuhdafbvjhsafg)*&^TFU&^*(_UV$^%^$%_)($%!(*@#KUGAFVD872134509yfsd";

const char *subscribeTopic = "update/4";
const char *publishTopic = "baa";

esp_mqtt_client_config_t mqtt_cfg;
esp_mqtt_client_handle_t mqtt_client;

int pubCount = 0;

void onMqttConnect(esp_mqtt_client_handle_t client)
{
    Serial.println("onMqttConnect");
    esp_mqtt_client_subscribe(client, subscribeTopic, 0);
    Serial.print("subscribed to: ");
    Serial.println(subscribeTopic);
}

void onMqttData(esp_mqtt_event_handle_t event)
{
    char *buf = (char *)malloc(event->data_len + 1);
    memcpy(buf, event->data, event->data_len);
    buf[event->data_len] = '\0';
    Serial.printf("data=%s\n", buf);
    json_to_DateTime(buf);
    free(buf);
}

void onError(esp_mqtt_event_handle_t event)
{
    esp_mqtt_error_codes_t *err;
    err = event->error_handle;

    Serial.printf("error_type: ");
    switch (err->error_type)
    {
    case MQTT_ERROR_TYPE_NONE:
        Serial.println("None");
        break;
    case MQTT_ERROR_TYPE_TCP_TRANSPORT:
        Serial.println("TCP Transport");
        break;
    case MQTT_ERROR_TYPE_CONNECTION_REFUSED:
        Serial.println("Connection refused");
        break;
    default:
        Serial.println("Undefined");
        break;
    }

    Serial.printf("connect_ret_code: ");
    switch (err->connect_return_code)
    {
    case MQTT_CONNECTION_ACCEPTED:
        Serial.println("Connection accepted");
        break;
    case MQTT_CONNECTION_REFUSE_PROTOCOL:
        Serial.println("Wrong protocol");
        break;
    case MQTT_CONNECTION_REFUSE_ID_REJECTED:
        Serial.println("ID rejected");
        break;
    case MQTT_CONNECTION_REFUSE_SERVER_UNAVAILABLE:
        Serial.println("Server unavailable");
        break;
    case MQTT_CONNECTION_REFUSE_BAD_USERNAME:
        Serial.println("Wrong user");
        break;
    case MQTT_CONNECTION_REFUSE_NOT_AUTHORIZED:
        Serial.println("Wrong username or password");
        break;
    default:
        Serial.println("Undefined");
        break;
    }
}

static esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event)
{
    Serial.println("mqtt_event_handler");

    esp_mqtt_client_handle_t mqttclient = event->client;
    int msg_id;
    switch (event->event_id)
    {
    case MQTT_EVENT_CONNECTED:
        onMqttConnect(mqttclient);
        break;
    case MQTT_EVENT_DATA:
        onMqttData(event);
        break;
    case MQTT_EVENT_ANY:
        Serial.println("MQTT_EVENT_ANY");
        break;
    case MQTT_EVENT_ERROR:
        onError(event);
        break;
    case MQTT_EVENT_DISCONNECTED:
        Serial.println("MQTT_EVENT_DISCONNECTED");
        break;
    case MQTT_EVENT_SUBSCRIBED:
        Serial.println("MQTT_EVENT_SUBSCRIBED");
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        Serial.println("MQTT_EVENT_UNSUBSCRIBED");
        break;
    case MQTT_EVENT_PUBLISHED:
        Serial.println("MQTT_EVENT_PUBLISHED");
        break;
    case MQTT_EVENT_BEFORE_CONNECT:
        Serial.println("MQTT_EVENT_BEFORE_CONNECT");
        break;
    case MQTT_EVENT_DELETED:
        Serial.println("MQTT_EVENT_DELETED");
        break;
    default:
        break;
    }
    return ESP_OK;
}

void mqtt_setup()
{
    Serial.print("mqtt_setup: ");
    Serial.println(server);

    mqtt_cfg.event_handle = mqtt_event_handler;
    mqtt_cfg.uri = server;
    // mqtt_cfg.skip_cert_common_name_check = true;
    // mqtt_cfg.use_secure_element = true;
    mqtt_cfg.cert_pem = srv_cert;

    mqtt_cfg.username = usr;
    mqtt_cfg.password = pwd;

    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_start(mqtt_client);
}

void printTime()
{

    struct tm time;

    if (!getLocalTime(&time))
    {
        Serial.println("Could not obtain time info");
        return;
    }
    int year, month, mday, hour, min, sec;
    year = 1900 + time.tm_year;
    month = time.tm_mon + 1;

    Serial.printf("printTime(): %d, %d-%d-%d %d:%d:%d\n",
                  dt_to_weekday(&time), time.tm_mday, month, year, time.tm_hour,
                  time.tm_min, time.tm_sec);
}

void str_to_tm(const char *str, struct tm *t)
{
    int hh = 0, mm = 0, ss = 0;

    hh += (str[0] - 48) * 10;
    hh += str[1] - 48;

    mm += (str[3] - 48) * 10;
    mm += str[4] - 48;

    ss += (str[6] - 48) * 10;
    ss += str[7] - 48;

    t->tm_hour = hh;
    t->tm_min = mm;
    t->tm_sec = ss;
    t->tm_year = DATE_SET_FLAG;
}

bool json_to_DateTime(const char *json_str)
{

    const cJSON *drugName = NULL;
    const cJSON *column = NULL;
    const cJSON *openTimes = NULL;
    const cJSON *dayTimes = NULL;
    const cJSON *shootTime = NULL;
    const cJSON *light = NULL;
    const cJSON *sound = NULL;
    bool status = false;
    struct tm tmp_t;
    const char *week[] = {"poniedzialek", "wtorek", "sroda", "czwartek", "piatek", "sobota", "niedziela"};
    int i, j;

    cJSON *msg_json = cJSON_Parse(json_str);
    if (msg_json == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            Serial.printf("json_to_DateTime(): Error before: %s\n", error_ptr);
        }
        status = false;
        goto end;
    }

    drugName = cJSON_GetObjectItemCaseSensitive(msg_json, "drugName");
    column = cJSON_GetObjectItemCaseSensitive(msg_json, "column");

    Serial.printf("json_to_DateTime(): drugName=%s\n", drugName->valuestring);
    Serial.printf("json_to_DateTime(): column=%s\n", column->valuestring);

    openTimes = cJSON_GetObjectItemCaseSensitive(msg_json, "openTimes");
    if (!cJSON_IsObject(openTimes))
    {
        Serial.println("json_to_DateTime(): openTimes not object");
        status = false;
        goto end;
    }

    for (i = 0; i < 7; i++)
    {
        dayTimes = cJSON_GetObjectItemCaseSensitive(openTimes, week[i]);
        if (!cJSON_IsArray(dayTimes))
        {
            Serial.println("json_to_DateTime(): dayTimes not array");
            status = false;
            goto end;
        }
        j = 0;
        cJSON_ArrayForEach(shootTime, dayTimes)
        {
            str_to_tm(shootTime->valuestring, &tmp_t);
            schedule[column->valuestring[0] - 97][i][j] = tmp_t;
            j++;
            Serial.printf("json_to_DateTime(): Set schedule column: %s, day: %s, hour: %d:%d:%d\n", column->valuestring, week[i], tmp_t.tm_hour, tmp_t.tm_min, tmp_t.tm_sec);
        }
    }

end:
    cJSON_Delete(msg_json);
    return status;
}

unsigned int dt_to_weekday(struct tm *t)
{
    int d, m, y;
    int weekday;

    d = t->tm_mday;
    m = t->tm_mon + 1;
    y = t->tm_year + 1900;

    weekday = (d += m < 3 ? y-- : y - 2, 23 * m / 9 + d + 4 + y / 4 - y / 100 + y / 400) % 7;

    return (weekday + 6) % 7; // 0=Monday, 6=Sunday
}

void DispatchPill(int i)
{
    switch (i)
    {
    case 0:
        openColumn1();
        break;
    case 1:
        openColumn2();
        break;
    case 2:
        openColumn3();
        break;
    case 3:
        openColumn4();
        break;
    case 4:
        openColumn5();
        break;
    }
}

void try_dispense_pill()
{
    int i, j, k;
    static unsigned int last_day;
    struct tm time;
    int curr_secs;
    int tmp_secs;
    int tmp2_secs;

    if (!getLocalTime(&time))
    {
        Serial.println("try_dispense_pill(): Could not obtain time info");
        return;
    }

    if (last_day != dt_to_weekday(&time))
    {
        for (i = 0; i < NUM_BANKS; i++)
        {
            for (j = 0; j < NUM_MAX_N_TIMES; j++)
            {
                last_time_dispensed[i] = {0};
            }
        }
        last_day = dt_to_weekday(&time);
    }

    for (i = 0; i < NUM_BANKS; i++)
    {
        for (j = 0; j < NUM_WEEK_DAYS; j++)
        {
            for (k = 0; k < NUM_MAX_N_TIMES; k++)
            {
                if (schedule[i][j][k].tm_year != DATE_SET_FLAG)
                {
                    continue; // this Bank/week/time_slot wasn't set
                }
                curr_secs = time.tm_hour * 60 * 60 + time.tm_min * 60 + time.tm_sec;
                tmp_secs = schedule[i][dt_to_weekday(&time)][k].tm_hour * 60 * 60 +
                           schedule[i][dt_to_weekday(&time)][k].tm_min * 60 +
                           schedule[i][dt_to_weekday(&time)][k].tm_sec;
                tmp2_secs = last_time_dispensed[i].tm_hour * 60 * 60 +
                            last_time_dispensed[i].tm_min * 60 + last_time_dispensed[i].tm_sec;

                if (
                    (((curr_secs - tmp_secs >= 0) &&
                      ((last_time_dispensed[i].tm_year == DATE_SET_FLAG) &&
                       (tmp_secs > tmp2_secs))) ||
                     ((curr_secs - tmp_secs >= 0) &&
                      (last_time_dispensed[i].tm_year != DATE_SET_FLAG))) &&
                    curr_secs - tmp_secs <= 30)
                {
                    Serial.printf("try_dispense_pill(): Dispensing pill from bank %c...\n", i + 97);
                    DispatchPill(i);
                    // i - numer przegrody od 0 do 4
                    last_time_dispensed[i].tm_hour = time.tm_hour;
                    last_time_dispensed[i].tm_min = time.tm_min;
                    last_time_dispensed[i].tm_sec = time.tm_sec;
                    last_time_dispensed[i].tm_year = DATE_SET_FLAG;
                }
            }
        }
    }
}

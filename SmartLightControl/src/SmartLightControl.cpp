#include "SmartLightControl.h"
#include "certificates.h"

#define BUFLEN 256

HTTPClient https;
WiFiClientSecure* client = new WiFiClientSecure;

int32_t audio_buf[BUFLEN];

// I2S configuration
static const i2s_port_t i2s_num = I2S_NUM_0;

static const i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = 22050,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
    .intr_alloc_flags = 0,
    .dma_buf_count = 8,
    .dma_buf_len = 64,
    .use_apll = false
};

static const i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_WS,
    .ws_io_num = I2S_SCK,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_SD
};

SmartLightControl::SmartLightControl() {}

void SmartLightControl::begin() {
    Serial.begin(115200);

    initI2S();
    initWiFi();

    pinMode(LDR_PIN, INPUT);

    // LED setup
    ledcSetup(0, 5000, 8);
    ledcSetup(1, 5000, 8);
    ledcSetup(2, 5000, 8);
    ledcAttachPin(RED_PIN, 0);
    ledcAttachPin(GREEN_PIN, 1);
    ledcAttachPin(BLUE_PIN, 2);
}

// initialize i2C communication
void SmartLightControl::initI2S() {
    i2s_driver_install(i2s_num, &i2s_config, 0, NULL);
    REG_SET_BIT(I2S_TIMING_REG(i2s_num), BIT(9));
    REG_SET_BIT(I2S_CONF_REG(i2s_num), I2S_RX_MSB_SHIFT);
    i2s_set_pin(i2s_num, &pin_config);
    delay(500);
}

// initiate wifi connection
void SmartLightControl::initWiFi() {
    WiFi.begin(SSID, PASSWORD);
    pinMode(ONBOARD_LED_PIN, OUTPUT);

    while (WiFi.status() != WL_CONNECTED) {
        digitalWrite(ONBOARD_LED_PIN, LOW); delay(250);
        digitalWrite(ONBOARD_LED_PIN, HIGH); delay(250);
    }
    digitalWrite(ONBOARD_LED_PIN, LOW);
    Serial.println("Connected to WiFi");
    Serial.println(WiFi.localIP());
}

void SmartLightControl::setColor(int r, int g, int b) {
    ledcWrite(0, r);
    ledcWrite(1, g);
    ledcWrite(2, b);
}

// http requests handler
String SmartLightControl::httpGETRequest(const char* url) {
    client->setCACert(root_cacert);
    String payload = "";

    if (https.begin(*client, url)) {
        https.addHeader("Content-Type", "application/x-www-form-urlencoded");
        String request = "ldr_sensor_API_KEY=" + String(API_KEY);
        int code = https.POST(request);
        if (code == 200) {
            payload = https.getString();
        } else {
            Serial.printf("HTTP error code: %d\n", code);
        }
        https.end();
    }
    return payload;
}

void SmartLightControl::update() {
    size_t bytes_read = 0;
    i2s_read(i2s_num, &audio_buf, BUFLEN, &bytes_read, portMAX_DELAY);

    int32_t cleanBuf[BUFLEN / 2] = {0};
    int cleanBufIdx = 0;
    for (int i = 0; i < BUFLEN; i++) {
        if (audio_buf[i] != 0) {
            cleanBuf[cleanBufIdx++] = audio_buf[i] >> 14;
        }
    }

    float mean = 0;
    for (int i = 0; i < cleanBufIdx; i++) {
        mean += cleanBuf[i];
    }
    mean /= cleanBufIdx;

    for (int i = 0; i < cleanBufIdx; i++) {
        cleanBuf[i] -= mean;
    }

    float minVal = 100000, maxVal = -100000;
    for (int i = 0; i < cleanBufIdx; i++) {
        if (cleanBuf[i] < minVal) minVal = cleanBuf[i];
        if (cleanBuf[i] > maxVal) maxVal = cleanBuf[i];
    }

    loudness = maxVal - minVal;

    // connect to cloud platform and get response
    String res = httpGETRequest(server_url);
    JSONVar parsed = JSON.parse(res);
    if (JSON.typeof(parsed) == "undefined") {
        Serial.println("Failed to parse server response");
        return;
    }

    // decode set LDR threshold
    ldr_threshold = (int)parsed["ldr_threshold"];

    // read LDR sensor value
    lightVal = analogRead(LDR_PIN);
    ldr_status = "0";

    // control smart light
    if (lightVal > ldr_threshold && loudness > 2000) {
        setColor(100, 150, 130);
        ldr_status = "1";
    } else {
        setColor(0, 0, 0);
    }

    // Update LDR state to server
    https.begin(*client, server_url);
    https.addHeader("Content-Type", "application/x-www-form-urlencoded");
    String postData = "ldr_sensor_API_KEY=" + String(API_KEY) + "&ldr_status=" + ldr_status;
    https.POST(postData);
    https.end();
}

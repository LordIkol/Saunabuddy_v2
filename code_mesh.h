#include <esp_now.h>

static uint8_t broadcast_mac[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

typedef struct __attribute__((packed)) esp_now_msg_t
{
	uint32_t address;
    uint32_t order;
	// Can put lots of things here...
} esp_now_msg_t;

static void send_msg(esp_now_msg_t* msg)
{
    // Pack
    uint16_t packet_size = sizeof(esp_now_msg_t);
    uint8_t msg_data[packet_size];
    memcpy(&msg_data[0], msg, sizeof(esp_now_msg_t));

    esp_err_t status = esp_now_send(broadcast_mac, msg_data, packet_size);
    if (ESP_OK != status)
    {
        Serial.println("Error sending message");
    }
    else {
        Serial.println("done sending message");
    }
}

static void msg_send_cb(const uint8_t* mac, esp_now_send_status_t sendStatus)
{

    switch (sendStatus)
    {
    case ESP_NOW_SEND_SUCCESS:
        Serial.println("Send success");
        break;

    case ESP_NOW_SEND_FAIL:
        Serial.println("Send Failure");
        break;

    default:
        break;
    }
}

static void msg_recv_cb(const uint8_t* mac_addr, const uint8_t* data, int len)
{
    if (len == sizeof(esp_now_msg_t))
    {
        esp_now_msg_t msg;
        memcpy(&msg, data, len);

        Serial.print("Counter: ");
        Serial.println(msg.order);
    }
}

static void initialize_mesh(void)
{
    Serial.println("MAC: " + WiFi.macAddress());
    
    if (esp_now_init() == ESP_OK)
    {
        Serial.println("ESPNow Init Success");
    }
    else
    {
        Serial.println("ESPNow Init Failed");
        delay(3000);
        ESP.restart();
    }

    esp_now_peer_info_t peer_info = {};
    memcpy(peer_info.peer_addr, broadcast_mac, 6);
    peer_info.channel = 0;
    peer_info.encrypt = 0;
    esp_err_t status = esp_now_add_peer(&peer_info);
    if (ESP_OK != status)
    {
        Serial.println("Could not add peer");
    }
    if (ESP_OK == status)
    {
        Serial.println("peer added");
    }
    // Set up callback
    status = esp_now_register_recv_cb(msg_recv_cb);
    if (ESP_OK != status)
    {
        Serial.println("Could not register callback");
    }

    status = esp_now_register_send_cb(msg_send_cb);
    if (ESP_OK != status)
    {
        Serial.println("Could not register send callback");
    }
}

void broadcast() {
    Serial.println("Sending Message");
    esp_now_msg_t msg;
    msg.address = 1;
    msg.order = 1;
    Serial.println("Meep");
    send_msg(&msg);
    Serial.println("sent");

}

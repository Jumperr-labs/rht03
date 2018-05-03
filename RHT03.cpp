#include "RHT03.h"
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <iostream>

RHT03::RHT03() {
    // Set a callback to be called when host sets level on data pin
    SetPinChangeLevelEventCallback(GetPinNumber("data"), std::bind(&RHT03::OnPinChangeLevelEvent, this, std::placeholders::_1));
}

void RHT03::Stop() {

}

void RHT03::Main() {

}

void RHT03::OnPinChangeLevelEvent(std::vector<WireLogicLevelEvent>& notifications) {
    WireLogicLevelEvent notification = notifications[0];
    transition_type_t transition = notification.type;

    switch (state_) {
        case (IDLE): {
            if (transition == FALLING) {
                state_ = INIT;
            }
            break;
        }
        case (INIT): {
            if (transition == RISING) {
                state_ = TRANSMIT;
                StartTransmit();
            }
            break;
        }
        case (TRANSMIT): {
            break;
        }
        case (END): {
            if (transition == RISING) {
                state_ = IDLE;
            }
        }
        default: {
            break;
        }
    }
}

void RHT03::StartTransmit() {
    // Sensor Response
    int delay_ns = 30 * 1000;
    std::string pin_name = "data";
    AddTimedCallback(delay_ns, std::bind(&RHT03::SetDataLow, this), true); // Set "0" 20~40us after start signal from host
    delay_ns += 80 * 1000;
    AddTimedCallback(delay_ns, std::bind(&RHT03::SetDataHigh, this), true); // Set "1" after 80us
    delay_ns += 80 * 1000; // Hold "1" for 80us

    double humidity = GetNextDoubleFromDataGenerator("humidity");
    double temperature = GetNextDoubleFromDataGenerator("temperature");
    uint64_t humidity_value = (uint64_t) std::round((humidity * 10.0));
    uint64_t temperature_value = (uint64_t) std::round((std::abs(temperature) * 10.0));
    if (temperature < 0) {
        temperature_value = temperature_value | (1U << 15);
    }
    uint64_t checksum =
            ((humidity_value >> 8) & 0xff) + (humidity_value & 0xff) + ((temperature_value >> 8) & 0xff) + (temperature_value & 0xff);
    uint64_t data = ((humidity_value & 0xffff) << 24) | ((temperature_value & 0xffff) << 8) | (checksum & 0xff);
//    std::cout << "0x" << std::hex << data << std::dec << std::endl;
    SendData(data, delay_ns); //Hold for 80us and send data
}

void RHT03::SendData(uint64_t data, int delay_ns) {
    for (int bit_number = 39; bit_number >= 0; bit_number--) {
        bool bit = (data >> bit_number) & 1;
        AddTimedCallback(delay_ns, std::bind(&RHT03::SetDataLow, this), true); //Set "0" for 50us
        delay_ns += 50 * 1000;
        AddTimedCallback(delay_ns, std::bind(&RHT03::SetDataHigh, this), true); //Set "1"
        if (bit) {
            delay_ns += 70 * 1000; //Hold pin level for 70us for a logic "1"
        } else {
            delay_ns += 27 * 1000; //Hold pin level for 26~28us for a logic "0"
        }
    }
    AddTimedCallback(delay_ns, std::bind(&RHT03::EndTransmition, this), true);
}

void RHT03::EndTransmition() {
    SetDataLow();
    state_ = END;
}

void RHT03::SetDataHigh() {
    SetPinLevel("data", true);
}

void RHT03::SetDataLow() {
    SetPinLevel("data", false);
}
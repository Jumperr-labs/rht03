#pragma once
#include "ModelingFramework.h"

class RHT03 : public ExternalPeripheral {
 public:
    RHT03();
    void Main() override;
    void Stop() override;

    RHT03(const RHT03&) = delete;
    RHT03& operator=(const RHT03&) = delete;

  private:
    void OnPinChangeLevelEvent(std::vector<WireLogicLevelEvent>& notifications);
    void StartTransmit();
    void SendData(uint64_t data, int delay_ns);
    void EndTransmition();
    void SetDataHigh();
    void SetDataLow();

    enum {
        IDLE,
        INIT,
        TRANSMIT,
        END
    } state_ {IDLE};
};

DLL_EXPORT ExternalPeripheral *PeripheralFactory() {
    return new RHT03();
}

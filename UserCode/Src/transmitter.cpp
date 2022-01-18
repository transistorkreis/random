//! \brief     Class definition for setting the outputs and LEDs via SPI.
//! \details   There are 4 outputs and 1 LED for each output on the front panel.
//! \file      transmitter.cpp
//! \author    André Niederlein
//! \date      2020-07-31
//! \copyright GNU General Public License v3:
//! \copyright https://opensource.org/licenses/GPL-3.0
//
//  compliant to the Google C++ Style Guide:
//  https://google.github.io/styleguide/cppguide.html

// INCLUDES --------------------------------------------------------------------
#include "transmitter.hpp"

// MICS ------------------------------------------------------------------------
namespace tkrandom {

// MEMBER FUNCTIONS ------------------------------------------------------------
Transmitter::Transmitter(SPI_HandleTypeDef* const spi_handle,
                         GPIO_TypeDef* const gpio_port_nss,
                         const uint16_t pin_nss,
                         GPIO_TypeDef* const gpio_port_dac,
                         const uint16_t pin_dac)
    : spi_handle_(spi_handle),
      kTimeout_(1000U),
      kWriteCommand_(0x30U),
      gpio_port_nss_(gpio_port_nss),
      gpio_port_dac_(gpio_port_dac),
      kPinNss_(pin_nss),
      kPinDac_(pin_dac),
      kLedOffValue_(27500U) {
  HAL_GPIO_WritePin(gpio_port_nss_, kPinNss_, GPIO_PIN_SET);  // SPI NSS line
}
//------------------------------------------------------------------------------
void Transmitter::Init() {
  HAL_GPIO_WritePin(gpio_port_dac_,
                    kPinDac_,
                    GPIO_PIN_RESET);
  for (volatile uint8_t i = 0U; i < 50U; ++i) {
    // just waits a short moment
  }
  // gets DAC out of reset
  HAL_GPIO_WritePin(gpio_port_dac_,
                    kPinDac_,
                    GPIO_PIN_SET);
}
//------------------------------------------------------------------------------
TransmitterStatus Transmitter::SetVoltage(const Output output,
                                          const uint16_t value) const {
  const uint8_t address = static_cast<uint8_t>(output);
  return TransmitValue(address, value);
}
//------------------------------------------------------------------------------
TransmitterStatus Transmitter::SetLedBrightness(const Led led,
                                                const uint16_t value) const {
  const uint8_t address = static_cast<uint8_t>(led);
  // considers that a minimum voltage is required to let an LED illuminate
  const double value_ratio =
      static_cast<double>(value) / static_cast<double>(0xffffU);
  const uint16_t range = 0xffffU - kLedOffValue_;
  const double range_part = value_ratio * static_cast<double>(range);
  const uint16_t new_value = static_cast<uint16_t>(range_part) + kLedOffValue_;

  return TransmitValue(address, new_value);
}
//------------------------------------------------------------------------------
TransmitterStatus Transmitter::TransmitValue(const uint8_t address,
                                             const uint16_t value) const {
  TransmitterStatus return_value = TransmitterStatus::kError;

  const uint8_t first_byte = kWriteCommand_ + address;
  const uint8_t second_byte = static_cast<uint8_t>(value >> 8U);
  const uint8_t third_byte = static_cast<uint8_t>(value);
  uint8_t data[3U] = {first_byte, second_byte, third_byte};
  HAL_GPIO_WritePin(gpio_port_nss_, kPinNss_, GPIO_PIN_RESET);
  const HAL_StatusTypeDef hal_status =
      HAL_SPI_Transmit(spi_handle_, data, 3U, kTimeout_);
  HAL_GPIO_WritePin(gpio_port_nss_, kPinNss_, GPIO_PIN_SET);

  if (hal_status == HAL_OK) {
    return_value = TransmitterStatus::kSuccess;
  }

  return return_value;
}

}  // namespace tkrandom

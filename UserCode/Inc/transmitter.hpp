//! \brief     Class declaration for setting the outputs and LEDs via SPI.
//! \details   There are 4 outputs and 1 LED for each output on the front panel.
//! \file      transmitter.hpp
//! \author    André Niederlein
//! \date      2020-07-31
//! \copyright GNU General Public License v3:
//! \copyright https://opensource.org/licenses/GPL-3.0
//
//  compliant to the Google C++ Style Guide:
//  https://google.github.io/styleguide/cppguide.html

#ifndef TRANSMITTER_HPP_
#define TRANSMITTER_HPP_

// INCLUDES --------------------------------------------------------------------
#include "stm32l4xx_hal.h"

namespace tkrandom {

// TYPE DECLARATIONS -----------------------------------------------------------
//! enum for the front panel voltage outputs
//! \details enum integer value equals DAC output address
enum class Output {
  kOutput1 = 0U,  //!< front panel output 1
  kOutput2 = 1U,  //!< front panel output 2
  kOutput3 = 2U,  //!< front panel output 3
  kOutput4 = 3U   //!< front panel output 4
};

//! enum for the front panel LEDs
//! \details enum integer value equals DAC output address
enum class Led {
  kLed1 = 4U,  //!< front panel LED 1
  kLed2 = 5U,  //!< front panel LED 2
  kLed3 = 6U,  //!< front panel LED 3
  kLed4 = 7U   //!< front panel LED 4
};

//! enum type for Transmitter member function return values
enum class TransmitterStatus {
  kSuccess = 0U,  //!< successful execution
  kError          //!< an error occurred
};

// CLASS DECLARATION -----------------------------------------------------------
//! Transmitter class declaration
class Transmitter {
 public:
  //! constructor
  //! \param[in] spi_handle pointer to SPI instance of HAL SPI driver
  //! \param[in] gpio_port_nss pionter to GPIO port for SPI NSS
  //! \param[in] pin_nss pin number of SPI NSS
  Transmitter(SPI_HandleTypeDef* const spi_handle,
              GPIO_TypeDef* const gpio_port_nss,
              const uint16_t pin_nss,
              GPIO_TypeDef* const gpio_port_dac,
              const uint16_t pin_dac);

  //! destructor
  ~Transmitter() {}

  //! no copy constructor allowed since there is only one instance
  Transmitter(const Transmitter&) = delete;

  //! no assignment operator allowed since there is only one instance
  Transmitter& operator=(Transmitter const&) = delete;

  //! initializes DAC
  void Init(void);

  //! sets the value of a front panel voltage output
  //! \param[in] output output the value is set for
  //! \param[in] value value that is set (0 .. 2^16-1)
  //! \return returns kSuccess if no error occurs
  TransmitterStatus SetVoltage(const Output output, const uint16_t value) const;

  //! sets the value of a front panel LED
  //! \param[in] output output whose LED brightness is set
  //! \param[in] value value that is set (0 .. 2^16-1)
  //! \return returns kSuccess if no error occurs
  TransmitterStatus SetLedBrightness(const Led output,
                                     const uint16_t value) const;

 private:
  //! transmits the value for a voltage output or LED to the DAC via SPI
  //! \param[in] address output port of DAC
  //! \param[in] value value that is set (0 .. 2^16-1)
  //! \return returns kSuccess if no error occurs
  TransmitterStatus TransmitValue(const uint8_t address,
                                  const uint16_t value) const;

  //! pointer to SPI instance of HAL SPI driver
  SPI_HandleTypeDef* spi_handle_;

  //! timeout value for SPI HAL driver
  const uint32_t kTimeout_;

  //! DAC write command for SPI transfer
  const uint8_t kWriteCommand_;

  //! pionter to GPIO port for SPI NSS
  GPIO_TypeDef* const gpio_port_nss_;

  //! pionter to GPIO port for DAC reset line
  GPIO_TypeDef* const gpio_port_dac_;

  //! pin number of SPI NSS
  const uint16_t kPinNss_;

  //! pin number of DAC reset line
  const uint16_t kPinDac_;

  //! DAC output value that must be exceeded to make an LED illuminate
  // \details this value is set to an DAC LED output if the random value is Zero
  const uint16_t kLedOffValue_;


};

}  // namespace tkrandom

#endif  // TRANSMITTER_HPP_

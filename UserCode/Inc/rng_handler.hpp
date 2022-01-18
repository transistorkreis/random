//! \brief     Class declaration for the random number buffers handling.
//! \details   Handles number buffers with uniform and normal distribution.
//! \file      rng_handler.hpp
//! \author    André Niederlein
//! \date      2021-01-07
//! \copyright GNU General Public License v3:
//! \copyright https://opensource.org/licenses/GPL-3.0
//
//  compliant to the Google C++ Style Guide:
//  https://google.github.io/styleguide/cppguide.html

#ifndef RNG_HANDLER_HPP_
#define RNG_HANDLER_HPP_

// INCLUDES --------------------------------------------------------------------
#include "transmitter.hpp"
#include "generator.hpp"

namespace tkrandom {

// TYPE DECLARATIONS -----------------------------------------------------------
//! enum type for the output voltage distribution
enum class Distribution {
  kUniform,  //!< uniform distribution
  kNormal    //!< normal distribution
};

//! enum type for Calculator member function return values
enum class RngHandlerStatus {
  kSuccess,        //!< successful execution
  kErrorTransfer,  //!< error with SPI transfer
  kErrorRng        //!< error with random number generation
};

// CLASS DECLARATION -----------------------------------------------------------
//! RngHandler class declaration
class RngHandler {
 public:
  //! constructor
  //! \param[in] generator Generator reference for generation of random numbers
  //! \param[in] transmitter Transmitter reference for SPI transfers to DAC
  RngHandler(Generator& generator, Transmitter& transmitter);

  //! destructor
  ~RngHandler(void) {}

  //! no copy constructor allowed since there is only one instance
  RngHandler(const RngHandler&) = delete;

  //! no assignment operator allowed since there is only one instance
  RngHandler& operator=(RngHandler const&) = delete;

  //! initializes Transmitter instance and DAC, fills random buffers
  void Init(void);

  //! sets random voltage for front panel outputs and LEDs 1, 2, 3 and/or 4
  //! \param[in] is_gate_1 outputs and LEDs 1, 2, 3 are set if true
  //! \param[in] is_gate_2 outputs and LEDs 4 are set if true
  //! \return kSuccess if no error occurred
  RngHandlerStatus SetOutputsLeds(const bool is_gate_1, const bool is_gate_2);

  //! sets distribution of the random voltage for the given output
  //! \param[in] output output the distribution is set for
  //! \param[in] distribution distribution that is set for output
  void SetDistribution(const Output output, const Distribution distribution);

  //! uses timer interrupt to reset STM32 RNG periodically
  //! \details periodical reset since RNG seed error detection is switched off
  void ProcessTick();

 private:
  //! fills up random buffers with new random numbers
  //! \return kSuccess if no error occurred
  RngHandlerStatus FillBuffers(void);

  //! sets random voltages for front panel outputs 1, 2, 3
  //! \return kSuccess if no error occurred
  TransmitterStatus SetOutputs123(void);

  //! sets random values for front panel LEDs 1, 2, 3
  //! \return kSuccess if no error occurred
  TransmitterStatus SetLeds123(void);

  //! sets random voltage for front panel output 4
  //! \return kSuccess if no error occurred
  TransmitterStatus SetOutput4(void);

  //! sets random value for front panel LED 4
  //! \return kSuccess if no error occurred
  TransmitterStatus SetLed4(void);

  //! resets buffer index to previous value in order to set LED values
  //! \details modifies buffer index of buffer_uniform and/or buffer_normal
  //! \param[in] is_gate_1 adds 3 to buffer index if true
  //! \param[in] is_gate_2 adds 1 to buffer index if true
  void RollbackBufferIndexes(const bool is_gate_1, const bool is_gate_2);

  //! getter for the distribution of the random voltage for the given output
  //! \param[in] output output whose distribution is requested
  //! \return distribution of the random voltage of the given output
  Distribution GetDistribution(const Output output) const;

  //! returns one random number depending on distribution for given output
  //! \param[in] output output the random number is used for
  //! \return one random number from buffer_uniform_[] or buffer_normal_[]
  uint16_t GetRandomNumber(const Output output);

  //! TODO
  Generator& generator_;

  //! Transmitter reference for SPI transfers to DAC
  Transmitter& transmitter_;

  //! random voltage distribution of output 1
  Distribution distribution_1_;

  //! random voltage distribution of output 2
  Distribution distribution_2_;

  //! random voltage distribution of output 3
  Distribution distribution_3_;

  //! random voltage distribution of output 4buffer_index_
  Distribution distribution_4_;

  //! current number of random values in buffer with uniform distribution
  uint32_t index_uniform_;

  //! current number of random values in buffer with normal distribution
  uint32_t index_normal_;

  //! size of buffer with random numbers
  static const uint32_t kBufferSize_ = 4U;

  //! buffer with generated random numbers
  uint16_t buffer_uniform_[kBufferSize_];

  //! buffer with generated random numbers
  uint16_t buffer_normal_[kBufferSize_];
};

}  // namespace tkrandom

#endif  // RNG_HANDLER_HPP_

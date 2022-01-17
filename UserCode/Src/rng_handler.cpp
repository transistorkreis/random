//  published under the MIT License
//  [github.com/transistorkreis/tkrandom]
//
//  compliant to the Google C++ Style Guide
//  [https://google.github.io/styleguide/cppguide.html]
//
//! \brief   class defintion for the random number buffers handling
//! \details there are random numbers with uniform and normal distribution
//! \file    rng_buffers.cpp
//! \author  transistorkreis
//! \date    2021-01-07

// INCLUDES --------------------------------------------------------------------
#include "rng_handler.hpp"

// MICS ------------------------------------------------------------------------
namespace tkrandom {

// MEMBER FUNCTIONS ------------------------------------------------------------
RngHandler::RngHandler(Generator& generator,
                       Transmitter& transmitter)
    : generator_(generator),
      transmitter_(transmitter),
      distribution_1_(Distribution::kUniform),
      distribution_2_(Distribution::kUniform),
      distribution_3_(Distribution::kUniform),
      distribution_4_(Distribution::kUniform),
      index_uniform_(0U),
      index_normal_(0U) {

}
//------------------------------------------------------------------------------
void RngHandler::Init() {
  transmitter_.Init();
  FillBuffers();
}
//------------------------------------------------------------------------------
RngHandlerStatus RngHandler::SetOutputsLeds(const bool is_gate_1,
                                            const bool is_gate_2) {
  RngHandlerStatus return_value = RngHandlerStatus::kSuccess;

  // sets all voltage outputs ahead of LEDs to reduce output latency
  if (is_gate_1) {
    if (SetOutputs123() != TransmitterStatus::kSuccess) {
      return_value = RngHandlerStatus::kErrorTransfer;
    }
  }
  if (is_gate_2) {
    if (SetOutput4() != TransmitterStatus::kSuccess) {
      return_value = RngHandlerStatus::kErrorTransfer;
    }
  }
  // rollback of random buffer indexes for LED values
  RollbackBufferIndexes(is_gate_1, is_gate_2);
  // sets brightness of LEDs
  if (is_gate_1) {
    if (SetLeds123() != TransmitterStatus::kSuccess) {
      return_value = RngHandlerStatus::kErrorTransfer;
    }
  }
  if (is_gate_2) {
    if (SetLed4() != TransmitterStatus::kSuccess) {
      return_value = RngHandlerStatus::kErrorTransfer;
    }
  }
  // fills random buffers with new random numbers
  if (FillBuffers() != RngHandlerStatus::kSuccess) {
    return_value = RngHandlerStatus::kErrorRng;
  }

  return return_value;
}
//------------------------------------------------------------------------------
void RngHandler::SetDistribution(const Output output,
                                 const Distribution distribution) {
  switch (output) {
    case Output::kOutput1:
      distribution_1_ = distribution;
      break;
    case Output::kOutput2:
      distribution_2_ = distribution;
      break;
    case Output::kOutput3:
      distribution_3_ = distribution;
      break;
    case Output::kOutput4:
      distribution_4_ = distribution;
      break;
    default:
      break;
  }

}
//------------------------------------------------------------------------------
RngHandlerStatus RngHandler::FillBuffers() {
  RngHandlerStatus return_value = RngHandlerStatus::kSuccess;
  uint16_t rng_number = 0U;

  // fills random buffer with uniform distribution
  while (index_uniform_ < kBufferSize_) {
    const GeneratorStatus generator_status =
        generator_.GetUniformRandomNumber(&rng_number);
    if (generator_status == GeneratorStatus::kSuccess) {
      buffer_uniform_[index_uniform_] = rng_number;
    }
    else {
      return_value = RngHandlerStatus::kErrorRng;
    }
    index_uniform_++;  // incremented also in error case to prevent deadlock
  }
  // fills random buffer with normal distribution
  while (index_normal_ < kBufferSize_) {
    const GeneratorStatus generator_status =
        generator_.GetNormalRandomNumber(&rng_number);
    if (generator_status == GeneratorStatus::kSuccess) {
      buffer_normal_[index_normal_] = rng_number;
    }
    else {
      return_value = RngHandlerStatus::kErrorRng;
    }
      index_normal_++;  // incremented also in error case to prevent deadlock
  }

  return return_value;
}
//------------------------------------------------------------------------------
TransmitterStatus RngHandler::SetOutputs123(void) {
  TransmitterStatus return_value = TransmitterStatus::kSuccess;
  uint8_t output = static_cast<uint8_t>(Output::kOutput1);

  while (output <= static_cast<uint8_t>(Output::kOutput3)) {
    const uint16_t random_number = GetRandomNumber(static_cast<Output>(output));
    const TransmitterStatus transmitter_status =
        transmitter_.SetVoltage(static_cast<Output>(output), random_number);
    if (transmitter_status != TransmitterStatus::kSuccess) {
      return_value = TransmitterStatus::kError;
    }
    output++;
  }

  return return_value;
}
//------------------------------------------------------------------------------
TransmitterStatus RngHandler::SetLeds123(void) {
  TransmitterStatus return_value = TransmitterStatus::kSuccess;
  uint8_t output = static_cast<uint8_t>(Output::kOutput1);
  uint8_t led = static_cast<uint8_t>(Led::kLed1);

  while (output <= static_cast<uint8_t>(Output::kOutput3)) {
    const uint16_t random_number = GetRandomNumber(static_cast<Output>(output));
    const TransmitterStatus transmitter_status =
        transmitter_.SetLedBrightness(static_cast<Led>(led), random_number);
    if (transmitter_status != TransmitterStatus::kSuccess) {
      return_value = TransmitterStatus::kError;
    }
    led++;
    output++;
  }

  return return_value;
}
//------------------------------------------------------------------------------
TransmitterStatus RngHandler::SetOutput4(void) {
  return transmitter_.SetVoltage(Output::kOutput4,
                                 GetRandomNumber(Output::kOutput4));
}
//------------------------------------------------------------------------------
TransmitterStatus RngHandler::SetLed4(void) {
  return transmitter_.SetLedBrightness(Led::kLed4,
                                       GetRandomNumber(Output::kOutput4));
}
//------------------------------------------------------------------------------
void RngHandler::RollbackBufferIndexes(const bool is_gate_1,
                                       const bool is_gate_2) {
  if (is_gate_1) {
    uint8_t output = static_cast<uint8_t>(Output::kOutput1);
    while (output <= static_cast<uint8_t>(Output::kOutput3)) {
      const Distribution distribution =
          GetDistribution(static_cast<Output>(output));
      if (distribution == Distribution::kNormal) {
        index_normal_++;
      }
      else {
        index_uniform_++;
      }
      output++;
    }
  }
  if (is_gate_2) {
    if (distribution_4_ == Distribution::kNormal) {
      index_normal_++;
    }
    else {
      index_uniform_++;
    }
  }
}
//------------------------------------------------------------------------------
Distribution RngHandler::GetDistribution(const Output output) const {
  Distribution return_value = Distribution::kUniform;

  switch (output) {
    case Output::kOutput1:
      return_value = distribution_1_;
      break;
    case Output::kOutput2:
      return_value = distribution_2_;
      break;
    case Output::kOutput3:
      return_value = distribution_3_;
      break;
    case Output::kOutput4:
      return_value = distribution_4_;
      break;
    default:
      break;
  }

  return return_value;
}
//------------------------------------------------------------------------------
uint16_t RngHandler::GetRandomNumber(const Output output) {
  uint16_t return_value = 0U;

  if (GetDistribution(output) == Distribution::kNormal) {
    return_value = buffer_normal_[(index_normal_ - 1U)];
    index_normal_--;
  }
  else {
    return_value = buffer_uniform_[(index_uniform_ - 1U)];
    index_uniform_--;
  }

  return return_value;
}
//------------------------------------------------------------------------------
void RngHandler::ProcessTick() {
  generator_.ResetRng();
}

}  // namespace tkrandom

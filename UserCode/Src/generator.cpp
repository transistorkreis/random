//  published under the MIT License
//  [github.com/transistorkreis/Random]
//
//  compliant to the Google C++ Style Guide
//  [https://google.github.io/styleguide/cppguide.html]
//
//! \brief   class definition for random number generation
//! \details generates 16-bit random numbers (uniform and normal distribution)
//! \file    generator.cpp
//! \author  transistorkreis
//! \date    2021-01-06

// INCLUDES --------------------------------------------------------------------
#include "generator.hpp"

// MICS ------------------------------------------------------------------------
namespace tkrandom {

// MEMBER FUNCTIONS ------------------------------------------------------------
Generator::Generator(RNG_HandleTypeDef* const random_handle)
    : random_handle_(random_handle) {

}
//------------------------------------------------------------------------------
GeneratorStatus Generator::GetUniformRandomNumber(uint16_t* number) const {
  GeneratorStatus return_value = GeneratorStatus::kSuccess;
  uint32_t rng_number = 0U;
  const HAL_StatusTypeDef hal_status =
      HAL_RNG_GenerateRandomNumber(random_handle_, &rng_number);
  if (hal_status == HAL_OK) {
    *number = static_cast<uint16_t>(rng_number);
  }
  else {
    *number = 0U;
    return_value = GeneratorStatus::kError;
  }
  return return_value;
}
//------------------------------------------------------------------------------
GeneratorStatus Generator::GetNormalRandomNumber(uint16_t* number) const {
  GeneratorStatus return_value = GeneratorStatus::kSuccess;
  uint32_t rng_number = 0U;
  uint32_t sum = 0U;

  // simple and computationally lightweight algorithm for normal distribution
  // with musically reasonable result (high standard deviation):
  // divides the sum of 4 uniform random numbers by 4
  for (uint8_t i = 0U; i < 2U; ++i) {
    const HAL_StatusTypeDef hal_status =
        HAL_RNG_GenerateRandomNumber(random_handle_, &rng_number);
    if (hal_status == HAL_OK) {
      sum += static_cast<uint16_t>(rng_number);
      sum += static_cast<uint16_t>(rng_number >> 16U);
    }
    else {
      sum = 0U;
      return_value = GeneratorStatus::kError;
      break;
    }
  }
  *number = static_cast<uint16_t>(sum/4U);

  return return_value;
}
//------------------------------------------------------------------------------
void Generator::ResetRng() {
  HAL_RNG_DeInit(random_handle_);
  HAL_RNG_Init(random_handle_);
}

}  // namespace tkrandom

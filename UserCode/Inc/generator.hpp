//! \brief     Class declaration for random number generation.
//! \details   Generates 16-bit random numbers (uniform or normal distribution).
//! \file      generator.hpp
//! \author    André Niederlein
//! \date      2021-01-05
//! \copyright GNU General Public License v3, see ../LICENSE
//
//  compliant to the Google C++ Style Guide:
//  https://google.github.io/styleguide/cppguide.html

#ifndef GENERATOR_HPP_
#define GENERATOR_HPP_

// INCLUDES --------------------------------------------------------------------
#include "stm32l4xx_hal.h"

namespace tkrandom {

// TYPE DECLARATIONS -----------------------------------------------------------
//! enum type for Generator member function return values
enum class GeneratorStatus {
  kSuccess = 0U,  //!< successful execution
  kError          //!< an error occurred
};

// CLASS DECLARATION -----------------------------------------------------------
//! Generator class declaration
class Generator {
 public:
  //! constructor
  //! \param[in] random_handle pointer to RNG instance of HAL RNG driver
  Generator(RNG_HandleTypeDef* const random_handle);

  //! deconstructor
  ~Generator(void) {}

  //! no copy constructor allowed since there is only one instance
  Generator(const Generator&) = delete;

  //! no assignment operator allowed since there is only one instance
  Generator& operator=(Generator const&) = delete;

  //! Getter for a 16-bit random number with uniform distribution
  //! \param[out] number 16-bit random number and 0U if an error occurred
  //! \return kSuccess if no error occurred
  GeneratorStatus GetUniformRandomNumber(uint16_t* number) const;

  //! Getter for a 16-bit random number with normal distribution
  //! \param[out] number 16-bit random number and 0U if an error occurred
    //! \return kSuccess if no error occurred
  GeneratorStatus GetNormalRandomNumber(uint16_t* number) const;

  //! Reinitializes STM32 RNG in order to fully recover from a seed error
  void ResetRng(void);

 private:
  //! pointer to RNG instance of HAL RNG driver
  RNG_HandleTypeDef* const random_handle_;
};

}  // namespace tkrandom

#endif  // GENERATOR_HPP_

/**
 * @file methods/ann/layer/leaky_relu_impl.hpp
 * @author Dhawal Arora
 *
 * Implementation of LeakyReLU layer first introduced in the acoustic model,
 * Andrew L. Maas, Awni Y. Hannun, Andrew Y. Ng,
 * "Rectifier Nonlinearities Improve Neural Network Acoustic Models", 2014
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef MLPACK_METHODS_ANN_LAYER_LEAKYRELU_IMPL_HPP
#define MLPACK_METHODS_ANN_LAYER_LEAKYRELU_IMPL_HPP

// In case it hasn't yet been included.
#include "leaky_relu.hpp"

namespace mlpack {
namespace ann /** Artificial Neural Network. */ {

template<typename InputType, typename OutputType>
LeakyReLUType<InputType, OutputType>::LeakyReLUType(
    const double alpha) : alpha(alpha)
{
  // Nothing to do here.
}

template<typename InputType, typename OutputType>
LeakyReLUType<InputType, OutputType>::LeakyReLUType(
    const LeakyReLUType& other) :
    Layer<InputType, OutputType>(other),
    alpha(other.alpha)
{
  // Nothing to do.
}

template<typename InputType, typename OutputType>
LeakyReLUType<InputType, OutputType>::LeakyReLUType(
    LeakyReLUType&& other) :
    Layer<InputType, OutputType>(std::move(other)),
    alpha(std::move(other.alpha))
{
  // Nothing to do.
}

template<typename InputType, typename OutputType>
LeakyReLUType<InputType, OutputType>&
LeakyReLUType<InputType, OutputType>::operator=(
    const LeakyReLUType& other)
{
  if (&other != this)
  {
    Layer<InputType, OutputType>::operator=(other);
    alpha = other.alpha;
  }

  return *this;
}

template<typename InputType, typename OutputType>
LeakyReLUType<InputType, OutputType>&
LeakyReLUType<InputType, OutputType>::operator=(
    LeakyReLUType&& other)
{
  if (&other != this)
  {
    Layer<InputType, OutputType>::operator=(std::move(other));
    alpha = std::move(other.alpha);
  }

  return *this;
}

template<typename InputType, typename OutputType>
void LeakyReLUType<InputType, OutputType>::Forward(
    const InputType& input, OutputType& output)
{
  output = arma::max(input, alpha * input);
}

template<typename InputType, typename OutputType>
void LeakyReLUType<InputType, OutputType>::Backward(
    const InputType& input, const OutputType& gy, OutputType& g)
{
  OutputType derivative;
  derivative.set_size(arma::size(input));
  for (size_t i = 0; i < input.n_elem; ++i)
    derivative(i) = (input(i) >= 0) ? 1 : alpha;

  g = gy % derivative;
}

template<typename InputType, typename OutputType>
template<typename Archive>
void LeakyReLUType<InputType, OutputType>::serialize(
    Archive& ar,
    const uint32_t /* version */)
{
  ar(cereal::base_class<Layer<InputType, OutputType>>(this));

  ar(CEREAL_NVP(alpha));
}

} // namespace ann
} // namespace mlpack

#endif

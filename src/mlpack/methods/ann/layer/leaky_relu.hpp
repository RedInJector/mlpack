/**
 * @file methods/ann/layer/leaky_relu.hpp
 * @author Dhawal Arora
 *
 * Definition of LeakyReLU layer first introduced in the acoustic model,
 * Andrew L. Maas, Awni Y. Hannun, Andrew Y. Ng,
 * "Rectifier Nonlinearities Improve Neural Network Acoustic Models", 2014
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef MLPACK_METHODS_ANN_LAYER_LEAKYRELU_HPP
#define MLPACK_METHODS_ANN_LAYER_LEAKYRELU_HPP

#include <mlpack/prereqs.hpp>

#include "layer.hpp"

namespace mlpack {
namespace ann /** Artificial Neural Network. */ {

/**
 * The LeakyReLU activation function, defined by
 *
 * @f{eqnarray*}{
 * f(x) &=& \max(x, alpha*x) \\
 * f'(x) &=& \left\{
 *   \begin{array}{lr}
 *     1 & : x > 0 \\
 *     alpha & : x \le 0
 *   \end{array}
 * \right.
 * @f}
 *
 * @tparam InputType The type of the layer's inputs. The layer automatically
 *     cast inputs to this type (Default: arma::mat).
 * @tparam OutputType The type of the computation which also causes the output
 *     to also be in this type. The type also allows the computation and weight
 *     type to differ from the input type (Default: arma::mat).
 */
template<typename InputType = arma::mat, typename OutputType = arma::mat>
class LeakyReLUType : public Layer<InputType, OutputType>
{
 public:
  /**
   * Create the LeakyReLU object using the specified parameters.
   * The non zero gradient can be adjusted by specifying the parameter
   * alpha in the range 0 to 1. Default (alpha = 0.03)
   *
   * @param alpha Non zero gradient.
   */
  LeakyReLUType(const double alpha = 0.03);

  //! Clone the LeakyReLUType object. This handles polymorphism correctly.
  LeakyReLUType* Clone() const { return new LeakyReLUType(*this); }

  // Virtual destructor.
  virtual LeakyReLUType() { }

  //! Copy the given LeakyReLUType.
  LeakyReLUType(const LeakyReLUType& other);
  //! Take ownership of the given LeakyReLUType.
  LeakyReLUType(LeakyReLUType&& other);
  //! Copy the given LeakyReLUType.
  LeakyReLUType& operator=(const LeakyReLUType& other);
  //! Take ownership of the given LeakyReLUType.
  LeakyReLUType& operator=(LeakyReLUType&& other);

  /**
   * Ordinary feed forward pass of a neural network, evaluating the function
   * f(x) by propagating the activity forward through f.
   *
   * @param input Input data used for evaluating the specified function.
   * @param output Resulting output activation.
   */
  void Forward(const InputType& input, OutputType& output);

  /**
   * Ordinary feed backward pass of a neural network, calculating the function
   * f(x) by propagating x backwards through f. Using the results from the feed
   * forward pass.
   *
   * @param input The propagated input activation.
   * @param gy The backpropagated error.
   * @param g The calculated gradient.
   */
  void Backward(const InputType& input, const OutputType& gy, OutputType& g);

  //! Get the non zero gradient.
  double const& Alpha() const { return alpha; }
  //! Modify the non zero gradient.
  double& Alpha() { return alpha; }

  //! Serialize the layer.
  template<typename Archive>
  void serialize(Archive& ar, const uint32_t /* version */);

 private:
  //! Leakyness Parameter in the range 0 <alpha< 1
  double alpha;
}; // class LeakyReLUType

// Convenience typedefs.

// Standard LeakyReLU layer.
typedef LeakyReLUType<arma::mat, arma::mat> LeakyReLU;


} // namespace ann
} // namespace mlpack

// Include implementation.
#include "leaky_relu_impl.hpp"

#endif

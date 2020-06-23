/**
 * @file methods/ann/init_rules/glorot_init.hpp
 * @author Prabhat Sharma
 *
 * Definition and implementation of the Glorot initialization method. This
 * initialization rule initialize the weights to maintain activation variances
 * and back-propagated gradients variance as one moves up or down the network.
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef MLPACK_METHODS_ANN_INIT_RULES_GLOROT_INIT_HPP
#define MLPACK_METHODS_ANN_INIT_RULES_GLOROT_INIT_HPP

#include <mlpack/prereqs.hpp>
#include "random_init.hpp"
#include "gaussian_init.hpp"

using namespace mlpack::math;

namespace mlpack {
namespace ann /** Artificial Neural Network. */ {

/**
 * This class is used to initialize the weight matrix with the Glorot
 * Initialization method. The method is defined by
 *
 * @f{eqnarray*}{
 * \mathrm{Var}[w_i] &=& \frac{2}{n_i + n_{i+1}} \\
 * w_i \sim \mathrm{U}[-\frac{\sqrt{6}}{\sqrt{n_i + n_{i+1}}},
 * \frac{\sqrt{6}}{\sqrt{n_i + n_{i+1}}}]
 * @f}
 *
 * where @f$ n_{i+1} @f$ is the number of neurons in the outgoing layer, @f$ n_i
 * @f$ represents the number of neurons in the ingoing layer. Here Normal
 * Distribution may also be used if needed
 *
 * For more information, see the following paper.
 *
 * @code
 * @inproceedings {pmlr-v9-glorot10a,
 *  title     = {Understanding the difficulty of training
 *               deep feedforward neural networks},
 *  author    = {Xavier Glorot and Yoshua Bengio},
 *  booktitle = {Proceedings of the Thirteenth International Conference
 *               on Artificial Intelligence and Statistics},
 *  year      = {2010}
 * }
 * @endcode
 *
 */
template<bool Uniform = true>
class GlorotInitializationType
{
 public:
  /**
   * Initialize the Glorot initialization object.
   */
  GlorotInitializationType()
  {
    // Nothing to do here.
  }

  /**
   * Initialize the elements weight matrix with glorot initialization method.
   *
   * @param W Weight matrix to initialize.
   * @param rows Number of rows.
   * @param cols Number of columns.
   */
  template<typename eT>
  void Initialize(arma::Mat<eT>& W,
                  const size_t rows,
                  const size_t cols);

  /**
   * Initialize the elements weight matrix with glorot initialization method.
   *
   * @param W Weight matrix to initialize.
   */
  template<typename eT>
  void Initialize(arma::Mat<eT>& W);

  /**
   * Initialize the elements of the specified weight 3rd order tensor with
   * glorot initialization method.
   *
   * @param W Weight matrix to initialize.
   * @param rows Number of rows.
   * @param cols Number of columns.
   * @param slices Number of slices.
   */
  template<typename eT>
  void Initialize(arma::Cube<eT>& W,
                  const size_t rows,
                  const size_t cols,
                  const size_t slices);

  /**
   * Initialize the elements of the specified weight 3rd order tensor with
   * glorot initialization method.
   *
   * @param W Weight matrix to initialize.
   */
  template<typename eT>
  void Initialize(arma::Cube<eT>& W);
}; // class GlorotInitializationType

template <>
template<typename eT>
inline void GlorotInitializationType<false>::Initialize(arma::Mat<eT>& W,
                                                       const size_t rows,
                                                       const size_t cols)
{
  if (W.is_empty())
  W = arma::mat(rows, cols);

  double var = 2.0/double(rows + cols);
  GaussianInitialization normalInit(0.0, var);
  normalInit.Initialize(W, rows, cols);
}

template <>
template<typename eT>
inline void GlorotInitializationType<false>::Initialize(arma::Mat<eT>& W)
{
  if (W.is_empty())
    Log::Fatal << "Cannot initialize and empty matrix." << std::endl;

  double var = 2.0 / double(W.n_rows + W.n_cols);
  GaussianInitialization normalInit(0.0, var);
  normalInit.Initialize(W, W.n_rows, W.n_cols);
}

template <>
template<typename eT>
inline void GlorotInitializationType<true>::Initialize(arma::Mat<eT>& W,
                                                       const size_t rows,
                                                       const size_t cols)
{
  if (W.is_empty())
  W = arma::mat(rows, cols);

  // Limit of distribution.
  double a = sqrt(6) / sqrt(rows + cols);
  RandomInitialization randomInit(-a, a);
  randomInit.Initialize(W, rows, cols);
}

template <>
template<typename eT>
inline void GlorotInitializationType<true>::Initialize(arma::Mat<eT>& W)
{
  if (W.is_empty())
    Log::Fatal << "Cannot initialize an empty matrix." << std::endl;

  // Limit of distribution.
  double a = sqrt(6) / sqrt(W.n_rows + W.n_cols);
  RandomInitialization randomInit(-a, a);
  randomInit.Initialize(W, W.n_rows, W.n_cols);
}

template <bool Uniform>
template<typename eT>
inline void GlorotInitializationType<Uniform>::Initialize(arma::Cube<eT>& W,
                                                          const size_t rows,
                                                          const size_t cols,
                                                          const size_t slices)
{
  if (W.is_empty())
  {
    W = arma::cube(rows, cols, slices);
  }
  for (size_t i = 0; i < slices; ++i)
    Initialize(W.slice(i), rows, cols);
}

template <bool Uniform>
template<typename eT>
inline void GlorotInitializationType<Uniform>::Initialize(arma::Cube<eT>& W)
{
  if (W.is_empty())
  {
    Log::Fatal << "Cannot initialize an empty matrix." << std::endl;
  }
  for (size_t i = 0; i < W.n_slices; ++i)
    Initialize(W.slice(i), W.n_rows, W.n_cols);
}

// Convenience typedefs.

/**
 * XavierInitilization is the popular name for this method.
 */
using XavierInitialization = GlorotInitializationType<true>;

/**
 * GlorotInitialization uses uniform distribution.
 */
using GlorotInitialization = GlorotInitializationType<false>;
// Uses normal distribution
} // namespace ann
} // namespace mlpack

#endif

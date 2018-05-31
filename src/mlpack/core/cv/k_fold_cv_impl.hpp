/**
 * @file simple_cv_impl.hpp
 * @author Kirill Mishchenko
 *
 * The implementation of k-fold cross-validation.
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef MLPACK_CORE_CV_K_FOLD_CV_IMPL_HPP
#define MLPACK_CORE_CV_K_FOLD_CV_IMPL_HPP

namespace mlpack {
namespace cv {

template<typename MLAlgorithm,
         typename Metric,
         typename MatType,
         typename PredictionsType,
         typename WeightsType>
KFoldCV<MLAlgorithm,
        Metric,
        MatType,
        PredictionsType,
        WeightsType>::KFoldCV(const size_t k,
                              const MatType& xs,
                              const PredictionsType& ys) :
    KFoldCV(Base(), k, xs, ys)
{ /* Nothing left to do. */ }

template<typename MLAlgorithm,
         typename Metric,
         typename MatType,
         typename PredictionsType,
         typename WeightsType>
KFoldCV<MLAlgorithm,
        Metric,
        MatType,
        PredictionsType,
        WeightsType>::KFoldCV(const size_t k,
                              const MatType& xs,
                              const PredictionsType& ys,
                              const size_t numClasses) :
    KFoldCV(Base(numClasses), k, xs, ys)
{ /* Nothing left to do. */ }

template<typename MLAlgorithm,
         typename Metric,
         typename MatType,
         typename PredictionsType,
         typename WeightsType>
KFoldCV<MLAlgorithm,
        Metric,
        MatType,
        PredictionsType,
        WeightsType>::KFoldCV(const size_t k,
                              const MatType& xs,
                              const data::DatasetInfo& datasetInfo,
                              const PredictionsType& ys,
                              const size_t numClasses) :
    KFoldCV(Base(datasetInfo, numClasses), k, xs, ys)
{ /* Nothing left to do. */ }

template<typename MLAlgorithm,
         typename Metric,
         typename MatType,
         typename PredictionsType,
         typename WeightsType>
KFoldCV<MLAlgorithm,
        Metric,
        MatType,
        PredictionsType,
        WeightsType>::KFoldCV(const size_t k,
                              const MatType& xs,
                              const PredictionsType& ys,
                              const WeightsType& weights) :
    KFoldCV(Base(), k, xs, ys, weights)
{ /* Nothing left to do. */ }

template<typename MLAlgorithm,
         typename Metric,
         typename MatType,
         typename PredictionsType,
         typename WeightsType>
KFoldCV<MLAlgorithm,
        Metric,
        MatType,
        PredictionsType,
        WeightsType>::KFoldCV(const size_t k,
                              const MatType& xs,
                              const PredictionsType& ys,
                              const size_t numClasses,
                              const WeightsType& weights) :
    KFoldCV(Base(numClasses), k, xs, ys, weights)
{ /* Nothing left to do. */ }

template<typename MLAlgorithm,
         typename Metric,
         typename MatType,
         typename PredictionsType,
         typename WeightsType>
KFoldCV<MLAlgorithm,
        Metric,
        MatType,
        PredictionsType,
        WeightsType>::KFoldCV(const size_t k,
                              const MatType& xs,
                              const data::DatasetInfo& datasetInfo,
                              const PredictionsType& ys,
                              const size_t numClasses,
                              const WeightsType& weights) :
    KFoldCV(Base(datasetInfo, numClasses), k, xs, ys, weights)
{ /* Nothing left to do. */ }

template<typename MLAlgorithm,
         typename Metric,
         typename MatType,
         typename PredictionsType,
         typename WeightsType>
KFoldCV<MLAlgorithm,
        Metric,
        MatType,
        PredictionsType,
        WeightsType>::KFoldCV(Base&& base,
                              const size_t k,
                              const MatType& xs,
                              const PredictionsType& ys) :
  base(std::move(base)), k(k)
{
  if (k < 2)
    throw std::invalid_argument("KFoldCV: k should not be less than 2");

  Base::AssertDataConsistency(xs, ys);

  InitKFoldCVMat(xs, this->xs);
  InitKFoldCVMat(ys, this->ys);
}

template<typename MLAlgorithm,
         typename Metric,
         typename MatType,
         typename PredictionsType,
         typename WeightsType>
KFoldCV<MLAlgorithm,
        Metric,
        MatType,
        PredictionsType,
        WeightsType>::KFoldCV(Base&& base,
                              const size_t k,
                              const MatType& xs,
                              const PredictionsType& ys,
                              const WeightsType& weights) :
    KFoldCV(std::move(base), k, xs, ys)
{
  Base::AssertWeightsConsistency(xs, weights);

  InitKFoldCVMat(weights, this->weights);
}

template<typename MLAlgorithm,
         typename Metric,
         typename MatType,
         typename PredictionsType,
         typename WeightsType>
template<typename... MLAlgorithmArgs>
double KFoldCV<MLAlgorithm,
               Metric,
               MatType,
               PredictionsType,
               WeightsType>::Evaluate(const MLAlgorithmArgs&... args)
{
  return TrainAndEvaluate(args...);
}

template<typename MLAlgorithm,
         typename Metric,
         typename MatType,
         typename PredictionsType,
         typename WeightsType>
MLAlgorithm& KFoldCV<MLAlgorithm,
                     Metric,
                     MatType,
                     PredictionsType,
                     WeightsType>::Model()
{
  if (modelPtr == nullptr)
    throw std::logic_error(
        "KFoldCV::Model(): attempted to access an uninitialized model");

  return *modelPtr;
}

template<typename MLAlgorithm,
         typename Metric,
         typename MatType,
         typename PredictionsType,
         typename WeightsType>
template<typename DataType>
void KFoldCV<MLAlgorithm,
             Metric,
             MatType,
             PredictionsType,
             WeightsType>::InitKFoldCVMat(const DataType& source,
                                          DataType& destination)
{
  binSize = source.n_cols / k;
  trainingSubsetSize = binSize * (k - 1);
  lastBinSize = source.n_cols - ((k - 1) * binSize);

  destination = (k == 2) ? source : arma::join_rows(source,
      source.cols(0, trainingSubsetSize - binSize - 1));
}

template<typename MLAlgorithm,
         typename Metric,
         typename MatType,
         typename PredictionsType,
         typename WeightsType>
template<typename... MLAlgorithmArgs, bool Enabled, typename>
double KFoldCV<MLAlgorithm,
                Metric,
                MatType,
                PredictionsType,
                WeightsType>::TrainAndEvaluate(const MLAlgorithmArgs&... args)
{
  arma::vec evaluations(k);

  for (size_t i = 0; i < k; ++i)
  {
    MLAlgorithm&& model  = base.Train(GetTrainingSubset(xs, i),
        GetTrainingSubset(ys, i), args...);
    evaluations(i) = Metric::Evaluate(model, GetValidationSubset(xs, i),
        GetValidationSubset(ys, i));
    if (i == k - 1)
      modelPtr.reset(new MLAlgorithm(std::move(model)));
  }

  return arma::mean(evaluations);
}

template<typename MLAlgorithm,
         typename Metric,
         typename MatType,
         typename PredictionsType,
         typename WeightsType>
template<typename... MLAlgorithmArgs, bool Enabled, typename, typename>
double KFoldCV<MLAlgorithm,
                Metric,
                MatType,
                PredictionsType,
                WeightsType>::TrainAndEvaluate(const MLAlgorithmArgs&... args)
{
  arma::vec evaluations(k);

  for (size_t i = 0; i < k; ++i)
  {
    MLAlgorithm&& model = (weights.n_elem > 0) ?
        base.Train(GetTrainingSubset(xs, i), GetTrainingSubset(ys, i),
            GetTrainingSubset(weights, i), args...) :
        base.Train(GetTrainingSubset(xs, i), GetTrainingSubset(ys, i),
            args...);
    evaluations(i) = Metric::Evaluate(model, GetValidationSubset(xs, i),
        GetValidationSubset(ys, i));
    if (i == k - 1)
      modelPtr.reset(new MLAlgorithm(std::move(model)));
  }

  return arma::mean(evaluations);
}

template<typename MLAlgorithm,
         typename Metric,
         typename MatType,
         typename PredictionsType,
         typename WeightsType>
size_t KFoldCV<MLAlgorithm,
               Metric,
               MatType,
               PredictionsType,
               WeightsType>::ValidationSubsetFirstCol(const size_t i)
{
  // Use as close to the beginning of the dataset as we can.
  return (i == 0) ? trainingSubsetSize : binSize * (i - 1);
}

template<typename MLAlgorithm,
         typename Metric,
         typename MatType,
         typename PredictionsType,
         typename WeightsType>
template<typename ElementType>
arma::Mat<ElementType> KFoldCV<MLAlgorithm,
                               Metric,
                               MatType,
                               PredictionsType,
                               WeightsType>::GetTrainingSubset(
    arma::Mat<ElementType>& m,
    const size_t i)
{
  // If this is the last fold, we have to handle it a little bit differently,
  // since the last fold may not contain 'binSize' points.
  const size_t subsetSize = (i == k - 1) ? lastBinSize + (k - 2) * binSize :
      trainingSubsetSize;

  return arma::Mat<ElementType>(m.colptr(binSize * i), m.n_rows, subsetSize,
      false, true);
}

template<typename MLAlgorithm,
         typename Metric,
         typename MatType,
         typename PredictionsType,
         typename WeightsType>
template<typename ElementType>
arma::Row<ElementType> KFoldCV<MLAlgorithm,
                               Metric,
                               MatType,
                               PredictionsType,
                               WeightsType>::GetTrainingSubset(
    arma::Row<ElementType>& r,
    const size_t i)
{
  // If this is the last fold, we have to handle it a little bit differently,
  // since the last fold may not contain 'binSize' points.
  const size_t subsetSize = (i == k - 1) ? lastBinSize + (k - 2) * binSize :
      trainingSubsetSize;

  return arma::Row<ElementType>(r.colptr(binSize * i), subsetSize, false, true);
}

template<typename MLAlgorithm,
         typename Metric,
         typename MatType,
         typename PredictionsType,
         typename WeightsType>
template<typename ElementType>
arma::Mat<ElementType> KFoldCV<MLAlgorithm,
                               Metric,
                               MatType,
                               PredictionsType,
                               WeightsType>::GetValidationSubset(
    arma::Mat<ElementType>& m,
    const size_t i)
{
  const size_t subsetSize = (i == 0) ? lastBinSize : binSize;
  return arma::Mat<ElementType>(m.colptr(ValidationSubsetFirstCol(i)), m.n_rows,
      subsetSize, false, true);
}

template<typename MLAlgorithm,
         typename Metric,
         typename MatType,
         typename PredictionsType,
         typename WeightsType>
template<typename ElementType>
arma::Row<ElementType> KFoldCV<MLAlgorithm,
                               Metric,
                               MatType,
                               PredictionsType,
                               WeightsType>::GetValidationSubset(
    arma::Row<ElementType>& r,
    const size_t i)
{
  const size_t subsetSize = (i == 0) ? lastBinSize : binSize;
  return arma::Row<ElementType>(r.colptr(ValidationSubsetFirstCol(i)),
      subsetSize, false, true);
}

} // namespace cv
} // namespace mlpack

#endif

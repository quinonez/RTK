/*=========================================================================
 *
 *  Copyright RTK Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#ifndef __rtkTiffLutImageFilter_h
#define __rtkTiffLutImageFilter_h

#include "rtkLutImageFilter.h"
#include <itkNumericTraits.h>

namespace rtk
{

/** \class TiffLutImageFilter
 * \brief Lookup table for tiff projection images.
 *
 * The lookup table converts the raw values to the logarithm of the value divided by the max
 *
 *
 * \author Simon Rit
 *
 * \ingroup TiffLutImageFilter
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT TiffLutImageFilter : public LutImageFilter<TInputImage, TOutputImage>
{

public:
  /** Standard class typedefs. */
  typedef TiffLutImageFilter                        Self;
  typedef LutImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                   Pointer;
  typedef itk::SmartPointer<const Self>             ConstPointer;

  typedef typename TInputImage::PixelType           InputImagePixelType;
  typedef typename TOutputImage::PixelType          OutputImagePixelType;
  typedef typename Superclass::FunctorType::LutType LutType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(TiffLutImageFilter, LutImageFilter);
protected:
  TiffLutImageFilter();
  virtual ~TiffLutImageFilter() {}

private:
  TiffLutImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&);     //purposely not implemented

};

} // end namespace rtk

template <class TInputImage, class TOutputImage>
rtk::TiffLutImageFilter<TInputImage, TOutputImage>::TiffLutImageFilter()
{
  // Create the lut
  typename LutType::Pointer lut = LutType::New();
  typename LutType::SizeType size;
  size[0] = itk::NumericTraits<InputImagePixelType>::max()-itk::NumericTraits<InputImagePixelType>::min()+1;
  lut->SetRegions( size );
  lut->Allocate();

  // Iterate and set lut
  OutputImagePixelType logRef = log(OutputImagePixelType(size[0]+1) );
  itk::ImageRegionIteratorWithIndex<LutType> it( lut, lut->GetBufferedRegion() );
  it.GoToBegin();

  // 0 value is assumed to correspond to no attenuation
  it.Set(0.);
  ++it;

  //Conventional lookup table for the rest
  while( !it.IsAtEnd() ) {
    it.Set( logRef - log( OutputImagePixelType(it.GetIndex()[0]+1) ) );
    ++it;
    }
  // Set the lut to member and functor
  this->SetLut(lut);
}

#endif

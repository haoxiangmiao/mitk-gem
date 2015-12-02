#pragma once

#include <mitkImage.h>
#include <mitkUnstructuredGridToUnstructuredGridFilter.h>

#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkUnstructuredGrid.h>
#include <vtkImageStencil.h>

#include "BoneDensityFunctor.h"
#include "PowerLawFunctor.h"

class MaterialMappingFilter : public mitk::UnstructuredGridToUnstructuredGridFilter {
public:
    mitkClassMacro(MaterialMappingFilter, UnstructuredGridToUnstructuredGridFilter)
    itkFactorylessNewMacro(Self)
    itkCloneMacro(Self)

    void SetIntensityImage(mitk::Image::Pointer _p){
        m_IntensityImage = _p;
    }

    void SetDensityFunctor(BoneDensityFunctor &&_f){
        m_BoneDensityFunctor = _f;
    }

    void SetPowerLawFunctor(PowerLawFunctor &&_f){
        m_PowerLawFunctor = _f;
    }

    virtual void GenerateOutputInformation() override;
    virtual void GenerateData() override;
protected:
    using VtkImage = vtkSmartPointer<vtkImageData>;
    using VtkStencil = vtkSmartPointer<vtkImageStencil>;
    using VtkUGrid = vtkSmartPointer<vtkUnstructuredGridBase>;

    MaterialMappingFilter();
    virtual ~MaterialMappingFilter(){};

    VtkUGrid extractSurface(const VtkUGrid);
    // convert surface to inverted binary mask (=> 0 inside, 1 outside)
    VtkImage extractVOI(const VtkImage, const VtkUGrid);
    VtkImage createStencil(const VtkUGrid, const VtkImage);
    VtkImage createPeeledMask(const VtkImage _img, const VtkImage _mask);
    void inplaceExtendImage(VtkImage _img, VtkImage _mask, bool _maxVal);
    vtkSmartPointer<vtkDoubleArray> createDataArray(std::string);

    mitk::UnstructuredGrid::Pointer m_VolumeMesh;
    mitk::Image::Pointer m_IntensityImage;
    BoneDensityFunctor m_BoneDensityFunctor;
    PowerLawFunctor m_PowerLawFunctor;
};
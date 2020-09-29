
## About

dcm_qa_ge is a simple DICOM to NIfTI validator script and dataset to test conversion of [GE](https://www.gehealthcare.com/products/magnetic-resonance-imaging) magnetic resonance imaging (MRI) data. Specifically, these datasets provide examples of several tags useful for research scientists.

The BIDS tag [ParallelReductionFactorInPlane](https://bids-specification.readthedocs.io/en/latest/04-modality-specific-files/01-magnetic-resonance-imaging-data.html#in-plane-spatial-encoding) corresponds to the public DICOM tag [Parallel Reduction Factor In-plane (0018,9069)](http://dicomlookup.com/lookup.asp?sw=Tnumber&q=(0018,9069)). Current GE images do not provide this tag, but the GE private tag `Asset R Factors` (0043,1083) does provide allow one to infer this value. Specifically, the first value of this tag corresponds to the reciprocal of this value (and the second value is the reciprocal of [Parallel Reduction Factor out-of-plane (0018,9155)](http://dicomlookup.com/lookup.asp?sw=Tnumber&q=(0018,9155))). Therefore, a scan with a ParallelReductionFactorInPlane of 2 will have this tag:

```
 (0043,1083) DS [0.5\1] 
```

The BIDS tag [MultibandAccelerationFactor](https://bids-specification.readthedocs.io/en/latest/04-modality-specific-files/01-magnetic-resonance-imaging-data.html#slice-acceleration) is reported in the GE private tag 0043,10b6. This tag reports at multiple values, the first is the Multiband (`HyperBand`) Factor, the second is the Slice FOV Shift Factor, and the third refers to the Calibration Method. An example of this tag is:
 
```
(0043,10b6) LO [2\4\19\\\\] 
```

The BIDS tag [PhaseEncodingDirection](https://bids-specification.readthedocs.io/en/latest/04-modality-specific-files/01-magnetic-resonance-imaging-data.html#in-plane-spatial-encoding) combines the DICOM tags [In-plane Phase Encoding Direction (0018,1312)](http://dicomlookup.com/lookup.asp?sw=Tnumber&q=(0018,1312)) and [Rectilinear Phase Encode Reordering (0018,9034)](http://dicomlookup.com/lookup.asp?sw=Tnumber&q=(0018,9034)). While (0018,1312) has been provided for years, GE software Rx27 introduced reporting of (0018,9034). An example of these tags is:
 
```
(0018,1312) CS [ROW]
(0018,9034) CS [REVERSE_LINEAR]
```

The images were acquired on a GE 3T SIGNA Pioneer with software versions 28\LX\MR29.1_EA_2039.g by Jaemin Shin. 

## Series

* 01_Ax_DWI_TENSOR_R2
  * Description: Axial diffusion-weighted sequence with ParallelReductionFactorInPlane of 2.
  
* 02_Ax_DWI_TENSOR_R3
  * Description: Axial diffusion-weighted sequence with ParallelReductionFactorInPlane of 3.

* 03_Ax_DWI_TENSOR_R1p5
  *  Description: Axial diffusion-weighted sequence with ParallelReductionFactorInPlane of 1.5.

* 04_Ax_DWI_TENSOR_R2MB2
  * Description: Axial diffusion-weighted sequence with ParallelReductionFactorInPlane of 2 and Multiband (HyperBand) of 2.

* 05_3D_Ax_T1_MP_RAGE_R22
  *  Description: Axial T1-weighted sequence with ParallelReductionFactorInPlane of 2 and ParallelReductionFactorOutOfPlane of 2.

; ModuleID = 'Python compiler'
source_filename = "Python compiler"

define float @target() {
entry:
  %return_value = alloca float
  %r = alloca float
  %pi = alloca float
  store float 0x400921CAC0000000, float* %pi
  store float 3.000000e+00, float* %r
  %pi1 = load float, float* %pi
  %r2 = load float, float* %r
  %multmp = fmul float %pi1, %r2
  %r3 = load float, float* %r
  %multmp4 = fmul float %multmp, %r3
  store float %multmp4, float* %return_value
  %return_value5 = load float, float* %return_value
  ret float %return_value5
}

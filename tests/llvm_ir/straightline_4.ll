; ModuleID = 'Python compiler'
source_filename = "Python compiler"

define float @target() {
entry:
  %return_value = alloca float
  %r = alloca float
  %b = alloca float
  %a = alloca float
  %pi = alloca float
  store float 0x400921CAC0000000, float* %pi
  store float 0x40024924A0000000, float* %a
  store float 0x3FFE666680000000, float* %b
  %a1 = load float, float* %a
  %b2 = load float, float* %b
  %multmp = fmul float %a1, %b2
  store float %multmp, float* %r
  %pi3 = load float, float* %pi
  %multmp4 = fmul float %pi3, 2.000000e+00
  %r5 = load float, float* %r
  %multmp6 = fmul float %multmp4, %r5
  store float %multmp6, float* %return_value
  %return_value7 = load float, float* %return_value
  ret float %return_value7
}

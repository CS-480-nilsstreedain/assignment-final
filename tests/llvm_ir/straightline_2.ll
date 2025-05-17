; ModuleID = 'Python compiler'
source_filename = "Python compiler"

define float @target() {
entry:
  %return_value = alloca float
  store float 2.000000e+00, float* %return_value
  %return_value1 = load float, float* %return_value
  ret float %return_value1
}

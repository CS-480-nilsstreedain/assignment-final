; ModuleID = 'Python compiler'
source_filename = "Python compiler"

define float @target() {
entry:
  %return_value = alloca float
  store float 0x400921CAC0000000, float* %return_value
  %return_value1 = load float, float* %return_value
  ret float %return_value1
}

; ModuleID = 'Python compiler'
source_filename = "Python compiler"

define float @target() {
entry:
  %return_value = alloca float
  %n = alloca float
  store float 8.000000e+00, float* %n
  br i1 true, label %ifBlock, label %ifContinueBlock

ifBlock:                                          ; preds = %entry
  %n1 = load float, float* %n
  %multmp = fmul float %n1, 2.000000e+00
  store float %multmp, float* %n
  br label %ifContinueBlock

ifContinueBlock:                                  ; preds = %entry, %ifBlock
  %n2 = load float, float* %n
  store float %n2, float* %return_value
  %return_value3 = load float, float* %return_value
  ret float %return_value3
}

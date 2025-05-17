; ModuleID = 'Python compiler'
source_filename = "Python compiler"

define float @target() {
entry:
  %return_value = alloca float
  %v = alloca float
  %b = alloca float
  %a = alloca float
  store float 5.000000e+00, float* %a
  store float 5.000000e+00, float* %b
  store float 0.000000e+00, float* %v
  %a1 = load float, float* %a
  %b2 = load float, float* %b
  %eqtmp = fcmp ueq float %a1, %b2
  %booltmp = uitofp i1 %eqtmp to float
  %ifcond10 = fcmp one float %booltmp, 0.000000e+00
  br i1 %ifcond10, label %ifBlock, label %ifContinueBlock11

ifBlock:                                          ; preds = %entry
  %a3 = load float, float* %a
  %gttmp = fcmp ugt float %a3, 4.000000e+00
  %booltmp4 = uitofp i1 %gttmp to float
  %ifcond = fcmp one float %booltmp4, 0.000000e+00
  br i1 %ifcond, label %ifBlock5, label %elseBlock

ifBlock5:                                         ; preds = %ifBlock
  %a6 = load float, float* %a
  %b7 = load float, float* %b
  %addtmp = fadd float %a6, %b7
  store float %addtmp, float* %v
  br label %ifContinueBlock

elseBlock:                                        ; preds = %ifBlock
  %a8 = load float, float* %a
  %b9 = load float, float* %b
  %multmp = fmul float %a8, %b9
  store float %multmp, float* %v
  br label %ifContinueBlock

ifContinueBlock:                                  ; preds = %elseBlock, %ifBlock5
  br label %ifContinueBlock11

ifContinueBlock11:                                ; preds = %entry, %ifContinueBlock
  %v12 = load float, float* %v
  store float %v12, float* %return_value
  %return_value13 = load float, float* %return_value
  ret float %return_value13
}

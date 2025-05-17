; ModuleID = 'Python compiler'
source_filename = "Python compiler"

define float @target() {
entry:
  %return_value = alloca float
  %v = alloca float
  %b = alloca float
  %a = alloca float
  store float 5.000000e+00, float* %a
  store float 1.000000e+01, float* %b
  store float 0.000000e+00, float* %v
  %b1 = load float, float* %b
  %a2 = load float, float* %a
  %divtmp = fdiv float %b1, %a2
  %neqtmp = fcmp une float %divtmp, 2.000000e+00
  %booltmp = uitofp i1 %neqtmp to float
  %ifcond23 = fcmp one float %booltmp, 0.000000e+00
  br i1 %ifcond23, label %ifBlock, label %elseBlock10

ifBlock:                                          ; preds = %entry
  %a3 = load float, float* %a
  %gtetmp = fcmp uge float %a3, 5.000000e+00
  %booltmp4 = uitofp i1 %gtetmp to float
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
  %subtmp = fsub float %a8, %b9
  store float %subtmp, float* %v
  br label %ifContinueBlock

ifContinueBlock:                                  ; preds = %elseBlock, %ifBlock5
  br label %ifContinueBlock24

elseBlock10:                                      ; preds = %entry
  %a11 = load float, float* %a
  %gtetmp12 = fcmp uge float %a11, 5.000000e+00
  %booltmp13 = uitofp i1 %gtetmp12 to float
  %ifcond21 = fcmp one float %booltmp13, 0.000000e+00
  br i1 %ifcond21, label %ifBlock14, label %elseBlock17

ifBlock14:                                        ; preds = %elseBlock10
  %a15 = load float, float* %a
  %b16 = load float, float* %b
  %multmp = fmul float %a15, %b16
  store float %multmp, float* %v
  br label %ifContinueBlock22

elseBlock17:                                      ; preds = %elseBlock10
  %a18 = load float, float* %a
  %b19 = load float, float* %b
  %divtmp20 = fdiv float %a18, %b19
  store float %divtmp20, float* %v
  br label %ifContinueBlock22

ifContinueBlock22:                                ; preds = %elseBlock17, %ifBlock14
  br label %ifContinueBlock24

ifContinueBlock24:                                ; preds = %ifContinueBlock22, %ifContinueBlock
  %v25 = load float, float* %v
  store float %v25, float* %return_value
  %return_value26 = load float, float* %return_value
  ret float %return_value26
}

; ModuleID = 'Python compiler'
source_filename = "Python compiler"

define float @target() {
entry:
  %return_value = alloca float
  %j = alloca float
  %v = alloca float
  %i = alloca float
  store float 0.000000e+00, float* %i
  store float 0.000000e+00, float* %v
  br label %whileCondBlock

whileCondBlock:                                   ; preds = %ifContinueBlock17, %entry
  br i1 true, label %whileBlock, label %whileContinueBlock18

whileBlock:                                       ; preds = %whileCondBlock
  store float 0.000000e+00, float* %j
  br label %whileCondBlock1

whileCondBlock1:                                  ; preds = %ifContinueBlock, %whileBlock
  br i1 true, label %whileBlock2, label %whileContinueBlock

whileBlock2:                                      ; preds = %whileCondBlock1
  %v3 = load float, float* %v
  %i4 = load float, float* %i
  %j5 = load float, float* %j
  %multmp = fmul float %i4, %j5
  %addtmp = fadd float %v3, %multmp
  store float %addtmp, float* %v
  %j6 = load float, float* %j
  %addtmp7 = fadd float %j6, 1.000000e+00
  store float %addtmp7, float* %j
  %j8 = load float, float* %j
  %i9 = load float, float* %i
  %gtetmp = fcmp uge float %j8, %i9
  %booltmp = uitofp i1 %gtetmp to float
  %ifcond = fcmp one float %booltmp, 0.000000e+00
  br i1 %ifcond, label %ifBlock, label %ifContinueBlock

ifBlock:                                          ; preds = %whileBlock2
  br label %whileContinueBlock

ifContinueBlock:                                  ; preds = %whileBlock2
  br label %whileCondBlock1

whileContinueBlock:                               ; preds = %ifBlock, %whileCondBlock1
  %i10 = load float, float* %i
  %addtmp11 = fadd float %i10, 1.000000e+00
  store float %addtmp11, float* %i
  %i12 = load float, float* %i
  %gtetmp13 = fcmp uge float %i12, 1.000000e+01
  %booltmp14 = uitofp i1 %gtetmp13 to float
  %ifcond16 = fcmp one float %booltmp14, 0.000000e+00
  br i1 %ifcond16, label %ifBlock15, label %ifContinueBlock17

ifBlock15:                                        ; preds = %whileContinueBlock
  br label %whileContinueBlock18

ifContinueBlock17:                                ; preds = %whileContinueBlock
  br label %whileCondBlock

whileContinueBlock18:                             ; preds = %ifBlock15, %whileCondBlock
  %v19 = load float, float* %v
  store float %v19, float* %return_value
  %return_value20 = load float, float* %return_value
  ret float %return_value20
}

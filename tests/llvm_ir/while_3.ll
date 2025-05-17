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

whileCondBlock:                                   ; preds = %whileContinueBlock, %entry
  %i1 = load float, float* %i
  %lttmp = fcmp ult float %i1, 1.000000e+01
  %booltmp = uitofp i1 %lttmp to float
  %whilecond = fcmp one float %booltmp, 0.000000e+00
  br i1 %whilecond, label %whileBlock, label %whileContinueBlock16

whileBlock:                                       ; preds = %whileCondBlock
  store float 0.000000e+00, float* %j
  br label %whileCondBlock2

whileCondBlock2:                                  ; preds = %whileBlock8, %whileBlock
  %j3 = load float, float* %j
  %i4 = load float, float* %i
  %lttmp5 = fcmp ult float %j3, %i4
  %booltmp6 = uitofp i1 %lttmp5 to float
  %whilecond7 = fcmp one float %booltmp6, 0.000000e+00
  br i1 %whilecond7, label %whileBlock8, label %whileContinueBlock

whileBlock8:                                      ; preds = %whileCondBlock2
  %v9 = load float, float* %v
  %i10 = load float, float* %i
  %j11 = load float, float* %j
  %multmp = fmul float %i10, %j11
  %addtmp = fadd float %v9, %multmp
  store float %addtmp, float* %v
  %j12 = load float, float* %j
  %addtmp13 = fadd float %j12, 1.000000e+00
  store float %addtmp13, float* %j
  br label %whileCondBlock2

whileContinueBlock:                               ; preds = %whileCondBlock2
  %i14 = load float, float* %i
  %addtmp15 = fadd float %i14, 1.000000e+00
  store float %addtmp15, float* %i
  br label %whileCondBlock

whileContinueBlock16:                             ; preds = %whileCondBlock
  %v17 = load float, float* %v
  store float %v17, float* %return_value
  %return_value18 = load float, float* %return_value
  ret float %return_value18
}

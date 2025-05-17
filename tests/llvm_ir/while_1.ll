; ModuleID = 'Python compiler'
source_filename = "Python compiler"

define float @target() {
entry:
  %return_value = alloca float
  %k = alloca float
  %i = alloca float
  store float 0.000000e+00, float* %i
  store float 0.000000e+00, float* %k
  br label %whileCondBlock

whileCondBlock:                                   ; preds = %whileBlock, %entry
  %i1 = load float, float* %i
  %lttmp = fcmp ult float %i1, 1.000000e+01
  %booltmp = uitofp i1 %lttmp to float
  %whilecond = fcmp one float %booltmp, 0.000000e+00
  br i1 %whilecond, label %whileBlock, label %whileContinueBlock

whileBlock:                                       ; preds = %whileCondBlock
  %i2 = load float, float* %i
  %addtmp = fadd float %i2, 1.000000e+00
  store float %addtmp, float* %i
  %k3 = load float, float* %k
  %i4 = load float, float* %i
  %multmp = fmul float %i4, 2.000000e+00
  %addtmp5 = fadd float %k3, %multmp
  store float %addtmp5, float* %k
  br label %whileCondBlock

whileContinueBlock:                               ; preds = %whileCondBlock
  %k6 = load float, float* %k
  store float %k6, float* %return_value
  %return_value7 = load float, float* %return_value
  ret float %return_value7
}

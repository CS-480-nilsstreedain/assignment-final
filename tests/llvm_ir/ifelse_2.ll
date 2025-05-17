; ModuleID = 'Python compiler'
source_filename = "Python compiler"

define float @target() {
entry:
  %return_value = alloca float
  %v = alloca float
  %r = alloca float
  %pi = alloca float
  store float 0x400921CAC0000000, float* %pi
  store float 3.000000e+00, float* %r
  store float 0.000000e+00, float* %v
  %pi1 = load float, float* %pi
  %r2 = load float, float* %r
  %multmp = fmul float %pi1, %r2
  %r3 = load float, float* %r
  %multmp4 = fmul float %multmp, %r3
  %ltetmp = fcmp ule float %multmp4, 2.800000e+01
  %booltmp = uitofp i1 %ltetmp to float
  %ifcond = fcmp one float %booltmp, 0.000000e+00
  br i1 %ifcond, label %ifBlock, label %elseBlock

ifBlock:                                          ; preds = %entry
  %pi5 = load float, float* %pi
  %r6 = load float, float* %r
  %multmp7 = fmul float %pi5, %r6
  %r8 = load float, float* %r
  %multmp9 = fmul float %multmp7, %r8
  store float %multmp9, float* %v
  br label %ifContinueBlock

elseBlock:                                        ; preds = %entry
  %pi10 = load float, float* %pi
  %multmp11 = fmul float %pi10, 2.000000e+00
  %r12 = load float, float* %r
  %multmp13 = fmul float %multmp11, %r12
  store float %multmp13, float* %v
  br label %ifContinueBlock

ifContinueBlock:                                  ; preds = %elseBlock, %ifBlock
  %v14 = load float, float* %v
  store float %v14, float* %return_value
  %return_value15 = load float, float* %return_value
  ret float %return_value15
}

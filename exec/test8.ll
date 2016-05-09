; ModuleID = 'src/test8.c'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: nounwind uwtable
define i32 @main(i32 %argc, i8** %argv) #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i8**, align 8
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %c = alloca i32, align 4
  %d = alloca i32, align 4
  %f1 = alloca float, align 4
  %f2 = alloca float, align 4
  %f3 = alloca float, align 4
  %f4 = alloca float, align 4
  store i32 0, i32* %1, align 4
  store i32 %argc, i32* %2, align 4
  store i8** %argv, i8*** %3, align 8
  store i32 1, i32* %a, align 4
  store i32 2, i32* %b, align 4
  %4 = load i32, i32* %a, align 4
  %5 = load i32, i32* %b, align 4
  %6 = add nsw i32 %4, %5
  store i32 %6, i32* %c, align 4
  %7 = load i32, i32* %a, align 4
  %8 = load i32, i32* %b, align 4
  %9 = mul nsw i32 %7, %8
  store i32 %9, i32* %d, align 4
  %10 = load i32, i32* %d, align 4
  %11 = load i32, i32* %b, align 4
  %12 = sdiv i32 %10, %11
  store i32 %12, i32* %a, align 4
  %13 = load i32, i32* %d, align 4
  %14 = load i32, i32* %a, align 4
  %15 = sub nsw i32 %13, %14
  store i32 %15, i32* %b, align 4
  %16 = load i32, i32* %a, align 4
  %17 = load i32, i32* %b, align 4
  %18 = icmp eq i32 %16, %17
  br i1 %18, label %19, label %23

; <label>:19                                      ; preds = %0
  %20 = load i32, i32* %c, align 4
  %21 = load i32, i32* %a, align 4
  %22 = srem i32 %20, %21
  br label %25

; <label>:23                                      ; preds = %0
  %24 = load i32, i32* %b, align 4
  store i32 %24, i32* %a, align 4
  br label %25

; <label>:25                                      ; preds = %23, %19
  store float 1.500000e+00, float* %f1, align 4
  store float 2.500000e+00, float* %f2, align 4
  %26 = load float, float* %f1, align 4
  %27 = load float, float* %f2, align 4
  %28 = fadd float %26, %27
  store float %28, float* %f3, align 4
  %29 = load float, float* %f1, align 4
  %30 = load float, float* %f2, align 4
  %31 = fmul float %29, %30
  store float %31, float* %f4, align 4
  %32 = load float, float* %f4, align 4
  %33 = load float, float* %f2, align 4
  %34 = fdiv float %32, %33
  store float %34, float* %f1, align 4
  %35 = load float, float* %f4, align 4
  %36 = load float, float* %f1, align 4
  %37 = fsub float %35, %36
  store float %37, float* %f2, align 4
  %38 = load float, float* %f1, align 4
  %39 = load float, float* %f2, align 4
  %40 = fcmp oeq float %38, %39
  br i1 %40, label %41, label %42

; <label>:41                                      ; preds = %25
  store float 0.000000e+00, float* %f1, align 4
  br label %42

; <label>:42                                      ; preds = %41, %25
  ret i32 0
}

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"Ubuntu clang version 3.8.0-svn257311-1~exp1 (trunk) (based on LLVM 3.8.0)"}

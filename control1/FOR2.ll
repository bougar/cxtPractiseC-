; ModuleID = 'examples/FOR2.c'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: nounwind uwtable
define void @foo(i64 %a) #0 {
  %1 = alloca i64, align 8
  %v = alloca i64, align 8
  store i64 %a, i64* %1, align 8
  store i64 0, i64* %v, align 8
  store i64 1, i64* %v, align 8
  ret void
}

; Function Attrs: nounwind uwtable
define void @foo2(i64 %a) #0 {
  %1 = alloca i64, align 8
  %v = alloca i64, align 8
  store i64 %a, i64* %1, align 8
  store i64 0, i64* %v, align 8
  store i64 111, i64* %1, align 8
  br label %2

; <label>:2                                       ; preds = %6, %0
  %3 = load i64, i64* %1, align 8
  %4 = icmp eq i64 %3, 222
  br i1 %4, label %5, label %9

; <label>:5                                       ; preds = %2
  store i64 1, i64* %v, align 8
  br label %6

; <label>:6                                       ; preds = %5
  %7 = load i64, i64* %1, align 8
  %8 = add i64 %7, 999
  store i64 %8, i64* %1, align 8
  br label %2

; <label>:9                                       ; preds = %2
  ret void
}

; Function Attrs: nounwind uwtable
define void @foo3(i64 %a) #0 {
  %1 = alloca i64, align 8
  %v = alloca i64, align 8
  store i64 %a, i64* %1, align 8
  store i64 0, i64* %v, align 8
  %2 = load i64, i64* %v, align 8
  %3 = add i64 %2, 1
  store i64 %3, i64* %v, align 8
  %4 = load i64, i64* %v, align 8
  %5 = mul i64 %4, 27
  store i64 %5, i64* %v, align 8
  ret void
}

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"Ubuntu clang version 3.8.0-svn257311-1~exp1 (trunk) (based on LLVM 3.8.0)"}

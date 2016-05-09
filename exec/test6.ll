; ModuleID = 'src/test6.cpp'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%class.Test = type { i8 }

; Function Attrs: norecurse uwtable
define i32 @main() #0 {
  %1 = alloca i32, align 4
  %main_1 = alloca %class.Test*, align 8
  store i32 0, i32* %1, align 4
  %2 = call noalias i8* @_Znwm(i64 1) #3
  %3 = bitcast i8* %2 to %class.Test*
  store %class.Test* %3, %class.Test** %main_1, align 8
  %4 = load %class.Test*, %class.Test** %main_1, align 8
  %5 = icmp eq %class.Test* %4, null
  br i1 %5, label %8, label %6

; <label>:6                                       ; preds = %0
  %7 = bitcast %class.Test* %4 to i8*
  call void @_ZdlPv(i8* %7) #4
  br label %8

; <label>:8                                       ; preds = %6, %0
  %9 = load i32, i32* %1, align 4
  ret i32 %9
}

; Function Attrs: nobuiltin
declare noalias i8* @_Znwm(i64) #1

; Function Attrs: nobuiltin nounwind
declare void @_ZdlPv(i8*) #2

attributes #0 = { norecurse uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nobuiltin "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nobuiltin nounwind "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { builtin }
attributes #4 = { builtin nounwind }

!llvm.ident = !{!0}

!0 = !{!"Ubuntu clang version 3.8.0-svn257311-1~exp1 (trunk) (based on LLVM 3.8.0)"}

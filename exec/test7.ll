; ModuleID = 'src/test7.cpp'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%class.Test = type { i8 }

; Function Attrs: nounwind uwtable
define void @_Z5free1P4Test(%class.Test* %free1_arg_1) #0 {
  %1 = alloca %class.Test*, align 8
  store %class.Test* %free1_arg_1, %class.Test** %1, align 8
  %2 = load %class.Test*, %class.Test** %1, align 8
  %3 = icmp eq %class.Test* %2, null
  br i1 %3, label %6, label %4

; <label>:4                                       ; preds = %0
  %5 = bitcast %class.Test* %2 to i8*
  call void @_ZdlPv(i8* %5) #5
  br label %6

; <label>:6                                       ; preds = %4, %0
  ret void
}

; Function Attrs: nobuiltin nounwind
declare void @_ZdlPv(i8*) #1

; Function Attrs: uwtable
define void @_Z4res1P4Test(%class.Test* %res1_arg_1) #2 {
  %1 = alloca %class.Test*, align 8
  store %class.Test* %res1_arg_1, %class.Test** %1, align 8
  %2 = call noalias i8* @_Znwm(i64 1) #6
  %3 = bitcast i8* %2 to %class.Test*
  store %class.Test* %3, %class.Test** %1, align 8
  ret void
}

; Function Attrs: nobuiltin
declare noalias i8* @_Znwm(i64) #3

; Function Attrs: norecurse uwtable
define i32 @main() #4 {
  %main_1 = alloca %class.Test*, align 8
  %1 = load %class.Test*, %class.Test** %main_1, align 8
  call void @_Z4res1P4Test(%class.Test* %1)
  %2 = load %class.Test*, %class.Test** %main_1, align 8
  call void @_Z5free1P4Test(%class.Test* %2)
  ret i32 0
}

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nobuiltin nounwind "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nobuiltin "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { norecurse uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #5 = { builtin nounwind }
attributes #6 = { builtin }

!llvm.ident = !{!0}

!0 = !{!"Ubuntu clang version 3.8.0-svn257311-1~exp1 (trunk) (based on LLVM 3.8.0)"}

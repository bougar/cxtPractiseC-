; ModuleID = 'src/test5.c'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: nounwind uwtable
define void @free1(i32* %free1_arg_1) #0 {
  %1 = alloca i32*, align 8
  store i32* %free1_arg_1, i32** %1, align 8
  %2 = load i32*, i32** %1, align 8
  %3 = bitcast i32* %2 to i8*
  call void @free(i8* %3) #2
  ret void
}

; Function Attrs: nounwind
declare void @free(i8*) #1

; Function Attrs: nounwind uwtable
define void @res1(i32* %res1_arg_1) #0 {
  %1 = alloca i32*, align 8
  %res1_1 = alloca i32*, align 8
  store i32* %res1_arg_1, i32** %1, align 8
  %2 = call noalias i8* @malloc(i64 4) #2
  %3 = bitcast i8* %2 to i32*
  store i32* %3, i32** %res1_1, align 8
  %4 = load i32*, i32** %res1_1, align 8
  %5 = bitcast i32* %4 to i8*
  call void @free(i8* %5) #2
  %6 = call noalias i8* @malloc(i64 4) #2
  %7 = bitcast i8* %6 to i32*
  store i32* %7, i32** %1, align 8
  ret void
}

; Function Attrs: nounwind
declare noalias i8* @malloc(i64) #1

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
  %main_1 = alloca i32*, align 8
  %1 = load i32*, i32** %main_1, align 8
  call void @res1(i32* %1)
  %2 = load i32*, i32** %main_1, align 8
  call void @free1(i32* %2)
  ret i32 0
}

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }

!llvm.ident = !{!0}

!0 = !{!"Ubuntu clang version 3.8.0-svn257311-1~exp1 (trunk) (based on LLVM 3.8.0)"}

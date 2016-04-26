; ModuleID = 'FOR.c'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: nounwind uwtable
define void @foo(i64 %a) #0 !dbg !4 {
  %1 = alloca i64, align 8
  %v = alloca i64, align 8
  store i64 %a, i64* %1, align 8
  call void @llvm.dbg.declare(metadata i64* %1, metadata !13, metadata !14), !dbg !15
  call void @llvm.dbg.declare(metadata i64* %v, metadata !16, metadata !14), !dbg !17
  store i64 0, i64* %v, align 8, !dbg !17
  store i64 111, i64* %1, align 8, !dbg !18
  br label %2, !dbg !20

; <label>:2                                       ; preds = %6, %0
  %3 = load i64, i64* %1, align 8, !dbg !21
  %4 = icmp eq i64 %3, 222, !dbg !24
  br i1 %4, label %5, label %9, !dbg !25

; <label>:5                                       ; preds = %2
  store i64 1, i64* %v, align 8, !dbg !26
  br label %6, !dbg !28

; <label>:6                                       ; preds = %5
  %7 = load i64, i64* %1, align 8, !dbg !29
  %8 = add i64 %7, 999, !dbg !31
  store i64 %8, i64* %1, align 8, !dbg !32
  br label %2, !dbg !33

; <label>:9                                       ; preds = %2
  ret void, !dbg !34
}

; Function Attrs: nounwind readnone
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!10, !11}
!llvm.ident = !{!12}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "Ubuntu clang version 3.8.0-svn257311-1~exp1 (trunk) (based on LLVM 3.8.0)", isOptimized: false, runtimeVersion: 0, emissionKind: 1, enums: !2, subprograms: !3)
!1 = !DIFile(filename: "FOR.c", directory: "/home/user1/devel/cxt-bitbucket/examples")
!2 = !{}
!3 = !{!4}
!4 = distinct !DISubprogram(name: "foo", scope: !1, file: !1, line: 3, type: !5, isLocal: false, isDefinition: true, scopeLine: 3, flags: DIFlagPrototyped, isOptimized: false, variables: !2)
!5 = !DISubroutineType(types: !6)
!6 = !{null, !7}
!7 = !DIDerivedType(tag: DW_TAG_typedef, name: "uint64_t", file: !8, line: 55, baseType: !9)
!8 = !DIFile(filename: "/usr/include/stdint.h", directory: "/home/user1/devel/cxt-bitbucket/examples")
!9 = !DIBasicType(name: "long unsigned int", size: 64, align: 64, encoding: DW_ATE_unsigned)
!10 = !{i32 2, !"Dwarf Version", i32 4}
!11 = !{i32 2, !"Debug Info Version", i32 3}
!12 = !{!"Ubuntu clang version 3.8.0-svn257311-1~exp1 (trunk) (based on LLVM 3.8.0)"}
!13 = !DILocalVariable(name: "a", arg: 1, scope: !4, file: !1, line: 3, type: !7)
!14 = !DIExpression()
!15 = !DILocation(line: 3, column: 19, scope: !4)
!16 = !DILocalVariable(name: "v", scope: !4, file: !1, line: 4, type: !7)
!17 = !DILocation(line: 4, column: 12, scope: !4)
!18 = !DILocation(line: 5, column: 11, scope: !19)
!19 = distinct !DILexicalBlock(scope: !4, file: !1, line: 5, column: 3)
!20 = !DILocation(line: 5, column: 9, scope: !19)
!21 = !DILocation(line: 5, column: 19, scope: !22)
!22 = !DILexicalBlockFile(scope: !23, file: !1, discriminator: 1)
!23 = distinct !DILexicalBlock(scope: !19, file: !1, line: 5, column: 3)
!24 = !DILocation(line: 5, column: 21, scope: !22)
!25 = !DILocation(line: 5, column: 3, scope: !22)
!26 = !DILocation(line: 6, column: 7, scope: !27)
!27 = distinct !DILexicalBlock(scope: !23, file: !1, line: 5, column: 44)
!28 = !DILocation(line: 7, column: 3, scope: !27)
!29 = !DILocation(line: 5, column: 34, scope: !30)
!30 = !DILexicalBlockFile(scope: !23, file: !1, discriminator: 2)
!31 = !DILocation(line: 5, column: 36, scope: !30)
!32 = !DILocation(line: 5, column: 32, scope: !30)
!33 = !DILocation(line: 5, column: 3, scope: !30)
!34 = !DILocation(line: 8, column: 1, scope: !4)
